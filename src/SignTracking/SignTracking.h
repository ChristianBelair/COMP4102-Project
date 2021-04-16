#pragma once

#include <memory>
#include <thread>
#include <iostream>
#include <vector>
#include <sys/stat.h>

#include "opencv4/opencv2/core/base.hpp"
#include "opencv4/opencv2/core.hpp"
#include "opencv4/opencv2/videoio.hpp"
#include "opencv4/opencv2/imgproc.hpp"
#include "opencv4/opencv2/imgcodecs.hpp"
#include "opencv4/opencv2/objdetect.hpp"
#include "opencv4/opencv2/ml.hpp"

namespace ass {
    class SignTracker {
        public:
            SignTracker();
            void trackSigns(int);
            cv::Mat trackSigns(const cv::Mat);
        private:
            bool exists(const std::string);
    };
}