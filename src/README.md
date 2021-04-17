# Automotive Safety Suite
## Description
The Automotive Safety Suite (ASS) is a collection of safety features which try to solve distracted driving problems using computer vision. The ASS has three main components: Gaze Tracking, Pedestrian Tracking, and Road Sign Tracking. Gaze tracking determines where eyes exist in the driver's camera view and derives gaze in order to determine where it is the driver is looking. Pedestrian Tracking involves trying to identify pedestrains in the application's road view. This component is meant to identify where within the view pedestrians are if they exist. The last component is Road Sign Tracking where using the same road view as Pedestrian Tracking it detects and identifies where within the video frame road signs are if they exist. The main goal is to have a suite of features that work together to avoid distracted driving by notifying the driver whenever their gaze is not within an acceptable range of oncoming road signs or pedestrians.

## Dependencies
* Current version of the project is running using OpenCV 4.5.1
* Uses the eyeLike repository for creating bounding boxes for the EyeTracking component

## Build Instructions
1. Clone the repository
2. Run "git submodule update --init" to import the eyeLike submodule used in EyeTracking
3. Run "cmake ." in the "src/" directory to create the Makefile
4. Run "make" in the "src/" directory to compile the executable

## Program Instructions
1. Run "./AutomotiveSafetySuite" in the "src/" directory to run the executable
  * **NOTE** In the event an internal error occurs after launching the application you can click the stop button in the lower right and hit play again to load the default camera again
2. Click on the "Open File" button in the bottom left
3. Select a video file from your computer
4. While the driver view (Top video feed of default camera device) is running, hit the play button next to the "Open File" button.
5. The video should start playing in greyscale with a text label saying either "Paying Attention" or "Distracted"
6. Watch the video and if any pedestrians of road signs are identified then look at them
  1. Look at the left, center, or right regions of the video where there are the most boxes to be considered "Paying Attention"
  2. Look away from left, center, or right regions of the video where there are the most boxes to be considered "Distracted"