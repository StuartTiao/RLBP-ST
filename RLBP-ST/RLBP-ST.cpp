#include <iostream>
#include <string>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "RLBP.h"

using namespace std;
using namespace cv;
int main(int argc, char** argv)
{
	// process arguments
	std::string sInImg, sOutFile;
	if (argc <= 1)
	{
		cout << " Usage: Please input two string for input and output images" << endl;
		return -1;
	}
	else if(argc == 2)
	{
		cout << " Usage: no argument for output images, will use default output name: output.bmp" << endl;
		sInImg = argv[1];
		sOutFile = "RLBPHist.csv";
	}
	else if (argc == 3)
	{
		sInImg = argv[1];
		sOutFile = argv[2];
	}

	// load image
	Mat inImg;
	inImg = imread(sInImg, CV_LOAD_IMAGE_GRAYSCALE);

	if(!inImg.data)
	{
		cout << "could not open the input image" <<std::endl;
		return -1;
	}

	//LBP
	std::vector<float> LBPHist;
	LBP((unsigned char*)inImg.data, inImg.cols, inImg.rows,inImg.step, LBPHist);
	
	float sum = 0.0f;
	std::for_each(LBPHist.begin(),LBPHist.end(),[&sum](float h){sum+=h;});
	if(abs(sum - 1.0f) > 0.0001f)
		return -1;

	//RLBP LOOKUP
	std::vector<std::vector<unsigned char> > H2RHLkup;
	ProduceRLBPLookUp(H2RHLkup);

	//RLBP HIST
	std::map<unsigned char, float> RH; //<Hi in [0, 255], frequency in [0.0, 1.0]
	MapH2RH(H2RHLkup, LBPHist, RH);

	//save output
	std::fstream outf;
	try{

		outf.open(sOutFile.c_str(), std::fstream::out);
		if (outf.good())
		{
			outf << "RH_Bin, Frequency" << std::endl;
			outf.unsetf(std::ios::floatfield);
			outf.precision(5);
			for (auto bf : RH)
			{
				outf << int(bf.first) <<", "<< bf.second << std::endl;
			}
		}
		outf.close();
	}
	catch(...)
	{
		cout<<"Writing output file "<<sOutFile<<" failed"<<std::endl;
		return -1;
	}





}