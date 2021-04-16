#include "imagepipeline.h"
#include "EyeTracking/EyeTracking.h"

namespace ass {
    ImagePipeline::ImagePipeline()
    {
    }

    QImage ImagePipeline::ProcessFrame(QImage &src, bool viewFlag) {
        // Convert QImage to OpenCV Mat
        cv::Mat mat = QtOcv::image2Mat_shared(src);
        cv::Mat outMat;

        if (viewFlag) {
            //std::cout << "EyeTrackingPipeline tick " << std::endl;
            // Do something with the mat, like make it grayscale
            cv::cvtColor(mat, mat, cv::COLOR_BGR2GRAY);
            outMat.create(mat.size(), mat.type());
            mat.copyTo(outMat);
            cv::cvtColor(outMat, outMat, cv::COLOR_GRAY2BGRA);
            cv::resize(outMat, outMat, cv::Size(800,600));

            // Perform Eye tracking
            EyeTrackingResult res = eyeTracker.EyeTrackingPipeline(outMat);
            outMat = res.result;
            if (res.eyeState != 99) {
                sharedState.SetEyeTrackingState(res.eyeState);
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

            // Log eyeState
            cv::putText(outMat, sharedState.GetEyeTrackingStateString(), cv::Point(20, 80), cv::FONT_HERSHEY_PLAIN, 2.0, cv::Scalar(0,64,255));
        }
        else {
            //std::cout << "PedTrackingPipeline tick " << std::endl;
            cv::cvtColor(mat, mat, cv::COLOR_BGRA2GRAY);
            outMat.create(mat.size(), mat.type());
            mat.copyTo(outMat);
            cv::cvtColor(outMat, outMat, cv::COLOR_GRAY2BGRA);
            cv::resize(outMat, outMat, cv::Size(800,600));

            outMat = pedTracker.PedTrackingPipeline(outMat);

            cv::resize(outMat, outMat, mat.size());
        }

        // Convert outmat to QImage and return
        QImage out = QtOcv::mat2Image(outMat);
        return out;
    }
}
