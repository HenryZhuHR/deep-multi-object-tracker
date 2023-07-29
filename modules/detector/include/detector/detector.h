#ifndef __DETECTOR_H__
#define __DETECTOR_H__
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
class ObjectDetector
{
private:
    /* data */
    cv::Size input_size=cv::Size(640,640);
public:
    ObjectDetector();
    ~ObjectDetector();

protected:
    /**
     * @brief 模型推理，子类必须实现
     */
    virtual cv::Mat infer_model(const cv::Mat &frame) = 0;

public:
    int infer(const cv::Mat &frame);
};
#endif // __DETECTOR_H__
