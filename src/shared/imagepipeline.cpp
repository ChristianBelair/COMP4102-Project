#include "imagepipeline.h"

namespace ass {
    ImagePipeline::ImagePipeline()
    {

    }

    QImage ImagePipeline::ProcessFrame(QImage &src) {
        // Convert QImage to OpenCV Mat
        cv::Mat mat = QtOcv::image2Mat_shared(src);
        cv::Mat outMat;

        // Do something with the mat, like make it grayscale
        cv::cvtColor(mat, mat, cv::COLOR_BGR2GRAY);
        outMat.create(mat.size(), mat.type());
        cv::Canny(mat, outMat, 50, 200);
        cv::cvtColor(outMat, outMat, cv::COLOR_GRAY2BGRA);

        cv::putText(outMat,
                    "Automotive Safety Suite",
                    cv::Point(20,20), // Coordinates
                    cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
                    1.0, // Scale. 2.0 = 2x bigger
                    cv::Scalar(255,255,255));

        // Convert outmat to QImage and return
        QImage out = QtOcv::mat2Image(outMat);
        return out;
    }
}
