#pragma once
//金字塔（图像的上采样和降采样）
//图像金字塔:图像金字塔是一种以多分辨率来解释图像的有效但概念简单的结构。应用于图像分割，机器视觉和图像压缩。一幅图像的金字塔是一系列以金字塔形状排列的分辨率逐步降低，
//且来源于同一张原始图的图像集合。其通过梯次向下采样获得，直到达到某个终止条件才停止采样。金字塔的底部是待处理图像的高分辨率表示，而顶部是低分辨率的近似。
//我们将一层一层的图像比喻成金字塔，层级越高，则图像越小，分辨率越低。

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

	//上采样（zoom in 放大）
	pyrUp(src, dst, Size(src.cols * 2, src.rows * 2));
	imshow("zoom in", dst);
	//降采样（zoom out 缩小）
	pyrDown(src, dst1, Size(src.cols / 2, src.rows / 2));
	imshow("zoom out", dst1);

	/*
	dst = src;
	dst1 = dst;
	/// 循环
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


	//高斯不同DOG（different of gaussian）
	//就是把同一张图像在不同的参数下做高斯模糊之后的结果相减，得到的输出图像;高斯不同是图像的内在特征，在灰度图像增强、角点检测中经常用到。
	Mat gray_src, dst2, dst3, dog_Image;
	cvtColor(src, gray_src, COLOR_BGR2GRAY);
	GaussianBlur(gray_src, dst2, Size(3, 3), 0, 0);
	imshow("dst2..", dst2);
	GaussianBlur(dst2, dst3, Size(3, 3), 0, 0);
	imshow("dst3..", dst3);
	subtract(dst2, dst3, dog_Image);

	//归一化显示
	normalize(dog_Image, dog_Image, 255, 0, NORM_MINMAX);
	//src输入;dst输出，支持原地运算;alpha:range normalization模式的最小值;beta:range normalization模式的最大值，不用于norm normalization(范数归一化)模式。
	//normType
	//归一化的类型，可以有以下的取值：
	//NORM_MINMAX : 数组的数值被平移或缩放到一个指定的范围，线性归一化，一般较常用。
	//NORM_INF : 此类型的定义没有查到，根据OpenCV 1的对应项，可能是归一化数组的C - 范数(绝对值的最大值)
	//NORM_L1 : 归一化数组的L1 - 范数(绝对值的和)
	//NORM_L2 : 归一化数组的(欧几里德)L2 - 范数

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
	cvtColor(src, src_gray, COLOR_BGR2GRAY);//灰度化
	GaussianBlur(src, src, Size(3, 3), 0.1, 0, BORDER_DEFAULT);
	blur(src_gray, src_gray, Size(3, 3)); //滤波
	namedWindow("image",WINDOW_AUTOSIZE);
	imshow("image", src);
	moveWindow("image", 20, 20);
	//定义Canny边缘检测图像
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//利用canny算法检测边缘
	Canny(src_gray, canny_output, thresh, thresh * 3, 3);
	namedWindow("canny", WINDOW_AUTOSIZE);
	imshow("canny", canny_output);
	moveWindow("canny", 550, 20);
	//查找轮廓
	findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	//计算轮廓矩
	vector<Moments> mu(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
	}
	//计算轮廓的质心
	vector<Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mc[i] = Point2d(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}

	//画轮廓及其质心并显示
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	printf("\t\t 几何特性\n");
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
		circle(drawing, mc[i], 4, color, -1, 8, 0);
		rectangle(drawing, boundingRect(contours.at(i)), Scalar(0, 255, 0));
		printf("目标%d - 面积：%.2f - 周长: %.2f ", i, mu[i].m00, contourArea(contours[i]), arcLength(contours[i], true));
		RotatedRect r = fitEllipse(contours.at(i));
		double majorAxis = r.size.height > r.size.width ? r.size.height : r.size.width;//长轴大小
		double minorAxis = r.size.height > r.size.width ? r.size.width : r.size.height;//短轴大小
		double area = mu[i].m00;//面积
		int perimeter = arcLength(contours.at(i), true);
		double orientation = r.angle;
		double orientation_rads = orientation * 3.1416 / 180;
		printf("- 偏移角度: %.1f\n\n", orientation);
		double diameter = sqrt((4 * area) / 3.1416);//直径
		double eccentricity = sqrt(1 - pow(minorAxis / majorAxis, 2));//离心率
		double roundness = pow(perimeter, 2) / (2 * 3.1416 * area);//圆滑度
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