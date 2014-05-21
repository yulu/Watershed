#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <cstdio>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

class SuperPixel{

	public:
		SuperPixel();
		void feedPixel(Point pos);
		void calculateColor(Mat& refImage);
		
		Vec3b m_color;

	private:
		vector<Point> m_pixels;
		int m_total;
};

/*
 * Constructor, init some value if necessary
 */
SuperPixel::SuperPixel(){
}


/*
 * Feed in a pixel from the reference image
 */
void SuperPixel::feedPixel(Point pos){
	m_pixels.push_back(pos);
}

/*
 *Calculate the color of the super pixel
 */
void SuperPixel::calculateColor(Mat& refImage){
	m_total = m_pixels.size();
	int r=0, g=0, b=0;
	int i;
	cout<<"m_total: "<<m_total<<endl;

	for(i = 0; i < m_total; i++){
		int x = m_pixels.at(i).x;
		int y = m_pixels.at(i).y;
		Vec3b intensity = refImage.at<Vec3b>(x, y);

		r += (int)refImage.at<Vec3b>(x, y).val[0];
		g += (int)refImage.at<Vec3b>(x, y).val[1];
		b += (int)refImage.at<Vec3b>(x, y).val[2];
	}
	r = r==0 ? 0 : r/m_total;
	b = b==0 ? 0 : b/m_total;
	g = g==0 ? 0 : g/m_total;

	m_color = Vec3b((uchar)(r+10), (uchar)(g+10), (uchar)(b+10));

}


