#ifndef GGRAPHPLUGIN_H_HEADER_INCLUDED_BC7D63CB
#define GGRAPHPLUGIN_H_HEADER_INCLUDED_BC7D63CB
#include "GBaseObject.h"
#include "GRectObject.h"

const static int GD_DEVTYPE_HMI = 101;
const static int GD_DEVTYPE_PAINTER = 102;

class GScaleFactor;
class GFile;

//加载动态库回调函数
//symbol = gdoFactoryBuild
//
//typedef GBaseObject* (*GDOFB_PROC)();
//##ModelId=4382CFEF0128
//##Documentation
//## 定义：
//##        图元插件类
//## 功能：
//##        定义图元插件类
//## 设计：
//##        宋永生
//## 日期：
//##       2005-11-22
class GDE_EXPORT_DES GGraphPlugin : public GRectObject
{
  public:
    //##ModelId=4382D15A037A
    //##Documentation
    //## 功能：
    //##     构造函数   
    //## 参数：
    //##     无
    //## 返回：
    //##     无
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-11-10
    GGraphPlugin();

    //##ModelId=4382D15A038A
    //##Documentation
    //## 功能：
    //##     构造函数   
    //## 参数：
    //##     const GGraphPlugin&
    //## 返回：
    //##     无
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-11-10
    GGraphPlugin(const GGraphPlugin& copy);

	//##ModelId=439EABD202DE
    //##Documentation
    //## 功能：
    //##     析构函数   
    //## 参数：
    //##     无
    //## 返回：
    //##     无
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-12-13
    virtual ~GGraphPlugin();

	//##ModelId=439EA9E701B5
    //##Documentation
    //## 功能：
    //##   设置图元填充风格操作
    //## 参数：  
    //##   const GFillStyle& //填充风格
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-6
    virtual void setFillStyle(const GFillStyle& style);
    //##ModelId=439EA9ED0177
    //##Documentation
    //## 功能：
    //##   设置图元线风格
    //## 参数：  
    //##   const GLineStyle& style //线风格
    //## 返回：
    //##   void
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-6
    virtual void setLineStyle(const GLineStyle& style);
    //##ModelId=439EAA2A035B
    //##Documentation
    //## 功能：
    //##      设置图元文本风格信息
    //## 参数：  
    //##     const GTextStyle&
    //## 返回：
    //##    无
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-5
    virtual void setTextStyle(const GTextStyle& style);
    //##ModelId=439EAA3703A9
    //##Documentation
    //## 功能：
    //##   设置图元ID操作
    //## 参数：  
    //##   uint32 uID //指定图元ID
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-6
    virtual void setID(uint32 uID);
	//##ModelId=43A0C3E1002E
    //##Documentation
    //## 功能：
    //##   设置内部实时值
    //## 参数：
    //##   FLOAT8 v
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-12-15
    virtual void setValue(FLOAT8 value);
	//##ModelId=43A607EC036B
    //##Documentation
    //## 功能：
    //##   测试该对象是否可以旋转
    //## 参数：
    //##   无
    //## 返回：
    //##   if is return 1
    //##   else return  0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-25
    virtual uint8 canRotate() const;
	//##ModelId=43A60320004E
    //##Documentation
    //## 功能：
    //##   绘制时需要多点或矩形
	//##   用于插件图元绘制
    //## 参数：
    //##   无
    //## 返回：
    //##   if 多点 return 1
    //##   else return  0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-25
    virtual uint8 needPoints(int32 mode=0) const;
	//##ModelId=43A60320004E
    //##Documentation
    //## 功能：
    //##   绘制时需要多点或矩形
	//##   用于插件图元绘制
    //## 参数：
    //##   无
    //## 返回：
    //##   if 多点 return 1
    //##   else return  0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-25
	virtual void  setPoints(const GListPOINTS& pts);

    //##ModelId=4382D15A038C
    //##Documentation
    //## 功能：
    //##     赋值函数   
    //## 参数：
    //##     const GGraphPlugin&
    //## 返回：
    //##     const GGraphPlugin&
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-11-10
    const GGraphPlugin& operator=(const GGraphPlugin& copy);

    //##ModelId=4382D15A038E
    //##Documentation
    //## 功能：
    //##   实现图元绘制功能，产生图元静态图象
    //## 参数：
    //##   GDC* pDC //绘制环境
    //##   GScaleFactor* pSF//缩放因子
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-3
    virtual uint8 drawObj(GDC* pDC, GScaleFactor* pSF = NULL);

    //##ModelId=4382D15A0391
    //##Documentation
    //## 功能：
    //##   计算图元矩形
    //## 参数：  
    //##   无
    //## 返回：
    //##   uint32
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-6
    virtual void calcRect();

