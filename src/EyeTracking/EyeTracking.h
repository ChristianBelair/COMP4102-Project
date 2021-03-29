#pragma once
#include "opencv2/opencv.hpp"

namespace ass {
    class EyeTracking {
        public:
            EyeTracking();
            cv::Mat EyeTrackingPipeline(cv:: Mat &in);

        private:
            cv::Mat findEyes(cv::Mat img, cv::Rect face);

            cv::CascadeClassifier classifier;

            const cv::String cascadeFile = "res/haarcascade_frontalface_alt.xml";
    };
}
