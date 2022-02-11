# Face_Detection
It mainly uses local description feature descriptor and interest points to design the process based on face detection, emotion recognition and age recognition

## 编译环境

* Windows 10
* Microsoft Visual Studio Community 2019
* OpenCV_VERSION 4.5.4
* opencv_contrib-4.5.4
* CMake 3.22.2

编译原因：项目中需要用到 [opencv_contrib](https://github.com/opencv/opencv_contrib/tags) 中的一些方法，如：face.hpp等。

编译方法：使用 [cmake](https://cmake.org/) + VS2019 编译源码 [OpenCV 4.5.4](https://opencv.org/) 和 opencv_contrib 4.5.4。

编译目标：生成方便VS使用的lib库和dll文件。（注：忽略生成opencv_python.lib）

### 编译步骤

1. 下载好相应的源文件，并解压（一定要检查文件是否下载齐全！）

![](.//Detection//image//file//1.png)

2. 设置cmake的相关内容，并configure

![](.//Detection//image//file//2.png)

3. 配置几个变量（各个变量的意思可到官网上查询）
* OPENCV_EXTRA_MODULES_PATH : E:/DEV C++/opencv_contrib-4.5.4/modules （注：根据自己解压后的路径修改）
* OPENCV_ENABLE_NONFREE : √
* OPENCV_GENERATE_SETUPVARS : 不打√

（注：尽量不选BUILD_OPENCV_WORLD，该值选上后，你在编译完成时，会把各种lib库打包成一个。之所以建议不选，是因为在编译时，可能会出现各种问题，如果中间某个包出现错误，则得不偿失！）

4. 可能遇到的问题：
* cmake下载某些中间文件不成功，一般下载的文件会放置在opencv源文件的.cache文件中，可以通过cmake的提示，找到CMakeDownloadLog.txt文件，查看下载失败的内容。下载失败原因，主要有网络不稳定，不是外网，自己电脑的Git设置了SSH秘钥……。
* VS编译出错，一般是cmake操作时没有把所欲标红的情况解决完，就生成project。或者是源文件缺失，或者是源文件出错。

5. 解决方法：
* 可以通过迅雷下载中间文件，只需要把CMakeDownloadLog.txt中提示的网址一个一个下载，并重命名（规则看提示），放置在相应的文件加下（看提示）。

6. VS打开cmake配置好的项目OpenCV.sln，点击【生成】→【批生成】→【ALL BUILD】→【debug】→【生成】，编译完后，【生成】→【批生成】→【INSTALL】→【debug】→【生成】。

7. 一切做完后，可以把install文件中的内容拿出来使用。

