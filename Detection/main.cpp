#include "ClassicFaceRecognition.h"
#include <iostream>

using namespace cv;
using namespace std;

string haar_face_datapath = ".//data//haarcascade_frontalface_alt_tree.xml";
void Collect_Face(); 
int FaceRecognition(string filename);

int pits = 0;

int main(int argv, char** argc)
{
	//Collect_Face();
	string filename = string(".//image//myface//image.csv"); // 上传之后，已把本人的照片删掉，首次使用时，应先运行Collect_Face()；
	//ClassicFaceRecongnitiion(OPERATIONS::EIGENFACER, filename);
	
	/*Mat src = imread(".//image//ORL_Faces//s1//10.pgm");
	printf("row: %d, col: %d\n", src.rows, src.cols);
	imshow("src", src);*/
	FaceRecognition(filename);

	waitKey(0);
	return 0;
}

// 采集人脸
void Collect_Face()
{
	VideoCapture capture(0); // open camera
	if (!capture.isOpened())
	{
		cout << "could not open camera..." << endl;
		return ;
	}
	Size S = Size((int)capture.get(CAP_PROP_FRAME_WIDTH), (int)capture.get(CAP_PROP_FRAME_HEIGHT));
	int fps = capture.get(CAP_PROP_FPS);

	CascadeClassifier faceDetector;
	faceDetector.load(haar_face_datapath);

	Mat frame;
	namedWindow("camera-demo", WINDOW_AUTOSIZE);
	vector<Rect> faces;
	int count = 0;
	while (capture.read(frame))
	{
		flip(frame, frame, 1);
		faceDetector.detectMultiScale(frame, faces, 1.1, 1, 0, Size(100, 120), Size(380, 400));
		for (int i = 0; i < faces.size(); i++)
		{
			if (count % 10 == 0)
			{
				Mat dst;
				resize(frame(faces[i]), dst, Size(92, 112));
				imwrite(format(".//image//myface//face_%d.jpg", pits), dst);
				pits++;
			}

			rectangle(frame, faces[i], Scalar(0, 0, 255), 2, 8, 0);

		}
		imshow("camera-demo", frame);
		char c = waitKey(10);
		//if (c == 27) break;
		if (pits >= 30)break;
		count++;
	}

	capture.release();
}

int FaceRecognition(string filename)
{
	vector<Mat> images;
	vector<int> labels;
	int ok = ReadFile(filename, images, labels);
	if (ok == -1)return -1;

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

	CascadeClassifier faceDetector;
	faceDetector.load(haar_face_datapath);

	VideoCapture capture(0); // open camera
	if (!capture.isOpened())
	{
		cout << "could not open camera..." << endl;
		return -1;
	}
	Mat frame;
	namedWindow("face-recognition", WINDOW_AUTOSIZE);
	vector<Rect> faces;
	Mat dst;
	while (capture.read(frame))
	{
		flip(frame, frame, 1);
		faceDetector.detectMultiScale(frame, faces, 1.1, 1, 0, Size(80, 100), Size(380, 400));
		for (int i = 0; i < faces.size(); i++)
		{
			Mat roi = frame(faces[i]);
			cvtColor(roi, dst, COLOR_BGR2GRAY);
			resize(dst, testSample, testSample.size());
			int label = model->predict(testSample);
			rectangle(frame, faces[i], Scalar(255, 0, 0), 2, 8, 0);
			putText(frame, format("I'm %s", (label == 1 ? "Jianle" : "Unknow")), faces[i].tl(), FONT_HERSHEY_PLAIN, 1.0, Scalar(0, 0, 255), 2, 8);
		}
		imshow("face-recognition", frame);
		char c = waitKey(10);
		if (c == 27)break;
	}

	return 0;
}
