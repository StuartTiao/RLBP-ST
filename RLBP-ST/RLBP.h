#ifndef RLBP_API
#define RLBP_API

#include <vector>
#include <map>


void LBP(unsigned char* pArr, int width, int height, int nStepByte, std::vector<unsigned char>& LBPHist);

void ProduceRLBPLookUp(std::vector<std::vector<unsigned char> >& H2RHLkup);

bool IsUniformBP(unsigned char bp);

void MapH2RH(const std::vector<std::vector<unsigned char> >& H2RHLkup, 
			std::vector<unsigned char>& LBPHist,
			std::map<unsigned char, float>& RH);




#endif //RLBP_API