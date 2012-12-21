/*------------------------------------------------------------------------------------------*\
   This file contains material supporting chapter 3 of the cookbook:  
   Computer Vision Programming using the OpenCV Library 
   Second Edition 
   by Robert Laganiere, Packt Publishing, 2013.

   This program is free software; permission is hereby granted to use, copy, modify, 
   and distribute this source code, or portions thereof, for any purpose, without fee, 
   subject to the restriction that the copyright notice may not be removed 
   or altered from any source or altered source distribution. 
   The software is released on an as-is basis and without any warranties of any kind. 
   In particular, the software is not guaranteed to be fault-tolerant or free from failure. 
   The author disclaims all warranties with regard to this software, any use, 
   and any consequent failure, is purely the responsibility of the user.
 
   Copyright (C) 2013 Robert Laganiere, www.laganiere.name
\*------------------------------------------------------------------------------------------*/

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <vector>

int main()
{
	// read the image
	cv::Mat image= cv::imread("boldt.jpg");
	if (!image.data)
		return 0; 

	// show original image
	cv::namedWindow("Original image");
	cv::imshow("Original image",image);

	// convert into HSV space
	cv::Mat hsv;
	cv::cvtColor(image, hsv, CV_BGR2HSV);

	// split the 3 channels into 3 images
	std::vector<cv::Mat> channels;
	cv::split(hsv,channels);
	// channels[0] is the Hue
	// channels[1] is the Saturation
	// channels[2] is the Value

	// display value
	cv::namedWindow("Value");
	cv::imshow("Value",channels[2]);

	// display saturation
	cv::namedWindow("Saturation");
	cv::imshow("Saturation",channels[1]);

	// display hue
	cv::namedWindow("Hue");
	cv::imshow("Hue",channels[0]);

	// image with fixed value
	cv::Mat newImage;
	cv::Mat tmp(channels[2].clone());
	// Value channel will be 255 for all pixels
	channels[2]= 255;  
	// merge back the channels
	cv::merge(channels,hsv);
	// re-convert to BGR
	cv::cvtColor(hsv,newImage,CV_HSV2BGR);

	cv::namedWindow("Fixed Value Image");
	cv::imshow("Fixed Value Image",newImage);

	// image with fixed saturation
	channels[1]= 255;
	channels[2]= tmp;
	cv::merge(channels,hsv);
	cv::cvtColor(hsv,newImage,CV_HSV2BGR);

	cv::namedWindow("Fixed saturation");
	cv::imshow("Fixed saturation",newImage);

	// image with fixed value and fixed saturation
	channels[1]= 255;
	channels[2]= 255;
	cv::merge(channels,hsv);
	cv::cvtColor(hsv,newImage,CV_HSV2BGR);

	cv::namedWindow("Fixed saturation/value");
	cv::imshow("Fixed saturation/value",newImage);

	// Testing skin detection

	// read the image
	image= cv::imread("girl.jpg");
	if (!image.data)
		return 0; 

	// show original image
	cv::namedWindow("Original image");
	cv::imshow("Original image",image);

	// convert into HSV space
	cv::cvtColor(image, hsv, CV_BGR2HSV);

	// split the 3 channels into 3 images
	cv::split(hsv,channels);

	// detection mask
	cv::Mat mask1; // under 20 degrees
	cv::threshold(channels[0],mask1,10,255,cv::THRESH_BINARY_INV);
	cv::Mat mask2; // or over 320 degrees
	cv::threshold(channels[0],mask2,160,255,cv::THRESH_BINARY);
	cv::Mat mask= mask1 | mask2;

	// use value channel to display result
	cv::Mat detected;
	detected= channels[2]&mask;
	cv::namedWindow("Detection result");
	cv::imshow("Detection result",detected);

	// A test comparing luminance and brightness

	// create linear intensity image
	cv::Mat linear(100,256,CV_8U);
	for (int i=0; i<256; i++) {

		linear.col(i)= i;
	}

	// create a Lab image
	linear.copyTo(channels[0]);
	cv::Mat constante(100,256,CV_8U,cv::Scalar(128));
	constante.copyTo(channels[1]);
	constante.copyTo(channels[2]);
	cv::merge(channels,image);

	// convert back to BGR
	cv::Mat brightness;
	cv::cvtColor(image,brightness, CV_Lab2BGR);
	cv::split(brightness, channels);

	// create combined image
	cv::Mat combined(200,256, CV_8U);
	cv::Mat half1(combined,cv::Rect(0,0,256,100));
	linear.copyTo(half1);
	cv::Mat half2(combined,cv::Rect(0,100,256,100));
	channels[0].copyTo(half2);

	cv::namedWindow("Luminance vs Brightness");
	cv::imshow("Luminance vs Brightness",combined);

	cv::waitKey();
}
