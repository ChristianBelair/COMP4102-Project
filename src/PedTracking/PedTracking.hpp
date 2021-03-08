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
#include <opencv4/opencv2/core/utility.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/videoio.hpp>
#include <opencv4/opencv2/objdetect.hpp>

namespace ass {
    class PedTracking {
        public:
            void VideoTest();
            void VideoTest(int cameraNumber);
            void VideoTest(std::string video);
            void TrackPeds();
            void TrackPeds(int cameraNumber);
            void TrackPeds(std::string video);
    };
}

#endif // ndef PEDTRACKING_H