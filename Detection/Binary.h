#pragma once
// Ѱ����ֵ
#include <opencv2/opencv.hpp>

using namespace std;

// ��
void Histogram(Mat image)
{
	//int rows = image.rows;
	//int cols = image.cols;

	//// �ҶȻ�
	//Mat img(rows, cols, CV_8UC1, Scalar(0));

	//cout << img.channels() << endl;

	//MeanGray(image, img);

	//MatND dstHist;       // ��cv����CvHistogram *hist = cvCreateHist
	//int dims = 1;         // һά
	//float hranges[] = { 0, 255 };    // �Ҷ�����
	//const float* ranges[] = { hranges };   // ������ҪΪconst����
	//int size = 256;  // �Ҷȼ���
	//int channels = 0;
	////
	//calcHist(&img, 1, &channels, Mat(), dstHist, dims, &size, ranges);    // cv ����cvCalcHist

	////int i;        // ��������ĩβλ��
	//int j;        // ����������ʼλ��
	//int min;    // ����������СԪ��λ��

	//for (i = 0; i < n; i++)
	//{
	//	min = i;

	//	//��"a[i+1]..a[n]"֮����СԪ�أ�������min
	//	for (j = i + 1; j < n; j++)
	//	{
	//		if (a[j] < a[min])
	//			min = j;
	//	}

	//	//��min!=i���򽻻� a[i] �� a[min]��
	//	//�����󣬱�֤��a[0]..a[i]֮��Ԫ������
	//	if (min != i)
	//		swap(a[i], a[min]);
	//}

	//int scale = 1;
	//
	//Mat dstImage(size * scale, size, CV_8U, Scalar(0)); // ȫ0����
	//
	//double minValue = 0;
	//double maxValue = 0;
	//minMaxLoc(dstHist, &minValue, &maxValue, 0, 0);  //  ��cv���õ���cvGetMinMaxHistValue
	//
	//int hpt = saturate_cast<int>(0.9 * size);
 //   for (int i = 0; i < 256; i++)
	//{
	//    float binValue = dstHist.at<float>(i);           //   ע��hist����float����    ����OpenCV1.0������cvQueryHistValue_1D
	//	int realValue = saturate_cast<int>(binValue * hpt / maxValue);
	//	rectangle(dstImage, Point(i * scale, size - 1), Point((i + 1) * scale - 1, size - realValue), Scalar(255));
	//}
	//imshow("11111", dstImage);
	// 
}