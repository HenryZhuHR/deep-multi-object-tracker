#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "onnxruntime_cxx_api.h"

#include "./detector.h"
// https://onnxruntime.ai/docs/tutorials/api-basics


class ObjectDetectorOpenCV : public ObjectDetector
{

private:
    /* data */
    cv::dnn::Net model;
    cv::Mat infer_model(const cv::Mat &frame);

public:
    ObjectDetectorOpenCV(const std::string &model_path, std::vector<std::string> class_indices);
    ~ObjectDetectorOpenCV();
};
