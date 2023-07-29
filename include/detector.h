#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

class DetectResult
{
public:
    int classId;
    float confidence;
    cv::Rect_<float> box;
};

class ObjectDetectorOpenCV
{

private:
    /* data */

    cv::dnn::Net model;
    const float confidence_threshold_ = 0.25f;
    const float nms_threshold_ = 0.4f;

    const int model_input_width_ = 640;
    const int model_input_height_ = 640;

public:
    ObjectDetectorOpenCV(const std::string &model_path, std::vector<std::string> class_indices);
    ~ObjectDetectorOpenCV();
    std::vector<DetectResult> infer(cv::Mat &frame);
};
