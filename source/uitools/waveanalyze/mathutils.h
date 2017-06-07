#ifndef MAHTUTILS_H
#define MAHTUTILS_H

const double PI = 3.1415926535897932384626433832795;
const double PI2 = PI * 2;
const double SQRT3 = 1.732;
const double RADIO = 180.0 / PI;

typedef struct tagLinePara
{
	double fPs1;		// 正序阻抗角
	double fRate;		// 二次测额定电流
	double fKX;			// 零序电抗补偿系数
	double fKR;			// 零序电阻补偿系数
	double fLength;		// 每阻抗的单位长度 
}LINEPARA;


void CalAngle(		 double& AReal, 
					 double& AImag);
void Decompose(		 double* pSam,									 
					 double* pRe, 
					 double* pIm, 
					 int nSum, 
					 int nSample,
					 int nHarm = 1,
					 int nType = 0, 
					 bool bHarm = false);
void CycleDecompose( double* pSam, 
					 double& fRe, 
					 double& fIm, 
					 int nSample, 
					 int nHarm = 1,
					 int nType = 0,
					 bool bHarm = false);
void CalSequence(	 double* pA, 
					 double* pB, 
					 double* pC, 
					 double* pSeq, 
					 int nSum, 
					 int nSample,
					 int nType = 0);
void CalRealPower2(	 double* pIA, 
					 double* pIB, 
					 double* pIC,    
					 double* pUA, 
					 double* pUB, 
					 double* pUC,
					 double* pPQ2, 
					 int nSum, 
					 int nSample);
void CalPower(		 double *pSamIA, 
					 double *pSamIB, 
					 double *pSamIC,
					 double *pSamUA, 
					 double *pSamUB, 
					 double *pSamUC,
					 double *pVal, 
					 int nSum, 
					 int nSample,
					 int nType = 0);
void CalFrequency(	 double *pSam,
					 double *pFreq,
					 int nSum, 
					 int nSample);
double CalAverage(	 double* pSam, 
					 int nSample);
void CalSingleZ(	 double* pV,
					 double* pIA,
					 double* pIB,
					 double* pIC,
					 double* pRe,
					 double* pIm,
					 const LINEPARA Line,
					 int nSum,
					 int nSample,
					 int nPhase,
					 int nType = 0);
void CalBreak(		 double* pIn, 
					 double* pOut,
					 int nSum,
					 int nChange);
void CalZ(			 double* pVol, 
					 double* pCur,    
					 double* pRe, 
					 double* pIm, 
					 int nSum, 
					 int nSample,
					 int nType = 0);
void CalCycleZ(		 double* pVol, 
					 double* pCur,    
					 double& Re, 
					 double& Im, 
					 int nSample, 
					 int nType = 0);
void CalCycleSingleZ(double* pV,
					 double* pIA,
					 double* pIB,
					 double* pIC,
					 double& Re,
					 double& Im,
					 const LINEPARA Line,
					 int nSample,
					 int nPhase,
					 int nType = 0);
void CalSequence2(	 double* pA, 
					 double* pC, 
					 double* pSeq, 
					 int nSum, 
					 int nSample,
					 int nType);


#endif