//
//  SURF_Matcher.cpp
//  drone
//
//  Created by 童博涵 on 2021/2/26.
//

#include "SURF_Matcher.hpp"
const int LOOP_NUM = 1;
const int GOOD_PTS_MAX = 50;
const float GOOD_PORTION = 0.15f;

int64 work_begin = 0;
int64 work_end = 0;
SURF_Matcher::SURF_Matcher() {

}
SURF_Matcher::SURF_Matcher(Mat input_img1, Mat input_img2) {
    this->img1 = input_img1;
    this->img2 = input_img2;
}
SURF_Matcher::~SURF_Matcher() {

}

void SURF_Matcher::workBegin()
{
    work_begin = getTickCount();
}

void SURF_Matcher::workEnd()
{
    work_end = getTickCount() - work_begin;
}

double SURF_Matcher::getTime()
{
    return work_end / ((double)getTickFrequency()) * 1000.;
}

struct SURF_Matcher::SURFDetector
{
    Ptr<Feature2D> surf;
    SURFDetector(double hessian = 500.0)
    {
        surf = SURF::create(hessian);
    }
    template<class T>
    void operator()(const T& in, const T& mask, std::vector<cv::KeyPoint>& pts, T& descriptors, bool useProvided = false)
    {
        surf->detectAndCompute(in, mask, pts, descriptors, useProvided);
    }
};

template<class KPMatcher>
struct SURF_Matcher::SURFMatcher
{
    KPMatcher matcher;
    template<class T>
    void match(const T& in1, const T& in2, std::vector<cv::DMatch>& matches)
    {
        matcher.match(in1, in2, matches);
    }
};

void SURF_Matcher::start_thread() {
    thread SURF_thread(&SURF_Matcher::start_match, this);
    SURF_thread.detach();
}

void SURF_Matcher::start_match() {

    if (done_flag == false) {
        cout << "initizing SURF matcher" << endl;
        //instantiate detectors/matchers
        SURF_Matcher::SURFDetector surf;
        SURF_Matcher::SURFMatcher<BFMatcher> matcher;

        //declare input/output
        std::vector<KeyPoint> keypoints1, keypoints2;
        std::vector<DMatch> matches;
        Mat descriptors1, descriptors2;
        double surf_time = 0.;
        //-- start of timing section

        for (int i = 0; i <= LOOP_NUM; i++)
        {
            if (i == 1) workBegin();
            surf(img1, Mat(), keypoints1, descriptors1);
            surf(img2, Mat(), keypoints2, descriptors2);
            matcher.match(descriptors1, descriptors2, matches);
        }
        workEnd();
        std::cout << "FOUND " << keypoints1.size() << " keypoints on first image" << std::endl;
        std::cout << "FOUND " << keypoints2.size() << " keypoints on second image" << std::endl;

        surf_time = getTime();
        std::cout << "SURF run time: " << surf_time / LOOP_NUM << " ms" << std::endl << "\n";


        std::vector<Point2f> corner;
        Mat img_matches = drawGoodMatches(img1, img2, keypoints1, keypoints2, matches, corner);

        // 对齐匹配的点对
        vector<KeyPoint> alignedpt1, alignedpt2;
        for (auto i = 0; i < matches.size(); i++) {
            alignedpt1.push_back(keypoints1[matches[i].queryIdx]);
            alignedpt2.push_back(keypoints2[matches[i].trainIdx]);
        }
        // 取得特征点的像素坐标
        vector<Point2f> ps1, ps2;
        for (auto i = 0; i < alignedpt1.size(); i++) {
            ps1.push_back(alignedpt1[i].pt);
            ps2.push_back(alignedpt2[i].pt);
        }
        // 计算基础矩阵
        Mat F;
        F = findFundamentalMat(ps1, ps2, FM_RANSAC);
        cout << F << endl;
        estimatePose(F);
        //-- Show detected matches

        //namedWindow("surf matches", 0);
        //imshow("surf matches", img_matches);
        imwrite("match.jpg", img_matches);
        //waitKey(1000);
        //destroyAllWindows();
        done_flag = true;
        cout << "SURF process done" << endl;
    }
    else
    {
        cout << "waiting" << endl;
        return;
    }
}


