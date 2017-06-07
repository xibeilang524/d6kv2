#ifndef PROTOCOL_CDT92_H_
#define PROTOCOL_CDT92_H_
#include <QList>
#include <QListIterator>
#include "msgview_string.h"

static QString cdt_header =  _C2Q("(同步字6字节)");
static QString cdt_header_error =  _C2Q("(同步字错误)");
static QString cdt_crc_error =  _C2Q("(校验码错误)");
static QString cdt_len_error =  _C2Q("(报文长度错误)");

//static QString remind_space = " ";
//static QString remind_leftkuohao = "(";
//static QString remind_rightkuohao = ")";

static QString cdt_control_field =  _C2Q("(控制字节)");
static QString cdt_frame_sort =  _C2Q("帧类别:");
static QString cdt_frame_len =  _C2Q("信息字数:");
static QString cdt_source_addr =  _C2Q("源站地址:");
static QString cdt_target_addr =  _C2Q("目的站地址:");
static QString cdt_frame_crc =  _C2Q("校验码");

//帧类别
static QString cdt_frame_a =  _C2Q("重要遥测A帧");
static QString cdt_frame_b =  _C2Q("次要遥测B帧");
static QString cdt_frame_c =  _C2Q("一般遥测C帧");
static QString cdt_frame_di =  _C2Q("遥信状态");
static QString cdt_frame_ps =  _C2Q("电能脉冲计数值");
static QString cdt_frame_soe =  _C2Q("SOE信息");
static QString cdt_frame_ctrl_sel =  _C2Q("遥控选择");
static QString cdt_frame_ctrl_exe =  _C2Q("遥控执行");
static QString cdt_frame_ctrl_can =  _C2Q("遥控撤销");
static QString cdt_frame_ctrl_con =  _C2Q("遥控反校");
static QString cdt_frame_updown_sel =  _C2Q("升降选择");
static QString cdt_frame_updown_exe =  _C2Q("升降执行");
static QString cdt_frame_updown_cle =  _C2Q("升降撤销");
static QString cdt_frame_idi_set =  _C2Q("设定命令");
static QString cdt_frame_clock_set =  _C2Q("设置时钟");
static QString cdt_frame_clock_con =  _C2Q("设置时钟校验值");
static QString cdt_frame_clock_sum =  _C2Q("召唤子站时钟");
static QString cdt_frame_reset =  _C2Q("复归命令");
static QString cdt_frame_brod =  _C2Q("广播命令");


//功能码
static QString cdt_func_yx_begin =  _C2Q("遥信起始序号:");
static QString cdt_func_yc_begin =  _C2Q("遥测起始序号:");
static QString cdt_func_yk_begin =  _C2Q("遥控起始序号:");
static QString cdt_func_ym_begin =  _C2Q("遥脉起始序号:");
static QString cdt_func_soe_begin =  _C2Q("soe起始序号:");

static QString cdt_func_yx_value =  _C2Q("遥信值");
static QString cdt_func_yc_value =  _C2Q("遥测");
static QString cdt_func_yk_value =  _C2Q("遥控值:");
static QString cdt_func_ym_value =  _C2Q("遥脉值:");
static QString cdt_func_soe_value =  _C2Q("soe:");

static QString cdt_func_fre =  _C2Q("频率");
static QString cdt_func_reset =  _C2Q("复归命令");
static QString cdt_func_brod =  _C2Q("广播命令");

static QString cdt_func_ctrl_sel =  _C2Q("遥控选择功能码");
static QString cdt_func_ctrl_con =  _C2Q("遥控返校功能码");
static QString cdt_func_ctrl_exe =  _C2Q("遥控执行功能码");
static QString cdt_func_ctrl_cle =  _C2Q("遥控撤销功能码");

static QString cdt_func_updown_sel =  _C2Q("升降选择");
static QString cdt_func_updown_con =  _C2Q("升降返校");
static QString cdt_func_updown_exe =  _C2Q("升降执行");
static QString cdt_func_updown_cle =  _C2Q("升降撤销");

