#pragma once
#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


/*
*	���������������淶����
*	Date��2022/1/24
*	Author��ZLC
*		RESULT_ASIS��ԭ��
*		RESULT_PLUS_VALUE���������ֵ
*		RESULT_MINUS_VALUE�������ȥֵ
*		RESULT_PLUS_ORIGINAL_TIMES_VALUE���������ԭʼֵ����ֵ
*		RESULT_MINUS_ORIGINAL_TIMES_VALUE�������ȥԭʼֵ����ֵ
*		ORIGINAL_PLUS_RESULT_TIMES_VALUE��ԭʼֵ���Ͻ������ֵ
*		ORIGINAL_MINUS_RESULT_TIMES_VALUE��ԭʼֵ��ȥ�������ֵ
*		ORIGINAL_LOW_CLIP�����ȡֵ���
*		ORIGINAL_HIGH_CLIP�����ȡֵС��
*		
*/
#define RESULT_ASIS 0 
#define RESULT_PLUS_VALUE 1
#define RESULT_MINUS_VALUE 2
#define RESULT_PLUS_ORIGINAL_TIMES_VALUE 3
#define RESULT_MINUS_ORIGINAL_TIMES_VALUE 4
#define ORIGINAL_PLUS_RESULT_TIMES_VALUE 5
#define ORIGINAL_MINUS_RESULT_TIMES_VALUE 6
#define ORIGINAL_LOW_CLIP 7
#define ORIGINAL_HIGH_CLIP 8




// �淶����ģ��
//template <typename T> T Max(T a, T b) {
//    return a > b ? a : b;
//}
template <typename T> T StandardData(T sum, T value, T result, int post_processor = 0)
{
	switch (post_processor)
	{
	case RESULT_ASIS:
		break;
	case RESULT_PLUS_VALUE:
		sum += value;
		break;
	case RESULT_MINUS_VALUE:
		sum -= value;
		break;
	case RESULT_PLUS_ORIGINAL_TIMES_VALUE:
		sum = sum + (result * value);
		break;
	case RESULT_MINUS_ORIGINAL_TIMES_VALUE:
		sum = sum - (result * value);
		break;
	case ORIGINAL_PLUS_RESULT_TIMES_VALUE:
		sum = result + (sum * value);
		break;
	case ORIGINAL_MINUS_RESULT_TIMES_VALUE:
		sum = result - (sum * value);
		break;
	case ORIGINAL_LOW_CLIP:
		sum = (result < value ? value : result);
		break;
	case ORIGINAL_HIGH_CLIP:
		sum = (result > value ? value : result);
		break;
	}
	return sum;
}


