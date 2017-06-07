#ifndef _GCOMM_INTERFACE_H
#define _GCOMM_INTERFACE_H

#include "gcommfunc.h"
class GDC;

//颜色转换
#define G2Q_RGB(rgb) convG2Q_RGB(rgb)
#define Q2G_RGB(rgb) convQ2G_RGB(rgb)
#define G2Q_CLR(rgb) convG2Q_CLR(rgb)
#define Q2G_CLR(rgb) convQ2G_GLR(rgb)
#define GPL2QPA(pl,pa) convGPL_QPA(pl,pa)
#define QPA2GPL(pa,pl) convQPA_GPL(pa,pl)

//路径设置
#define DEFAULT_ENV			getDefaultEnv()
#define DEFAULT_PATH		getDefaultPath()
#define DEFAULT_GRAPH_PATH	getDefaultGraphPath()
#define DEFAULT_PLUGIN_PATH getDefaultPluginPath()
#define DEFAULT_CUSTOM_PATH getDefaultCustomPath()
#define DEFAULT_SYMBOL_PATH getDefaultSymbolPath()
#define DEFAULT_EXEC_PATH	getDefaultExecPath()
#define DEFAULT_INI_PATH	getDefaultIniPath()

GDE_EXPORT_DES uint8	existFilePath(const QString& strPath);
GDE_EXPORT_DES void		createDirectory(const QString& strPath);
//相关路径设置
//设置获取环境变量
GDE_EXPORT_DES void    setDefaultEnv(const char* strEnv);
GDE_EXPORT_DES const char*   getDefaultEnv();

//初始化相关路径
GDE_EXPORT_DES void    initDefaultPath();
GDE_EXPORT_DES uint8   isInitDefaultPath();

//获取相关路径
GDE_EXPORT_DES const QString getDefaultPath();
GDE_EXPORT_DES const QString getDefaultGraphPath();
GDE_EXPORT_DES const QString getDefaultPluginPath();
GDE_EXPORT_DES const QString getDefaultCustomPath();
GDE_EXPORT_DES const QString getDefaultSymbolPath();
GDE_EXPORT_DES const QString getDefaultExecPath();
GDE_EXPORT_DES const QString getDefaultIniPath();

//允许释放空闲时间处理刷新
GDE_EXPORT_DES void	QProcessEvents();

//过滤检查图元是否允许搜索
typedef uint8 (*GTS_FILTER_FUNC)(GBaseObject *pObj,void* clientData);

#define DLGF_MST_INFO		0x00
#define DLGF_MST_DESC		0x01
#define DLGF_MST_WARNING	0x02
#define DLGF_MST_ERROR		0x03
#define DLGF_MST_STATUS		0x04

//加载过程处理回调，用于动态显示加载过程
typedef void (*DYNLOAD_GRAPH_FUNC)(const char* msg,uint32 msgtype,uint32 n,uint32 total);

//设置动态显示加载过程
GDE_EXPORT_DES void setDynLoadFunc(DYNLOAD_GRAPH_FUNC dynLoad);

//获取动态加载过程
GDE_EXPORT_DES DYNLOAD_GRAPH_FUNC getDynLoadFunc();

//调用动态加载过程
GDE_EXPORT_DES void dynLoadFunc(const char* msg="",uint32 nsgtype=DLGF_MST_STATUS,uint32 n=0,uint32 total=0);	

//图元着色
GDE_EXPORT_DES void setObjectColor(GBaseObject* pObj,uint32 color);

//变压器图元着色
GDE_EXPORT_DES void	setObjectColor(GBaseObject* pObj,uint32 color,int32 sno);

//获取设备参数
GDE_EXPORT_DES GGraphDynData* getDevPara(GBaseObject* pObj);

//获取实时参数
GDE_EXPORT_DES GGraphDynData* getRealPara(GBaseObject* pObj);

//获取数据相关数据类型对应值
GDE_EXPORT_DES uint8 getDynDataPara(GGraphDynData* pData,int32 type,QString& value);

//设置数据相关数据类型对应值
GDE_EXPORT_DES uint8 setDynDataPara(GGraphDynData* pData,int32 type,const QString& value);

//接收图元对象事件
GDE_EXPORT_DES int32 receiveMouseEvent(GBaseObject* pObject,GDC* pDC , const GPoint& pt,uint8 lbtn , uint8 down , uint32 uflags );

#endif
