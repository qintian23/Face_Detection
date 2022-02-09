#pragma once
#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

const int blue[] = { 0,0 }, green[] = { 1,1 }, red[] = { 2,2 };  // ͨ������
const float YIQ[3][3] = { {0.297226, 0.587809, 0.114965}, {0.592189, -0.27283, -0.319359}, {0.210212, -0.522018, 0.311806} };
const float RYIQ[3][3] = { {1, 0.9663, 0.621}, {1, -0.2721, -0.6474}, {1, -1.107, 1.7046} };

/*************************************************
* 
* ��ɫ�ռ�ת��
*   ɫ��ͨ������
*   �Ҷȴ���
*   ��ֵ������
*       Author��zlc
*       Date��2022/1/27
**************************************************/
void RGBShow(Mat image)
{
    /* ����1 */
    Mat Rimg(image.rows, image.cols, CV_8UC3, Scalar(0, 0, 0));
    Mat Gimg(image.rows, image.cols, CV_8UC3, Scalar(0, 0, 0));
    Mat Bimg(image.rows, image.cols, CV_8UC3, Scalar(0, 0, 0));

    double time0 = static_cast<double>(getTickCount());

    mixChannels(&image, 1, &Rimg, 1, red, 1);
    mixChannels(&image, 1, &Gimg, 1, green, 1);
    mixChannels(&image, 1, &Bimg, 1, blue, 1);

    time0 = ((double)getTickCount() - time0) / getTickFrequency();
    cout << "\t�˷�������ʱ��Ϊ�� " << time0 << "��" << endl;  //�������ʱ��

    imshow("1.��ɫ����ͼ��", Rimg);
    imshow("1.��ɫ����ͼ��", Gimg);
    imshow("1.��ɫ����ͼ��", Bimg);

    /* ����2 */
    Mat rimg(image.rows, image.cols, CV_8UC3, Scalar(0, 0, 0));
    Mat gimg(image.rows, image.cols, CV_8UC3, Scalar(0, 0, 0));
    Mat bimg(image.rows, image.cols, CV_8UC3, Scalar(0, 0, 0));

    double time1 = static_cast<double>(getTickCount());

    for (int i = 0; i < image.rows; i++) {
        uchar* data = image.ptr<uchar>(i);  //��ȡ��i�е��׵�ַ
        uchar* rdata = rimg.ptr<uchar>(i);
        uchar* gdata = gimg.ptr<uchar>(i);
        uchar* bdata = bimg.ptr<uchar>(i);
        for (int j = 0; j < image.cols * 3; j += 3) {
            bdata[j] = data[j];
            gdata[j + 1] = data[j + 1];
            rdata[j + 2] = data[j + 2];
        }
    }

    time1 = ((double)getTickCount() - time1) / getTickFrequency();
    cout << "\t�˷�������ʱ��Ϊ�� " << time1 << "��" << endl;  //�������ʱ��

    imshow("2.��ɫ����ͼ��", rimg);
    imshow("2.��ɫ����ͼ��", gimg);
    imshow("2.��ɫ����ͼ��", bimg);
}

// ƽ���ҶȻ�
void MeanGray(Mat image)
{
    Mat img(image.rows, image.cols, CV_8UC1, Scalar(0));
    for (int i = 0; i < image.rows; i++)
    {
        uchar* data = image.ptr<uchar>(i);
        uchar* idata = img.ptr<uchar>(i);
        //int gray = 0;
        for (int j = 0; j < image.cols * 3; j += 3)
        {
            uchar gray = (data[j] + data[j + 1] + data[j + 2]) / 3;

            //gray = gray > 255 ? 255 : gray;
            //gray = gray < 0 ? 0 : gray;

            idata[j / 3] = gray;
            //cout << gray << endl;
        }

    }
    imshow("RGBƽ��ֵ�Ҷ�ͼ", img);
}

