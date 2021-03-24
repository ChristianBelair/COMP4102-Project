#ifndef ASSIMAGEPIPELINE_H
#define ASSIMAGEPIPELINE_H
#include <opencv2/opencv.hpp>
#include <QImage>
#include <string>
#include "cvmatandqimage.h"
#include "EyeTracking/EyeTracking.h"

namespace ass {
    class ImagePipeline
    {
    public:
        ImagePipeline();

        QImage ProcessFrame(QImage &frame);

    private:
        EyeTracking eyeTracker;
    };
}

#endif // ASSIMAGEPIPELINE_H
