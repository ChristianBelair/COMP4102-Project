#include "./PedTracking.hpp"

void ass::PedTracking::VideoTest() {
    VideoTest(0);
}

void ass::PedTracking::VideoTest(int camera) {
    cv::VideoCapture cap;

    if(!cap.open(camera)) {
        std::cout << "NO CAM 4 U" << std::endl;
    }

    while(true) {
        cv::Mat frame;
        cap >> frame;
        cv::imshow("u", frame);
        if( cv::waitKey(10) == 27 ) break;
    }
}

void ass::PedTracking::VideoTest(std::string vFile) {
    cv::VideoCapture cap;

    if(!cap.open(vFile)) {
        std::cout << "NO CAM 4 U" << std::endl;
    }

    while(true) {
        cv::Mat frame;
        cap >> frame;
        cv::imshow("u", frame);
        if( cv::waitKey(10) == 27 ) break;
    }
}