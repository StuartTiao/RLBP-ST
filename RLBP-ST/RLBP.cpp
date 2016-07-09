#include <iostream>
#include <algorithm>
#include "RLBP.h"

#define A2D(p,i,j,step) p[j*step+i]


/**
 *  @function   LBP
 *  @brief      produce LBP histogram based on an uchar image
 *  @param      unsigned char * pArr
 *  @param      int width
 *  @param      int height
 *  @param      int nStepByte
 *  @param      std::vector<unsigned char> & LBPHist
 *  @return     void
 */
void LBP(unsigned char* pArr, int width, int height, int nStepByte, std::vector<unsigned char>& LBPHist)
{
	if(!pArr) 
		return;
	LBPHist = std::vector<unsigned char>(256,0);

	int sb = nStepByte;

	for(int j = 0; j < height; j++)
	{
		for(int i=0; i<width; i++)
		{
			int im = std::max(0, i-1);				//i-1
			int ip = std::min(width-1, i+1);		//i+1
			int jm = std::max(0, j-1);				//j-1
			int jp = std::min(height-1, j+1);		//j+1

			unsigned char lbp =  ((pArr[im	+	jm*sb]	>= pArr[i+j*sb])?1:0)		+ 
								  (((pArr[i	+	jm*sb]	>= pArr[i+j*sb])?1:0) << 1)	+
								  (((pArr[ip	+	jm*sb]	>= pArr[i+j*sb])?1:0) << 2)	+
								  (((pArr[ip	+	j*sb]	>= pArr[i+j*sb])?1:0) << 3)	+
								  (((pArr[ip	+	jp*sb]	>= pArr[i+j*sb])?1:0) << 4)	+
								  (((pArr[i	+	jp*sb]	>= pArr[i+j*sb])?1:0) << 5)	+
								  (((pArr[im	+	jp*sb]	>= pArr[i+j*sb])?1:0) << 6)	+
								  (((pArr[im	+	j*sb]	>= pArr[i+j*sb])?1:0) << 7);	
			LBPHist[lbp]++;						
		}
	}
	
}

/**
 *  @function   ProduceRLBPLookUp
 *  @brief      Produce lookup table for RLBP
 *  @param      const std::vector<unsigned char> & LBPHist
 *  @param      std::vector<unsigned char> & RLBPHist
 *  @return     void
 */
void ProduceRLBPLookUp(std::vector<std::vector<unsigned char> >& H2RHLkup)
{
	lkup1 = std::vector<unsigned char>(256, 0);
	lkup2 = std::vector<unsigned char>(256, 0);

	int m1 = 0x02;	//010
	int m2 = 0x05;	//101
	int m3 = 0x07;	//111
	for(int i = 0; i<256; i++)
	{
		int m = i;
		for(int s = 0; s < 8-2; s++)
		{
			m = (i ^ (m1 << s)) & (m3 << s);	//check 010
			if(m == 0)
			{
				m = i ^ (m1 << s);	//toggle middle bit
			}		
		}

		for (int s = 0; s < 8 - 2; s++)
		{
			m = (i ^ (m2 << s)) & (m3 << s);	//check 101
			if (m == 0)
			{
				m = i ^ (m1 << s);	//toggle middle bit
			}
		}
		lkup1[i] = m;
	}

	for (int i = 0; i < 256; i++)
	{
		int m = i;
		for (int s = 0; s < 8 - 2; s++)
		{
			m = (i ^ (m2 << s)) & (m3 << s);	//check 101
			if (m == 0)
			{
				m = i ^ (m1 << s); //toggle middle bit
			}
		}

		for (int s = 0; s < 8 - 2; s++)
		{
			m = (i ^ (m1 << s)) & (m3 << s);	//check 010
			if (m == 0)
			{
				m = i ^ (m1 << s); //toggle middle bit
			}
		}
		lkup2[i] = m;
	}

}

bool IsUniformBP(unsigned char bp)
{
	int cnt = 0;
	while(bp>0)
	{
		if((bp&0x03^0x02 == 0)		//check 010
		|| (bp&0x03^0x05 == 0))		//check 101
			cnt++;
		bp>>1;
	}

	return (cnt < 3);
}

void MapH2RH(const std::vector<std::vector<unsigned char> >& H2RHLkup,
			std::vector<unsigned char>& LBPHist,
			std::map<unsigned char, float>& RH)
{
	for(int i = 0; i<256; i++)
	{
		auto rhvec = H2RHLkup.at(i);
		if(rhvec.size() == 1)	//uniform LBP and non-uniform LBP
		{
			RH[i] += LBPHist[i]; 
		}
		if(rhvec.size() > 1)						//one-to-many RLBP mapping
		{

		}
	}
}