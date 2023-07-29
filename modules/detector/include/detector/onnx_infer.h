#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "onnxruntime_cxx_api.h"

#include "./detector.h"
// https://onnxruntime.ai/docs/tutorials/api-basics

struct onnx_infer_context
{
    /* data */
    OrtSession *session;
    std::vector<const char *> input_node_names;         // 模型输入节点名
    std::vector<std::vector<int64_t>> input_node_dims;  // 模型输入节点维度
    std::vector<ONNXTensorElementDataType> input_types; // 模型输入节点数据类型
    std::vector<OrtValue *> input_tensors;
    size_t num_output_nodes;
    std::vector<const char *> output_node_names;
    std::vector<std::vector<int64_t>> output_node_dims;
    std::vector<OrtValue *> output_tensors;
};

class ObjectDetectorONNXInfer : public ObjectDetector
{
private:
    const OrtApi *g_ort = OrtGetApiBase()->GetApi(ORT_API_VERSION);
    onnx_infer_context context;

private:
    bool init(std::string model_path);
    cv::Mat infer_model(const cv::Mat &frame);

public:
    ObjectDetectorONNXInfer(std::string model_path, std::vector<std::string> class_indices);
    ~ObjectDetectorONNXInfer();
};
