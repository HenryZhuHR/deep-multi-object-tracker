#include <iostream>
#include <filesystem>
#include <fstream>
#include <gflags/gflags.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include "detector.h"

DEFINE_string(weight_file, "../resource/yolov5s.onnx", "weight_file");
DEFINE_string(label_file, "../resource/coco_80_labels_list.txt", "");
DEFINE_string(video_source, "../resource/palace.mp4", "video_source");

int main(int argc, char *argv[])
{
    // 解析命令行参数
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::ShutDownCommandLineFlags();
    std::cout << "weight_file: " << FLAGS_weight_file << std::endl;

    // 读取标签文件
    std::vector<std::string> class_indices;
    std::ifstream label_file(FLAGS_label_file);
    if (label_file.is_open())
    {
        std::string line;
        while (std::getline(label_file, line))
        {
            class_indices.push_back(line);
        }
        label_file.close();
        {
            std::cout << "[class]: ";
            for (auto &item : class_indices)
            {
                std::cout << item << ", ";
            }
            std::cout << std::endl;
        }
    }
    else
    {
        std::cout << "label file open failed!" << std::endl;
        return -1;
    }

    cv::VideoCapture video_cap(FLAGS_video_source);
    if (!video_cap.isOpened())
    {
        std::cout << "video open failed!" << std::endl;
        return -1;
    }

    ObjectDetectorOpenCV detector=ObjectDetectorOpenCV(std::string(FLAGS_weight_file), class_indices);

    std::string tmp_dir = "../tmp";
    std::filesystem::create_directories(tmp_dir);
    int num_frames = 0;
    while (true)
    {
        cv::Mat frame;

        if (!video_cap.read(frame)) // if not success, break loop
        {
            std::cout << "\n Cannot read the video file. please check your video.\n";
            break;
        }
        num_frames++;
        detector.infer(frame);

        cv::imwrite(tmp_dir + "/frame.jpg", frame);

        break; // 测试用
    }

    return 0;
}
