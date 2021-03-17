//
//  SURF_Matcher.hpp
//  drone
//
//  Created by Í¯²©º­ on 2021/2/26.
//

#ifndef SURF_Matcher_hpp
#define SURF_Matcher_hpp

#include <cstdio>
#define _USE_MATH_DEFINES
#include <cmath>
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
#include <Eigen/Core>
#include <Eigen/Dense>
#include <opencv2/core/eigen.hpp>
#include <vector>
#include <thread>
using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

class SURF_Matcher {
public:
    SURF_Matcher();
    SURF_Matcher(Mat input_img1, Mat input_img2);
    ~SURF_Matcher();
    void solveICP(std::vector<Point2f> ps1, std::vector<Point2f> ps2);
    Mat img1, img2;
    bool done_flag = false;
    void start_thread();
    float height;
    float dx = 0.0;
    float dy = 0.0;
private:
    Mat cam, dist;
    int direction = 0;
    struct SURFDetector;
    template<class KPMatcher> struct SURFMatcher;
    void workBegin();
    void workEnd();
    double getTime();
    void start_match();
    void estimatePose(Mat F, std::vector<Point2f> ps1, std::vector<Point2f> ps2);
    std::vector<Point2f> obj;
    std::vector<Point2f> scene;
    Mat drawGoodMatches(const Mat& img1,
        const Mat& img2,
        const std::vector<KeyPoint>& keypoints1,
        const std::vector<KeyPoint>& keypoints2,
        std::vector<DMatch>& matches,
        std::vector<Point2f>& scene_corners_);

};

#endif /* SURF_Matcher_hpp */
