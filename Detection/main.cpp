#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/face.hpp>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;
using namespace cv::face;

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
	while (getline(file,line))
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

	waitKey(0);
	return 0;
}
