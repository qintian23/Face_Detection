#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <vector>

using namespace std;
using namespace cv;

double calcPCAOrientation(vector<Point> &pts, Mat &image);

int main(int argv, char** argc)
{
	string path = "./image/pca/pca_test1.jpg";
	Mat src = imread(path);

	if (src.empty())
	{
		printf("could not load image...\n");
		return -1;
	}

	namedWindow("input image", WINDOW_AUTOSIZE);
	imshow("input image", src);

	Mat gray, binary;
	cvtColor(src, gray, COLOR_BGR2GRAY);
	threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);
	imshow("binary image", binary);

	vector<Vec4i> hireachy;
	vector <vector<Point>> contours;
	findContours(binary, contours, hireachy, RETR_LIST, CHAIN_APPROX_NONE);
	Mat result = src.clone();
	for (int i = 0; i < contours.size(); i++)
	{
		double area = contourArea(contours[i]);
		if (area > 1e5 || area < 1e2) continue;
		drawContours(result, contours, i, Scalar(0, 0, 255), 2, 8);
		double theta = calcPCAOrientation(contours[i], result);
	}
	imshow("contours result", result);

	waitKey(0);
	return 0;
}

double calcPCAOrientation(vector<Point>& pts, Mat& image)
{
	int size = static_cast<int>(pts.size());
	Mat data_pts = Mat(size, 2, CV_64FC1);
	for (int i = 0; i < size; i++)
	{
		data_pts.at<double>(i, 0) = pts[i].x;
		data_pts.at<double>(i, 1) = pts[i].y;
	}

	// perform PCA projection
	PCA pca_analysis(data_pts, Mat(), PCA::DATA_AS_ROW);
	Point cnt = Point(static_cast<int>(pca_analysis.mean.at<double>(0, 0)), static_cast<int>(pca_analysis.mean.at<double>(0, 1)));
	circle(image, cnt, 2, Scalar(0, 255, 0), 2, 8, 0);

	return 0.0;
}
