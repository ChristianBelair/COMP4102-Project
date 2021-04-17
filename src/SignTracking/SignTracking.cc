#include "SignTracking.h"


ass::SignTracker::SignTracker() {
    if(exists("svm_data.xml")) {
        // Loads in the sign detecting svm
        // For a proper explination, see:
        // https://docs.opencv.org/master/d0/df8/samples_2cpp_2train_HOG_8cpp-example.html#a55
        cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
        svm = cv::Algorithm::load<cv::ml::SVM>("svm_data.xml");
        cv::Mat sv = svm->getSupportVectors();
        const int sv_total = sv.rows;
        cv::Mat alpha, svidx;
        double rho = svm->getDecisionFunction(0, alpha, svidx);

        std::vector<float> hog_detector(sv.cols + 1);
        memcpy(&hog_detector[0], sv.ptr(), sv.cols * sizeof(hog_detector[0]));
        hog_detector[sv.cols] = (float)-rho;

        cv::HOGDescriptor hog;
        hog.winSize = cv::Size(64,64);
        hog.setSVMDetector(hog_detector);
        hog.save("sign_detector.yml");
    }
    else {
        // Do some sort of warning logging
        return;
    }
}

void ass::SignTracker::trackSigns(int c) {

}

cv::Mat ass::SignTracker::trackSigns(const cv::Mat frame) {
    cv::HOGDescriptor hog;
    hog.load("sign_detector.yml");

    cv::Mat img = frame.clone();
    std::vector<cv::Rect> found;
    std::vector<double> weights;

    hog.detectMultiScale(img, found, weights, 0.0, cv::Size(8,8));

    for(size_t i = 0; i < found.size(); ++i) {
        if(weights[i] > .7) {
            cv::Rect r = found[i];
            cv::rectangle(img, r, cv::Scalar(255, 0, 0), 3);
            std::ostringstream buf;
            buf << weights[i];
            cv::putText(img, buf.str(), cv::Point(found[i].x, found[i].y + 50), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 0, 0));
        }
    }
    return img;
}

// This functions locks us fully into POSIX system
// Just checks if a file exists, stat is the fastest method
bool ass::SignTracker::exists(const std::string name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0); 
}

int main() {
    ass::SignTracker tracker;
    cv::Mat img = cv::imread("stop.png", cv::IMREAD_COLOR);
    cv::Mat copy;
    std::cout << "Searching for signs" << std::endl;
    copy = tracker.trackSigns(img);
    cv::imwrite("test.png", copy);
}