void MeanGray(Mat image, Mat &img)
{
    for (int i = 0; i < image.rows; i++)
    {
        uchar* data = image.ptr<uchar>(i);
        uchar* idata = img.ptr<uchar>(i);
        for (int j = 0; j < image.cols * 3; j += 3)
        {
            uchar gray = (data[j] + data[j + 1] + data[j + 2]) / 3;
            idata[j / 3] = gray;
        }
    }
}
// ���޸�
void Binary_image(Mat image, int threshold = 70)
{
    int threshold = 50;
    int rows = image.rows;
    int cols = image.cols;

    Mat bimage(rows, cols, CV_8U, Scalar(0));
    for (int i = 0; i < rows; i++)
    {
    	uchar* data = image.ptr<uchar>(i);
    	uchar* idata = bimage.ptr<uchar>(i);
    	for (int j = 0; j < cols * 3; j += 3)
    	{
    		if ((data[j] + data[j + 1] + data[j + 2]) / 3 >= threshold)
    		{
    			idata[j / 3] = 255;
    		}
    	}
    }
    imshow("��ֵͼ", bimage);
}

/*******************************************************************
*   ���ش���������ɫͨ���Ĵ���
*   ����ɫ�ʿռ��ת����RGB��YIQ��
*   �������
* 
********************************************************************/
// ���ؾ��
void Pixel_convolution(Mat image, const float value[][3], Mat &img)
{
    //float red;
    //float green;
    //float blue;
    //Mat img(image.rows, image.cols, CV_8UC3, Scalar(0, 0, 0));
    int a = 0;
    int b = 0;
    int gray = 0;
    for (int i = 0; i < image.rows; i++)
    {
        uchar* data = image.ptr<uchar>(i);
        uchar* idata = img.ptr<uchar>(i);
        gray = 0;

        for (int j = 0; j < image.cols * 3; j += 3)
        {
            for (int x = 0; x < 3; x++)
            {
                gray = value[x][0] * data[j] + value[x][1] * data[j + 1] + value[x][2] * data[j + 2];
                //a = a < gray ? a : gray;
                //b = b > gray ? b : gray;
                // 
                gray = gray < 0 ? -gray : gray;
                gray = gray > 255 ? 255 : gray;
                idata[j + x] = gray;
            }
            /*blue = data[j];
            green = data[j + 1];
            red = data[j + 2];

            idata[j] = int(YIQ[0][0] * red + 0.587 * green + 0.114 * blue);
            idata[j + 1] = int(0.595716 * red - 0.274453 * green - 0.321263 * blue);
            idata[j + 2] = int(0.211456 * red - 0.522591 * green + 0.311135 * blue);*/
            /*	cout << int(0.299 * red + 0.587 * green + 0.114 * blue) << endl;
                cout << int(0.595716 * red - 0.274453 * green - 0.321263 * blue) << endl;
                cout << int(0.211456 * red - 0.522591 * green + 0.311135 * blue) << endl;*/
                //gray = gray > 255 ? 255 : gray;
                //gray = gray < 0 ? 0 : gray;
                //cout << (int)idata[j] << ", " << (int)idata[j + 1] << ", " << (int)idata[j + 2] << endl;
        }
    }
    //cout << a << ", " << b << endl;
}

// ���ؾ��
void Pixel_convolution(Mat image, const float value[][3], Mat& img, double a,double b)
{
    float d = 0.0;
    int gray = 0;
    double c = 255.0 / (b - a);
    int t1 = 0;
    int t2 = 0;

    for (int i = 0; i < image.rows; i++)
    {
        uchar* data = image.ptr<uchar>(i);
        uchar* idata = img.ptr<uchar>(i);
        gray = 0;

        for (int j = 0; j < image.cols * 3; j += 3)
        {
            for (int x = 0; x < 3; x++)
            {
                d = value[x][0] * data[j] + value[x][1] * data[j + 1] + value[x][2] * data[j + 2];
                gray = int((d - a) * c);
                t1 = t1 < gray ? t1 : gray;
                t2 = t2 > gray ? t2 : gray;

                idata[j + x] = gray;
            }
        }
    }
    cout << t1 << ", " << t2 << endl;
}

