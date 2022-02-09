#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

#define WINDOW_NAME1 "��ԭʼͼ��"
#define WINDOW_NAME2 "��ͼ��������"

Mat g_srcImage; Mat g_grayImage;
int g_nThresh = 100;
int g_nMaxThresh = 255;
RNG g_rng(12345);
Mat g_cannyMat_output;
vector<vector<Point>> g_vContours;
vector<Vec4i> g_vHierarvhy;

void on_ThreshChange(int, void*);

int Draw_Contour(String imgpath = "girl.png")
{
	system("color 1E");

	//String imgpath = "beauty.png";
	//String imgpath = "girl.png";
	g_srcImage = imread(imgpath, 1);
	if (g_srcImage.empty()) return 0; // ��ȡͼ��ʧ��
	//imshow("����", g_srcImage);
	//Test(1, image);
	//Test(5, image);
	//Test(2, image);

	// ��ԭͼ��ת��Ϊ�Ҷ�ͼ�񲢽���ƽ��
	cvtColor(g_srcImage, g_grayImage, COLOR_RGB2GRAY);
	blur(g_grayImage, g_grayImage, Size(3, 3));

	// �����´���
	namedWindow(WINDOW_NAME1, WINDOW_AUTOSIZE);
	imshow(WINDOW_NAME1, g_srcImage);

	// ���������������г�ʼ��
	createTrackbar("	��ֵ", WINDOW_NAME1, &g_nThresh, g_nMaxThresh, on_ThreshChange);
	on_ThreshChange(0, 0);

	waitKey(0);
	return 0;
}

void on_ThreshChange(int, void*)
{
	// ��Ե���
	Canny(g_grayImage, g_cannyMat_output, g_nThresh, g_nThresh * 2, 3);

	// �ҵ�����
	findContours(g_cannyMat_output, g_vContours, g_vHierarvhy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	// �����
	vector<Moments> mu(g_vContours.size());
	for (unsigned int i = 0; i < g_vContours.size(); i++)
	{
		mu[i] = moments(g_vContours[i], false);
	}

	// �������ľ�
	vector<Point2f> mc(g_vContours.size());
	for (unsigned int i = 0; i < g_vContours.size(); i++)
	{
		mc[i] = Point2f(static_cast<float>(mu[i].m10 / mu[i].m00), static_cast<float>(mu[i].m01 / mu[i].m00));
	}

	// ��������
	Mat drawing = Mat::zeros(g_cannyMat_output.size(), CV_8UC3);
	for (unsigned int i = 0; i < g_vContours.size(); i++)
	{
		Scalar color = Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255)); // ���������ɫֵ
		drawContours(drawing, g_vContours, i, color, 2, 8, g_vHierarvhy, 0, Point()); // ���������ڲ�����
		circle(drawing, mc[i], 4, color, -1, 8, 0); // ����Բ
	}

	// ��ʾ
	namedWindow(WINDOW_NAME2, WINDOW_AUTOSIZE);
	imshow(WINDOW_NAME2, drawing);

	printf("\t ������ݣ��������������\n");
	for (unsigned int i = 0; i < g_vContours.size(); i++)
	{
		printf(" >ͨ��m00���������[%d]�ģ�(M_00)=%.2f\n OpenCv��������������=%.2f, ���ȣ�%.2f \n\n", i, mu[i].m00, contourArea(g_vContours[i]), arcLength(g_vContours[i], true));
		Scalar color = Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255));
		drawContours(drawing, g_vContours, i, color, 2, 8, g_vHierarvhy, 0, Point());
		circle(drawing, mc[i], 4, color, -1, 8, 0);
	}
}
