#include "detector/detector.h"

ObjectDetector::ObjectDetector(/* args */)
{
}

ObjectDetector::~ObjectDetector()
{
}

int ObjectDetector::infer(const cv::Mat &frame)
{
    // cv::Mat frame_resized;
    // cv::resize(frame, frame_resized, this->input_size);
    // this->infer_model(frame_resized);
    return 1;
}