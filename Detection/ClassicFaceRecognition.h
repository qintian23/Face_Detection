#pragma once

#include <opencv2/face.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <vector>
#include <fstream>

using namespace std;
using namespace cv;
using namespace cv::face;

enum OPERATIONS
{
	EIGENFACER = 1,
	FISHERFACER,
	LBPHFACER
};

int ReadFile(string filename, vector<Mat> &images, vector<int> &labels);
void EigenFace_Demo(vector<Mat> images, vector<int> labels);
void FisherFace_Demo(vector<Mat> images, vector<int> labels);
void LBPHFace_Demo(vector<Mat> images, vector<int> labels);
void Normalizer(Mat intput, Mat& output);
void ShowFace(Mat facevector, int start, int end, int step, int height);
void RconFace(Mat facevector, Mat meanface, int start, int end, int step, int height, int Type, vector<Mat> images);

// Test
int ClassicFaceRecongnitiion(int operation, string filename)
{
	//string path = ".//image//ORL_Faces//s1//1.pgm";
	//string filename = string(".//image//ORL_Faces//image.csv");
	//Mat src = imread(path);
	//imshow("face",src);
	vector<Mat> images;
	vector<int> labels;
	int ok = ReadFile(filename, images, labels);
	if (ok == -1)return -1;

	switch (operation)
	{
	case EIGENFACER:
		EigenFace_Demo(images, labels);
		break;
	case FISHERFACER:
		FisherFace_Demo(images, labels);
		break;
	case LBPHFACER:
		LBPHFace_Demo(images, labels);
		break;
	default:
		printf("请输入正确的操作！！！\n");
		break;
	}
	//waitKey(0);
	return 0;
}

inline int ReadFile(string filename, vector<Mat>& images, vector<int>& labels)
{
	ifstream file(filename.c_str(), ifstream::in);
	if (!file)
	{
		printf("could not load file correctly...\n");
		return -1;
	}

	string line, path, classlabel;
	
	char separator = ';';
	while (getline(file, line))
	{
		stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, classlabel);
		if (!path.empty() && !classlabel.empty())
		{
			//printf("path : %s\n", path.c_str());
			images.push_back(imread(path, 0));
			labels.push_back(atoi(classlabel.c_str()));
		}
	}
	if (images.size() < 1 || labels.size() < 1)
	{
		printf("invalid image path\n");
		return -1;
	}
	return 0;
}

void EigenFace_Demo(vector<Mat> images, vector<int> labels)
{
	int height = images[0].rows;
	int width = images[0].cols;
	printf("height: %d, width: %d\n", height, width);

	Mat testSample = images[images.size() - 1];
	int testLabel = labels[labels.size() - 1];
	images.pop_back();
	labels.pop_back();

	// train it
	Ptr<BasicFaceRecognizer> model = EigenFaceRecognizer::create();
	model->train(images, labels);

	// recognition face
	int predictedLabel = model->predict(testSample);
	printf("actual label: %d, predict label: %d\n", testLabel, predictedLabel); 

	Mat eigenvalues = model->getEigenValues();
	Mat eigenvectors = model->getEigenVectors();
	Mat mean = model->getMean();
	Mat meanFace = mean.reshape(1, height);
	Mat dst;

	Normalizer(meanFace, dst);

	// show eigen face
	ShowFace(eigenvectors, 0, min(10, eigenvectors.cols), 1, height);

	// 重建人脸
	RconFace(eigenvectors, mean, min(10, eigenvectors.cols), min(300, eigenvectors.cols), 15, height, 0, images);
}

void FisherFace_Demo(vector<Mat> images, vector<int> labels)
{
	int height = images[0].rows;
	int width = images[0].cols;
	printf("height: %d, width: %d\n", height, width);

	Mat testSample = images[images.size() - 1];
	int testLabel = labels[labels.size() - 1];
	images.pop_back();
	labels.pop_back();

	// train it
	Ptr<BasicFaceRecognizer> model = FisherFaceRecognizer::create();
	model->train(images, labels);

	// recognition face
	int predictedLabel = model->predict(testSample);
	printf("actual label: %d, predict label: %d\n", testLabel, predictedLabel);

	Mat eigenvalues = model->getEigenValues();
	Mat eigenvectors = model->getEigenVectors();
	Mat mean = model->getMean();
	Mat meanFace = mean.reshape(1, height);
	Mat dst;

	Normalizer(meanFace, dst);

	// show eigen face
	ShowFace(eigenvectors, 0, min(16, eigenvectors.cols), 1, height);

	// 重建人脸
	RconFace(eigenvectors, mean, 0, min(16, eigenvectors.cols), 1, height, 1, images);
}

void LBPHFace_Demo(vector<Mat> images, vector<int> labels)
{
	int height = images[0].rows;
	int width = images[0].cols;
	printf("height: %d, width: %d\n", height, width);

	Mat testSample = images[images.size() - 1];
	int testLabel = labels[labels.size() - 1];
	images.pop_back();
	labels.pop_back();

	// train it
	Ptr<LBPHFaceRecognizer> model = LBPHFaceRecognizer::create();
	model->train(images, labels);

	// recognition face
	int predictedLabel = model->predict(testSample);
	printf("actual label: %d, predict label: %d\n", testLabel, predictedLabel);

	// print parameters     
	printf("radius : %d\n", model->getRadius());//中心像素点到周围像素点的距离
	printf("neibs : %d\n", model->getNeighbors());//周围像素点的个数
	printf("grad_x : %d\n", model->getGridX());//将一张图片在x方向分成几块
	printf("grad_y : %d\n", model->getGridY()); //将一张图片在y方向分成几块
	printf("threshold : %.2f\n\n", model->getThreshold());//相似度阈值   
}

void Normalizer(Mat intput, Mat& output)
{
	if (intput.channels() == 1)
		normalize(intput, output, 0, 255, NORM_MINMAX, CV_8UC1);
	else if (intput.channels() == 3)
		normalize(intput, output, 0, 255, NORM_MINMAX, CV_8UC3);
}

void ShowFace(Mat facevector, int start, int end, int step, int height)
{
	// show eigen face
	for (int i = start; i < end; i += step)
	{
		Mat ev = facevector.col(i).clone();
		Mat grayscale;
		Mat Face = ev.reshape(1, height);
		Normalizer(Face, grayscale);
		Mat colorface;
		applyColorMap(grayscale, colorface, COLORMAP_RAINBOW);
		char* winTitle = new char[128];
		sprintf(winTitle, "Face_%d", i);
		imshow(winTitle, colorface);
	}
}

void RconFace(Mat facevector, Mat meanface, int start, int end, int step, int height, int Type, vector<Mat> images)
{
	// 重建人脸
	Mat evs;
	for (int num = start; num < end; num += step)
	{
		if (Type == EIGENFACER)
			evs = Mat(facevector, Range::all(), Range(0, num));
		else
			evs = facevector.col(num).clone();

		Mat projection = LDA::subspaceProject(evs, meanface, images[0].reshape(1, 1));
		Mat reconstruction = LDA::subspaceReconstruct(evs, meanface, projection);
		Mat result = reconstruction.reshape(1, height);
		Normalizer(result, reconstruction);
		char* winTitle = new char[128];
		sprintf(winTitle, "recon_face_%d", num);
		imshow(winTitle, reconstruction);
	}
}

