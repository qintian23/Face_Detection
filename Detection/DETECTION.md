## 特征集

1. 面部特征点的相对位置，如眼睛，眉毛和嘴。
2. 用特征点之间的相对比例和比值来确定年龄、性别和情感。
3. 在每个特征点上的FREAK描述子。
4. 眼睛颜色。

## Eigen Face 

* 人脸数据
* 平均脸
* 特征脸

开始--读取训练数据--计算平均脸--计算协方差矩阵--计算特征值--特征矩阵--PCA降维--子空间模型--检测/距离

下载opencv的工具：
https://github.com/opencv/opencv_contrib/tags
选择适合自己版本的下载。

https://www.cnblogs.com/qxym2016/p/7103273.html?utm_source=itdadao

https://blog.csdn.net/qinchang1/article/details/105776132/

https://docs.opencv.org/4.5.5/da/d60/tutorial_face_main.html