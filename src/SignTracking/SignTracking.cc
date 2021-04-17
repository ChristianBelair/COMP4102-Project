#include "SignTracking.h"

cv::Mat ass::SignTracker::trackSigns(const cv::Mat frame) {
    cv::Mat img = frame.clone();
    cv::Mat res = frame.clone();
    cv::cvtColor(img, img, cv::COLOR_BGR2HSV);
    
    // Searches the red space
    cv::Mat mask1, mask2, maskRed;
    cv::Mat canny;
    cv::inRange(img, cv::Scalar(0, 70, 60), cv::Scalar(10, 255, 255), mask1);
    cv::inRange(img, cv::Scalar(170, 70, 60), cv::Scalar(180, 255, 255), mask2);
    cv::bitwise_or(mask1, mask2, maskRed);

    cv::Canny(maskRed, canny, 100, 255);

    cv::Mat kernel = cv::Mat::ones(cv::Size(5,5), CV_8U);
    cv::dilate(canny, canny, kernel);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::findContours(canny, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

    for(auto iter = contours.begin(); iter != contours.end(); ++iter) {
        if (cv::contourArea(*iter) > 500) {
            double peri = cv::arcLength(*iter, true);
            std::vector<cv::Point> approx;
            cv::approxPolyDP(*iter, approx, .03 * peri, true);
            if (approx.size() == 8 || approx.size() == 3) {
                std::cout << approx.size() << std::endl;
                cv::Rect bounds = cv::boundingRect(approx);
                cv::rectangle(res, bounds, cv::Scalar(255,0,0), 3);
            }
        }
    }

    // Searches the yellow space
    cv::Mat maskYel;
    cv::inRange(img, cv::Scalar(20, 100, 100), cv::Scalar(30, 255, 255), maskYel);
    
    cv::Canny(maskRed, canny, 100, 255);

    cv::dilate(canny, canny, kernel);

    cv::findContours(canny, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
    for(auto iter = contours.begin(); iter != contours.end(); ++iter) {
        if (cv::contourArea(*iter) > 500) {
            double peri = cv::arcLength(*iter, true);
            std::vector<cv::Point> approx;
            cv::approxPolyDP(*iter, approx, .03 * peri, true);
            if (approx.size() == 4 || approx.size() == 5) {
                std::cout << approx.size() << std::endl;
                cv::Rect bounds = cv::boundingRect(approx);
                cv::rectangle(res, bounds, cv::Scalar(255,0,0), 3);
            }
        }
    }
    return res;
}

ass::SignTrackingResult ass::SignTracker::SignTrackingPipeline(cv::Mat frame) {
    cv::Mat img = frame.clone();
    cv::Mat response = frame.clone();
    std::vector<cv::Rect> found;

    cv::cvtColor(img, img, cv::COLOR_BGR2HSV);
    
    // Searches the red space
    cv::Mat mask1, mask2, maskRed;
    cv::Mat canny;
    cv::inRange(img, cv::Scalar(0, 70, 60), cv::Scalar(10, 255, 255), mask1);
    cv::inRange(img, cv::Scalar(170, 70, 60), cv::Scalar(180, 255, 255), mask2);
    cv::bitwise_or(mask1, mask2, maskRed);

    cv::Canny(maskRed, canny, 100, 255);

    cv::Mat kernel = cv::Mat::ones(cv::Size(5,5), CV_8U);
    cv::dilate(canny, canny, kernel);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::findContours(canny, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

    for(auto iter = contours.begin(); iter != contours.end(); ++iter) {
        if (cv::contourArea(*iter) > 500) {
            double peri = cv::arcLength(*iter, true);
            std::vector<cv::Point> approx;
            cv::approxPolyDP(*iter, approx, .03 * peri, true);
            if (approx.size() == 8 || approx.size() == 3) {
                std::cout << approx.size() << std::endl;
                cv::Rect bounds = cv::boundingRect(approx);
                found.push_back(bounds);
                cv::rectangle(response, bounds, cv::Scalar(255,0,0), 3);
            }
        }
    }

    // Searches the yellow space
    cv::Mat maskYel;
    cv::inRange(img, cv::Scalar(20, 100, 100), cv::Scalar(30, 255, 255), maskYel);
    
    cv::Canny(maskYel, canny, 100, 255);

    cv::dilate(canny, canny, kernel);

    cv::findContours(canny, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
    for(auto iter = contours.begin(); iter != contours.end(); ++iter) {
        if (cv::contourArea(*iter) > 250) {
            double peri = cv::arcLength(*iter, true);
            std::vector<cv::Point> approx;
            cv::approxPolyDP(*iter, approx, .03 * peri, true);
            if (approx.size() == 4 || approx.size() == 5) {
                std::cout << approx.size() << std::endl;
                cv::Rect bounds = cv::boundingRect(approx);
                found.push_back(bounds);
                cv::rectangle(response, bounds, cv::Scalar(255,0,0), 3);
            }
        }
    }

    cv::Mat maskWhi;
    cv::inRange(img, cv::Scalar(0, 0, 255), cv::Scalar(255, 255, 255), maskYel);
    
    cv::Canny(maskWhi, canny, 100, 255);

    cv::dilate(canny, canny, kernel);

    cv::findContours(canny, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
    for(auto iter = contours.begin(); iter != contours.end(); ++iter) {
        if (cv::contourArea(*iter) > 250) {
            double peri = cv::arcLength(*iter, true);
            std::vector<cv::Point> approx;
            cv::approxPolyDP(*iter, approx, .03 * peri, true);
            if (approx.size() == 4) {
                std::cout << approx.size() << std::endl;
                cv::Rect bounds = cv::boundingRect(approx);
                found.push_back(bounds);
                cv::rectangle(response, bounds, cv::Scalar(255,0,0), 3);
            }
        }
    }

    ass::SignTrackingResult res;
    res.result = img;
    res.signRegions = found;
    return res;
}

int main() {
    ass::SignTracker tracker;
    cv::Mat img = cv::imread("stop.png", cv::IMREAD_COLOR);
    cv::Mat copy;
    copy = tracker.trackSigns(img);
    cv::imwrite("stoptest.png", copy);

    std::cout << "test 2" << std::endl;
    img = cv::imread("yield.png", cv::IMREAD_COLOR);
    img = tracker.trackSigns(img);
    cv::imwrite("yieldTest.png", img);
}