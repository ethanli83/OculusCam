//
// Created by Ethan Li on 20/04/2014.
//


#ifndef __WebCam_H_
#define __WebCam_H_

#include <opencv2/highgui/highgui.hpp>
#include "AppHelper.h"

class WebCam {
public:
    WebCam(int camIndex) : deviceId(camIndex) {

    }

    void startCapture() {
        capture.open(deviceId);

        if (!capture.isOpened()) {
            throwError("cam not found");
        }
        width = (int)capture.get(CV_CAP_PROP_FRAME_WIDTH);
        height = (int)capture.get(CV_CAP_PROP_FRAME_HEIGHT);
    }

    int width;
    int height;
    cv::VideoCapture capture;
private:
    int deviceId;
};


#endif //__WebCam_H_
