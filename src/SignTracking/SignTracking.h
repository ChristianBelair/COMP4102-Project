#pragma once

#include <memory>
#include <thread>
#include <iostream>
#include <vector>
#include <sys/stat.h>

#include "opencv2/core/base.hpp"
#include "opencv2/core.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/ml.hpp"

namespace ass {
    class SignTrackingResult {
        public:
            cv::Mat result;
            std::vector<cv::Rect> signRegions;
    };

    class SignTracker {
        public:
            SignTrackingResult SignTrackingPipeline(const cv::Mat frame);
    };
}
