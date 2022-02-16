#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <iostream>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

int Test(int argc, char** argv) {
    Mat src = imread("test.jpg", IMREAD_GRAYSCALE);
    if (src.empty()) {
        printf("could not load image...\n");
        return -1;
    }
    namedWindow("input image", WINDOW_AUTOSIZE);
    imshow("input image", src);

    // SIFT��������
    int minHessian = 100;
    Ptr<SIFT> detector = SIFT::create(minHessian);//��surf������ֻ��SURF��SIFT
    vector<KeyPoint> keypoints;
    detector->detect(src, keypoints, Mat());//�ҳ��ؼ���

    // ���ƹؼ���
    Mat keypoint_img;
    drawKeypoints(src, keypoints, keypoint_img, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    imshow("KeyPoints Image", keypoint_img);

    waitKey(0);
    return 0;
}