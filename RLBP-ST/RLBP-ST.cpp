#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;
int main(int argc, char** argv)
{
	// process arguments
	std::string sInImg, sOutImg;
	if (argc <= 1)
	{
		cout << " Usage: Please input two string for input and output images" << endl;
		return -1;
	}
	else if(argc == 2)
	{
		cout << " Usage: no argument for output images, will use default output name: output.bmp" << endl;
		sInImg = argv[1];
		sOutImg = "output.bmp";
	}
	else if (argc == 3)
	{
		sInImg = argv[1];
		sOutImg = argv[2];
	}

	// load image
	Mat inImg;
	inImg = imread(sInImg, CV_LOAD_IMAGE_COLOR);

	if(!inImg.data)
	{
		cout << "could not open the input image" <<std::endl;
		return -1;
	}



}