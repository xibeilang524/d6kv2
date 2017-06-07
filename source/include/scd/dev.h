#ifndef _dev_objname_h
#define _dev_objname_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "devdef.h"
#include "rdb/rdbdef.h"


//////////////// 系统公用代码定义 //////////////////

#define  SYS_SUBST_NAME		"sys"

#define  SYS_P_NAME			"SsystotalP"
#define  SYS_Q_NAME			"SsystotalQ"
#define  SYS_HZ_NAME		"SsysPl"	

#define  SYS_MPKWH_NAME		"SsystotalKWH"

#define  SYS_LAST_TIME		"SsysstopTime"


//////////////// 对象类型分类方法定义 /////////////

#define OBJTPMODE_SCDTAB				0	//按SCADA表分类

//////////////// 对象类型类别定义  ////////////////////

#define  OBJTPCLASS_NULL			   -1	//空类别

//按SCADA表分类
#define  OBJTPCLASS_SCDTAB_ANALOG		1	//模拟量类
#define  OBJTPCLASS_SCDTAB_KWH			2	//电度类
#define  OBJTPCLASS_SCDTAB_BREAK		3	//开关类
#define  OBJTPCLASS_SCDTAB_SWITCH		4	//刀闸类
#define  OBJTPCLASS_SCDTAB_PROTSIG		5	//保护信号类
#define  OBJTPCLASS_SCDTAB_STATUS		6	//状态量类
#define  OBJTPCLASS_SCDTAB_DMSSTATUS	7	//配电故障检测状态量类

//////////////// 对象类型代码定义 //////////////////

#define  OBJTYPENM_I				   "I"	//电流
#define  OBJTYPENM_Ia				   "Ia"	//A相电流
#define  OBJTYPENM_Ib				   "Ib"	//B相电流
#define  OBJTYPENM_Ic				   "Ic"	//C相电流
#define  OBJTYPENM_Iab				   "Id"	//AB相电流
#define  OBJTYPENM_Ibc			       "Ie"	//BC相电流
#define  OBJTYPENM_Ica				   "If"	//CA相电流

#define  OBJTYPENM_I1				   "I1"	//高电流
#define  OBJTYPENM_I2				   "I2"	//中电流
#define  OBJTYPENM_I3				   "I3"	//低电流
#define  OBJTYPENM_Ia1				   "Ia1"	//A相高电流
#define  OBJTYPENM_Ib1				   "Ib1"	//B相高电流
#define  OBJTYPENM_Ic1				   "Ic1"	//C相高电流
#define  OBJTYPENM_Ia2				   "Ia2"	//A相中电流
#define  OBJTYPENM_Ib2				   "Ib2"	//B相中电流
#define  OBJTYPENM_Ic2				   "Ic2"	//C相中电流
#define  OBJTYPENM_Ia3				   "Ia3"	//A相低电流
#define  OBJTYPENM_Ib3				   "Ib3"	//B相低电流
#define  OBJTYPENM_Ic3				   "Ic3"	//C相低电流

#define  OBJTYPENM_I00				   "I00"	//零序电流
#define  OBJTYPENM_I11				   "I11"	//正序电流
#define  OBJTYPENM_I22				   "I22"	//负序电流


#define  OBJTYPENM_P				   "P"	//有功
#define  OBJTYPENM_Pa				   "Pa"	//A相有功
#define  OBJTYPENM_Pb				   "Pb"	//B相有功
#define  OBJTYPENM_Pc				   "Pc"	//C相有功
#define  OBJTYPENM_Pab				   "Pd"	//AB相有功
#define  OBJTYPENM_Pbc				   "Pe"	//BC相有功
#define  OBJTYPENM_Pca				   "Pf"	//CA相有功

#define  OBJTYPENM_P1				   "P1"	//高有功
#define  OBJTYPENM_P2				   "P2"	//中有功
#define  OBJTYPENM_P3				   "P3"	//低有功
#define  OBJTYPENM_Pa1				   "Pa1"//A相高有功
#define  OBJTYPENM_Pa2				   "Pa1"//A相中有功
#define  OBJTYPENM_Pa3				   "Pa1"//A相低有功
#define  OBJTYPENM_Pb1				   "Pb1"//B相高有功
#define  OBJTYPENM_Pb2				   "Pb2"//B相中有功
#define  OBJTYPENM_Pb3				   "Pb3"//B相低有功
#define  OBJTYPENM_Pc1				   "Pc1"//C相高有功
#define  OBJTYPENM_Pc2				   "Pc2"//C相中有功
#define  OBJTYPENM_Pc3				   "Pc3"//C相低有功


