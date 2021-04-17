#include "./PedTracking.h"

// Constructor
ass::PedTracking::PedTracking() {

}

// Function that displays the camera feed of default camera
void ass::PedTracking::VideoTest() {
    VideoTest(0);
}

// Function that displays the camera feed of a provided camera
void ass::PedTracking::VideoTest(int camera) {
    cv::VideoCapture cap;

    // Checks if the camera was opened
    if(!cap.open(camera)) {
        std::cout << "Error: Could not open camera" << std::endl;
    }

    // Sets up display loop for camera feed
    while(true) {
        // Takes in a frame from the camera feed and displays it
        cv::Mat frame;
        cap >> frame;
        PedTrackingResult res = PedTrackingPipeline(frame);
        cv::imshow("u", res.result);

        // Exits the loop when user quits
        if( cv::waitKey(1) == 27 || cv::waitKey(1) == 'q' ) break;
    }
}

// Function used to display the contents of a video file
void ass::PedTracking::VideoTest(std::string video) {
    cv::VideoCapture cap;

    // Checks to see if the video could be opened
    if(!cap.open(video)) {
        std::cout << "Error: Could not open video file" << std::endl;
    }

    // Sets up display loop for video feed
    while(true) {
        // Takes in a frame from the video feed and displays it
        cv::Mat frame;
        cap >> frame;
        cv::imshow("u", frame);

        // Exists the loop when a user quits
        if( cv::waitKey(10) == 27 || cv::waitKey(1) == 'q' ) break;
    }
}

// Function that uses the HOG descriptor to identify human objects in a frame
ass::PedTrackingResult ass::PedTracking::PedTrackingPipeline(cv::Mat frame) {
    // Initialize HOG descriptor
    cv::HOGDescriptor hog;
    hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());

    // Clones the source frame
    cv::Mat img = frame.clone();

        // Initialize vectors for found human objects and they associated weights
        std::vector<cv::Rect> found;
        std::vector<double> weights;

        // Converts from BGRA (CV_8UC4) to greyscale (CV_8U)
        // This conversion process is needed for detectMultiScale()
        cv::cvtColor(img, img, cv::COLOR_BGRA2GRAY);

        // Applies HOG descriptor to detect human objects
        hog.detectMultiScale(img, found, weights, 0.0, cv::Size(16, 16));

        // Iterates through all found human objects
        for (size_t i = 0; i < found.size(); ++i) {
            // Creates a cv:Rect to draw arectangle in the frame where the current object is
            cv::Rect r = found[i];
            cv::rectangle(img, r, cv::Scalar(0, 0, 255), 3);

            // Creates a label for the associated weight and add it to the frame
            std::ostringstream buf;
            buf << weights[i];
            cv::putText(img, buf.str(), cv::Point(found[i].x, found[i].y + 50), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255));
        }

        // Convert the modified frame back to its original format
        cv::cvtColor(img, img, cv::COLOR_GRAY2BGRA);

        // Returns new frame and results from HOG descriptor
        PedTrackingResult res;
        res.result = img;
        res.pedRegions = found;
        return res;
}