    //##ModelId=4382D15A0392
    //##Documentation
    //## 功能：
    //##      重载读取函数，读取所有数据包含父类
    //## 参数：
    //##      GFile& fFile
    //## 返回：（int32）
    //##      if success return read real size
    //##      else return -1
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual int32 loadFile(GFile& fFile);

    //##ModelId=4382D15A0394
    //##Documentation
    //## 功能：
    //##      重载写入函数
    //## 参数：
    //##      GFile& fFile //文件
    //## 返回：（int32 ）
    //##      if success return write real size
    //##      else return -1
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual int32 saveFile(GFile& fFile);

    //##ModelId=4382D15A0396
    //##Documentation
    //## 功能：
    //##      重载读取函数
    //## 参数：
    //##      QDomElement& dom
    //## 返回：（uint8）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-25
    virtual uint8 readXML(QDomElement& dom);

    //##ModelId=4382D15A0398
    //##Documentation
    //## 功能：
    //##      重载保存函数
    //## 参数：
    //##      QTextStream& stream
    //##      int32 indent
    //## 返回：（uint8）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-25
    virtual uint8 saveXML(QDomElement& dom);

    //##ModelId=4382D15A039B
    //##Documentation
    //## 功能：
    //##   提供对象缩放计算，无返回，直接改变绘制对象
    //##   默认忽略，不要在该函数中调用pSF->scaleObject
    //## 参数： 
    //##   GScaleFactor *pSF //缩放因子
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-5
    virtual void scaleFactor(GScaleFactor* pSF);

    //##ModelId=4382D15A039D
    //##Documentation
    //## 功能：
    //##   测试点在图元指定范围
    //## 参数：  
    //##   const GPoint& pt
    //## 返回：
    //##   uint32
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-12
    virtual uint32 hitTest(const GPoint& pt);

    //##ModelId=4382D15A039F
    //##Documentation
    //## 功能：
    //##   测试图元是否在指定范围
    //## 参数：  
    //##   const GRect& rc
    //## 返回：
    //##   uint32
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-12
    virtual uint32 hitTest(const GRect& rc);

    //##ModelId=4382D15A03A1
    //##Documentation
    //## 功能：
    //##   实现图元绘制功能，产生图元静态图象
    //## 参数：
    //##   GDC* pDC //绘制环境
    //##   uint32 nFlags
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-3
    virtual void drawFocus(GDC* pDC, uint32 nFlags);

    //##ModelId=4382D15A03A4
    //##Documentation
    //## 功能：
    //##     获取操作点值，用于进行基本操作
    //## 参数：
    //##     无
    //## 返回：
    //##     操作点个数
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-11-12
    virtual int32 getOPT(GPoint* pts = NULL);

    //##ModelId=4382D15A03A6
    //##Documentation
    //## 功能：
    //##     获取旋转中心点，用于进行旋转操作
    //## 参数：
    //##     无
    //## 返回：
    //##     旋转中心点
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-11-12
    virtual GPoint getRO();

    //##ModelId=4382D15A03A9
    //##Documentation
    //## 功能：
    //##     获取旋转操作点，用于进行旋转操作
    //## 参数：
    //##     GPoint*
    //## 返回：
    //##     操作点坐标个数
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-11-12
    virtual int32 getROPT(GPoint* pts);

    //##ModelId=4382D15A03AB
    //##Documentation
    //## 功能：
    //##   X,Y偏移量
    //## 参数：
    //##   FLOAT8 x
    //##   FLOAT8 y
    //##   uint32 flags
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-14
    virtual void offsetXY(FLOAT8 x, FLOAT8 y, uint32 flags = 0);

    //##ModelId=4382D15A03AF
    //##Documentation
    //## 功能：
    //##   X,Y调整偏移量
    //## 参数：
    //##   const GPoint& pt
    //##   FLOAT8 x
    //##   FLOAT8 y
    //##   uint32 flags
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-14
    virtual void resizeXY(const GPoint& pt, FLOAT8 x, FLOAT8 y, uint32 flags = 0);

    //##ModelId=4382D15A03B4
    //##Documentation
    //## 功能：
    //##   对图元进行镜像处理
    //## 参数：
    //##   uint32 op
    //##   uint32 flags
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-15
    virtual void mirrorXY(uint32 op, uint32 flags = 0);

    //##ModelId=4382D15A03B9
    //##Documentation
    //## 功能：
    //##   以指定原点对图元进行镜像处理
    //## 参数：
    //##   const GPoint& pto
    //##   uint32 op
    //##   uint32 flags
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-15
    virtual void mirrorXY(const GPoint& pto, uint32 op, uint32 flags = 0);

