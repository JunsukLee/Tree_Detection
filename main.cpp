#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
#include <stdio.h>
#include "cv.h"
#include "highgui.h"
#include <vector>
#include <sys/time.h>

#define   width_remove 80

using namespace cv;
using namespace std;

void detected_tree_color();
cv::Mat detectAndDisplay(cv::Mat frame);
void CallBackFunc(int event, int x, int y, int flags, void* userdata);

String tree_cascade_name = "result.xml";
CascadeClassifier tree_cascade;

cv::Mat src, src_trainning, src_hls;
cv::Mat dst, detected_edges;

std::string tree_str = "Detection Tree!!";

vector<int> height_line_check;


int main(int argc, char *argv[])
{
    struct timeval start_point, end_point; // TIME Measure
    double operating_time;                 // TIME Measure

    height_line_check.assign(640-width_remove*2, 0);

    /// Load an image
    //src = imread("/home/junsuk/study/Capstone_2016/tree_image/tree_image/20161001_181123.jpg");
    //src = imread("/home/junsuk/study/Capstone_2016/tree_image/tree_image/20161001_100700.jpg");
    //src = imread("/home/junsuk/study/Capstone_2016/tree_image/tree_image/20161001_100821.jpg");
    //src = imread("/home/junsuk/study/Capstone_2016/tree_image/tree_image/20161001_100954.jpg");
    //src = imread("/home/junsuk/study/Capstone_2016/tree_image/tree_image/20161001_181133.jpg");

    /// Load an Video
    cv::VideoCapture video("./1.mp4");
    if(!video.isOpened()){std::cout << "Video_File is not existed." << std::endl; return 0;}
    video.set(CAP_PROP_FRAME_WIDTH,  640);
    video.set(CAP_PROP_FRAME_HEIGHT, 480);

    /// Create a window
    namedWindow( "Default", CV_WINDOW_AUTOSIZE );
    //namedWindow( "src_hls", CV_WINDOW_AUTOSIZE );
    //namedWindow( "Trainning", CV_WINDOW_AUTOSIZE );

    if( !tree_cascade.load( tree_cascade_name ) ){ printf("--(!)Error loading tree cascade\n"); return -1; };



    while(1)
    {
        gettimeofday(&start_point, NULL);      // TIME Measure
        video >> src;
        cv::resize(src, src, Size(640, 480), 0, 0, CV_INTER_NN);

        /// Convert the image to grayscale
        cvtColor( src, src_hls,  CV_BGR2HLS );

        height_line_check.assign(640-width_remove*2, 0);

        detected_tree_color();
        imshow( "Default", src );
        //imshow( "src_hls", src_hls );

        if (waitKey(30) >= 0) break;

        gettimeofday(&end_point, NULL); // TIME Measure
        operating_time = (double)(end_point.tv_sec)+(double)(end_point.tv_usec)/1000000.0-(double)(start_point.tv_sec)-(double)(start_point.tv_usec)/1000000.0; // TIME Measure
        std::cout << "Execute Time : " << operating_time << std::endl; // TIME Measure
    }


/*
    cv::resize(src, src, Size(640, 480), 0, 0, CV_INTER_NN);
    src_trainning = src.clone();
    if( !src.data )
    { return -1; }
    if( !tree_cascade.load( tree_cascade_name ) ){ printf("--(!)Error loading tree cascade\n"); return -1; };

    /// Create a matrix of the same type and size as src (for dst)
    dst.create( src.size(), src.type() );

    /// Convert the image to grayscale
    cvtColor( src, src_hls,  CV_BGR2HLS );




    cv::setMouseCallback("src_hls", CallBackFunc, NULL);
    detected_tree_color();
    //detectAndDisplay(src_trainning);


    imshow( "Default", src );
    imshow( "src_hls", src_hls );
    //imshow( "Trainning", src_trainning );

    /// Create a Trackbar for user to enter threshold
    ///createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );

    /// Show the image
    ///CannyThreshold(0, 0);

    /// Wait until user exit program by pressing a key
    waitKey(0);
*/


    return 0;

}

