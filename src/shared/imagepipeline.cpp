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

        // Checks to see if the view flag for the driver view was set
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
            cv::Mat temp2;
            outMat.create(mat.size(), mat.type());
            cv::resize(outMat, outMat, cv::Size(800,600));
            SignTrackingResult signRes = signTracker.SignTrackingPipeline(outMat);
            cv::cvtColor(mat, mat, cv::COLOR_BGRA2GRAY);
            temp2.create(mat.size(), mat.type());
            mat.copyTo(temp2);
            cv::cvtColor(temp2, temp2, cv::COLOR_GRAY2BGRA);
            cv::resize(temp2, temp2, cv::Size(800,600));

            PedTrackingResult res = pedTracker.PedTrackingPipeline(temp2);
            
            for(auto iter = signRes.signRegions.begin(); iter != signRes.signRegions.end(); ++iter) {
                cv::rectangle(outMat, *iter, cv::Scalar(255,0,0), 3);
            }
            for(auto iter = res.pedRegions.begin(); iter != res.pedRegions.end(); ++iter) {
                cv::rectangle(outMat, *iter, cv::Scalar(0,0,255), 3);
            }

            // Get the current eye state
            int eyeState = sharedState.GetEyeTrackingState();
            int count = 0;

            // Iterate through all found pedestrian objects
            for (int i = 0; i < res.pedRegions.size(); ++i) {
                // Calculate the center of the pedestrian object's width
                int reg_center = res.pedRegions[i].x + (res.pedRegions[i].width / 2);
                // Initialize object position as in center third of the frame
                int pedState = 0;

                // Checks if the width center is in the left or right third of the frame
                if (reg_center < (int)(outMat.cols / 3)) {
                    pedState = -1;
                }
                else if (reg_center > (int)((outMat.cols / 3) * 2)) {
                    pedState = 1;
                }

                // Check if pedestrian state matches eye state
                if (pedState == eyeState) {
                    ++count;
                }
            }

            // Initialize empty response string
            std::string response = "";

            // Checks to see if at least one third of total pedestrian objects are being looked at
            if (count >= (int)(res.pedRegions.size() / 3)) {
                response += "Paying Attention";
            }
            else {
                response += "Distracted";
            }

            // Resize frame Mat to original size
            cv::resize(outMat, outMat, mat.size());

            // Dispalys response label to frame
            cv::putText(outMat, response, cv::Point(20, 80), cv::FONT_HERSHEY_PLAIN, 2.0, cv::Scalar(0,64,255));
        }

        // Convert outmat to QImage and return
        QImage out = QtOcv::mat2Image(outMat);
        return out;
    }
}
