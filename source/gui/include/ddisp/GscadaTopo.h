#ifndef _G_SCADATOPO_H
#define _G_SCADATOPO_H

#ifdef _WIN32
	#pragma warning(disable: 4251)
	#pragma warning(disable: 4275)
#endif

#include "rdb/rdbapi.h"
#include "GList.h"

#define CODE_LEN				 24
#define MAX_PORTNUM_DEV   6				//一个设备的最大可能连接节点
#define MAX_DEVNUM_POINT 60 			//一个节点上可连接别的设备的最大数
#define MAX_MESH_DEV   100
#define MAX_MESH_NUM   30


//站内设备
typedef struct
{
	char  devname[CODE_LEN];				//设备代码
	int   type;											//设备类型
	int   nodeno[MAX_PORTNUM_DEV];	//首端节点号,末端节点号	
																	//高压侧节点号,中压侧节点号,
																	//低压侧节点号,中性点节点号 etc.
	int   external_device;					//表明该设备是一个站外设备
	unsigned char	  power_flag;			//带电与否
	char	  obligate[32];						//保留字段，用于邦定

	//add by lipeng 
	int		MeshFlag;						//环网标志

} DEVINFO_INTERNAL;						

//站间设备,站间只有端到端的对接设备,T接线应增加新站或将一部分处理成站内线路
typedef struct
{
	char devname[CODE_LEN];	//设备代码
	int	type;									//设备类型
	int	head_station;					//首端厂站号
	int	head_node;						//首端节点号	//高压侧节点号
	int	tail_station;					//末端厂站号
	int	tail_node;						//末端节点号	//低压侧节点号

	//add by lipeng 
	int		MeshFlag;						//环网标志

} DEVINFO_EXTERNAL;

//节点挂接设备清单
typedef struct _tagNODEINFO
{
	int   devnum;						//设备个数
	//add by lipeng
	int   stindex;						//厂站索引
	///////////////////////
	int   nodeno;						//节点序号
	int   index[MAX_DEVNUM_POINT];	//节点挂接设备下标
	int   port[MAX_DEVNUM_POINT];	//节点挂接设备端口
	unsigned char   is_bus;						//实际母线标志

	_tagNODEINFO()
	{
		devnum	= 0;
		is_bus  = 0;
		nodeno	= -1;
		stindex = -1;

		for( int i = 0 ; i < MAX_DEVNUM_POINT ; i ++ )
		{
			index[i] = -1;
			port[i]= -1;
		}
	}

} NODEINFO;

//计算使用
typedef struct 
{
	int stno;//厂站号
	int ndno;//节点号
} NODEPOS;

typedef struct 
{
	int stno  ;//厂站号
	int index ;//设备下标
} DEVPOS;

//厂站信息
class  GStations
{
public:

	GStations()
	{
		memset(stname,0,sizeof(stname));
		m_pDevices	= NULL;
		m_pNodes	= NULL;
		m_deviceCnt = 0;
		m_nodeCnt	= 0;
		MAXNODENO	= 0;
		m_node2Indexs = NULL;
	}

	~GStations()
	{
		if(m_pDevices)		delete[] m_pDevices;
		if(m_pNodes)		delete[] m_pNodes ;
		if(m_node2Indexs)	delete [] m_node2Indexs;
	}

	char stname[CODE_LEN];			//厂站名称

	DEVINFO_INTERNAL	*m_pDevices;		//站内设备清单
	int		m_deviceCnt;					//站内设备个数

	NODEINFO*		  m_pNodes;			  	//节点挂接设备清单
	int*				m_node2Indexs; 		//序号对应下标

	int m_nodeCnt;									//用节点的序号（数字）即可表示
	int MAXNODENO;									//最大节点序号

	DEVINFO_INTERNAL* devices() { return m_pDevices; }
	DEVINFO_INTERNAL* device(int index) { if(index>m_deviceCnt) return NULL ; return &m_pDevices[index]; }

	NODEINFO* nodes() { return m_pNodes; }
	NODEINFO* node(int index) { if( index > m_nodeCnt) return NULL; return &m_pNodes[index]; }
	
	int nodeCnt() const { return m_nodeCnt; }
	int deviceCnt() const { return m_deviceCnt; }
	int no() const { return MAXNODENO; }
	unsigned char contains(int n) { if((m_node2Indexs==NULL)||(m_node2Indexs[n]==-1)) return 0; return 1; }
	int index(int n) { return m_node2Indexs[n]; }

	void  setNo(int n) { MAXNODENO = n ; }
};