void detected_tree_color()
{
    for(int row=0; row < src_hls.rows; row++){
        for(int col=(0+width_remove); col < (src_hls.cols-width_remove); col++){
            if(((int)src_hls.at<Vec3b>(row, col)[1] >= 15) && ((int)src_hls.at<Vec3b>(row, col)[1] <= 60)){
                if((int)src_hls.at<Vec3b>(row, col)[0] >= 0 && (int)src_hls.at<Vec3b>(row, col)[0] <= 35)
                    if((int)src_hls.at<Vec3b>(row, col)[2] >= 0 && (int)src_hls.at<Vec3b>(row, col)[2] <= 90){
                        //rectangle(src, Rect(row, col, 1 , 1), Scalar(255,0,255), 1);
                        rectangle(src_hls, Rect(row, col, 1 , 1), Scalar(0,255,255), 1);
                        height_line_check[col-width_remove]++;
                    }
            }
        }
    }
    int max_value=0, location=-1;
    int second_value=0, second_location=-1;
    for(int col=(0+width_remove); col < (src_hls.cols-width_remove); col++){
        if(max_value < height_line_check.at(col-width_remove)){
            second_value    = max_value;
            second_location = location;

            max_value = height_line_check.at(col-width_remove);
            location = col;
        }
    }
    height_line_check.clear();
    //printf("location  : %d\n", location);
    //printf("max_value : %d\n", max_value);
    //rectangle(src_hls, Rect(0, location, 200 , 200), Scalar(160,160,160), 20);

    if(max_value > 100){
        if(location != -1){
            rectangle(src, Rect(location, 0, 20 , src_hls.rows), Scalar(0,255,255), 5);
            cv::putText(src, tree_str.c_str(), cv::Point(200, 300), 2, 1.0, Scalar(0,0,255), 2);
            //rectangle(src_hls, Rect(location, 0, 10 , src_hls.rows), Scalar(160,160,160), 2);
            //cv::putText(src_hls, tree_str.c_str(), cv::Point(200, 300), 2, 1.0, Scalar(0,0,255), 2);
        }
        if(second_location != -1){
            rectangle(src, Rect(second_location, 0, 20 , src_hls.rows), Scalar(0,255,255), 5);
            cv::putText(src, tree_str.c_str(), cv::Point(200, 300), 2, 1.0, Scalar(0,0,255), 2);
            //rectangle(src_hls, Rect(second_location, 0, 10 , src_hls.rows), Scalar(160,160,160), 2);
            //cv::putText(src_hls, tree_str.c_str(), cv::Point(200, 300), 2, 1.0, Scalar(0,0,255), 2);
        }
    }
}

cv::Mat detectAndDisplay(cv::Mat frame)
{
    std::vector<Rect> trees;
    cv::Mat frame_gray;

    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    ///Detect trees
    tree_cascade.detectMultiScale( frame_gray, trees, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );

    for(size_t i = 0; i < trees.size(); i++)
    {
        Point center( trees[i].x + trees[i].width/2, trees[i].y + trees[i].height/2 );
        ellipse( src_trainning, center, Size( trees[i].width/2, trees[i].height/2 ), 0, 0, 360, Scalar( 0, 255, 255 ), 4, 8, 0 );
        //rectangle(src, Rect(trees[i].width, trees[i].height, 10 , 10), Scalar(255,0,255), 1);
        //printf("aaaaa\n");
    }
    return frame;
}


void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
        if(event == EVENT_LBUTTONDOWN){
                std::cout << "left button click : (x, y) == (" << x << ", " << y << ")" << std::endl;
                //std::cout << "SRC_GRAY \t: " << (int)src.at<uchar>(y, x) << std::endl;
                //std::cout << "HLS        (hue)\t: " << (int)HLS.at<Vec3b>(y, x)[0] << std::endl;
                //std::cout << "HLS        (lightness)\t: " << (int)HLS.at<Vec3b>(y, x)[1] << std::endl;
                //std::cout << "HLS        (saturation)\t: " << (int)HLS.at<Vec3b>(y, x)[2] << std::endl;
                std::cout << "src_hls (hue)\t\t: " << (int)src_hls.at<Vec3b>(y, x)[0] << std::endl;
                std::cout << "src_hls (lightness)\t: " << (int)src_hls.at<Vec3b>(y, x)[1] << std::endl;
                std::cout << "src_hls (saturation)\t: " << (int)src_hls.at<Vec3b>(y, x)[2] << std::endl;
        }
}
