#ifndef _dev_def_h
#define _dev_def_h

//////////////// 厂站名、设备名长度定义 ////////////////
#define SUBSTNAME_LEN       24		//厂站名长度
#define POWDEVNAME_LEN      24		//设备名长度
#define OBJTYPENAME_LEN     4		//对象类型名长度


#define	OBJECTNAME_LEN		24		//对象名长度
#define OBJDESCRIBE_LEN		40		//对象描述长度
#define OBJDESCRIBE_LLEN		64		//对象描述长度
#define EVENT_LEN          128
//////////////// 设备类型定义 //////////////////////

#define  DEVTYPE_NULL				0   // 无效类型
#define  DEVTYPE_BREAKER			1   // 开关
#define  DEVTYPE_SWITCH				2   // 刀闸
#define  DEVTYPE_MELT			    3	// 熔断器
#define  DEVTYPE_HANDBREAK		    4	// 手车
#define  DEVTYPE_TRANSFORMER		5   // 主变压器（三绕组）
#define  DEVTYPE_PDTRANSFORMER		6   // 配电变压器（二绕组）
#define  DEVTYPE_CAPACITOR			7   // 电容器
#define  DEVTYPE_REACTOR			8   // 电抗器
#define  DEVTYPE_COIL				9	// 消弧线圈	
#define  DEVTYPE_PT					10	// 电压互感器
#define  DEVTYPE_CT					11	// 电流互感器
#define  DEVTYPE_ARRESTER			12	// 避雷器
#define  DEVTYPE_GNDSWITCH			13  // 接地线
#define  DEVTYPE_LINE    		    14	// 架空线--------线路
#define  DEVTYPE_CABLE		        15	// 电缆----------线路
#define  DEVTYPE_BUS				16  // 母线
#define  DEVTYPE_GENERATOR			17  // 发电机
#define  DEVTYPE_WELD				18  // 焊点
#define  DEVTYPE_LINKLINE			19  // 连接线
#define  DEVTYPE_JOINT  			20  // 端子 联接点
#define  DEVTYPE_LOAD				21  // 负荷
#define  DEVTYPE_PROTECT			22  // 保护装置
#define  DEVTYPE_MLBREAKER			23  // 母联开关
#define  DEVTYPE_ZBR				24  // 零阻抗支路ZBR

#define  DEVTYPE_DECK				48  // 根
#define  DEVTYPE_CO					49  // 省调
#define  DEVTYPE_DISTRICT			50  // 地区
#define  DEVTYPE_OTHERDEV			51  // 其它设备
#define  DEVTYPE_SUBSTATION			52  // 厂站
#define  MAXDEVTYPE         		100	// 电力设备最大类型号

//////////////// 设备类型名定义 //////////////////////
#define  DEVTYPENM_BREAKER			'K' // 开关
#define  DEVTYPENM_SWITCH			'D' // 刀闸
#define  DEVTYPENM_LINE				'X' // 架空线--------线路
#define  DEVTYPENM_CABLE			'x' // 电缆----------线路
#define  DEVTYPENM_BUS				'M' // 母线
#define  DEVTYPENM_TRANSFORMER		'B' // 主变压器----变压器
#define  DEVTYPENM_PDTRANSFORMER	'b' // 配电变压器----变压器
#define  DEVTYPENM_GENERATOR		'F' // 发电机

#define  DEVTYPENM_CAPACITOR		'C' // 电容器
#define  DEVTYPENM_CT				'I' // 电流互感器
#define  DEVTYPENM_PT				'U' // 电压互感器
#define  DEVTYPENM_REACTOR			'R' // 电抗器
#define  DEVTYPENM_ARRESTER			'L' // 避雷器
#define  DEVTYPENM_PROTECT			'P' // 保护装置
#define  DEVTYPENM_COIL				'Q' // 消弧线圈	
#define  DEVTYPENM_LOAD				'H' // 负荷
#define  DEVTYPENM_JOINT			'J' // 联接点
#define  DEVTYPENM_MLBREAKER		'l' // 母联开关
#define  DEVTYPENM_MELT				'r' // 熔断器
#define  DEVTYPENM_GNDSWITCH		'G' // 接地线
#define  DEVTYPENM_LINKLINE			'j' // 连接线
#define  DEVTYPENM_ZBR				'Z' // 零阻抗支路ZBR

#define  DEVTYPENM_DISTRICT			'd' // 地区
#define  DEVTYPENM_OTHERDEV			'O' // 其它设备
#define  DEVTYPENM_SUBSTATION		'S' // 厂站


///////////////// 开关刀闸参数表 中 开关类型 域值定义 /////////

#define  BREAKTYPE_NULL				 0  // 无效类型
#define  BREAKTYPE_NORMAL_SW		 1  // 普通刀闸
#define  BREAKTYPE_BYPASS_SW		 2  // 旁路刀闸
#define  BREAKTYPE_BUSLINK_SW		 3  // 母联刀闸
#define  BREAKTYPE_GROUND_SW		 4  // 地刀
#define  BREAKTYPE_NORMAL_BK		 5  // 普通开关
#define  BREAKTYPE_CAR_BK			 6  // 小车开关
#define  BREAKTYPE_BUSLINK_BK		 7  // 母联开关
#define  BREAKTYPE_BUSLINK_CAR_BK    8  // 母联小车开关
#define  BREAKTYPE_BYBUS_BK			 9  // 旁母开关
#define  BREAKTYPE_BYBUSLINK_BK		10 // 旁母兼母联开关

