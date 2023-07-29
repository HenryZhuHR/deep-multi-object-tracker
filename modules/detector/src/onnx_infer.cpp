#include "detector/onnx_infer.h"

bool CheckStatus(const OrtApi *g_ort, OrtStatus *status)
{
    if (status != nullptr)
    {
        const char *msg = g_ort->GetErrorMessage(status);
        std::cerr << msg << std::endl;
        g_ort->ReleaseStatus(status);
        throw Ort::Exception(msg, OrtErrorCode::ORT_EP_FAIL);
    }
    return true;
}

ObjectDetectorONNXInfer::ObjectDetectorONNXInfer(std::string model_path, std::vector<std::string> class_indices)
{
    this->init(model_path);
}

ObjectDetectorONNXInfer::~ObjectDetectorONNXInfer()
{
}

bool ObjectDetectorONNXInfer::init(std::string model_path)
{
    OrtEnv *env;
    CheckStatus(g_ort, g_ort->CreateEnv(ORT_LOGGING_LEVEL_WARNING, "test", &env)); // Can set to ORT_LOGGING_LEVEL_INFO or ORT_LOGGING_LEVEL_VERBOSE for more info

    // 初始化session的选项
    OrtSessionOptions *session_options;
    CheckStatus(g_ort, g_ort->CreateSessionOptions(&session_options));
    CheckStatus(g_ort, g_ort->SetIntraOpNumThreads(session_options, 1));
    CheckStatus(g_ort, g_ort->SetSessionGraphOptimizationLevel(session_options, ORT_ENABLE_BASIC));

    // CheckStatus(g_ort, g_ort->SessionOptionsAppendExecutionProvider(session_options, "QNN", options_keys.data(),
    //                                                                 options_values.data(), options_keys.size()));
    OrtSession *session;
    CheckStatus(g_ort, g_ort->CreateSession(env, model_path.c_str(), session_options, &session)); // 使用模型初始化

    OrtAllocator *allocator;
    CheckStatus(g_ort, g_ort->GetAllocatorWithDefaultOptions(&allocator));
    size_t num_input_nodes;
    CheckStatus(g_ort, g_ort->SessionGetInputCount(session, &num_input_nodes));

    // --------------------------------
    // 输入节点
    // --------------------------------
    // std::vector<const char *> input_node_names;         // 模型输入节点名
    // std::vector<std::vector<int64_t>> input_node_dims;  // 模型输入节点维度
    // std::vector<ONNXTensorElementDataType> input_types; // 模型输入节点数据类型
    // std::vector<OrtValue *> input_tensors;

    this->context.input_node_names.resize(num_input_nodes);
    this->context.input_node_dims.resize(num_input_nodes);
    this->context.input_types.resize(num_input_nodes);
    this->context.input_tensors.resize(num_input_nodes);

    for (size_t i = 0; i < num_input_nodes; i++)
    {
        // Get input node names
        char *input_name;
        CheckStatus(g_ort, g_ort->SessionGetInputName(session, i, allocator, &input_name));
#ifdef DEBUG
        printf("Input %ld : name=%s\n", i, input_name);
#endif

        this->context.input_node_names[i] = input_name;

        // Get input node types
        OrtTypeInfo *typeinfo;
        CheckStatus(g_ort, g_ort->SessionGetInputTypeInfo(session, i, &typeinfo));
        const OrtTensorTypeAndShapeInfo *tensor_info;
        CheckStatus(g_ort, g_ort->CastTypeInfoToTensorInfo(typeinfo, &tensor_info));
        ONNXTensorElementDataType type;
        CheckStatus(g_ort, g_ort->GetTensorElementType(tensor_info, &type));
        this->context.input_types[i] = type;

        // Get input shapes/dims
        size_t num_dims;
        CheckStatus(g_ort, g_ort->GetDimensionsCount(tensor_info, &num_dims));
        this->context.input_node_dims[i].resize(num_dims);
        CheckStatus(g_ort, g_ort->GetDimensions(tensor_info, this->context.input_node_dims[i].data(), num_dims));

        size_t tensor_size;
        CheckStatus(g_ort, g_ort->GetTensorShapeElementCount(tensor_info, &tensor_size));

        if (typeinfo)
            g_ort->ReleaseTypeInfo(typeinfo);
    }

    // size_t num_output_nodes;
    // std::vector<const char *> output_node_names;
    // std::vector<std::vector<int64_t>> output_node_dims;
    // std::vector<OrtValue *> output_tensors;
    CheckStatus(g_ort, g_ort->SessionGetOutputCount(session, &this->context.num_output_nodes));
    this->context.output_node_names.resize(this->context.num_output_nodes);
    this->context.output_node_dims.resize(this->context.num_output_nodes);
    this->context.output_tensors.resize(this->context.num_output_nodes);

    for (size_t i = 0; i < this->context.num_output_nodes; i++)
    {
        // Get output node names
        char *output_name;
        CheckStatus(g_ort, g_ort->SessionGetOutputName(session, i, allocator, &output_name));
        this->context.output_node_names[i] = output_name;
#ifdef DEBUG
        printf("Output %ld : name=%s\n", i, output_name);
#endif

        OrtTypeInfo *typeinfo;
        CheckStatus(g_ort, g_ort->SessionGetOutputTypeInfo(session, i, &typeinfo));
        const OrtTensorTypeAndShapeInfo *tensor_info;
        CheckStatus(g_ort, g_ort->CastTypeInfoToTensorInfo(typeinfo, &tensor_info));

        // Get output shapes/dims
        size_t num_dims;
        CheckStatus(g_ort, g_ort->GetDimensionsCount(tensor_info, &num_dims));
        this->context.output_node_dims[i].resize(num_dims);
        CheckStatus(g_ort, g_ort->GetDimensions(tensor_info, (int64_t *)this->context.output_node_dims[i].data(), num_dims));

        size_t tensor_size;
        CheckStatus(g_ort, g_ort->GetTensorShapeElementCount(tensor_info, &tensor_size));

        if (typeinfo)
            g_ort->ReleaseTypeInfo(typeinfo);
    }
}