Mat SURF_Matcher::drawGoodMatches(const Mat& im1,
    const Mat& im2,
    const std::vector<KeyPoint>& keypoints1,
    const std::vector<KeyPoint>& keypoints2,
    std::vector<DMatch>& matches,
    std::vector<Point2f>& scene_corners_) {

    //-- Sort matches and preserve top 10% matches
    std::sort(matches.begin(), matches.end());
    std::vector< DMatch > good_matches;
    double minDist = matches.front().distance;
    double maxDist = matches.back().distance;

    const int ptsPairs = std::min(GOOD_PTS_MAX, (int)(matches.size() * GOOD_PORTION));
    for (int i = 0; i < ptsPairs; i++)
    {
        good_matches.push_back(matches[i]);
    }
    std::cout << "\nMax distance: " << maxDist << std::endl;
    std::cout << "Min distance: " << minDist << std::endl;

    std::cout << "Calculating homography using " << ptsPairs << " point pairs." << std::endl;

    // drawing the results
    Mat img_matches;

    drawMatches(im1, keypoints1, im2, keypoints2,
        good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
        std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    //-- Localize the object
    std::vector<Point2f> obj;
    std::vector<Point2f> scene;

    for (size_t i = 0; i < good_matches.size(); i++)
    {
        //-- Get the keypoints from the good matches
        obj.push_back(keypoints1[good_matches[i].queryIdx].pt);
        scene.push_back(keypoints2[good_matches[i].trainIdx].pt);
    }
    //-- Get the corners from the image_1 ( the object to be "detected" )
    std::vector<Point2f> obj_corners(4);
    obj_corners[0] = Point(0, 0);
    obj_corners[1] = Point(img1.cols, 0);
    obj_corners[2] = Point(img1.cols, img1.rows);
    obj_corners[3] = Point(0, img1.rows);
    std::vector<Point2f> scene_corners(4);

    Mat H = findHomography(obj, scene, RANSAC);
    perspectiveTransform(obj_corners, scene_corners, H);

    scene_corners_ = scene_corners;

    //-- Draw lines between the corners (the mapped object in the scene - image_2 )
    line(img_matches,
        scene_corners[0] + Point2f((float)img1.cols, 0), scene_corners[1] + Point2f((float)img1.cols, 0),
        Scalar(0, 255, 0), 2, LINE_AA);
    line(img_matches,
        scene_corners[1] + Point2f((float)img1.cols, 0), scene_corners[2] + Point2f((float)img1.cols, 0),
        Scalar(0, 255, 0), 2, LINE_AA);
    line(img_matches,
        scene_corners[2] + Point2f((float)img1.cols, 0), scene_corners[3] + Point2f((float)img1.cols, 0),
        Scalar(0, 255, 0), 2, LINE_AA);
    line(img_matches,
        scene_corners[3] + Point2f((float)img1.cols, 0), scene_corners[0] + Point2f((float)img1.cols, 0),
        Scalar(0, 255, 0), 2, LINE_AA);
    return img_matches;
}

void SURF_Matcher::solveICP(vector<Point2f> ps1, vector<Point2f> ps2) {
    // 使用opencv Surface Matching 模块中的ICP函数， 匹配三维点云 -> 使用Airsim中深度相机
    // 后续完成
}

void SURF_Matcher::estimatePose(Mat F) {
    // 估计相机内参
    double CameraFOV = 90;
    double fx = img1.rows / (2 * tan(CameraFOV * M_PI / 360));
    double fy = fx;
    double cu = img1.rows / 2;
    double cv = img1.cols / 2;
    Mat cam = (Mat_<double>(3, 3) << fx, 0, cu, 0, fy, cv, 0, 0, 1);
    Mat dist = (Mat_<double>(4, 1) << 0, 0, 0, 0);

    Mat_<double> w, u, vt;
    Mat E = cam.t() * F * cam;
    SVD svd(E);
    Mat W = (Mat_<double>(3, 3) << 0, -1, 0, 1, 0, 0, 0, 0, 1);
    Mat_<double> R = svd.u * W * svd.vt;
    Mat_<double> t = svd.u.col(2);
    //    SVD::compute(F, w, u, vt);
    //    Mat ut = (Mat_<double>(3,1) << 0, 0, 1);
    //    Mat_<double> u1(1, 3, CV_64FC1);
    //    u1 = u * ut;
    //    Mat W = (Mat_<double>(3,3) << 0, -1, 0, 1, 0, 0, 0, 0, 1);
    //    Mat_<double> rvec, tvec;
    //    rvec = u * W * vt;
    //    tvec = u1;
    cout << "rotation matrix" << R << endl;
    cout << "translation vector" << t << endl;

    // 计算坐标变化量
    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> R_n;
    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> T_n;
    cv2eigen(R, R_n);
    cv2eigen(t, T_n);
    Eigen::Vector3f P_oc;
    P_oc = -R_n.inverse() * T_n;
    string str1 = to_string(P_oc(0));
    string str2 = to_string(P_oc(1));
    string str3 = to_string(P_oc(2));
    cout << "x:" << str1 << endl;
    cout << "y:" << str2 << endl;
    cout << "z:" << str3 << endl;
}