//add by lipeng for topo
#define MAX_NODE_LINK_NUM	4
#define MAX_NODE_NUM		1000
#define	MAX_POWER_NUM		4
typedef struct devInfo
{
	char	devcode[CODE_LEN];
	int		stindex;					//厂站号为-1的代表站间线路
}DEVINFO;

typedef struct NodeLink
{
	NODEINFO					m_NodeInfo;
	NodeLink*					ParentNode[MAX_NODE_LINK_NUM];
	int							ParentNum;							//定义略微不合理，任何节点的父节点只有一个
	NodeLink*					ChildNode[MAX_NODE_LINK_NUM];
	int							ChildNum;

	DEVINFO_INTERNAL			PowerSource[MAX_NODE_LINK_NUM];
	int							PowerNum;
	DEVINFO_INTERNAL			LoadInfo[MAX_NODE_LINK_NUM];
	int							LoadNum;
}NODELINK;

typedef struct TreeInfo
{
	int							m_IslandNo;						//树的岛号，一个岛就是一棵树
	NODELINK					m_NodeLink[MAX_NODE_NUM];		//树里的节点链表，从1开始
	int							m_NodeNum;						//树里的节点数目
	DEVINFO_INTERNAL			m_DevInfo[MAX_NODE_NUM];		//树里的设备列表
	int							m_DevNum;						//树里的设备数目
	DEVINFO_INTERNAL			PowerInfo[MAX_POWER_NUM];		//树的电源列表
	int							PowerNum;						//树的电源数目

	int							IsMeshFlag;						//环网标志
}TREEINFO;
////////////////////////////////////////////////////////
//add by zhangyun
//一条判断出来的环网
typedef struct  
{
	int meshIndex;
	int devNum;
	DEVINFO dev[MAX_MESH_DEV];
}MESH;

//一次环网判断的结果的集合
typedef struct
{
	int meshNum;
	MESH mesh[MAX_MESH_NUM];
}MESHRESULT;
typedef struct  
	{
		//char dev[MAX_MESH_DEV][CODE_LEN];

		DEVINFO devBreakSwitch[MAX_MESH_DEV];
		char meshBSDevVal[MAX_MESH_NUM][MAX_MESH_DEV];

		DEVINFO devLine[MAX_MESH_DEV];
		char LineInMesh[MAX_MESH_NUM][MAX_MESH_DEV];

		int devBSNum;
		int devLineNum;
		int modeNum;
		uchar readMeshModeOK;
	}MESHMODE;
///////////////////////////////////////

//电网的全局信息
class GPowerNets
{
public:

	GPowerNets()
	{
		m_stationCnt	= 0 ;
		m_lineCnt  = 0 ;
		m_deviceCnt = 0;
		m_pStations = NULL;
		m_pLines = NULL;
		m_pDevices= NULL;
	}

	~GPowerNets()
	{
		if(m_lineCnt )		delete[] m_pLines;
		if(m_stationCnt )	delete[] m_pStations;
		if(m_deviceCnt)		delete [] m_pDevices;

		m_stationCnt = 0;
		m_lineCnt = 0;
		m_deviceCnt= 0;
	}
	
	GStations		 *m_pStations;		//厂站信息清单			
	int			  m_stationCnt;		//厂站个数
	DEVINFO_EXTERNAL *m_pLines;			//站间线路清单
	int			  m_lineCnt;		//线路个数	
	DEVPOS			 *m_pDevices;		//起始计算设备清单
	int			  m_deviceCnt;		//设备个数
	//add by zhangyun
	MESHRESULT	  m_meshResult;		//环网判断的结果

	//add by lipeng for topo
	TREEINFO			*m_NodeTree;
	int					m_TreeNum;
	///////////////////////////////


	GStations*  station(int index) { if( index > m_stationCnt ) return NULL ; return &m_pStations[index]; }
	DEVINFO_EXTERNAL* line(int index) { if( index > m_lineCnt ) return NULL ; return &m_pLines[index]; }
	DEVPOS&	device(int index){ return m_pDevices[index]; }

	GStations* stations() const { return m_pStations; }
	DEVINFO_EXTERNAL* lines() const { return m_pLines; }
	DEVPOS* devices() const { return m_pDevices; }


	int		deviceCnt() const { return m_deviceCnt; }
	int		lineCnt() const { return m_lineCnt;     }
	int		stationCnt() const { return m_stationCnt; }

};

