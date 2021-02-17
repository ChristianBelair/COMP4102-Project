#include <math.h>
#include <iostream>
#include <string>
#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "./EyeTracking.hpp"

void ass::EyeTracking::VideoTest() {
    VideoTest(0);
}

void ass::EyeTracking::VideoTest(int camera) {
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