#define  OBJTYPENM_Q				   "Q"	//无功
#define  OBJTYPENM_Qa				   "Qa"	//A相无功
#define  OBJTYPENM_Qb				   "Qb"	//B相无功
#define  OBJTYPENM_Qc				   "Qc"	//C相无功
#define  OBJTYPENM_Qab			       "Qd"	//AB相无功
#define  OBJTYPENM_Qbc				   "Qe"	//BC相无功
#define  OBJTYPENM_Qca				   "Qf"	//CA相无功

#define  OBJTYPENM_Q1				   "Q1"	//高无功
#define  OBJTYPENM_Q2				   "Q2"	//中无功
#define  OBJTYPENM_Q3				   "Q3"	//低无功
#define  OBJTYPENM_Qa1				   "Qa1"	//A相高无功
#define  OBJTYPENM_Qa2				   "Qa2"	//A相中无功
#define  OBJTYPENM_Qa3				   "Qa3"	//A相低无功
#define  OBJTYPENM_Qb1				   "Qb1"	//B相高无功
#define  OBJTYPENM_Qb2				   "Qb2"	//B相中无功
#define  OBJTYPENM_Qb3				   "Qb3"	//B相低无功
#define  OBJTYPENM_Qc1				   "Qc1"	//C相高无功
#define  OBJTYPENM_Qc2				   "Qc2"	//C相中无功
#define  OBJTYPENM_Qc3				   "Qc3"	//C相低无功



#define  OBJTYPENM_S				   "S"	//视在功率
#define  OBJTYPENM_COSYC			   "Cs"	//力率（功率因数）

#define  OBJTYPENM_ANGLE			   "Jd"	//角度
#define  OBJTYPENM_HZ				   "Pl"	//频率
#define  OBJTYPENM_TEMPERATURE		   "Wd"	//温度
#define  OBJTYPENM_PRESS			   "Yl"	//压力
#define  OBJTYPENM_WATER			   "Sw"	//水位
#define  OBJTYPENM_FLOW				   "Ll"	//流量
#define  OBJTYPENM_HUMIDITY			   "Sd"	//湿度
#define  OBJTYPENM_DW				   "Dw"	//变压器档位
#define  OBJTYPENM_YC				   "Yc"	//其他遥测


#define  OBJTYPENM_V				   "V"	//电压
#define  OBJTYPENM_Va			  	   "Va" //A相电压
#define  OBJTYPENM_Vb				   "Vb" //B相电压
#define  OBJTYPENM_Vc				   "Vc" //C相电压
#define  OBJTYPENM_Vab				   "Vd" //AB相电压
#define  OBJTYPENM_Vbc				   "Ve" //BC相电压
#define  OBJTYPENM_Vca				   "Vf" //CA相电压
#define  OBJTYPENM_V0				   "V0" //零序电压
#define  OBJTYPENM_V1				   "V1" //正序电压
#define  OBJTYPENM_V2				   "V2" //负序电压


#define  OBJTYPENM_PKWH				   "Wh"	//有功电度
#define  OBJTYPENM_PosPKWH			   "Wp"	//正向有功电度
#define  OBJTYPENM_NegPKWH			   "Wn"	//负向有功电度
#define  OBJTYPENM_QKWH				   "Vh"	//无功电度
#define  OBJTYPENM_PosQKWH			   "Vp"	//正向无功电度
#define  OBJTYPENM_NegQKWH			   "Vn"	//负向无功电度
#define  OBJTYPENM_RATE				   "Rt"	//其它累积量

#define  OBJTYPENM_BREAK			   "Kg"	//开关状态
#define  OBJTYPENM_SWITCH			   "Dz"	//刀闸状态
#define  OBJTYPENM_SGZ				   "Sg"	//事故总
#define  OBJTYPENM_YG				   "Yg"	//预告信号
#define  OBJTYPENM_PROTSIG			   "Bh"	//保护信号
#define  OBJTYPENM_STATUS			   "Zt"	//其它状态量

#define  OBJTYPENM_DEVICE			   "Dv"	//设备对象


////////////////////////// 数据量类型定义 ////////////////////////////////


