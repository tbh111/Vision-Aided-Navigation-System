
//main.cpp
//drone

//Created by ͯ���� on 2021/2/25.


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
#include <conio.h>
#include <time.h>
#include <thread>
#include "Socket_client.hpp"
#include "SURF_Matcher.hpp"

using namespace cv;
using namespace cv::xfeatures2d;
using namespace msr::airlib;
using namespace std;

thread::id main_thread_id = this_thread::get_id();

void videoPipeline(MultirotorRpcLibClient& Client, Mat& im) {
    typedef ImageCaptureBase::ImageRequest ImageRequest;
    typedef ImageCaptureBase::ImageResponse ImageResponse;
    typedef ImageCaptureBase::ImageType ImageType;
    typedef common_utils::FileSystem FileSystem;

    std::vector<ImageRequest> request = { ImageRequest("3", ImageType::Scene) };
    const std::vector<ImageResponse>& response = Client.simGetImages(request);
    if (response.size() > 0) {
        for (const ImageResponse& image_info : response) {

            im = imdecode(image_info.image_data_uint8, IMREAD_COLOR);

     /*       imshow("cam1", im);
            waitKey(1);*/
        }
    }

}

std::string saveOneImage(MultirotorRpcLibClient& Client, FILE* f)
{
    typedef ImageCaptureBase::ImageRequest ImageRequest;
    typedef ImageCaptureBase::ImageResponse ImageResponse;
    typedef ImageCaptureBase::ImageType ImageType;
    typedef common_utils::FileSystem FileSystem;
    std::vector<ImageRequest> request = { ImageRequest("3", ImageType::Scene) };
    const std::vector<ImageResponse>& response = Client.simGetImages(request);
    auto barometer_data = Client.getBarometerData();
    auto gps_data = Client.getGpsData();
    cout << gps_data.gnss.time_utc << endl;
    std::string path;
    Mat img;
    if (response.size() > 0) {
        path = "D:/drone/pic3/" + to_string(gps_data.gnss.time_utc) + ".jpg";
        for (const ImageResponse& image_info : response) {
            // cout << "Image uint8 size: " << image_info.image_data_uint8.size() << endl;
            // cout << "Image float size: " << image_info.image_data_float.size() << endl;
            // cout << "gnss.geo point: " << gps_data.gnss.geo_point << endl;
            // cout << "altitude" << setprecision(10) << gps_data.gnss.geo_point.altitude << endl;
            // cout << "latitude" << setprecision(10) << gps_data.gnss.geo_point.latitude << endl;
            // cout << "longitude" << setprecision(10) << gps_data.gnss.geo_point.longitude << endl;
            cv::Mat im = cv::imdecode(image_info.image_data_uint8, cv::IMREAD_COLOR);
            //cv::Mat dest = convertVector2Mat(image_info.image_data_uint8, 3, 1280);
            cv::imwrite(path, im);
            //write time strap and GPS information into text file
            fprintf(f, "%llu", gps_data.gnss.time_utc);
            fprintf(f, ",");
            fprintf(f, "%.7f", gps_data.gnss.geo_point.altitude);
            fprintf(f, ",");
            fprintf(f, "%.7f", gps_data.gnss.geo_point.latitude);
            fprintf(f, ",");
            fprintf(f, "%.7f", gps_data.gnss.geo_point.longitude);
            fprintf(f, "\n");
        }
    }
    return path;
}