//这是一个跟GPowerNets而辅助配套的结构,
typedef struct 
{
	int   *pLnIndex;	//线路设备在实时表"scdtp"中的下标
	unsigned char  **pLnState;	//设备在实时库的分合状态
	int  **pDevIndex;	//厂站中的设备在实时表"scdtp"中的下标	
	unsigned char ***pDevState;	//设备在实时库的分合状态
} Rdb_LOCATE_STRU;


class GscadaTopo
{
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
		unsigned char vol5;
		unsigned char vol6;
		char  node1[CODE_LEN];
		char  node2[CODE_LEN];
		char  node3[CODE_LEN];
		char  node4[CODE_LEN];
		char  node5[CODE_LEN];
		char  node6[CODE_LEN];
		unsigned char power_flag;
		char  obligate[32];
		int busno1;
		int busno2;
		int busno3;
		int busno4;
		int busno5;
		int busno6;
		int islno1;
		int islno2;
		int islno3;
		int islno4;
		int islno5;
		int islno6;
		int active1;
		int active2;
		int active3;
		int active4;
		int active5;
		int active6;
		int active;

	}TPRCD,*PTPRCD;

public:

	GscadaTopo();
	~GscadaTopo();

	unsigned char initTP();
	void  clear();

	//按某个电压带电分析
	void  runActiveAnalysis(int stno, int ndno, int nIslno);
	void  runActiveAnalysis(int lnno, int nIslno);

	//全局带电分析
	void  runActiveAnalysis();

protected:

	//带电分析中是否继续越过该设备继续分析的判断函数
	unsigned char isActiveByVol(int stno, int devno);
	unsigned char isActiveBySt(int stno, int devno);

	//从指定岛的起点遍历相关节点
	void  travelNode(NODEPOS pos, int nIslno,TREEINFO *TreeInfo);

	//变量
	int			m_bInit;			//初始化的方式
	int			m_bMode;			//0 - 按某个电压带电分析 
										//1 - 全局带电分析
	GPowerNets			m_nets;				//全网拓扑结构信息
	PTPRCD					m_tpRcd;			//实时库拓扑表
	
public:

	int		  **m_pResult    ;	//分析拓扑结果

	//add by lipeng for topo  
	GList<TREEINFO>				m_Tree;
	DEVINFO GetDevBetweenParent(NODELINK node,int*  DevNum,DEVINFO** singleDev);	//获取与父节点之间的设备,singleDev里是父节点上的单端元件
	int  GetDevBetweenChild(NODELINK node,DEVINFO** Dev);	//获取与子节点之间的设备列表
	int  GetDevAtNode(NODELINK node,DEVINFO** Dev);			//获取节点上的设备列表
	int  GetPowerPath(char* devname,DEVINFO** path) ;			//获取到电源点的路径,注：供电路径分析只对负荷有效
	int  GetLoadPath(char* devname,DEVINFO** path) {return 1;};
	int	 GetPowerPathFromTree(DEVINFO* device,NODEPOS pos,DEVINFO* path,TREEINFO* queryTree);
	int  GetLoadPathOfLine(char* devname,DEVINFO** path);			//获取馈线供电路径，该设备向下的供电范围,注：供电范围分析只对馈线有效
																	//返回-1，是馈线在环网内，返回-2，是馈线在多电源树内
	int  GetLoadPathOfBreaker(char* devname,DEVINFO** path);			//获取开关所带负荷列表
	int	 GetLoadPathFromTree(DEVINFO* device,NODELINK node,DEVINFO** path,TREEINFO* queryTree);//如果全是负荷，则返回正的设备数目，
	int	 JudgeDoublePower(char* devname,DEVINFO* path);		//判断该设备是否是双端供电
	int GetPowerPathFromDev(const char *devName, char ***devList);
	int GetLoadPathFromDev(const char *devName, char ***devList);
	///////////////*/
	
	MESHMODE   m_meshMode;
	//add by zhangyun 
		//从文件中读取环网模式
	uchar readMeshMode();
	//从文件中读取环网模式的馈线部分
	uchar readMeshModeLine();
		//判断网内是否存在环网
	int MeshJudge();
	//判断一个设备是否在环网内
	int  JudgeMesh(char *devName);
	//返回某个设备所在环网的所有设备
	//int GetMeshFromDev(char* devName,DEVINFO_INTERNAL* meshDev){return 0;};
	int GetMeshFromDev(char* devName,DEVINFO* meshDev);
	//////////////////////////////////////////
};

class GstreamMsg
{
public:

	GstreamMsg(){};
	~GstreamMsg();

	unsigned char setStream(void (*func)(void *buf,int ord,int len,int num) = NULL);
	
	void stopStream();
	void readStream();

private:
	Rdb_QuickStream rdb_stream;

};

#endif