//遥测类型编码定义
#define		OBJTYPE_I					1	//电流
#define		OBJTYPE_Ia					2	//A相电流
#define		OBJTYPE_Ib					3	//B相电流
#define		OBJTYPE_Ic					4	//C相电流
#define     OBJTYPE_Iab					5   //AB相电流
#define     OBJTYPE_Ibc					6   //BC相电流
#define     OBJTYPE_Ica					7   //CA相电流
#define		OBJTYPE_I00					8	//零序电流
#define     OBJTYPE_I11					9   //正序电流
#define     OBJTYPE_I22					10  //负序电流

#define		OBJTYPE_P					20	//有功功率
#define		OBJTYPE_Pa					21  //A相有功
#define		OBJTYPE_Pb					22  //B相有功
#define		OBJTYPE_Pc					23  //C相有功
#define		OBJTYPE_Pab					24  //AB相有功
#define		OBJTYPE_Pbc					25  //BC相有功
#define		OBJTYPE_Pca					26  //CA相有功

#define		OBJTYPE_Q					40	//无功功率
#define		OBJTYPE_Qa					41  //A相无功
#define		OBJTYPE_Qb					42  //B相无功
#define		OBJTYPE_Qc					43  //C相无功
#define		OBJTYPE_Qab					44  //AB相无功
#define		OBJTYPE_Qbc					45  //BC相无功
#define		OBJTYPE_Qca					46  //CA相无功

#define		OBJTYPE_S					60	//视在功率
#define		OBJTYPE_COSYC				61	//力率（功率因数）

#define		OBJTYPE_I1					62  //高电流
#define		OBJTYPE_I2					63	//中电流
#define		OBJTYPE_I3				    64	//低电流
#define		OBJTYPE_Ia1					65	//A相高电流
#define		OBJTYPE_Ia2					66	//A相中电流
#define		OBJTYPE_Ia3					67	//A相低电流
#define		OBJTYPE_Ib1					68	//B相高电流
#define		OBJTYPE_Ib2					69	//B相中电流
#define		OBJTYPE_Ib3					70	//B相低电流
#define		OBJTYPE_Ic1					71	//C相高电流
#define		OBJTYPE_Ic2					72	//C相中电流
#define		OBJTYPE_Ic3					73	//C相低电流

#define		OBJTYPE_P1					74	//高有功功率
#define		OBJTYPE_P2					75	//中有功功率
#define		OBJTYPE_P3					76	//低有功功率
#define		OBJTYPE_Pa1					77  //A相高有功
#define		OBJTYPE_Pa2					78  //A相中有功
#define		OBJTYPE_Pa3					79  //A相低有功
#define		OBJTYPE_Pb1					80  //B相高有功
#define		OBJTYPE_Pb2					81  //B相中有功
#define		OBJTYPE_Pb3					82  //B相低有功
#define		OBJTYPE_Pc1					83  //C相高有功
#define		OBJTYPE_Pc2					84  //C相中有功
#define		OBJTYPE_Pc3					85  //C相低有功

#define		OBJTYPE_Q1					86	//高无功功率
#define		OBJTYPE_Q2					87	//中无功功率
#define		OBJTYPE_Q3					88	//低无功功率
#define		OBJTYPE_Qa1					89  //A相高无功
#define		OBJTYPE_Qa2					90  //A相中无功
#define		OBJTYPE_Qa3					91  //A相低无功
#define		OBJTYPE_Qb1					92  //B相高无功
#define		OBJTYPE_Qb2					93  //B相中无功
#define		OBJTYPE_Qb3					94  //B相低无功
#define		OBJTYPE_Qc1					95  //C相高无功
#define		OBJTYPE_Qc2					96  //C相中无功
#define		OBJTYPE_Qc3					97  //C相低无功


#define		OBJTYPE_ANGLE				100	//角度
#define		OBJTYPE_HZ					101	//频率
#define		OBJTYPE_TEMPERATURE			102	//温度
#define		OBJTYPE_PRESS				103	//压力
#define		OBJTYPE_WATER				104	//水位
#define		OBJTYPE_FLOW				105	//流量	 
#define		OBJTYPE_HUMIDITY			106	//湿度
#define		OBJTYPE_DW					107	//变压器档位

#define		OBJTYPE_YC					127	//其他遥测