void motion(MultirotorRpcLibClient& Client, FILE* fp, Socket_client& S, SURF_Matcher& matcher) {
    char a;
    cout << "Press key to move 5 meters with 1 m/s velocity" << endl;
    a = getch();
    if (a == 'w') {
        auto position = Client.getMultirotorState().getPosition(); // from current location
        //client.moveToPositionAsync(position.x() + 5, position.y(), position.z(), 1)->waitOnLastTask();
        cout << "forward" << endl;
        Client.moveToPositionAsync(position.x() + 10, position.y(), position.z(), 5);
    }
    /*else if (a == 'W') {
        client.moveByVelocityZAsync(5, 0, client.getMultirotorState().getPosition().z(), 5);
    }*/
    else if (a == 'a') {
        auto position = Client.getMultirotorState().getPosition(); // from current location
        Client.moveToPositionAsync(position.x(), position.y() - 10, position.z(), 5);
        cout << "left" << endl;
    }
    else if (a == 's') {
        auto position = Client.getMultirotorState().getPosition(); // from current location
        Client.moveToPositionAsync(position.x() - 10, position.y(), position.z(), 5);
        cout << "backward" << endl;
    }
    else if (a == 'd') {
        auto position = Client.getMultirotorState().getPosition(); // from current location
        Client.moveToPositionAsync(position.x(), position.y() + 10, position.z(), 5);
        cout << "right" << endl;
    }
    else if (a == 'e') {
        auto position = Client.getMultirotorState().getPosition(); // from current location
        Client.moveToPositionAsync(position.x(), position.y(), position.z() + 10, 5);
        cout << "down" << endl;
    }
    else if (a == 'q') {
        auto position = Client.getMultirotorState().getPosition(); // from current location
        Client.moveToPositionAsync(position.x(), position.y(), position.z() - 10, 5);
        cout << "up" << endl;
    }
    else if (a == 'r') {
        string path;
        saveOneImage(Client, fp);
    }

    else if (a == 'm') {
        
        std::string path = saveOneImage(Client, fp);
        S.connect(path);
    }
    else if (a == 'p') {
        Mat im1, im2;
        videoPipeline(Client, im1);
        cout << "get im1" << endl;
        imwrite("im1.jpg", im1);
        auto position = Client.getMultirotorState().getPosition(); // from current location
        //frame.copyTo(im1);
        //imshow("im1", im1);
        Client.moveToPositionAsync(position.x() + 20, position.y(), position.z(), 2);
        Sleep(5000);
        videoPipeline(Client, im2);
        cout << "get im2" << endl;
        imwrite("im2.jpg", im2);
        //frame.copyTo(im2);
        //imshow("im2", im2);
        //waitKey(0);
        matcher.img1 = im1;
        matcher.img2 = im2;
        matcher.start_match();
    }
    else if (a == 'o') {
        Mat im1, im2;
        videoPipeline(Client, im1);
        cout << "get im1" << endl;
        imwrite("im1.jpg", im1);
        auto position = Client.getMultirotorState().getPosition(); // from current location
        //frame.copyTo(im1);
        //imshow("im1", im1);
        Client.moveToPositionAsync(position.x() - 20, position.y(), position.z(), 2);
        Sleep(5000);
        videoPipeline(Client, im2);
        cout << "get im2" << endl;
        imwrite("im2.jpg", im2);
        //frame.copyTo(im2);
        //imshow("im2", im2);
        //waitKey(0);
        matcher.img1 = im1;
        matcher.img2 = im2;
        matcher.done_flag = true;
        //matcher.start_match();
    }
}

void getData(MultirotorRpcLibClient& Client, FILE* fp) {
    Mat frame;
    string path;
    cout << Client.simGetVehiclePose().position << endl;
    Pose start_pos = Pose(Vector3r(0, 0, -80), Quaternionr(1, 0, 0, 0));
    auto time = Client.getGpsData().gnss.time_utc;
    Client.simSetVehiclePose(start_pos, 0);

    for (auto i = -100; i <= 100; i = i + 100)
    {
        for (auto j = -120; j <= 120; j = j + 120)
        {
            Pose current_pos = Pose(Vector3r(i, j, -80), Quaternionr(1, 0, 0, 0));
            Client.simSetVehiclePose(current_pos, 0);
            cout << "current pose" << Client.simGetVehiclePose().position << endl;
            Sleep(1000);
            //saveOneImage(Client, fp, path);
            Sleep(1000);
        }

    }
    

}

int main(int argc, const char* argv[]) {
    Socket_client S;
    SURF_Matcher matcher;
    FILE* fp = fopen("result.txt", "a");
    msr::airlib::MultirotorRpcLibClient client;
    Mat im1, im2;
    thread SURF_thread(&SURF_Matcher::start_match, &matcher);

    client.confirmConnection();
    cout << "Press Enter to enable API control" << endl; cin.get();
    client.enableApiControl(true);

    cout << "Press Enter to arm the drone" << endl; cin.get();
    client.armDisarm(true);

    cout << "Press Enter to takeoff" << endl; cin.get();
    client.takeoffAsync(5)->waitOnLastTask();
    Mat frame;
    while (1) {

        //videoPipeline(client, frame);
 
        motion(client, fp, S, matcher);
        //videoPipeline(client, frame);
    }

    //cout << "Press Enter to land" << endl; cin.get();
    //client.landAsync(5)->waitOnLastTask();
    //fclose(fp);
    SURF_thread.detach();
    cout << "SURF thread killed" << endl;
    return 0;

    // Mat img1, img2;
    // SURF_Matcher matcher(img1, img2);
    // matcher.start_match();

}

