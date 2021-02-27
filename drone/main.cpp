//
//  main.cpp
//  drone
//
//  Created by 童博涵 on 2021/2/25.
//

#include <common/common_utils/StrictMode.hpp>
STRICT_MODE_OFF
#ifndef RPCLIB_MSGPACK
#define RPCLIB_MSGPACK clmdep_msgpack
#endif // !RPCLIB_MSGPACK
#include "rpc/rpc_error.h"
STRICT_MODE_ON
#include <vehicles/multirotor/api/MultirotorRpcLibClient.hpp>
#include <api/RpcLibClientBase.hpp>
#include <common/common_utils/FileSystem.hpp>
#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/xfeatures2d.hpp>
#include "SURF_Matcher.hpp"

using namespace cv;
using namespace cv::xfeatures2d;
using namespace msr::airlib;
using namespace std;



int main(int argc, const char * argv[]) {
    // insert code here...
    cout << "Hello, World!\n";
    cout << CV_VERSION << endl;
//
//    msr::airlib::MultirotorRpcLibClient client;
//    client.confirmConnection();
    Mat img1, img2;
    img1 = imread("/Users/tongbohan/Desktop/Vision-Aided-Navigation-System/pic/1606108492067071.jpg", IMREAD_GRAYSCALE);
    img2 = imread("/Users/tongbohan/Desktop/Vision-Aided-Navigation-System/pic/1606108515398569.jpg", IMREAD_GRAYSCALE);
    resize(img1, img1, Size(img1.cols/8, img1.rows/8), 0, 0, INTER_AREA);
    resize(img2, img2, Size(img2.cols/8, img2.rows/8), 0, 0, INTER_AREA);

    SURF_Matcher matcher(img1, img2);
    matcher.start_match();
    
    return 0;
}