#define		OBJTYPE_V					150	//电压
#define		OBJTYPE_Va					151	//A相电压
#define		OBJTYPE_Vb					152	//B相电压
#define		OBJTYPE_Vc					153	//C相电压
#define		OBJTYPE_Vab					154	//AB线电压
#define		OBJTYPE_Vbc					155	//BC线电压
#define		OBJTYPE_Vca					156	//CA线电压
#define		OBJTYPE_V0					157	//零序电压
#define     OBJTYPE_V1					158 //正序电压
#define     OBJTYPE_V2					159 //负序电压


//电度类型编码定义
#define		OBJTYPE_PKWH				200	//有功电度
#define		OBJTYPE_QKWH			    201	//无功电度
#define		OBJTYPE_PosPKWH				202	//正有功电度
#define		OBJTYPE_NegPKWH				203	//负有功电度
#define		OBJTYPE_PosQKWH				204	//正无功电度
#define		OBJTYPE_NegQKWH				205	//负无功电度
#define		OBJTYPE_RATE				206 //其它累积量


//遥信类型编码定义
#define		OBJTYPE_BREAK				300	//开关
#define		OBJTYPE_BRK_CAR				301	//小车开关
#define		OBJTYPE_BRK_BUSLKCAR		302 //母联小车开关
#define		OBJTYPE_BRK_BUSLINK			303 //母联开关
#define		OBJTYPE_BRK_BYBUS			304 //旁母开关
#define		OBJTYPE_BRK_BYBUSLK			305 //旁母兼母联开关

#define		OBJTYPE_STATUS				400	//状态

#define		OBJTYPE_SWITCH				500	//刀闸:不区分使用类型

#define		OBJTYPE_SW_LINE				501	//线路隔离刀
#define		OBJTYPE_SW_B1				502	//I母隔离刀
#define		OBJTYPE_SW_B2				503	//II母隔离刀
#define		OBJTYPE_SW_B3				504	//III母隔离刀
#define		OBJTYPE_SW_B4				505	//IV 母隔离刀
#define		OBJTYPE_SW_BYB				506 //旁母隔离刀
#define		OBJTYPE_SW_TRAN				507 //主变刀闸  
#define		OBJTYPE_SW_STRAN			508 //站变刀闸
#define		OBJTYPE_SW_PT				509	//PT刀闸
#define		OBJTYPE_SW_BLINK			510	//母联刀闸

#define		OBJTYPE_SW_L_GRD			541	//线路接地刀
#define		OBJTYPE_SW_TRAN_GRD			542	//主变接地刀
#define		OBJTYPE_SW_STRAN_GRD		543	//站变接地刀
#define		OBJTYPE_SW_PT_GRD			544	//PT接地刀

#define		OBJTYPE_SW_B1_GRD			571	//I母接地刀
#define		OBJTYPE_SW_B2_GRD           572	//II母接地刀
#define		OBJTYPE_SW_B3_GRD           573	//III母接地刀
#define		OBJTYPE_SW_B4_GRD           574	//IV 母接地刀
#define		OBJTYPE_SW_BYB_GRD          575 // 旁母接地刀

#define		OBJTYPE_PROTSIG				600	//保护
#define		OBJTYPE_SGZ					601	//事故总
#define		OBJTYPE_YG					602	//预告信号

//配网故障类型编码定义
#define		OBJTYPE_DIAG_SGZ            4001 //配网事故总信号/有故障
#define		OBJTYPE_DIAG_NOFAULT        4002 //无故障
#define		OBJTYPE_DIAG_INST           4003 //瞬间故障
#define		OBJTYPE_DIAG_UNKNOWN_FAULT  4004 //故障类型不明确
#define		OBJTYPE_DIAG_FAULT          4005 //有故障

#define		OBJTYPE_DIAG_S_GND          4011 //单相接地
#define		OBJTYPE_DIAG_S_GND_INST     4012 //单相瞬时接地
#define		OBJTYPE_DIAG_A_GND          4013 //A相接地
#define		OBJTYPE_DIAG_B_GND          4014 //B相接地
#define		OBJTYPE_DIAG_C_GND          4015 //C相接地
#define		OBJTYPE_DIAG_D_GND          4016 //相间接地
#define		OBJTYPE_DIAG_D_GND_INST     4017 //相间瞬时接地
#define		OBJTYPE_DIAG_AB_GND         4018 //AB相间接地
#define		OBJTYPE_DIAG_BC_GND         4019 //BC相间接地
#define		OBJTYPE_DIAG_CA_GND         4020 //4020 CA相间接地
#define		OBJTYPE_DIAG_T_GND          4021 //ABC相间接地

