#ifndef PEDTRACKING_H
#define PEDTRACKING_H

#include <math.h>
#include <iostream>
#include <string>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>

const float HIT_THRESHOLD = 0.6;

namespace ass {
    class PedTracking {
        public:
            PedTracking();
            void VideoTest();
            void VideoTest(int cameraNumber);
            void VideoTest(std::string video);
            void TrackPeds();
            void TrackPeds(int cameraNumber);
            cv::Mat PedTrackingPipeline(cv::Mat frame);
    };
}

#endif // ndef PEDTRACKING_H