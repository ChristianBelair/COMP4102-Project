#ifndef ASSIMAGEPIPELINE_H
#define ASSIMAGEPIPELINE_H
#include <opencv2/opencv.hpp>
#include <QImage>
#include "cvmatandqimage.h"

namespace ass {
    class ImagePipeline
    {
    public:
        ImagePipeline();

        QImage ProcessFrame(QImage &frame);
    };
}

#endif // ASSIMAGEPIPELINE_H
