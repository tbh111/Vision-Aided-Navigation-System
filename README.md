# Vision-Aided-Navigation-System
 A navigation system for UAV based on SIFT/SURF Features & CNN(finished).

#### 1. System environment

This project is based on Airsim library for drone simulation by Microsoft and Unreal Engine, and it is composed of upper & lower client, the development environment is listed as follows:

```
For lower client(the interface to simulation client):
On Windows:
Visual Studio 2019, OpenCV 4.4.0, AirLib 1.2, Eigen 3, C++
On macOS(not completed):
MacOS Big Sur 11.2.1, Xcode 12.4, OpenCV 4.0.0, Eigen 3, AirLib 1.2, C++
For upper client(the interface to users):
A simple Qt5 Widget Application with mingw7.3.0, Opencv 4.4.0, C++
```

#### 2. Movement estimation

If we want to know how the drone moves, without using imu data, we can use two photos shot by the camera on the UAV, find the feature points on them with popular algorithms like SIFT, SURF and ORB. In this system we use SURF, because it is faster than SIFT, and more accurate than ORB. After finding 50 best feature points, we calculate the essential matrix for UAV's movement, then we can estimate the rotation and translation vector.

The size of image we use is 640*480, and the average time cost for matching each two images is 1000ms, we use OpenGL to accelerate the process. (may use SURF_CUDA if it has better performance on my GTX 1660S)

#### 3. Location recognition

After getting the movement of UAV, we still need to know where the UAV is. So, to locate the UAV, totally based on the camera on it, here we can use a simple CNN: VGG-16/ResNet50/DenseNet to recognize the location, I choose ResNet50 currently. This method is pretty similar with the technology called search image. Compare the real-time image with the pre-stored map set, calculate the scores with each sliced map, finally we will get the UAV's location. Note that the height of which map and the image are taken should be same as far as possible.

This part of system is implemented with python and keras library. The image is sent to this python server by socket & UDP, then it would send the location information back to the main client.

~~Update 2021.3.24:~~

~~It was sad that this method performed poor in reality, I'm looking for the TF-IDF method which is widely used in loop closure in slam.~~

~~Update 2021.3.27:~~

~~It is a pity that the upper methods performs poor, expectedly. So I'm looking for another traditional(no Deep-Learning) method to get over this last obstacle.:weary: I will also upload this bullshit code later.~~

~~Update 2021.3.28:~~
~~I used the same methods as in section 2, that is, matching the image taken by camera with the image database, I will get it tomorrow.~~

#### 4. Upper client

The upper client is designed to have a GUI, so users can have a better experience. This application can show the environment's map, the location and movement of the drone. And you can control the drone with the button on the panel. This progress is done with socket & TCP.  The location of the drone will be shown on the map in the Qt client, here I use a simple red dot.

PS: At first, I figured the above function 1-3 on Visual Studio, and when I want to move it on Qt, but I find it's difficult to compile Airlib with mingw, so I grow this thought of develop a upper client.:confused:

#### 5. Future

I'm struggling to finish this project before April, 2021, the deadline of ourCollege Students' Innovative Entrepreneurial Training Plan Program(national grade) in DLUT, ~~as the other two members of my team are busy with their courses:sweat_smile:~~. Maybe, I will move this system on my Jetson nano, as it is more similar to actual applications:smile:.

Note: I'm just a common college student, feel free to contact me for comments on my work:handshake:.
