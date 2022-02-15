#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/** �������� */
void detectAndDisplay(Mat frame);

/** ȫ�ֱ��� */
string face_cascade_name = ".//data//haarcascade_frontalface_alt.xml";
string eyes_cascade_name = ".//data//haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
string window_name = "Capture - Face detection";
RNG rng(12345);

/** @������ */
int main(int argc, const char** argv)
{
    VideoCapture capture(0); // open camera
    if (!capture.isOpened())
    {
        cout << "could not open camera..." << endl;
        return 0;
    }
    Size S = Size((int)capture.get(CAP_PROP_FRAME_WIDTH), (int)capture.get(CAP_PROP_FRAME_HEIGHT));
    int fps = capture.get(CAP_PROP_FPS);

    Mat frame;

    //-- 1. ���ؼ����������ļ�
    if (!face_cascade.load(face_cascade_name)) { printf("--(!)Error loading\n"); return -1; };
    if (!eyes_cascade.load(eyes_cascade_name)) { printf("--(!)Error loading\n"); return -1; };

    //-- 2. ����������ͷ��Ƶ��
    namedWindow(window_name, WINDOW_AUTOSIZE);
    vector<Rect> faces;
    Mat frame_gray;
    while (capture.read(frame))
    {
        flip(frame, frame, 1);
        //-- 3. �Ե�ǰ֡ʹ�÷��������м��
        if (!frame.empty())
            detectAndDisplay(frame);
        else
        {
            printf(" --(!) No captured frame -- Break!");
            break;
        }
        //-- ��ʾ���ͼ��
        imshow(window_name, frame);

        int c = waitKey(10);
        if ((char)c == 'c') { break; }
    }

    //waitKey(0);
    return 0;
}

void detectAndDisplay(Mat frame)
{
    vector<Rect> faces;
    Mat frame_gray;

    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);
    //-- ��ߴ�������
    face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

    for (int i = 0; i < faces.size(); i++)
    {
        Point center(faces[i].x + faces[i].width * 0.5, faces[i].y + faces[i].height * 0.5);
        ellipse(frame, center, Size(faces[i].width * 0.5, faces[i].height * 0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);

        Mat faceROI = frame_gray(faces[i]);
        vector<Rect> eyes;

        //-- ��ÿ�������ϼ��˫��
        eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

        for (int j = 0; j < eyes.size(); j++)
        {
            Point center(faces[i].x + eyes[j].x + eyes[j].width * 0.5, faces[i].y + eyes[j].y + eyes[j].height * 0.5);
            int radius = cvRound((eyes[j].width + eyes[i].height) * 0.25);
            circle(frame, center, radius, Scalar(255, 0, 0), 4, 8, 0);
        }
    }
    //-- ��ʾ���ͼ��
    imshow(window_name, frame);
    
}
