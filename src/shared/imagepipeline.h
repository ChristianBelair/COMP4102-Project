#ifndef ASSIMAGEPIPELINE_H
#define ASSIMAGEPIPELINE_H
#include <opencv2/opencv.hpp>
#include <QImage>
#include <string>
#include "cvmatandqimage.h"
#include "EyeTracking/EyeTracking.h"
#include "PedTracking/PedTracking.h"

namespace ass {
    class ImagePipeline
    {
    public:
        ImagePipeline();

        QImage ProcessFrame(QImage &frame, bool viewFlag = true);

    private:
        EyeTracking eyeTracker;
        PedTracking pedTracker;
    };
}

#endif // ASSIMAGEPIPELINE_H
