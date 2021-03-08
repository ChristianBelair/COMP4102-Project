#include "./PedTracking.hpp"

void ass::PedTracking::VideoTest() {
    VideoTest(0);
}

void ass::PedTracking::VideoTest(int camera) {
    cv::VideoCapture cap;

    if(!cap.open(camera)) {
        std::cout << "NO CAM 4 U" << std::endl;
    }

    while(true) {
        cv::Mat frame;
        cap >> frame;
        cv::imshow("u", frame);
        if( cv::waitKey(10) == 27 ) break;
    }
}

void ass::PedTracking::VideoTest(std::string video) {
    cv::VideoCapture cap;

    if(!cap.open(video)) {
        std::cout << "NO VID 4 U" << std::endl;
    }

    while(true) {
        cv::Mat frame;
        cap >> frame;
        cv::imshow("u", frame);
        if( cv::waitKey(10) == 27 ) break;
    }
}

void ass::PedTracking::TrackPeds() {
    TrackPeds(0);
}

void ass::PedTracking::TrackPeds(int camera) {
    cv::VideoCapture cap;
    cv::Mat frame;

    if(!cap.open(camera)) {
        std::cout << "Error: Could not open camera." << std::endl;
    }

    cv::HOGDescriptor hog;
    hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());

    std::vector<cv::Point> track;

    while(true) {
        cap >> frame;

        cv::Mat img = frame.clone();
        cv::resize(img, img, cv::Size(640, 480));

        std::vector<cv::Rect> found;
        std::vector<double> weights;

        hog.detectMultiScale(img, found, weights, 0.0, cv::Size(8, 8));

        for (size_t i = 0; i < found.size(); ++i) {
            cv::Rect r = found[i];
            cv::rectangle(img, found[i], cv::Scalar(0, 0, 255), 3);
            std::stringstream tmp;
            tmp << weights[i];
            cv::putText(img, tmp.str(), cv::Point(found[i].x, found[i].y + 50), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255));
            track.push_back(cv::Point(found[i].x + found[i].width / 2, found[i].y + found[i].height / 2));
        }

        for (size_t i = 0; i < track.size(); ++i) {
            cv::line(img, track[i + 1], track[i], cv::Scalar(255, 255, 0), 2);
        }

        cv::imshow("u", frame);
        if( cv::waitKey(10) == 27 ) break;
    }
}

void ass::PedTracking::TrackPeds(std::string video) {
    cv::VideoCapture cap;

    if(!cap.open(video)) {
        std::cout << "NO VID 4 U" << std::endl;
    }

    while(true) {
        cv::Mat frame;
        cap >> frame;
        cv::imshow("u", frame);
        if( cv::waitKey(10) == 27 ) break;
    }
}