static QString cdt_func_idi_set =  _C2Q("设置命令");
static QString cdt_func_child_idi =  _C2Q("子站状态信息");
static QString cdt_func_clock_con =  _C2Q("设置时钟校正值");

//static char cdt_temp_buf[1024];
static BYTE cdt_complete_buf[1024];
//static char* cdt_complete_buf;
static bool cdt_frame_finish = false;
static bool cdt_frame_first = true;
//static int cdt_len_full = 0;
static QList <BYTE> cdt_frame_list;   //CDT分帧处理队列


//帧类别代码
enum CDT_TYPE
{

	CDT_FRAME_A = 0x61,    //重要遥测，A帧
	CDT_FRAME_B = 0xC2,    //次要遥测，B帧
	CDT_FRAME_C = 0xB3,    //一般遥测，C帧
	CDT_FRAME_DI = 0xF4,   //遥信状态
	CDT_FRAME_PS = 0x85,   //电能脉冲计数值
	CDT_FRAME_SOE = 0x26,  //时间顺序记录

    CDT_FRAME_CTRL_SEL = 0x61,  //遥控选择
    CDT_FRAME_CTRL_EXE = 0xC2,  //遥控执行
    CDT_FRAME_CTRL_CAN = 0xB3,  //遥控撤销
	CDT_FRAME_CTRL_CON  = 0X9E,      //遥控反校

	CDT_FRAME_UPDOWN_SEL = 0xF4,  //升降选择
	CDT_FRAME_UPDOWN_EXE = 0x85,  //升降执行
	CDT_FRAME_UPDOWN_CAN = 0x26,  //升降撤销

	CDT_FRAME_IDI_SET = 0x57,//设定命令

	CDT_FRAME_CLOCK_SET = 0x7A,  //设置时钟
	CDT_FRAME_CLOCK_CON = 0x0B,  //设置时钟校正值

	CDT_FRAME_CLOCK_SUM = 0x4C,   //召唤子站时钟

	CDT_FRAME_RESET = 0x3D,      //复归命令
	CDT_FRAME_BROD  = 0x9E       //广播命令

};

//功能码
enum CDT_FUNC
{
	//00H-7FH    遥测 256
	//F0H-FFH    遥信 512
    //80H-81H    SOE 4096
	//82H-83H    备用
	//84H-85H    子站时钟反送 1
	//86H-89H    总加遥测 8
	//8DH-92H    水位 6
	//93H-9FH    备用
	//A0H-DFH    电能脉冲计数值 64
	//EEH-EFH    设置时钟（下行）1


	CDT_FUNC_FRE = 0x8A,        //频率 2
	CDT_FUNC_RESET = 0x8B,      //复归命令（下行） 16
	CDT_FUNC_BROD = 0x8C,       //广播命令（下行） 16

	CDT_FUNC_CTRL_SEL = 0xE0,   //遥控选择（下行） 256
	CDT_FUNC_CTRL_CON = 0xE1,   //遥控返校（下行） 256
	CDT_FUNC_CTRL_EXE = 0xE2,   //遥控执行（下行） 256
	CDT_FUNC_CTRL_CAN = 0xE3,   //遥控撤销（下行） 256
	
	CDT_FUNC_UPDOWN_SEL = 0xE4,  //升降选择（下行） 256
	CDT_FUNC_UPDOWN_CON = 0xE5,  //升降返校（下行） 256
	CDT_FUNC_UPDOWN_EXE = 0xE6,  //升降执行（下行） 256
	CDT_FUNC_UPDOWN_CAN = 0xE7,  //升降撤销（下行） 256

	CDT_FUNC_IDI_SET = 0xE8,    //设置命令（下行） 256
	CDT_FUNC_CHILD_IDI = 0xEC,  //子站状态信息 1
	CDT_FUNC_CLOCK_CON = 0xED   //设置时钟校正值（下行） 1
};

#endif