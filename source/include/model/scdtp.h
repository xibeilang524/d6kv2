#ifndef _TOPO_H
#define _TOPO_H

#include "sysdef.h"

typedef struct
{
	char  name[CODE_LEN];
	unsigned char type;
	char  hstation[CODE_LEN];
	char  tstation[CODE_LEN];
	unsigned char vol1;
	unsigned char vol2;
	unsigned char vol3;
	unsigned char vol4;
	char  node1[CODE_LEN];
	char  node2[CODE_LEN];
	char  node3[CODE_LEN];
	char  node4[CODE_LEN];
	unsigned char power_flag;
	char  obligate[32];
	char servicestatus;//投退状态
	int feederno;	//馈线序号
	int nodeActive1;
	int nodeActive2;
	int nodeActive3;
	int nodeActive4;
	int realisland;	//实时岛号
	int active;
	int realfeederno; //实时馈线号
	int runstate;	//运行状态
	int statetime;	//运行状态开始时间
	unsigned char lossreason;	//失电原因
	int colorstate;//着色状态

}TPRCD,*PTPRCD;

#endif	