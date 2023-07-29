#include "detector.h"

ObjectDetectorOpenCV::ObjectDetectorOpenCV(const std::string &model_path, std::vector<std::string> class_indices)
{
    this->model = cv::dnn::readNetFromONNX(model_path);
}

ObjectDetectorOpenCV::~ObjectDetectorOpenCV()
{
}

std::vector<DetectResult> ObjectDetectorOpenCV::infer(cv::Mat &frame)
{
    cv::resize(frame, frame, cv::Size(model_input_width_, model_input_height_));
    int w = frame.cols;
    int h = frame.rows;
    int _max = std::max(h, w);
    cv::Mat image = cv::Mat::zeros(cv::Size(_max, _max), CV_8UC3);
    cv::Rect roi(0, 0, w, h);
    frame.copyTo(image(roi));

    float x_factor = static_cast<float>(image.cols) / w;
    float y_factor = static_cast<float>(image.rows) / h;


    cv::Mat blob = cv::dnn::blobFromImage(frame, 1 / 255.0, cv::Size(w, h), cv::Scalar(0, 0, 0), true, false);

    this->model.setInput(blob);

    cv::Mat preds = this->model.forward("output"); // outputname

    cv::Mat det_output(preds.size[1], preds.size[2], CV_32F, preds.ptr<float>());

    std::vector<cv::Rect> boxes;
    std::vector<int> classIds;
    std::vector<float> confidences;
    for (int i = 0; i < det_output.rows; i++)
    {
        float box_conf = det_output.at<float>(i, 4);
        if (box_conf < nms_threshold_)
        {
            continue;
        }

        cv::Mat classes_confidences = det_output.row(i).colRange(5, 85);
        cv::Point classIdPoint;
        double cls_conf;
        cv::minMaxLoc(classes_confidences, 0, &cls_conf, 0, &classIdPoint);

        if (cls_conf > confidence_threshold_)
        {
            float cx = det_output.at<float>(i, 0);
            float cy = det_output.at<float>(i, 1);
            float ow = det_output.at<float>(i, 2);
            float oh = det_output.at<float>(i, 3);
            int x = static_cast<int>((cx - 0.5 * ow) * x_factor);
            int y = static_cast<int>((cy - 0.5 * oh) * y_factor);
            int width = static_cast<int>(ow * x_factor);
            int height = static_cast<int>(oh * y_factor);
            cv::Rect box;
            box.x = x;
            box.y = y;
            box.width = width;
            box.height = height;

            boxes.push_back(box);
            classIds.push_back(classIdPoint.x);
            confidences.push_back(cls_conf * box_conf);
        }
    }

    std::vector<int> indexes;
    cv::dnn::NMSBoxes(boxes, confidences, confidence_threshold_, nms_threshold_, indexes);
    std::vector<DetectResult> results;
    for (size_t i = 0; i < indexes.size(); i++)
    {
        DetectResult dr;
        int index = indexes[i];
        int idx = classIds[index];
        dr.box = boxes[index];
        dr.classId = idx;
        dr.confidence = confidences[index];
        results.push_back(dr);
    }

    return results;
}