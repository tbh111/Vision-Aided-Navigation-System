# Vision-Aided-Navigation-System
 A navigation system for UAV based on SIFT/SURF Features & CNN.

#### 1. System environment

This project is based on Airsim library for drone simulation by Microsoft and Unreal Engine, the development environment is listed as follows:

```
On Windows:
Visual Studio 2019, OpenCV 4.4.0, AirLib 1.2, Eigen 3, C++
Pycharm community 2020.2.3, Python 3.6, Keras 2.2.0, tensorflow-gpu 1.10.0, Opencv 4.4.0
On macOS:
MacOS Big Sur 11.2.1, Xcode 12.4, OpenCV 4.0.0, Eigen 3, AirLib 1.2, C++
Qt Widget Application: Under development
```

#### 2. Movement estimation

If we want to know how the drone moves, without using imu data, we can use two photos shot by the camera on the UAV, find the feature points on them with popular algorithms like SIFT, SURF and ORB. In this system we use SURF, because it is faster than SIFT, and more accurate than ORB. After finding 50 best feature points, we calculate the essential matrix for UAV's movement, then we can estimate the rotation and translation vector.

The size of image we use is 640*480, and the average time cost for matching each two images is 1000ms, we use OpenGL to accelerate the process. (may use SURF_CUDA if it has better performance on my GTX 1660S)

#### 3. Location recognition

After getting the movement of UAV, we still need to know where the UAV is. So, to locate the UAV, totally based on the camera on it, here we use a simple CNN: VGG-16/Resnet/Densenet to recognize the location. This method is pretty similar with the technology called search image. Compare the real-time image with the pre-stored map set, calculate the scores with each sliced map, finally we will get the UAV's location.(This method is not very accurate, so I'm considering to do a second recognition in smaller sliced maps.)

This part of system is implemented with python and keras library. The image is sent to this python server by socket & UDP, then it would send the location information back to the main client.

#### 4. Future

I'm struggling to finish this project before April, 2021, the deadline of ourCollege Students' Innovative Entrepreneurial Training Plan Program(national grade) in DLUT, as the other two members of my team are busy with their courses:sweat_smile:. Later I'll first try to plot the figure of UAV's flight path, and develop a GUI application with Qt 5. Maybe, I will move this system on my Jaston nano, as it is more similar to actual applications:smile:.