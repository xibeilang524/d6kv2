#if !defined(_FEP_MEM_HEAD)
#define _FEP_MEM_HEAD

#include "netapi.h"
// #include "afert/law.hxx"

typedef struct
{
		int16s		cycle;
		int32s		group_no;		//组号
		int32s		rtunum;
        int32s		chnum;
        int32s		rtuno[G_RTUNUMS];
        int32s		channo[G_CHANNUMS];
		int32s		ycnum[G_RTUNUMS];
        int32s		yxnum[G_RTUNUMS];
        int32s		kwhnum[G_RTUNUMS];
		int32s		devnum[G_RTUNUMS];
#if defined(PROT_MON)
		floats		yc[G_YCNUMS];
#else
        int16s		yc[G_YCNUMS];
#endif
        int8u   	yx[G_YXBNUMS];
        char		kwh[G_KWHNUMS*4];
		uchar		devsta[G_STATUSNUMS]; 
        TDSTATUS   	chan[G_CHANNUMS];  //本机通道状态     		
		TDSTATUS	otherchan[G_CHANNUMS];		//另一个节点的通道状态
		DATAFLAG	first;
}NETDATA_N;


class FepMem
{
public:
	FepMem();
	virtual ~FepMem();

	COMPARA* GetComPara();
	NetStatus cfg;

protected:
	int ShmMem();

private:
};

class NetDataCvt : public NetDbg
{
public:
	NetDataCvt();
	virtual ~NetDataCvt();

	int LocalToNet(NETDATA* netdata_l,NETDATA_N* netdata_n);
	int NetToLocal(NETDATA_N* netdata_n,NETDATA* netdata_l);

	int NetToLocal_NUM(NETDATA_N* netdata_n,NETDATA* netdata_l);
};

#endif