#define		OBJTYPE_DIAG_D_SHORT        4031 //相间短路
#define		OBJTYPE_DIAG_D_SHORT_INST   4032 //相间瞬时短路
#define		OBJTYPE_DIAG_AB_SHORT       4033 //AB相间短路
#define		OBJTYPE_DIAG_BC_SHORT       4034 //BC相间短路
#define		OBJTYPE_DIAG_CA_SHORT       4035 //CA相间短路
#define		OBJTYPE_DIAG_T_SHORT        4036 //ABC三相短路
#define		OBJTYPE_DIAG_T_SHORT_INST   4037 //相间瞬时短路

//FTU线路故障类型编码定义
#define		OBJTYPE_REP_SGZ             4101 //配网事故总信号/有故障
#define		OBJTYPE_REP_NOFAULT         4102 //无故障
#define		OBJTYPE_REP_INST            4103 //瞬间故障
#define		OBJTYPE_REP_UNKNOWN_FAULT   4104 //故障类型不明确
#define		OBJTYPE_REP_FAULT           4105 //有故障
	
#define		OBJTYPE_REP_S_GND           4111 //单相接地
#define		OBJTYPE_REP_S_GND_INST      4112 //单相瞬时接地
#define		OBJTYPE_REP_A_GND			4113 //A相接地
#define		OBJTYPE_REP_B_GND			4114 //B相接地
#define		OBJTYPE_REP_C_GND			4115 //C相接地

#define		OBJTYPE_REP_S_SHORT		    4121 //单相短路
#define		OBJTYPE_REP_A_SHORT		    4122 //A相短路
#define		OBJTYPE_REP_B_SHORT			4123 //B相短路
#define		OBJTYPE_REP_C_SHORT			4124 //C相短路
#define		OBJTYPE_REP_N_SHORT			4125 //零相短路（限值）
#define		OBJTYPE_REP_D_SHORT			4126 //相间短路
#define		OBJTYPE_REP_D_SHORT_INST	4127 //相间瞬时短路
#define		OBJTYPE_REP_AB_SHORT		4128 //AB相间短路
#define		OBJTYPE_REP_BC_SHORT		4129 //BC相间短路
#define		OBJTYPE_REP_CA_SHORT		4130 //CA相间短路

#define		OBJTYPE_REP_T_SHORT		    4131 //三相间短路
#define		OBJTYPE_REP_T_SHORT_INST	4132 //三相间瞬时短路

#define		OBJTYPE_REP_S_OVER			4141 //单相过流
#define		OBJTYPE_REP_S_OVER_INST		4142 //单相瞬时过流
#define		OBJTYPE_REP_A_OVER			4143 //A相过流
#define		OBJTYPE_REP_B_OVER			4144 //B相过流
#define		OBJTYPE_REP_C_OVER			4145 //C相过流
#define		OBJTYPE_REP_N_OVER			4146 //零相过流

#define		OBJTYPE_REP_D_OVER			4147 //相间过流
#define		OBJTYPE_REP_D_OVER_INST		4148 //相间瞬时过流
#define		OBJTYPE_REP_AB_OVER			4149 //AB相间过流
#define		OBJTYPE_REP_BC_OVER			4150 //BC相间过流
#define		OBJTYPE_REP_CA_OVER			4151 //CA相间过流


//类型判断的宏定义
#define     IS_YC_TYPE(a)		        ((a)>=1 && (a)<200)
#define     IS_VOLT_TYPE(a)             ((a)>=OBJTYPE_V && (a)<=OBJTYPE_V2)
#define     IS_PYC_TYPE(a)	        	((a)>=OBJTYPE_P && (a)<=OBJTYPE_Pca)
#define     IS_QYC_TYPE(a)	        	((a)>=OBJTYPE_Q && (a)<=OBJTYPE_Qca)
#define     IS_IYC_TYPE(a)	        	((a)>=OBJTYPE_I && (a)<=OBJTYPE_I2)

#define     IS_KWH_TYPE(a)              ((a)>=200 && (a)<300)

#define     IS_BREAK_TYPE(a)            ((a)>=300 && (a)<400)
#define     IS_STATUS_TYPE(a)           ((a)>=400 && (a)<500)
#define     IS_SWITCH_TYPE(a)           ((a)>=500 && (a)<600)
#define     IS_PROT_TYPE(a)             ((a)>=600 && (a)<700)
#define     IS_DMSSTATUS_TYPE(a)        ((a)>=4000 && (a)<5000)     //add 20021123 作为普通遥信处理