    //##ModelId=4382D15A03BD
    //##Documentation
    //## 功能：
    //##   按指定旋转中心点ptc旋转th度
    //## 参数：
    //##   const GPoint& ptc
    //##   FLOAT8 th
    //##   uint32 flags
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-14
    virtual void rotateXY(const GPoint& ptc, FLOAT8 th, uint32 flags = 0);

    //##ModelId=4382D15A03C1
    //##Documentation
    //## 功能：
    //##   按图元内部旋转中心点旋转th度
    //## 参数：
    //##   FLOAT8 th
    //##   uint32 flags
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-14
    virtual void rotateXY(FLOAT8 th, uint32 flags = 0);

    //##ModelId=4382D15A03C4
    //##Documentation
    //## 功能：
    //##   重置中心点
    //## 参数：
    //##   无
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
    virtual void resetRO();
    //##ModelId=4387FB020138
    //##Documentation
    //## 功能：
    //##   图元对齐操作,根据指定功能对齐图元
    //## 参数：
    //##   uint32 op
    //##   FLOAT8 v
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
    virtual void alignPos(uint32 op, FLOAT8 v);
    //##ModelId=4387FB0201E4
    //##Documentation
    //## 功能：
    //##   图元等大小操作,根据指定功能等大小图元
    //## 参数：
    //##   uint32 op
    //##   FLOAT8 w
    //##   FLOAT8 h
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
    virtual void sameSize(uint32 op, FLOAT8 w, FLOAT8 h);
    //##ModelId=439E6ACA0110
    //##Documentation
    //## 功能：
    //##   鼠标进入操作
    //## 参数：
    //##   GDC* pDC
    //##   const GPoint& pt
    //##   uint32 flags
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-3
    virtual int32 onMouseEnter(GDC* pDC, const GPoint& pt, uint32 flags);
    //##ModelId=439E6ACA01AD
    //##Documentation
    //## 功能：
    //##   鼠标在对象中移动操作
    //## 参数：
    //##   GDC* pDC
    //##   const GPoint& pt
    //##   uint32 flags
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-16
    virtual int32 onMouseMove(GDC* pDC, const GPoint& pt, uint32 flags);
    //##ModelId=439E6ACA022A
    //##Documentation
    //## 功能：
    //##   鼠标离开操作
    //## 参数：
    //##   GDC* pDC
    //##   const GPoint& pt
    //##   uint32 flags
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    virtual int32 onMouseLeave(GDC* pDC, const GPoint& pt, uint32 flags);
    //##ModelId=439E6ACA02A7
    //##Documentation
    //## 功能：
    //##   鼠标点击左键弹起操作
    //## 参数：
    //##   GDC* pDC
    //##   const GPoint& pt
    //##   uint32 flags
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-16
    virtual int32 onLButtonUp(GDC* pDC, const GPoint& pt, uint32 flags);
    //##ModelId=439E6ACA0333
    //##Documentation
    //## 功能：
    //##   鼠标点击左键操作
    //## 参数：
    //##   GDC* pDC
    //##   const GPoint& pt
    //##   uint32 flags
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-16
    virtual int32 onLButtonDown(GDC* pDC, const GPoint& pt, uint32 flags);
    //##ModelId=439E6ACA03B0
    //##Documentation
    //## 功能：
    //##   鼠标点击右键弹起操作
    //## 参数：
    //##   GDC* pDC
    //##   const GPoint& pt
    //##   uint32 flags
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-16
    virtual int32 onRButtonUp(GDC* pDC, const GPoint& pt, uint32 flags);
    //##ModelId=439E6ACB0045
    //##Documentation
    //## 功能：
    //##   鼠标点击右键操作
    //## 参数：
    //##   GDC* pDC
    //##   const GPoint& pt
    //##   uint32 flags
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-16
    virtual int32 onRButtonDown(GDC* pDC, const GPoint& pt, uint32 flags);
    //##ModelId=439E6ACB00D2
    //##Documentation
    //## 功能：
    //##   获取控件提示信息
    //## 参数：
    //##   QString& tip
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
    virtual void getToolTip(QString& tip);

	//##ModelId=439EAA6800EA
    //##Documentation
    //## 功能：
    //##   创建实例图元
    //## 参数：
    //##   const QString& pluginsrc
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-12-13
	GBaseObject* newImpObject(const QString& pluginsrc);

	//##ModelId=439EAAE502FD
    //##Documentation
    //## 功能：
    //##   创建内部实例
    //## 参数：
    //##   无
    //## 返回：
    //##   if success return 1 
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-12-13
    uint8 createImpObject();

