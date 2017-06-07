/**
@file dataserver.h
@brief 数据处理头文件
@author lujiashun 
@version 1.0.0
@date 2016-10-25
*/
#ifndef DATASERVER_H
#define DATASERVER_H

#include "complexs.h"
#include "mathutils.h"

class Data_server
{
protected:
	static void GetThreePhase1(int a, int b, int c, int pos, Complex* data);
	static bool CheckDelta(int pos, double delta, int& pos1);
	static bool CheckLimit1(const Complex& c, bool b = false);
	static bool CheckLimit2(int c, int pos, bool b = false);
	static void GetDleta(double* p, int count, int delta);
	static void Mul(double* p, int count, double f);
	static void Move1(const double* src, double* dst, int count, int delta);

public:
	static Complex Factor120();
	static Complex Factor240();
	static Complex VectorToComplex(const Complex& c);
	static Complex GetVector(int channelNo, int pos);
	static Complex GetVector1(int channelNo, int pos);
	static Complex GetComplex(int channelNo, int pos);
	static Complex GetHarmValue(int channelNo, int pos, int harm);
	static Complex GetHarmVector(int channelNo, int pos, int harm);
	static Complex GetDirect(int channelNo, int pos);
	static Complex GetAverage(int channelNo, int pos, int count);
	static void GetThreePhase(int a, int b, int c, int pos, double cycle, Complex* data);
    static void GetDual(int count, const double* a, const double* b, double* ab);
	static Complex GetSinglePhaseZ(int u, int ia, int ib, int ic, 
		int phase, int pos, LINEPARA line);
	static Complex GetInterPhaseZ(int u1, int u2, int i1, int i2, int pos);
	static Complex GetSequenceZ(int ua, int ub, int uc, 
		int ia, int ib, int ic, int seqtype, int pos);
	static Complex GetAngle(int u1, int u2, int i, int pos);
	static Complex GetAngle1(int ua, int ub, int uc, 
		int ia, int ib, int ic, int seqtype, int pos);
	static Complex GetAngle2(int ua, int ub, int uc, 
		int ia, int ib, int ic, int seqtype, int pos, double delta);
	static Complex GetAngle3(int u1, int u2, int i1, int i2, int pos);
	static Complex GetAngle4(int u1, int u2, int i1, int i2, int pos, double delta);
	static Complex GetAngle5(int u, int i, int pos);
	static Complex GetAngle6(int u, int i, int pos, double delta);
	static Complex GetSeq(const Complex& a, const Complex& b, const Complex& c, int seqtype);
	static Complex GetSeq1(int a, int b, int c, int pos, int seqtype);
//	static COMPLEX GetDifferential();
//	static double* ComposeAnalog();
	static double* CreateHarm(int channel, int harm);
	static double GetFreq(int channel, int pos);

};

#endif