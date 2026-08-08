#include <opencv2/opencv.h>
#include <raspicam/raspicam_cv.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wiringPi.h>

// Image Processing variables
IplImage *frame, *Matrix, *framePers, *frameGray, *frameThresh, *frameEdge, *frameFinal, *frameFinalDuplicate, *frameFinalDuplicate1;
IplImage *ROILane, *ROILaneEnd;
int LeftLanePos, RightLanePos, frameCenter, laneCenter, Result, laneEnd;

RaspiCam_Cv Camera;

char ss[100];

int *histrogramLane;
int *histrogramLaneEnd;

CvPoint2D32f Source[] = {{40,135}, {360,135}, {0,185}, {400,185}};
CvPoint2D32f Destination[] = {{100,0}, {280,0}, {100,240}, {280,240}};

// Machine Learning variables
CvHaarClassifierCascade *Stop_Cascade, *Object_Cascade, *Traffic_Cascade;
IplImage *frame_Stop, *RoI_Stop, *gray_Stop, *frame_Object, *RoI_Object, *gray_Object, *frame_Traffic, *RoI_Traffic, *gray_Traffic;
CvRect *Stop, *Object, *Traffic;
int dist_Stop, dist_Object, dist_Traffic;

void Setup(int argc, char **argv, RaspiCam_Cv *Camera)
{
    Camera->set(CV_CAP_PROP_FRAME_WIDTH, 400);
    Camera->set(CV_CAP_PROP_FRAME_HEIGHT, 240);
    Camera->set(CV_CAP_PROP_BRIGHTNESS, 50);
    Camera->set(CV_CAP_PROP_CONTRAST, 50);
    Camera->set(CV_CAP_PROP_SATURATION, 50);
    Camera->set(CV_CAP_PROP_GAIN, 50);
    Camera->set(CV_CAP_PROP_FPS, 0);
}

void Capture()
{
    Camera.grab();
    Camera.retrieve(frame);
    cvCvtColor(frame, frame_Stop, CV_BGR2RGB);
    cvCvtColor(frame, frame_Object, CV_BGR2RGB);
    cvCvtColor(frame, frame_Traffic, CV_BGR2RGB);
    cvCvtColor(frame, frame, CV_BGR2RGB);
}

void Perspective()
{
    cvLine(frame, cvPointFrom32f(Source[0]), cvPointFrom32f(Source[1]), CV_RGB(0,0,255), 2, 8, 0);
    cvLine(frame, cvPointFrom32f(Source[1]), cvPointFrom32f(Source[3]), CV_RGB(0,0,255), 2, 8, 0);
    cvLine(frame, cvPointFrom32f(Source[3]), cvPointFrom32f(Source[2]), CV_RGB(0,0,255), 2, 8, 0);
    cvLine(frame, cvPointFrom32f(Source[2]), cvPointFrom32f(Source[0]), CV_RGB(0,0,255), 2, 8, 0);

    Matrix = cvCreateMat(3, 3, CV_32FC1);
    cvGetPerspectiveTransform(Source, Destination, Matrix);
    cvWarpPerspective(frame, framePers, Matrix, CV_INTER_LINEAR + CV_WARP_FILL_OUTLIERS, cvScalarAll(0));
}

void Threshold()
{
    cvCvtColor(framePers, frameGray, CV_RGB2GRAY);
    cvInRangeS(frameGray, cvScalar(230), cvScalar(255), frameThresh);
    cvCanny(frameGray, frameEdge, 900, 900, 3);
    cvAdd(frameThresh, frameEdge, frameFinal, NULL);
    cvCvtColor(frameFinal, frameFinal, CV_GRAY2RGB);
    cvCvtColor(frameFinal, frameFinalDuplicate, CV_RGB2BGR);
    cvCvtColor(frameFinal, frameFinalDuplicate1, CV_RGB2BGR);
}

void Histrogram()
{
    histrogramLane = (int*)malloc(400 * sizeof(int));
    memset(histrogramLane, 0, 400 * sizeof(int));

    for(int i=0; i<400; i++)
    {
        ROILane = cvCreateImageHeader(cvSize(1, 100), IPL_DEPTH_8U, 3);
        cvSetImageROI(frameFinalDuplicate, cvRect(i, 140, 1, 100));
        cvCopy(frameFinalDuplicate, ROILane, NULL);
        cvResetImageROI(frameFinalDuplicate);
        cvDivS(ROILane, cvScalar(255), ROILane, 1);
        histrogramLane[i] = cvSum(ROILane).val[0];
    }

    histrogramLaneEnd = (int*)malloc(400 * sizeof(int));
    memset(histrogramLaneEnd, 0, 400 * sizeof(int));

    for (int i = 0; i < 400; i++)
    {
        ROILaneEnd = cvCreateImageHeader(cvSize(1, 240), IPL_DEPTH_8U, 3);
        cvSetImageROI(frameFinalDuplicate1, cvRect(i, 0, 1, 240));
        cvCopy(frameFinalDuplicate1, ROILaneEnd, NULL);
        cvResetImageROI(frameFinalDuplicate1);
        cvDivS(ROILaneEnd, cvScalar(255), ROILaneEnd, 1);
        histrogramLaneEnd[i] = cvSum(ROILaneEnd).val[0];
    }

    laneEnd = cvSum(cvMat(1, 400, CV_32SC1, histrogramLaneEnd)).val[0];
    printf("Lane END = %d\n", laneEnd);
}

void LaneFinder()
{
    LeftLanePos = 0;
    for(int i = 0; i < 150; i++)
    {
        if(histrogramLane[i] > histrogramLane[LeftLanePos])
            LeftLanePos = i;
    }

    RightLanePos = 250;
    for(int i = 250; i < 400; i++)
    {
        if(histrogramLane[i] > histrogramLane[RightLanePos])
            RightLanePos = i;
    }

    cvLine(frameFinal, cvPoint(LeftLanePos, 0), cvPoint(LeftLanePos, 240), CV_RGB(0,255,0), 2, 8, 0);
    cvLine(frameFinal, cvPoint(RightLanePos, 0), cvPoint(RightLanePos, 240), CV_RGB(0,255,0), 2, 8, 0);
}

void LaneCenter()
{
    laneCenter = (RightLanePos-LeftLanePos)/2 + LeftLanePos;
    frameCenter = 188;

    cvLine(frameFinal, cvPoint(laneCenter, 0), cvPoint(laneCenter, 240), CV_RGB(0,255,0), 3, 8, 0);
    cvLine(frameFinal, cvPoint(frameCenter, 0), cvPoint(frameCenter, 240), CV_RGB(255,0,0), 3, 8, 0);

    Result = laneCenter-frameCenter;
}

