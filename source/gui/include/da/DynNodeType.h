//用于自动定位电源节点
//由于考虑到数据的实时变化与故障时或故障发生前的判据以及连续故障时可能的
//逻辑误判，现不用
/////////////////////////////////////////////////////
#ifndef _DYNNODETYPE_H_
#define _DYNNODETYPE_H_

class CDynNodeType
{
public:
	BOOL IsPowerNode(int i,float *val, int valnum);
	CDynNodeType();
	~CDynNodeType();
private:

};
#endif