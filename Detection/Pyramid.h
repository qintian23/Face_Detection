#pragma once
//��������ͼ����ϲ����ͽ�������
//ͼ�������:ͼ���������һ���Զ�ֱ���������ͼ�����Ч������򵥵Ľṹ��Ӧ����ͼ��ָ�����Ӿ���ͼ��ѹ����һ��ͼ��Ľ�������һϵ���Խ�������״���еķֱ����𲽽��ͣ�
//����Դ��ͬһ��ԭʼͼ��ͼ�񼯺ϡ���ͨ���ݴ����²�����ã�ֱ���ﵽĳ����ֹ������ֹͣ�������������ĵײ��Ǵ�����ͼ��ĸ߷ֱ��ʱ�ʾ���������ǵͷֱ��ʵĽ��ơ�
//���ǽ�һ��һ���ͼ������ɽ��������㼶Խ�ߣ���ͼ��ԽС���ֱ���Խ�͡�

#include<opencv2/opencv.hpp>
#include<iostream>

using namespace cv;

int Test1(int argc, int argv)
{
	Mat src, dst, dst1;
	src = imread("C:\\Users\\59235\\Desktop\\imag\\girl1.jpg");
	if (!src.data)
	{
		printf("could not load image...\n");
	}
	namedWindow("input", WINDOW_AUTOSIZE);
	imshow("input", src);

	//�ϲ�����zoom in �Ŵ�
	pyrUp(src, dst, Size(src.cols * 2, src.rows * 2));
	imshow("zoom in", dst);
	//��������zoom out ��С��
	pyrDown(src, dst1, Size(src.cols / 2, src.rows / 2));
	imshow("zoom out", dst1);

	/*
	dst = src;
	dst1 = dst;
	/// ѭ��
	while (true)
	{
	int c;
	c = waitKey(50);
	if ((char)c == 27)
	{
	break;
	}
	if ((char)c == 'u')
	{
	pyrUp(dst, dst1, Size(dst.cols * 2, dst.rows * 2));
	printf("** Zoom In: Image x 2 \n");
	//imshow("Zoom In", dst1);
	}
	else if ((char)c == 'd')
	{
	pyrDown(dst, dst1, Size(dst.cols / 2, dst.rows / 2));
	printf("** Zoom Out: Image / 2 \n");
	//imshow("Zoom Out", dst1);
	}
	imshow("Zoom Out", dst1);
	dst = dst1;
	}
	return 0;
	*/


	//��˹��ͬDOG��different of gaussian��
	//���ǰ�ͬһ��ͼ���ڲ�ͬ�Ĳ���������˹ģ��֮��Ľ��������õ������ͼ��;��˹��ͬ��ͼ��������������ڻҶ�ͼ����ǿ���ǵ����о����õ���
	Mat gray_src, dst2, dst3, dog_Image;
	cvtColor(src, gray_src, COLOR_BGR2GRAY);
	GaussianBlur(gray_src, dst2, Size(3, 3), 0, 0);
	imshow("dst2..", dst2);
	GaussianBlur(dst2, dst3, Size(3, 3), 0, 0);
	imshow("dst3..", dst3);
	subtract(dst2, dst3, dog_Image);

	//��һ����ʾ
	normalize(dog_Image, dog_Image, 255, 0, NORM_MINMAX);
	//src����;dst�����֧��ԭ������;alpha:range normalizationģʽ����Сֵ;beta:range normalizationģʽ�����ֵ��������norm normalization(������һ��)ģʽ��
	//normType
	//��һ�������ͣ����������µ�ȡֵ��
	//NORM_MINMAX : �������ֵ��ƽ�ƻ����ŵ�һ��ָ���ķ�Χ�����Թ�һ����һ��ϳ��á�
	//NORM_INF : �����͵Ķ���û�в鵽������OpenCV 1�Ķ�Ӧ������ǹ�һ�������C - ����(����ֵ�����ֵ)
	//NORM_L1 : ��һ�������L1 - ����(����ֵ�ĺ�)
	//NORM_L2 : ��һ�������(ŷ�����)L2 - ����

	namedWindow("result", WINDOW_AUTOSIZE);
	imshow("result", dog_Image);
	waitKey(0);
	return 0;
}

using namespace cv;
using namespace std;
Mat src; Mat src_gray;
int thresh = 30;
int max_thresh = 255;
RNG rng(12345);
int Test2() {
	src = imread("opencv-logo.png");
	cvtColor(src, src_gray, COLOR_BGR2GRAY);//�ҶȻ�
	GaussianBlur(src, src, Size(3, 3), 0.1, 0, BORDER_DEFAULT);
	blur(src_gray, src_gray, Size(3, 3)); //�˲�
	namedWindow("image",WINDOW_AUTOSIZE);
	imshow("image", src);
	moveWindow("image", 20, 20);
	//����Canny��Ե���ͼ��
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//����canny�㷨����Ե
	Canny(src_gray, canny_output, thresh, thresh * 3, 3);
	namedWindow("canny", WINDOW_AUTOSIZE);
	imshow("canny", canny_output);
	moveWindow("canny", 550, 20);
	//��������
	findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	//����������
	vector<Moments> mu(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
	}
	//��������������
	vector<Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mc[i] = Point2d(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}

	//�������������Ĳ���ʾ
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	printf("\t\t ��������\n");
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
		circle(drawing, mc[i], 4, color, -1, 8, 0);
		rectangle(drawing, boundingRect(contours.at(i)), Scalar(0, 255, 0));
		printf("Ŀ��%d - �����%.2f - �ܳ�: %.2f ", i, mu[i].m00, contourArea(contours[i]), arcLength(contours[i], true));
		RotatedRect r = fitEllipse(contours.at(i));
		double majorAxis = r.size.height > r.size.width ? r.size.height : r.size.width;//�����С
		double minorAxis = r.size.height > r.size.width ? r.size.width : r.size.height;//�����С
		double area = mu[i].m00;//���
		int perimeter = arcLength(contours.at(i), true);
		double orientation = r.angle;
		double orientation_rads = orientation * 3.1416 / 180;
		printf("- ƫ�ƽǶ�: %.1f\n\n", orientation);
		double diameter = sqrt((4 * area) / 3.1416);//ֱ��
		double eccentricity = sqrt(1 - pow(minorAxis / majorAxis, 2));//������
		double roundness = pow(perimeter, 2) / (2 * 3.1416 * area);//Բ����
		line(drawing, Point(mc[i].x, mc[i].y), Point(mc[i].x + 30 * cos(orientation_rads), mc[i].y + 30 * sin(orientation_rads)), Scalar(0, 0, 200), 3);
		char tam[100];
		sprintf(tam, "%.2f", orientation);
		putText(drawing, tam, Point(mc[i].x, mc[i].y), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 220, 120), 1.5);
	}
	namedWindow("Contours", WINDOW_AUTOSIZE);
	imshow("Contours", drawing);
	moveWindow("Contours", 1100, 20);
	waitKey(0);
	src.release();
	src_gray.release();
	return 0;
}