#define  BREAKTYPE_VIRTUAL_SW		99 // 虚刀闸

////////////////电容器参数表 中 电容器类型 域值定义 //////////
#define  CAPA_SHUNT					 0   //并联
#define  CAPA_SERIES				 1   //串联

///////////////电抗器参数表 中 电抗器类型 域值定义 ///////////
#define  REAC_SHUNT					 0   //并联
#define  REAC_SERIES			 	 1   //串联

///////////////变压器参数表 中 变压器类型 域值定义  ///////////
#define  TRAN_TWO					 1   //两绕组
#define  TRAN_THREE					 2   //三绕组
#define  TRAN_SELF					 3   //自藕变
#define  TRAN_FOUR					 4   //四绕组


///////////////母线参数表 中 工作类别 域值定义  ///////////////
#define  MXTYPE_GZ					 0  //工作母线（或工作母线一）
#define  MXTYPE_BY					 1  //备用母线（或工作母线二）
#define  MXTYPE_PL                   2  //旁路母线 
#define  MXTYPE_BYPL                 3  //旁路兼备用母线 
#define  MXTYPE_INVALID			  0xff  //其他类型


////////////// 量测映射表 中 应用类别 域值定义  ///////////////
#define  MAPYC_PUBLIC				 0  // 公用
#define  MAPYC_SCADA				 1  // SCADA
#define  MAPYC_PAS					 2  // PAS



//add for distribution network topo
//厂站类型
enum StationType
{
	STATIONTYPE_SUBSTATION = 0,			//变电站
	STATIONTYPE_DISTRIBUTION			//配电站
};

enum BreakerType
{
	BREAKERTYPE_NORMAL = 0,		//普通开关
	BREAKERTYPE_CONNECTION,		//联络开关
	BREAKERTYPE_LOAD			//负荷开关
};

enum SwitchType
{
	SWITCHTYPE_ISOLATOR = 0,	//隔离刀闸
	SWITCHTYPE_GROUNDING		//接地刀闸
};

//设备投运状态
enum DevServiceStatus
{
	DEVSERVICESTATUS_INSERVICE = 0,		//设备在服役中
	DEVSERVICESTATUS_OUTSERVICE,		//设备退出运行	
	DEVSERVICESTATUS_UNSERVICE			//设备未投入运行
};

//设备实际运行状态
enum DevRunState
{
	DEVRUNSTATE_POWERCUT = 0,				//失电
	DEVRUNSTATE_POWER_BEGIN = 1,			//带电，馈线号起始
	DEVRUNSTATE_POWER_END = 4999,			//带电，馈线号结束
	DEVRUNSTATE_LOOPNET = 10001,			//环网
	DEVRUNSTATE_TRANSFER,					//负荷转供
	DEVRUNSTATE_GROUNDING = 10100			//设备接地

};

//设备失电原因
enum LossPowerReason
{
	NORMAL_POWERCUT =0,     //正常停电
	FAULT_POWERCUT = 1,		//故障失电
	PLANNED_POWERCUT,		//计划停电
	TEMPORARY_POWERCUT,		//临时停电
	PROJECT_POWERCUT		//工程停电
};

//设备着色状态编号
enum devColorState
{	
	DEVCOLORSTATE_UNSERVICE = -1,				//设备未投入运行
	DEVCOLORSTATE_COLORDEFAULT = 0,
	DEVCOLORSTATE_COLORBYFEEDER_BEGIN = 1,	    //设备按馈线号着色开始
	DEVCOLORSTATE_COLORBYFEEDER_END = 4999,		//设备按馈线号着色结束
	DEVCOLORSTATE_COLORBYST_BEGIN = 5000,		//设备按变电站着色开始
	DEVCOLORSTATE_COLORBYST_END =5999,			//设备按变电站着色结束
	DEVCOLORSTATE_LOOPNET = 10001,				//设备环网
	DEVCOLORSTATE_TRANSFER = 10002,				//设备负荷转供
	DEVCOLORSTATE_GROUNDING = 10100,			//设备接地
	DEVCOLORSTATE_NORMAL_POWERCUT = 10200,      //设备正常停电
	DEVCOLORSTATE_FAULT_POWERCUT,		        //设备故障停电
	DEVCOLORSTATE_PLANNED_POWERCUT,				//设备计划停电
	DEVCOLORSTATE_TEMPORARY_POWERCUT,			//设备临时停电
	DEVCOLORSTATE_PROJECT_POWERCUT,				//设备工程停电
	DEVCOLORSTATE_SUPPLY_AREA = 10300,			//供电范围
	DEVCOLORSTATE_SUPPLY_ROUTE ,				//供电路径
	DEVCOLORSTATE_OUTSERVICE = 30000			//设备退出运行，退役
};
//end

#endif
