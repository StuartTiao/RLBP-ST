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
void LBP(unsigned char* pArr, int width, int height, int nStepByte, std::vector<float>& LBPHist)
{
	if(!pArr) 
		return;
	LBPHist = std::vector<float>(256,0.0f);
	float nTotal = float(width*height);

	int sb = nStepByte;

	for(int j = 0; j < height; j++)
	{
		for(int i=0; i<width; i++)
		{
			int im = std::max(0, i-1);				//i-1
			int ip = std::min(width-1, i+1);		//i+1
			int jm = std::max(0, j-1);				//j-1
			int jp = std::min(height-1, j+1);		//j+1

			unsigned char lbp =  ((pArr[im		+	jm*sb]	>= pArr[i+j*sb])?1:0)		+ 
								  (((pArr[i		+	jm*sb]	>= pArr[i+j*sb])?1:0) << 1)	+
								  (((pArr[ip	+	jm*sb]	>= pArr[i+j*sb])?1:0) << 2)	+
								  (((pArr[ip	+	j*sb]	>= pArr[i+j*sb])?1:0) << 3)	+
								  (((pArr[ip	+	jp*sb]	>= pArr[i+j*sb])?1:0) << 4)	+
								  (((pArr[i		+	jp*sb]	>= pArr[i+j*sb])?1:0) << 5)	+
								  (((pArr[im	+	jp*sb]	>= pArr[i+j*sb])?1:0) << 6)	+
								  (((pArr[im	+	j*sb]	>= pArr[i+j*sb])?1:0) << 7);	
			LBPHist[lbp] += 1.0f/nTotal;						
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
	int m1 = 0x02;	//010
	int m2 = 0x05;	//101
	int m3 = 0x07;	//111
	for(int i = 0; i<256; i++)
	{
		H2RHLkup.push_back(std::vector<unsigned char>());
		std::vector<unsigned char>& RHVec = H2RHLkup.back();
		if(IsUniformBP(i))
		{
			RHVec.push_back(i+1);
		}
		else		//map non-uniform
		{
			//replace 010 then 101
			int m = i;
			for(int s = 0; s < 8-2; s++)
			{
				if(((m ^ (m1 << s)) & (m3 << s)) == 0)	//check 010
				{
					m = m ^ (m1 << s);	//toggle middle bit
				}
			}
			for (int s = 0; s < 8 - 2; s++)
			{
				if (((m ^ (m2 << s)) & (m3 << s)) == 0)	//check 010
				{
					m = m ^ (m1 << s);	//toggle middle bit
				}
			}
			if(IsUniformBP(m))
			{
				RHVec.push_back(m+1);
			}
			else
			{
				RHVec.push_back(0);
			}

			//replace 101 then 010
			int mm = i;
			for (int s = 0; s < 8 - 2; s++)
			{
				if (((mm ^ (m2 << s)) & (m3 << s)) == 0)	//check 101
				{
					mm = mm ^ (m1 << s);	//toggle middle bit
				}
			}

			for (int s = 0; s < 8 - 2; s++)
			{
				if (((mm ^ (m1 << s)) & (m3 << s)) == 0)	//check 010
				{
					mm = mm ^ (m1 << s);	//toggle middle bit
				}
			}
			if(mm != m)
			{
				if (IsUniformBP(mm))
				{
					RHVec.push_back(mm + 1);
				}
				else
				{
					RHVec.push_back(0);
				}
			}



		}



	}


}

bool IsUniformBP(unsigned char bp)
{
	int cnt = 0;
	while(bp>0)
	{
		if(((bp^0x01)&0x03) == 0		//check 01
		|| ((bp^0x02)&0x03) == 0)		//check 10
			cnt++;
		bp = bp>>1;
	}

	return (cnt < 3);
}

void MapH2RH(const std::vector<std::vector<unsigned char> >& H2RHLkup,
			std::vector<float>& LBPHist,
			std::map<unsigned char, float>& RH)
{
	for(int i = 0; i<256; i++)
	{
		auto rhvec = H2RHLkup.at(i);
		for(auto rh : rhvec)
		{
			RH[rh] += LBPHist[i]/rhvec.size();
		}
	}
}