#define     IS_GLSWITCH_TYPE(a)	        ((a)>=OBJTYPE_SW_LINE && (a)<=OBJTYPE_SW_BLINK)
#define	    IS_GRDSWITCH_TYPE(a)	    ((a)>=OBJTYPE_SW_L_GRD && (a)<=OBJTYPE_SW_PT_GRD)
#define     IS_BUSGRDSWITCH_TYPE(a)     ((a)>=OBJTYPE_SW_B1_GRD && (a)<=OBJTYPE_SW_BYB_GRD)





///////////////////////////////////////////////////
////////////////  函数定义     ////////////////////
///////////////////////////////////////////////////


// 对象代码编码函数
// 输入：devtype 设备类型 ,  stname  站代码
//       devname 设备编号 ,  objtypename 对象类型
// 输出：objname 对象代码
// 返回：0 正确， 否则错误

#ifdef WIN32
int __declspec(dllexport)
#else
int
#endif
EncodeObjName(const char devtype,const char *stname,
				  const char *devname,const char  *objtypename,
				  char  *objname);


// 对象代码编码函数
// 输入：devtype 设备类型 ,  stname  站代码
//       devname 设备编号 ,  objtype 对象类型
// 输出：objname 对象代码
// 返回：0 正确， 否则错误
#ifdef WIN32
int __declspec(dllexport)
#else
int
#endif
EncodeObjName(const char devtype,const char *stname,
				  const char *devname,const short  objtype,
				  char  *objname);


// 对象代码解码函数
// 输入：objname  对象代码
// 输出：devtype  设备类型(同上)
//		 stname   站所线系代码(NULL 表示不输出)
//       devname  设备代码(同上)
//       objtypename 对象类型(同上)
// 返回：0 正确， 否则错误
#ifdef WIN32
int __declspec(dllexport)
#else
int
#endif
DecodeObjName(char *objname,char *devtype,
				   char *stname,char *devname,char *objtypename); 


// 对象代码解码函数
// 输入：objname  对象代码
// 输出：devtype  设备类型(同上)
//		 stname   站所线系代码(NULL 表示不输出)
//       devname  设备代码(同上)
//       objtype  对象类型(同上)
// 返回：0 正确， 否则错误
#ifdef WIN32
int __declspec(dllexport)
#else
int
#endif
DecodeObjName(char *objname,char *devtype,
				   char *stname,char *devname,short *objtype); 


// 对象代码重新编码函数

// 根据对象类型重新编码
// 输入：objname      对象代码
//       objtypename  新的对象类型
// 返回：0 正确， 否则错误
#ifdef WIN32
int __declspec(dllexport)
#else
int
#endif
EncodeObjName_ObjType(char *objname,const char *objtypename);


// 根据对象类型重新编码
// 输入：objname      对象代码
//       objtype      新的对象类型
// 返回：0 正确， 否则错误
#ifdef WIN32
int __declspec(dllexport)
#else
int
#endif
EncodeObjName_ObjType(char *objname,int objtype);



// 根据小车开关的设备编号编码相应开关和刀闸的编号
// 输入：brkdevname      小车开关的设备编号
// 输出：brkname         小车开关对应开关的设备编号
//       swname          小车开关对应刀闸的设备编号
// 返回：0 正确， 否则错误
#ifdef WIN32
int __declspec(dllexport)
#else
int
#endif
EncodeCarBreakName(const char *brkdevname,char *brkname,char *swname);


// 根据小车开关的设备编号编码相应开关和刀闸的编号
// 输入：brkswname       小车开关对应开关或刀闸的设备编号
// 输出：devname         小车开关的设备编号
// 返回：0 输入的是小车开关对应开关的编号
//       1 输入的是小车开关对应刀闸的编号
//       否则错误
#ifdef WIN32
int __declspec(dllexport)
#else
int
#endif
DecodeCarBreakName(const char *brkswname,char *devname);


// 获取对象类型的类别
// 输入: objtypename	对象类型名
//		 classmode		分类方法 （0---按SCADA表分类)
// 返回：类别

#ifdef WIN32
int __declspec(dllexport)
#else
int
#endif
ObjTypeClassId(const char *objtypename,short *objtype=NULL,int classmode=OBJTPMODE_SCDTAB);

