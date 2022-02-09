#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/// ȫ�ֱ���
Mat img; Mat templ; Mat result;
const char* image_window = "Source Image";
const char* result_window = "Result window";

int match_method;
int max_Trackbar = 5;

/// ��������
void MatchingMethod(int, void*);

/** @������ */
int Template_Matching(String imgPath = "beauty.png", String templPath = "smalleye.png")
{
    /// ����ԭͼ���ģ���
    img = imread(imgPath, 1);
    templ = imread(templPath, 1);

    /// ��������
    namedWindow(image_window, WINDOW_AUTOSIZE);
    namedWindow(result_window, WINDOW_AUTOSIZE);

    /// ����������
    const char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
    createTrackbar(trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod);

    MatchingMethod(0, 0);

    waitKey(0);
    return 0;
}

/**
 * @���� MatchingMethod
 * @�򵥵Ļ������ص�����
 */
void MatchingMethod(int, void*)
{
    /// ������ʾ��ԭͼ��
    Mat img_display;
    img.copyTo(img_display);

    /// �����������ľ���
    int result_cols = img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;

    result.create(result_cols, result_rows, CV_32FC1);

    /// ����ƥ��ͱ�׼��
    matchTemplate(img, templ, result, match_method);
    normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

    /// ͨ������ minMaxLoc ��λ��ƥ���λ��
    double minVal; double maxVal; Point minLoc; Point maxLoc;
    Point matchLoc;

    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

    /// ���ڷ��� SQDIFF �� SQDIFF_NORMED, ԽС����ֵ������ߵ�ƥ����. ��������������, ��ֵԽ��ƥ��Խ��
    if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED)
    {
        matchLoc = minLoc;
    }
    else
    {
        matchLoc = maxLoc;
    }

    /// ���ҿ����������ս��
    rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
    rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);

    imshow(image_window, img_display);
    imshow(result_window, result);

    return;
}