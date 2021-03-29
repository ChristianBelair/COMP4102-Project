#include "EyeTracking.h"

#include <opencv2/objdetect/objdetect.hpp>
#include <stdio.h>
// Library methods I use
#include "3rdparty/eyeLike/src/constants.h"
#include "3rdparty/eyeLike/src/findEyeCenter.h"
#include "3rdparty/eyeLike/src/findEyeCorner.h"

ass::EyeTracking::EyeTracking() {
    auto ret = classifier.load(cascadeFile);
    if (!ret) {
        std::cout << "!!! ERROR IN EYE TRACKING: classifier cascade file failed " << cascadeFile << std::endl;
    }
}

cv::Mat ass::EyeTracking::EyeTrackingPipeline(cv:: Mat &in) {
    std::vector<cv::Rect> faces;
    std::vector<cv::Mat> rgbChannels(3);
    cv::split(in, rgbChannels);
    cv::Mat gray_img = rgbChannels[2];

    classifier.detectMultiScale(gray_img, faces, 1.1, 2, 0|cv::CASCADE_SCALE_IMAGE|cv::CASCADE_FIND_BIGGEST_OBJECT, cv::Size(150, 150));

    if (faces.size() == 0) {
        // No faces found, can't perform eye tracking
        return in;
    }

    cv::Mat face = findEyes(gray_img, faces[0]);
    cv::Mat out;
    cv::cvtColor(face, out, cv::COLOR_GRAY2BGRA);

    // Resize the output to match the input
    cv::resize(out, out, in.size());
    return out;
}

cv::Mat ass::EyeTracking::findEyes(cv::Mat frame_gray, cv::Rect face) {
    cv::Mat faceROI = frame_gray(face);
    cv::Mat debugFace = faceROI;

    //-- Find eye regions and draw them
    int eye_region_width = face.width * (kEyePercentWidth/100.0);
    int eye_region_height = face.width * (kEyePercentHeight/100.0);
    int eye_region_top = face.height * (kEyePercentTop/100.0);

    cv::Rect leftEyeRegion(face.width*(kEyePercentSide/100.0),
                         eye_region_top,eye_region_width,eye_region_height);

    cv::Rect rightEyeRegion(face.width - eye_region_width - face.width*(kEyePercentSide/100.0),
                          eye_region_top,eye_region_width,eye_region_height);

    //-- Find Eye Centers
    cv::Point leftPupil = findEyeCenter(faceROI,leftEyeRegion,"Left Eye");
    cv::Point rightPupil = findEyeCenter(faceROI,rightEyeRegion,"Right Eye");

    // get corner regions
    cv::Rect leftRightCornerRegion(leftEyeRegion);

    leftRightCornerRegion.width -= leftPupil.x;
    leftRightCornerRegion.x += leftPupil.x;
    leftRightCornerRegion.height /= 2;
    leftRightCornerRegion.y += leftRightCornerRegion.height / 2;
    cv::Rect leftLeftCornerRegion(leftEyeRegion);

    leftLeftCornerRegion.width = leftPupil.x;
    leftLeftCornerRegion.height /= 2;
    leftLeftCornerRegion.y += leftLeftCornerRegion.height / 2;
    cv::Rect rightLeftCornerRegion(rightEyeRegion);

    rightLeftCornerRegion.width = rightPupil.x;
    rightLeftCornerRegion.height /= 2;
    rightLeftCornerRegion.y += rightLeftCornerRegion.height / 2;
    cv::Rect rightRightCornerRegion(rightEyeRegion);

    rightRightCornerRegion.width -= rightPupil.x;
    rightRightCornerRegion.x += rightPupil.x;
    rightRightCornerRegion.height /= 2;
    rightRightCornerRegion.y += rightRightCornerRegion.height / 2;

    rectangle(debugFace,leftRightCornerRegion,200);
    rectangle(debugFace,leftLeftCornerRegion,200);
    rectangle(debugFace,rightLeftCornerRegion,200);
    rectangle(debugFace,rightRightCornerRegion,200);

    // change eye centers to face coordinates
    rightPupil.x += rightEyeRegion.x;
    rightPupil.y += rightEyeRegion.y;
    leftPupil.x += leftEyeRegion.x;
    leftPupil.y += leftEyeRegion.y;

    // draw eye centers
    circle(debugFace, rightPupil, 3, 1234);
    circle(debugFace, leftPupil, 3, 1234);
    return faceROI;
}
