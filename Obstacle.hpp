#ifndef OBSTACLE_HPP_INCLUDED
#define OBSTACLE_HPP_INCLUDED

#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

using namespace std;
using namespace cv;

class Obstacle
{
public:
    Obstacle();
    void init();
    bool detect_obstacle(cv::Mat frame);

private:

};

#endif // OBSTACLE_HPP_INCLUDED
