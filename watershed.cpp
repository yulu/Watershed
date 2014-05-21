#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "superpixel.hpp"

#include <cstdio>
#include <iostream>

using namespace cv;
using namespace std;

Mat markerMask, img;
int sp_size = 20;
int kernel = 21;
float sigma = 2.0;

vector<SuperPixel> superPixels;

int main(int argc, char** argv)
{
	char* filename = argc >= 2 ? argv[1] : (char*)"sample.jpg";
	Mat img0 = imread(filename, 1), imgGray;

	if(img0.empty())
	{
		cout<<"Couldn't open image "<<filename<<endl;
		return 0;
	}

	//blur the image for a trial
	GaussianBlur(img0, img0, Size(kernel,kernel), sigma);

	img0.copyTo(img);
	cvtColor(img, markerMask, CV_BGR2GRAY);
	cvtColor(markerMask, imgGray, CV_GRAY2BGR);

	imshow("Original Image", img);
	
	//generate mask map
	int i, j, compCount = 0;
	Mat markers(markerMask.size(), CV_32S);
	markers = Scalar::all(0);
	
	superPixels.reserve(compCount);
	for(i = 0; i < img.rows; i++)
		for(j = 0; j < img.cols; j++){
			if(i % sp_size == 0 && j % sp_size == 0){
				
				circle(markers, Point(j, i), 1, Scalar::all(compCount+1), -1, 8, 0);
				SuperPixel sp = SuperPixel();
				superPixels.push_back(sp);
				compCount++;
			}
		}
	

		
	//run watershed
	watershed(img0, markers);

	Mat wshed(markers.size(), CV_8UC3);


	//paint the watershed image
	for(i = 0; i < markers.rows; i++) 
		for(j = 0; j < markers.cols; j++)
		{
			int index = markers.at<int>(i, j);
			if(index == -1)
				wshed.at<Vec3b>(i, j) = img.at<Vec3b>(i,j);
			else if(index <= 0 || index > compCount)
				wshed.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
			else{  
				superPixels.at(index-1).feedPixel(Point(i,j));
			}
		}

	for(i = 0; i < compCount; i++){
		superPixels.at(i).calculateColor(img);
	}

	for(i = 0; i < markers.rows; i++)
		for(j = 0; j < markers.cols; j++)
		{
			int index = markers.at<int>(i, j);
			if(index > 0 && index < compCount)
				wshed.at<Vec3b>(i,j) = superPixels.at(index-1).m_color;
		}

	imshow("watershed tranform", wshed);

	waitKey(0);
	return 0;

}  