cv::Mat ObjectDetectorONNXInfer::infer_model(const cv::Mat &frame)
{
    OrtMemoryInfo *memory_info;
    size_t input_data_size = 1 * 3 * frame.cols * frame.rows;
    std::vector<float> input_data(input_data_size, 1.0);
    std::vector<uint8_t> quantized_input_data(input_data_size * sizeof(uint8_t), 1);
    size_t input_data_length = input_data_size * sizeof(uint8_t);

    CheckStatus(g_ort, g_ort->CreateCpuMemoryInfo(OrtArenaAllocator, OrtMemTypeDefault, &memory_info));
    CheckStatus(g_ort, g_ort->CreateTensorWithDataAsOrtValue(
                           memory_info, reinterpret_cast<void *>(input_data.data()),
                           input_data_length,
                           this->context.input_node_dims[0].data(),
                           this->context.input_node_dims[0].size(),
                           this->context.input_types[0],
                           &this->context.input_tensors[0]));

    g_ort->ReleaseMemoryInfo(memory_info);

    // CheckStatus(g_ort, g_ort->Run(this->context.session, nullptr,
    //                               this->context.input_node_names.data(),
    //                               (const OrtValue *const *)this->context.input_tensors.data(),
    //                               this->context.input_tensors.size(),
    //                               this->context.output_node_names.data(),
    //                               this->context.output_node_names.size(),
    //                               this->context.output_tensors.data()));
    // size_t output_data_size = 1 * 1000;
    // std::vector<float> output_data(output_data_size);
    // void *output_buffer;
    // CheckStatus(g_ort, g_ort->GetTensorMutableData(this->context.output_tensors[0], &output_buffer));
    // float *float_buffer = nullptr;
    // float_buffer = reinterpret_cast<float *>(output_buffer);

    return frame;
}