// 获取对象类型的类别
// 输入: objtype	对象类型
//		 classmode		分类方法 （0---按SCADA表分类)
// 返回：类别

#ifdef WIN32
int __declspec(dllexport)
#else
int
#endif
ObjTypeClassId(const short objtype,char *objtypename=NULL,int classmode=OBJTPMODE_SCDTAB);

/////////////////////////////////////////////////////////////////////
/////////////////// 与公用设备表的接口 //////////////////////////////
/////////////////////////////////////////////////////////////////////
#ifdef WIN32
int __declspec(dllexport)
#else
int
#endif
GetDevParaRdbTableName(const char devtype,char *tabname);
/*
/////////////////////////////////////////////////////////////////////
////////////////////  公用表名定义 //////////////////////////////////
/////////////////////////////////////////////////////////////////////
#define  SCDBREAKDR_TABNAME			"ScdSw"
#define  SCDSWITCHDR_TABNAME		"ScdSw"
#define  SCDLINEDR_TABNAME			"ScdLn"
#define  SCDTRANSDR_TABNAME			"ScdTf"
#define  SCDCXDR_TABNAME			"ScdCb"
#define  SCDREACTORDR_TABNAME		"ScdRb"
#define  SCDBUSDR_TABNAME			"ScdBs"
#define  SCDLOADDR_TABNAME			"ScdLd"
#define  SCDWGDR_TABNAME			"ScdGn"
#define  SCDJOINTDR_TABNAME			"ScdJoint"	
#define  SCDBILEIDR_TABNAME			"ScdBilei"		// 避雷器表
#define  SCDCTDR_TABNAME			"ScdCurrent"	// 电流互感器
#define  SCDPTDR_TABNAME			"ScdVolact"		// 电压互感器
#define  SCDPROTECTDR_TABNAME		"ScdProtect"	// 保护装置
#define  SCDXHXQDR_TABNAME			"ScdXhxq"		// 消弧线圈

#define  EMSSWITCHP_TABNAME		"EMSSwitchP"
#define  EMSLINEP_TABNAME		"EMSLineP"
#define  EMSTRANSHP_TABNAME		"EMSTransP"
#define  EMSCXP_TABNAME			"EMSCXP"
#define  EMSREACTORP_TABNAME	"EMSDkqP"
#define  EMSBUSP_TABNAME		"EMSSBusP"
#define  EMSLOADP_TABNAME		"EMSLoadP"
#define  EMSGENERATORP_TABNAME	"EMSWgP"
#define  EMSJOINTP_TABNAME		"EMSJointP"
#define  EMSBILEIP_TABNAME		"EMSBiLeiP"
#define  EMSCTP_TABNAME			"EMSCurrentP"
#define  EMSPTP_TABNAME			"EMSVolactP"
#define  EMSPROTECTP_TABNAME	"EMSProtectP"
#define  EMSCOILP_TABNAME		"EMSXhxqP"


#define  EMSMSYSD_TABNAME		"EMSMsysD"
#define  EMSSWITCHD_TABNAME		"EMSSwitchD"
#define  EMSLINED_TABNAME		"EMSLineD"
#define  EMSTRANSHD_TABNAME		"EMSTransD"
#define  EMSCXD_TABNAME			"EMSCxD"
#define  EMSREACTORD_TABNAME	"EMSDkqD"
#define  EMSBUSD_TABNAME		"EMSSBusD"
#define  EMSLOADD_TABNAME		"EMSLoadD"
#define  EMSGENERATORD_TABNAME	"EMSWgD"
#define  EMSJOINTD_TABNAME		"EMSJointD"

#define  EMSMSYSDR_TABNAME		"EMSMsysDR"
#define  EMSSWITCHDR_TABNAME	"EMSSwitchDR"
#define  EMSLINEDR_TABNAME		"EMSLineDR"
#define  EMSTRANSHDR_TABNAME	"EMSTransDR"
#define  EMSCXDR_TABNAME		"EMSCxDR"
#define  EMSREACTORDR_TABNAME	"EMSDkqDR"
#define  EMSBUSDR_TABNAME		"EMSSBusDR"
#define  EMSLOADDR_TABNAME		"EMSLoadDR"
#define  EMSGENERATORDR_TABNAME	"EMSWgDR"
#define  EMSJOINTDR_TABNAME		"EMSJointDR"





/////////////////////////////////////////////////////////////////////////////////
///////////////  与电力设备对象的接口 ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////


typedef  struct    {
	char	devtype;			//设备类型代码
	char	substation[SUBSTNAME_LEN];	//站所线系代码 
	char	devname[POWDEVNAME_LEN];		//设备编号，已包含站所线系代码
	short	objtype;	//对象类型
	uchar	devobj_flag;	//2--取设备带电状态 1--指设备对象本身 0--指以设备命名的对象
}  DEV_OBJECT;
*/