// ����������ɫ�ʾ��
void Pixel_convolution(Mat image, const float value[][3], Mat& img, double a, double b, vector<vector<float>> &buffer)
{
    float d = 0.0;
    int gray = 0;
    double c = 255.0 / (b - a);
    int t1 = 0;
    int t2 = 0;

    for (int i = 0; i < image.rows; i++)
    {
        uchar* data = image.ptr<uchar>(i);
        uchar* idata = img.ptr<uchar>(i);
        gray = 0;

        for (int j = 0; j < image.cols * 3; j += 3)
        {
            for (int x = 0; x < 3; x++)
            {
                d = value[x][0] * data[j] + value[x][1] * data[j + 1] + value[x][2] * data[j + 2];
                gray = int((d - a) * c);
                t1 = t1 < gray ? t1 : gray;
                t2 = t2 > gray ? t2 : gray;

                idata[j + x] = gray;
                buffer[i][j + x] = d;
            }
        }
    }
    cout << t1 << ", " << t2 << endl;
}

void Pixel_convolution(vector<vector<float>>& buffer, const float value[][3])
{
    if (buffer.size() == 0 || buffer[0].size() == 0) cout<< "error" <<endl;
    vector<vector<float>> cv(buffer.size(), vector<float>(buffer[0].size()));
    cv.swap(buffer);
    for (int i = 0; i < cv.size(); i++)
    {
        for (int j = 0; j < cv[0].size(); j += 3)
        {
            for (int x = 0; x < 3; x++)
            {
                buffer[i][j + x] = value[x][0] * cv[i][j] + value[x][1] * cv[i][j + 1] + value[x][2] * cv[i][j + 2];
            }
        }
    }
}

// ���ݹ淶��
void Standard(vector<vector<float>> buffer, Mat& image)
{
    if (buffer.size() == 0 || buffer[0].size() == 0) cout << "data error" << endl;
    int gray = 0;
    for (int i = 0; i < buffer.size(); i++)
    {
        uchar* data = image.ptr<uchar>(i);
        for (int j = 0; j < buffer[0].size(); j++)
        {
            gray = buffer[i][j];
            gray = gray > 255 ? 255 : gray;
            gray = gray < 0 ? 0 : gray;
            data[j] = gray;
        }
    }
}

void Standard(vector<vector<float>> buffer, Mat& image, double a, double b)
{
    if (buffer.size() == 0 || buffer[0].size() == 0) cout << "data error" << endl;
    double c = 255.0 / (b - a);
    for (int i = 0; i < buffer.size(); i++)
    {
        uchar* data = image.ptr<uchar>(i);
        for (int j = 0; j < buffer[0].size(); j++)
        {
            data[j] = int((buffer[i][j] - a) * c);
        }
    }
}

// ������ͼ��Ĳ�
void ImgMinus(Mat image, Mat rimg, Mat &err)
{
    for (int i = 0; i < image.rows; i++)
    {
        uchar* data = image.ptr<uchar>(i);
        uchar* idata = rimg.ptr<uchar>(i);
        uchar* e = err.ptr<uchar>(i);

        for (int j = 0; j < image.cols * 3; j += 3)
        {
            for (int k = 0; k < 3; k++)
            {
                e[j + k] = data[j + k] - idata[j + k];
            }
        }
    }
}

