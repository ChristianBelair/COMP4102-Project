#include "./PedTracking.h"

ass::PedTracking::PedTracking() {

}

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
        if( cv::waitKey(1) == 27 ) break;
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
        frame = PedTrackingPipeline(frame);
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

    while(true) {
        cap >> frame;

        cv::Mat img = PedTrackingPipeline(frame);

        // cv::Mat img = frame.clone();

        // std::vector<cv::Rect> found;
        // std::vector<double> weights;

        // hog.detectMultiScale(img, found, weights, 0.0, cv::Size(8, 8));

        // for (size_t i = 0; i < found.size(); ++i) {
        //     cv::Rect r = found[i];
        //     cv::rectangle(img, r, cv::Scalar(0, 0, 255), 3);
        //     std::ostringstream buf;
        //     buf << weights[i];
        //     cv::putText(img, buf.str(), cv::Point(found[i].x, found[i].y + 50), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255));
        // }

        cv::imshow("u", img);
        if( cv::waitKey(1) == 27 || cv::waitKey(1) == 'q' ) break;
    }
}

cv::Mat ass::PedTracking::PedTrackingPipeline(cv::Mat frame) {
    cv::HOGDescriptor hog;
    hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());

    cv::Mat img = frame.clone();

        std::vector<cv::Rect> found;
        std::vector<double> weights;

        cv::cvtColor(img, img, cv::COLOR_BGRA2GRAY);
        std::cout << "img.type() = " << img.type() << std::endl;
        // if (img)

        hog.detectMultiScale(img, found, weights, 0.0, cv::Size(16, 16));

        for (size_t i = 0; i < found.size(); ++i) {
            cv::Rect r = found[i];
            cv::rectangle(img, r, cv::Scalar(0, 0, 255), 3);
            std::ostringstream buf;
            buf << weights[i];
            cv::putText(img, buf.str(), cv::Point(found[i].x, found[i].y + 50), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255));
        }
        cv::cvtColor(img, img, cv::COLOR_GRAY2BGRA);

        return img;
}