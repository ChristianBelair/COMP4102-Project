#ifndef PEDTRACKING_H
#define PEDTRACKING_H

#include <math.h>
#include <iostream>
#include <string>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

namespace ass {
    class PedTracking {
        public:
            void VideoTest();
            void VideoTest(int cameraNumber);
            void VideoTest(std::string vFile);
    };
}

#endif // ndef PEDTRACKING_H