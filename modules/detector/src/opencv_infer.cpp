#include "detector/opencv_infer.h"

ObjectDetectorOpenCV::ObjectDetectorOpenCV(const std::string &model_path, std::vector<std::string> class_indices)
{
    this->model = cv::dnn::readNetFromONNX(model_path);
}

ObjectDetectorOpenCV::~ObjectDetectorOpenCV()
{
}

cv::Mat ObjectDetectorOpenCV::infer_model(const cv::Mat &frame)
{
    int w = frame.cols;
    int h = frame.rows;
    // int _max = std::max(h, w);
    // cv::Mat image = cv::Mat::zeros(cv::Size(_max, _max), CV_8UC3);
    // cv::Rect roi(0, 0, w, h);
    // frame.copyTo(image(roi));

    // float x_factor = static_cast<float>(image.cols) / w;
    // float y_factor = static_cast<float>(image.rows) / h;
    printf("img_size %d %d\n", w, h);

    cv::Mat blob = cv::dnn::blobFromImage(frame, 1 / 255.0, cv::Size(w, h), cv::Scalar(0, 0, 0), true, false);

    this->model.setInput(blob);
    cv::Mat preds = this->model.forward("output"); // outputname

    return frame;
}