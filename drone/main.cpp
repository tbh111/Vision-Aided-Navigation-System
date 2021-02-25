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
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

using namespace cv;
using namespace msr::airlib;
using namespace std;

int main(int argc, const char * argv[]) {
    // insert code here...
    cout << "Hello, World!\n";
    cout << CV_VERSION << endl;
    msr::airlib::MultirotorRpcLibClient client;
    client.confirmConnection();
    return 0;
}