	//##ModelId=43A1744B0000
    //##Documentation
    //## 功能：
    //##   清除内部实例
    //## 参数：
    //##   无
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-12-13
    void clearImpObject();
	//##ModelId=439EA9C40271
    //##Documentation
    //## 功能：
    //##    插件交换内部数据,内部使用
    //## 参数：
    //##     GFile& fFile
    //##     uint8  bload
    //## 返回：
    //##     无
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-12-13
	virtual void transPluginData(GFile& fFile,uint8 bload=1);
	//##ModelId=439FC6B80180
    //##Documentation
    //## 功能：
    //##    插件交换位置信息
	//##    文件格式
    //##       GPoint  ptc中心点
    //##       FLOAT8  width 矩形宽的一半
    //##       FLOAT8  height矩形高的一半
    //## 参数：
    //##     GFile& fFile
    //##     uint8  bload
    //## 返回：
    //##     无
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-12-14
    virtual void transPluginPos(GFile& fFile, uint8 bload = 1);
	//##ModelId=43A0C20C0177
    //## 功能：
    //##    插件交换值信息 	
    //## 参数：
	//##	int npos
	//##	FLOAT8* pvalue
	//##	int vcnt
    //## 返回：
    //##     无
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-12-14
    virtual void transPluginValue(int npos,FLOAT8* pvalue,int vcnt=1);
	//##ModelId=43A0C0A70290
    //##Documentation
    //## 功能：
    //##    插件是否需要交换值信息 	
    //## 参数：
	//##	 无
    //## 返回：
    //##     无
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-12-14
	virtual uint8 needPluginTransValue() const;
	//##ModelId=439FA9900013
    //##Documentation
    //## 功能：
    //##     图元属性设置
    //## 参数：
    //##     QWidget *parent
    //## 返回：
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-12-14
    virtual uint8 showProperty(QWidget *parent);
	//##ModelId=43A0DA9C035B
    //##Documentation
    //## 功能：
    //##   获取控件名称
    //## 参数：
    //##   无
    //## 返回：
    //##   const char*
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-12-17
    virtual QString getPluginName() const;
	//##ModelId=43A0DA9C035B
    //##Documentation
    //## 功能：
    //##   获取控件位图
    //## 参数：
    //##   无
    //## 返回：
    //##   位图对象
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-12-17
	virtual QPixmap    getPixmap() const;
	//##ModelId=440522DF0224
    //##Documentation
    //## 功能：
    //##   实现图元定时响应功能
    //## 参数：
    //##   uint32 ymd //年月日=yyyymmdd
    //##   uint32 hmsm//时分秒毫秒=hhmmssmmm
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2006-2-27
    virtual uint8 onTimer(uint32 ymd, uint32 hmsm);
    //##ModelId=440522E00001
    //##Documentation
    //## 功能：
    //##   图元是否允许定时响应功能
    //## 参数：
    //##   无
    //## 返回：
    //##   if success return 1
    //##   return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2006-2-27
    virtual uint8 canTimer() const;

	//##ModelId=43A603AC0280
    //##Documentation
    //## 功能：
    //##   获取设备类型
    //## 参数：
    //##   无
    //## 返回：
    //##   uint32 
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
	virtual uint32 getDevType() const ;
	//##ModelId=43A603AC0280
    //##Documentation
    //## 功能：
    //##   设置设备类型
    //## 参数：
    //##   uint32 devType
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
	virtual void setDevType(uint32 devType);
	
	//##ModelId=43A603AC0280
    //##Documentation
    //## 功能：
    //##   设置行为
    //## 参数：
    //##   LPDA dynAct
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
	virtual void setAct(LPDA dynAct);	
	virtual void addAct( LPDA dynAct );
	virtual void rmvAct( LPDA dynAct );
	virtual void clrAct();

	//##ModelId=43A603AC0280
	//##Documentation
	//## 功能：
	//##   获取父窗体
	//## 参数：
	//##   无
	//## 返回：
	//##   QWidget*
	//## 设计：
	//##   lbh
	//## 日期：
	//##   2016-07-28
	virtual QWidget* getParentWidget() const;

	//##ModelId=43A603AC0280
	//##Documentation
	//## 功能：
	//##   设置父窗体
	//## 参数：
	//##   QWidget*
	//## 返回：
	//##   无
	//## 设计：
	//##   lbh
	//## 日期：
	//##   2016-07-28
	virtual void setParentWidget(QWidget* widget);

	
    //##ModelId=4383130A0177
    //##Documentation
    //## 插件源定义接口
    QString m_impSrc;
    //##ModelId=43A625C30203
    //##Documentation
    //## 内部图元实现
    GBaseObject* m_impObj;
};



#endif /* GGRAPHPLUGIN_H_HEADER_INCLUDED_BC7D63CB */
