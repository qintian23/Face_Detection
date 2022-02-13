#include <opencv2/face.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <vector>
#include <fstream>

using namespace std;
using namespace cv;
using namespace cv::face;

#define EIGENFACER 0

void EigenFace_Demo(vector<Mat> images, vector<int> labels);
void FisherFace_Demo(vector<Mat> images, vector<int> labels);
void Normalizer(Mat intput, Mat& output);
void ShowFace(Mat facevector, int start, int end, int step, int height);
void RconFace(Mat facevector, Mat meanface, int start, int end, int step, int height, int Type, vector<Mat> images);

int main(int argv, char** argc)
{
	//string path = ".//image//ORL_Faces//s1//1.pgm";
	string filename = string(".//image//ORL_Faces//image.csv");
	//Mat src = imread(path);
	//imshow("face",src);

	ifstream file(filename.c_str(), ifstream::in);
	if (!file)
	{
		printf("could not load file correctly...\n");
		return -1;
	}

	string line, path, classlabel;
	vector<Mat> images;
	vector<int> labels;
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

	EigenFace_Demo(images, labels);
	//FisherFace_Demo(images, labels);

	waitKey(0);
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

