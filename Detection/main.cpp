#include <opencv2/face.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <vector>
#include <fstream>

using namespace std;
using namespace cv;
using namespace cv::face;

void EigenFace_Demo(vector<Mat> images, vector<int> labels);

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

	if (meanFace.channels() == 1)
		normalize(meanFace, dst, 0, 255, NORM_MINMAX, CV_8UC1);
	else if (meanFace.channels() == 3)
		normalize(meanFace, dst, 0, 255, NORM_MINMAX, CV_8UC3);
	imshow("Mean Face", dst);

	// show eigen face
	for (int i = 0; i < min(10, eigenvectors.cols); i++)
	{
		Mat ev = eigenvectors.col(i).clone();
		Mat grayscale;
		Mat eigenFace = ev.reshape(1, height);
		if (eigenFace.channels() == 1)
			normalize(eigenFace, grayscale, 0, 255, NORM_MINMAX, CV_8UC1);
		else if (eigenFace.channels() == 3)
			normalize(eigenFace, grayscale, 0, 255, NORM_MINMAX, CV_8UC3);
		Mat colorface;
		applyColorMap(grayscale, colorface, COLORMAP_BONE);
		char* winTitle = new char[128];
		sprintf(winTitle, "eigenFace_%d", i);
		imshow(winTitle, colorface);
	}

	// ÖØ½¨ÈËÁ³
	for (int num = min(10, eigenvectors.cols); num < min(300, eigenvectors.cols); num += 15)
	{
		Mat evs = Mat(eigenvectors, Range::all(), Range(0, num));
		Mat projection = LDA::subspaceProject(evs, mean, images[0].reshape(1, 1));
		Mat reconstruction = LDA::subspaceReconstruct(evs, mean, projection);

		Mat result = reconstruction.reshape(1, height);
		if (result.channels() == 1)
			normalize(result, reconstruction, 0, 255, NORM_MINMAX, CV_8UC1);
		else if (result.channels() == 3)
			normalize(result, reconstruction, 0, 255, NORM_MINMAX, CV_8UC3);

		char* winTitle = new char[128];
		sprintf(winTitle, "recon_face_%d", num);
		imshow(winTitle, reconstruction);
	}
}