void Test(int process, Mat image)
{
    switch (process)
    {
    case 1:
        RGBShow(image);
        /*image=imread(imgpath, IMREAD_GRAYSCALE);
        imshow("1�Ҷ�ͼ", image);*/
        break;
    case 2:
        {
            // ����
            Mat img(image.rows, image.cols, CV_8UC3, Scalar(0, 0, 0));
            Mat rimg(image.rows, image.cols, CV_8UC3, Scalar(0, 0, 0));
            Mat err(image.rows, image.cols, CV_8UC3, Scalar(0, 0, 0));

            Pixel_convolution(image, YIQ, img);
            imshow("1��YIQɫ��ͼ", img);

            Pixel_convolution(img, RYIQ, rimg);
            imshow("1��YIQ��ԭRGBɫ��ͼ", rimg);

            Pixel_convolution(image, YIQ, img, -39, 227);
            imshow("2��YIQɫ��ͼ", img);
            Pixel_convolution(img, RYIQ, rimg, 0, 309);
            imshow("2��YIQ��ԭRGBɫ��ͼ", rimg);

            ImgMinus(image, rimg, err);
            imshow("errorͼ", err);
        }
        break;
    case 3:
        {
            //Mat kern = (Mat_<double>(3, 3) << 0.297226, 0.587809, 0.114965,  0.592189, -0.27283, -0.319359 ,  0.210212, -0.522018, 0.311806 );
            //Mat kern = (Mat_<double>(3, 3) << 0.0625, 0.125, 0.0625, 0.125, 0.25, 0.125, 0.0625, 0.125, 0.0625);
            Mat kern = (Mat_<int>(3, 3) << -1, -1, -1, -1, 8, -1, -1, -1, -1);

            Mat dstImage;
            filter2D(image, dstImage, image.depth(), kern);
            imshow("filter2D����", dstImage);
        }
        break;
    case 4:
        {
            int rows = image.rows;
            int cols = image.cols * 3; // ע��Խ��

            vector<vector<float>> buffer(rows, vector<float>(cols)); //�����ά��̬����rows��cols��
            Mat rimg(image.rows, image.cols, CV_8UC3, Scalar(0, 0, 0));

            double time0 = static_cast<double>(getTickCount());

            for (int i = 0; i < rows; i++)
            {
                uchar* data = image.ptr<uchar>(i);  //��ȡ��i�е��׵�ַ
                for (int j = 0; j < cols; j++)
                {
                    //buffer[i][j] = data[j] + 50;
                    buffer[i][j] = data[j];
                }
            }

            for (int i = 0; i < rows; i++)
            {
                uchar* data = rimg.ptr<uchar>(i);  //��ȡ��i�е��׵�ַ
                for (int j = 0; j < cols; j++)
                {
                    data[j] = buffer[i][j];
                }
            }

            time0 = ((double)getTickCount() - time0) / getTickFrequency();
            cout << "\t�˷�������ʱ��Ϊ�� " << time0 << "��" << endl;  //�������ʱ��

            imshow("���������Ĳ���", rimg);
        }
        break;
    case 5:
        {
            int rows = image.rows;
            int cols = image.cols * 3; // ע��Խ��

            vector<vector<float>> buffer(rows, vector<float>(cols)); //�����ά��̬����rows��cols��

            Mat img(image.rows, image.cols, CV_8UC3, Scalar(0, 0, 0));
            //Mat rimg(image.rows, image.cols, CV_8UC3, Scalar(0, 0, 0));
            Mat err(image.rows, image.cols, CV_8UC3, Scalar(0, 0, 0));
    
            for (int i = 0; i < rows; i++)
            {
    	        uchar* data = image.ptr<uchar>(i);  //��ȡ��i�е��׵�ַ
    	        for (int j = 0; j < cols; j++)
    	        {
    		        //buffer[i][j] = data[j] + 50;
    		        buffer[i][j] = data[j];
    	        }
            }

            if (buffer.size() == 0 || buffer[0].size() == 0) return ;

            Pixel_convolution(buffer, YIQ);
            //Standard(buffer, img, -39, 227);
            Standard(buffer, img);
            imshow("2��YIQɫ��ͼ", img);

            Pixel_convolution(buffer, RYIQ);
            //Standard(buffer, img, 0, 309);
            Standard(buffer, img);
            imshow("2��YIQ��ԭRGBɫ��ͼ", img);

            ImgMinus(image, img, err);
            imshow("errorͼ", err);
        }
        break;
    default:
        break;
    }
}