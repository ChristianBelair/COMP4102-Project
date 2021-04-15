#include "imagepipeline.h"
#include "EyeTracking/EyeTracking.h"

namespace ass {
    ImagePipeline::ImagePipeline()
    {

    }

    QImage ImagePipeline::ProcessFrame(QImage &src, bool viewFlag) {
        std::cout << "Pipeline tick " << std::endl;
        // Convert QImage to OpenCV Mat
        cv::Mat mat = QtOcv::image2Mat_shared(src);
        cv::Mat outMat;

        // Do something with the mat, like make it grayscale
        cv::cvtColor(mat, mat, cv::COLOR_BGR2GRAY);
        outMat.create(mat.size(), mat.type());
        mat.copyTo(outMat);
        cv::cvtColor(outMat, outMat, cv::COLOR_GRAY2BGRA);
        cv::resize(outMat, outMat, cv::Size(800,600));

        if (viewFlag) {
            // Perform Eye tracking
            outMat = eyeTracker.EyeTrackingPipeline(outMat);
        }
        else {
            outMat = pedTracker.PedTrackingPipeline(outMat);
        }
        
        cv::resize(outMat, outMat, mat.size());

        cv::putText(outMat,
                    "Automotive Safety Suite",
                    cv::Point(20,20), // Coordinates
                    cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
                    1.0, // Scale. 2.0 = 2x bigger
                    cv::Scalar(255,255,255));

        // Log output details
        std::string matSize = "Res: ";
        matSize.append(std::to_string(outMat.cols));
        matSize.append("x");
        matSize.append(std::to_string(outMat.rows));
        cv::putText(outMat, matSize, cv::Point(20,40), cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(0,64,255));

        // Convert outmat to QImage and return
        QImage out = QtOcv::mat2Image(outMat);
        return out;
    }
}