//第一态(大态)	定义
#define  FIRST_STATE_SCADA		0 //SCADA实时态
/*
#define  FIRST_STATE_SE			1 //状态估计
#define  FIRST_STATE_PF			2 //潮流计算
#define  FIRST_STATE_OPF		3 //最优潮流
#define  FIRST_STATE_OVR		4 //无功优化
#define  FIRST_STATE_SC			5 //短路计算
#define  FIRST_STATE_CGD		6 //安全校正
#define  FIRST_STATE_PASDM		7 //PAS断面

#define  FIRST_STATE_STS		9 //变电站仿真

#define  FIRST_STATE_OPT		10 //操作票
*/
//第二态（小态）定义
#define  SECOND_STATE_REAL		0 //实时态
/*
#define  SECOND_STATE_S1		1 //研究态1
#define  SECOND_STATE_S2		2 //研究态2
#define  SECOND_STATE_S3		3 //研究态3
#define  SECOND_STATE_S4		4 //研究态4
#define  SECOND_STATE_S5		5 //研究态5
#define  SECOND_STATE_S6		6 //研究态6
#define  SECOND_STATE_S7		7 //研究态7
#define  SECOND_STATE_S8		8 //研究态8
#define  SECOND_STATE_S9		9 //研究态9

#define  SECOND_STATE_OS		50 //操作票初态
#define  SECOND_STATE_OE		51 //操作票末态
*/



typedef  struct    {
	char	devtype;			//设备类型代码
	char	substation[SUBSTNAME_LEN];	//站所线系代码 
	char	devname[POWDEVNAME_LEN];		//设备编号，已包含站所线系代码
	short	objtype;	//对象类型代码
	char	fieldname[RDB_COLNAME_LEN];	//域名
	uchar   firststate;		//第一态(大态) 
	uchar   secondstate;	//第二态（小态）0 实时态,1~9 为研究态
	uchar	devobj_flag;	//2--取SCADA设备带电状态 3--取PAS设备带电状态
} RDB_DEV_FIELD;

typedef  struct   {
	uchar			fieldmode;  // 1--RDB_DEV_FIELD有效 0--FIELDINFO有效
	union  {
		RDB_DEV_FIELD   devfield;
		RDB_FIELD_STRU		dbfield;
	}  fieldinfo;
} RDB_MIX_FIELD;

/////////////////////////////////////////////////////////////////////
////////////// 具有设备对象转换功能的实时库访问 /////////////////////
/////////////////////////////////////////////////////////////////////


////// 单个设备对象结构RDB_DEV_FIELD 转换到 数据库对象结构FIELDINFO  ////////

// 根据SCADA规则
#ifdef RDB_OS_WIN32
int __declspec(dllexport)
#else
int
#endif
DevFieldToDbField_SCADA(const RDB_DEV_FIELD *devfield,RDB_FIELD_STRU *dbfield);


/*
// 根据PAS规则
#ifdef RDB_OS_WIN32
int __declspec(dllexport)
#else
int
#endif
DevFieldToDbField_PAS(const RDB_DEV_FIELD *devfield,RDB_FIELD_STRU *dbfield);


// 根据操作票规则
#ifdef RDB_OS_WIN32
int __declspec(dllexport)
#else
int
#endif
DevFieldToDbField_OPT(const RDB_DEV_FIELD *devfield,RDB_FIELD_STRU *dbfield);

// 根据RDB_DEV_FIELD中的firststate自动区分
#ifdef RDB_OS_WIN32
int __declspec(dllexport)
#else
int
#endif
DevFieldToDbField(const RDB_DEV_FIELD *devfield,RDB_FIELD_STRU *dbfield);


////// 多个混合对象结构RDB_MIX_FIELD 转换到 数据库对象结构FIELDINFO  ////////
#ifdef RDB_OS_WIN32
int __declspec(dllexport)
#else
int
#endif
MixFieldToDbField(int nums,const RDB_MIX_FIELD *mixfield,RDB_FIELD_STRU *dbfield);
*/



#endif
