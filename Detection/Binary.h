#pragma once
// 寻找阈值
#include <opencv2/opencv.hpp>

using namespace std;

// ？
void Histogram(Mat image)
{
	//int rows = image.rows;
	//int cols = image.cols;

	//// 灰度化
	//Mat img(rows, cols, CV_8UC1, Scalar(0));

	//cout << img.channels() << endl;

	//MeanGray(image, img);

	//MatND dstHist;       // 在cv中用CvHistogram *hist = cvCreateHist
	//int dims = 1;         // 一维
	//float hranges[] = { 0, 255 };    // 灰度区间
	//const float* ranges[] = { hranges };   // 这里需要为const类型
	//int size = 256;  // 灰度级数
	//int channels = 0;
	////
	//calcHist(&img, 1, &channels, Mat(), dstHist, dims, &size, ranges);    // cv 中是cvCalcHist

	////int i;        // 有序区的末尾位置
	//int j;        // 无序区的起始位置
	//int min;    // 无序区中最小元素位置

	//for (i = 0; i < n; i++)
	//{
	//	min = i;

	//	//找"a[i+1]..a[n]"之间最小元素，并赋给min
	//	for (j = i + 1; j < n; j++)
	//	{
	//		if (a[j] < a[min])
	//			min = j;
	//	}

	//	//若min!=i，则交换 a[i] 和 a[min]。
	//	//交换后，保证了a[0]..a[i]之间元素有序。
	//	if (min != i)
	//		swap(a[i], a[min]);
	//}

	//int scale = 1;
	//
	//Mat dstImage(size * scale, size, CV_8U, Scalar(0)); // 全0矩阵
	//
	//double minValue = 0;
	//double maxValue = 0;
	//minMaxLoc(dstHist, &minValue, &maxValue, 0, 0);  //  在cv中用的是cvGetMinMaxHistValue
	//
	//int hpt = saturate_cast<int>(0.9 * size);
 //   for (int i = 0; i < 256; i++)
	//{
	//    float binValue = dstHist.at<float>(i);           //   注意hist中是float类型    而在OpenCV1.0版中用cvQueryHistValue_1D
	//	int realValue = saturate_cast<int>(binValue * hpt / maxValue);
	//	rectangle(dstImage, Point(i * scale, size - 1), Point((i + 1) * scale - 1, size - realValue), Scalar(255));
	//}
	//imshow("11111", dstImage);
	// 
}