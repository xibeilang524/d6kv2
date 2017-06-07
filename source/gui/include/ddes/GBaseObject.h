#ifndef GBASEOBJECT_H_HEADER_INCLUDED_BCB8A08D
#define GBASEOBJECT_H_HEADER_INCLUDED_BCB8A08D
#include <QtGui/QPixmap>
#include <QtCore/QString>
#include <QtCore/QMutex>

#include "ddef/ddes_comm.h"
#include "ddef/GRect.h"
#include "GDC.h"
#include "GGraphDynData.h"
#include "GVersion.h"
#include "GLineStyle.h"
#include "GFillStyle.h"
#include "GTextStyle.h"
#include "GRange.h"
#include "GObjectCommon.h"
#include "GScaleFactor.h"
#include "gcomminterface.h"

class GScaleFactor;
class GGraphLayer;
class GFile;

//##ModelId=4339E604009C
//##Documentation
//## 定义：
//##        图元基类
//## 功能：
//##        定义图元基本属性
//##        其他图元都有此继承
//## 设计：
//##       宋永生
//## 日期：
//##       2005-9-30
class GDE_EXPORT_DES GBaseObject
{
  public:
	//##ModelId=4341030F0203
    //##Documentation
    //## 功能：
    //##       构造函数   
    //## 参数：
    //##     无
    //## 返回：
    //##     无
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-3
    GBaseObject();

    //##ModelId=4341032E029F
    //##Documentation
    //## 功能：
    //##       析构函数    
    //## 参数：
    //##     无
    //## 返回：
    //##     无
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-3
    virtual ~GBaseObject();
	//##ModelId=43A601AF0148
    //##Documentation
    //## 功能：
    //##      重载读取数据函数，
    //##      读取所有数据信息
    //## 参数：
    //##      GFile& fFile
    //## 返回：（int32）
    //##      if success return read real size
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual int32 loadData(GFile& fFile);
    //##ModelId=433FE3130261
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
	//##ModelId=43A6027D0186
    //##Documentation
    //## 功能：
    //##      从其他对象中读取数据
    //## 参数：
    //##      GBaseObject *pObj
    //## 返回：（int32）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual int32 loadObj(GBaseObject* pObj );
	//##ModelId=43A602D10128
    //##Documentation
    //## 功能：
    //##      从其他对象中读取数据
    //## 参数：
    //##      const GBaseObject& obj
    //## 返回：（int32）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual int32 loadObj(const GBaseObject& obj);
	//##ModelId=43A6024803B9
    //##Documentation
    //## 功能：
    //##      重载保存数据函数，
    //##      保存所有数据信息
    //## 参数：
    //##      GFile& fFile
    //## 返回：（int32）
    //##      if success return read real size
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual int32 saveData(GFile& fFile);
    //##ModelId=433FE2890128
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

	//##ModelId=433FE2890128
    //##Documentation
    //## 功能：
    //##      是否能绘制对象
    //## 参数：
    //##      无
    //## 返回：（uint8 ）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
	virtual uint8 canDrawObj(GDC* pDC) const;

    //##ModelId=4340A54901C5
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

	//##ModelId=4340A54901C5
    //##Documentation
    //## 功能：
    //##   实现图元绘制功能，产生图元静态图象
    //## 参数：
    //##   GDC* pDC //绘制环境
    //##   const GFillStyle& fs//填充属性
	//##   const GLineStyle& ls//边框属性
	//##   uint8 bFill=1//是否填充
	//##   uint8 bFrame=1//是否绘制边框
	//##   GScaleFactor* pSF//缩放因子
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-3
	virtual void drawNormal(GDC *pDC ,\
		const GFillStyle& fs,\
		const GLineStyle& ls ,\
		uint8 bFill=1, uint8 bFrame=1 ,\
		GScaleFactor *pSF=NULL );

	//##ModelId=4340A54901C5
    //##Documentation
    //## 功能：
    //##   实现图元绘制功能，产生动态图元图象
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
	virtual void drawAction(GDC *pDC , GScaleFactor *pSF=NULL);

    //##ModelId=43422A6D031C
    //##Documentation
    //## 功能：
    //##   提供图元克隆技术
    //## 参数：
    //##   无
    //## 返回：
    //##   返回图元内部数据
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-3
    virtual GBaseObject* clone();

    //##ModelId=4344F67700FA
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

    //##ModelId=4344F6F3038A
    //##Documentation
    //## 功能：
    //##   取得图元ID操作
    //## 参数：  
    //##   无
    //## 返回：
    //##   uint32 
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-6
    uint32 getID() const ;

	//##ModelId=4344F6F3038A
    //##Documentation
    //## 功能：
    //##   取得图元范围
    //## 参数：  
    //##   无
    //## 返回：
    //##   GRect& 
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-6
	GRect& getBox();
    const GRect& getBox() const ;

	//##ModelId=4344F6F3038A
    //##Documentation
    //## 功能：
    //##   取得图元范围
    //## 参数：  
    //##   无
    //## 返回：
    //##   GRect& 
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-6
	GListPOINTS& getBound();

	//##ModelId=4344F6F3038A
    //##Documentation
    //## 功能：
    //##   取得图元范围
    //## 参数：  
    //##   无
    //## 返回：
    //##   GRect& 
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-6
	const GListPOINTS& getBound() const ;

	//##ModelId=43A0C35201B5
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

	//##ModelId=43A0C35201B5
    //##Documentation
    //## 功能：
    //##   设置内部实时值
    //## 参数：
    //##   const char* v
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-12-15
    virtual void setValue(const char* value);
    //##ModelId=43A0C3590119
    //##Documentation
    //## 功能：
    //##   获取内部实时值
    //## 参数：
    //##   无
    //## 返回：
    //##   FLOAT8
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-12-15
    FLOAT8 getValue() const;

    //##ModelId=4344F7770128
    //##Documentation
    //## 功能：
    //##   获取图元类型操作
    //## 参数：  
    //##   无
    //## 返回：
    //##   uint32
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-6
    uint32 getType() const;

    //##ModelId=4344F81300EA
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

    //##ModelId=4344F84F0232
    //##Documentation
    //## 功能：
    //##   获取图元填充风格操作
    //## 参数：  
    //##   无
    //## 返回：
    //##   const GFillStyle&
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-6
    const GFillStyle& getFillStyle() const;

    //##ModelId=4344F88203A9
    //##Documentation
    //## 功能：
    //##   获取图元填充风格操作
    //## 参数：  
    //##   无
    //## 返回：
    //##   GFillStyle&
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-6
    GFillStyle& getFillStyle();

    //##ModelId=4344F8C5006D
    //##Documentation
    //## 功能：
    //##   获取图元线风格
    //## 参数：  
    //##   无
    //## 返回：
    //##   GLineStyle&
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-6
    GLineStyle& getLineStyle() ;

    //##ModelId=4344F8F102EE
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

    //##ModelId=4344F92D0148
    //##Documentation
    //## 功能：
    //##   获取图元线风格
    //## 参数：  
    //##   无
    //## 返回：
    //##   const GLineStyl&
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-6
    const GLineStyle& getLineStyle() const;

    //##ModelId=4344F95200BB
    //##Documentation
    //## 功能：
    //##   设置图元旋转角度
    //## 参数：  
    //##   FLOAT8 ra//旋转角度
    //## 返回：
    //##   void
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-6
    virtual void setRotate(FLOAT8 ra);

    //##ModelId=4344F998035B
    //##Documentation
    //## 功能：
    //##   获取图元旋转角度
    //## 参数：  
    //##   无
    //## 返回：
    //##   FLOAT8
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-6
    FLOAT8 getRotate() const;

    //##ModelId=4344F9B800CB
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
    virtual void calcRect(){};

    //##ModelId=4344F9EF0119
    //##Documentation
    //## 功能：
    //##   设置图元版本号
    //## 参数：  
    //##   const GVersion& v
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-6
    void setVers(const GVersion& v);


    //##ModelId=4344FA5E03C8
    //##Documentation
    //## 功能：
    //##  获取图元主版本号
    //## 参数：  
    //##   无
    //## 返回：
    //##   const GVersion&
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-6
    const GVersion& getVers() const;

    //##ModelId=4344FA8703A9
    //##Documentation
    //## 功能：
    //##  获取图元次版本号
    //## 参数：  
    //##   无
    //## 返回：
    //##   GVersion&
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-6
    GVersion& getVers();

    //##ModelId=4344FAC702DE
    //##Documentation
    //## 功能：
    //##  设置图元当前层
    //## 参数：  
    //##   GGraphLayer *pLayer//当前层
    //## 返回：
    //##   void
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-6
    virtual void setLayer(GGraphLayer* pLayer);

    //##ModelId=4344FB160399
    //##Documentation
    //## 功能：
    //##  获取图元所在层
    //## 参数：  
    //##   无
    //## 返回：
    //##   GGraphLayer*
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-6
    GGraphLayer* getLayer() const;

    //##ModelId=43471C1C0213
    //##Documentation
    //## 功能：
    //##   设置图元索引指针位置
    //## 参数：
    //##   GPOS gPos//指定图元索引位置
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-8
    void setPos(GPOS gPos);

    //##ModelId=43471C230290
    //##Documentation
    //## 功能：
    //##   取图元对应索引位置
    //## 参数：
    //##   无
    //## 返回：
    //##   GPOS
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-8
    GPOS getPos() const;
    //##ModelId=434DC77E011E
    //##Documentation
    //## 功能：
    //##   设置是否显示该图元
    //## 参数：
    //##   uint8 visable//显示标志
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-13
    //##   
    void setVis(uint8 bVis=1);
    //##ModelId=434DC7DD010E
    //##Documentation
    //## 功能：
    //##   检测是否显示该图元
    //## 参数：
    //##   无
    //## 返回：uint8
    //##   if visable return 1
    //##   else return 0 
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-13
    uint8 isVis() const;
	 //##ModelId=437C11C10280
    //##Documentation
    //## 功能：
    //##   是否为删除状态
    //## 参数：
    //##   无
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
    uint8 isDel() const;
    //##ModelId=437C11F2032C
    //##Documentation
    //## 功能：
    //##   设置删除状态
    //## 参数：
    //##   uint8 bDel
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
    void setDel(uint8 bDel = 1);
	//##ModelId=434DCD9402F0
    //##Documentation
    //## 功能：
    //##   设置图元运行期连接标志
    //## 参数：
    //##   uint32 uFlag
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-13
    void setRunFlag(uint32 uFlag );

	//##ModelId=43A603AC0280
    //##Documentation
    //## 功能：
    //##   获取图元运行期连接标志
    //## 参数：
    //##   无
    //## 返回：
    //##   if is return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
	uint32 getRunFlag() const;

	//##ModelId=434DCD9402F0
    //##Documentation
    //## 功能：
    //##   设置图元连接标志
    //## 参数：
    //##   uint32 uFlag
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-13
    void setFlag(uint32 uFlag );

	//##ModelId=43A603AC0280
    //##Documentation
    //## 功能：
    //##   获取图元连接标志
    //## 参数：
    //##   无
    //## 返回：
    //##   if is return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
	uint32 getFlag() const;
    //##ModelId=434DCD9402F0
    //##Documentation
    //## 功能：
    //##   图元是否激活
    //## 参数：
    //##   uint8 active
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-13
    virtual void setActive(int32 active );
	//##ModelId=43A603AC0280
    //##Documentation
    //## 功能：
    //##   获取图元是否激活
    //## 参数：
    //##   无
    //## 返回：
    //##   if is return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
	int32 getActive() const;    
    //##ModelId=434DE2D101C1
    //##Documentation
    //## 功能：
    //##   取得图元动态尺寸，用于存储图元
    //## 参数：
    //##   无
    //## 返回：uint32
    //##   if success return realsize
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-13
    uint32 getSize() ;
	 //##ModelId=434DE2D101C1
    //##Documentation
    //## 功能：
    //##   取得数据长度
    //## 参数：
    //##   无
    //## 返回：uint32
    //##   if success return realsize
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-13
	uint32 getDataSize();	
    //##ModelId=4354C073011F
    //##Documentation
    //## 功能：
    //##   设置图元可视范围
    //## 参数：  
    //##   const GRange&
    //## 返回：
    //##   void
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-6
    void setRange(const GRange& range );
    //##ModelId=4354C0A4013E
    //##Documentation
    //## 功能：
    //##   获取图元可视范围
    //## 参数：  
    //##   无
    //## 返回：
    //##   const GRange&
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-6
    const GRange& getRange() const;
    //##ModelId=435D919900D4
    //##Documentation
    //## 功能：
    //##   设置显示范围
    //## 参数：
    //##   FLOAT8 vMin //最小显示范围
    //##   FLOAT8 vMax //最大显示范围
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-25
    void setRange(FLOAT8 vm, FLOAT8 vn);
    //##ModelId=435D922101DD
    //##Documentation
    //## 功能：
    //##   测试指定值是否在显示范围中
    //## 参数：
    //##   FLOAT8 v
    //## 返回：
    //##   if is return 1
    //##   else return  0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-25
    uint8 canView(FLOAT8 v) const;
	//##ModelId=43A60320004E
    //##Documentation
    //## 功能：
    //##   绘制时需要多点或矩形
	//##   用于插件图元绘制
    //## 参数：
    //##   mode
	//##   if mode == 0 
	//##		if 需要按折线绘制 return 1
	//##		else return 0
	//##   if mode == 1
	//##		if 像按下CTRL键一样绘制 return 1
	//##		else return 0
	//##   if mode == 2
	//##		if 像按下SHIFT键一样绘制 return 1
	//##		else return 0
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
	//##ModelId=43A60320004E
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
    //##ModelId=435DA0F202A2
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


    //##ModelId=435DA1010031
    //##Documentation
    //## 功能：
    //##      重载保存函数
    //## 参数：
    //##      QDomElement& dom
    //## 返回：（uint8）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-25
    virtual uint8 saveXML(QDomElement& dom);
    //##ModelId=436C451A032C
    //##Documentation
    //## 功能：
    //##   获取图元可视范围
    //## 参数：  
    //##   无
    //## 返回：
    //##   const GRange&
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-6
    GRange& getRange();
    //##ModelId=436C46F50399
    //##Documentation
    //## 功能：
    //##      获取图元文本风格信息
    //## 参数：
    //##    无
    //## 返回：
    //##     const GTextStyle&
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-5
    const GTextStyle& getTextStyle() const;
    //##ModelId=436C4712033C
    //##Documentation
    //## 功能：
    //##      获取图元文本风格信息
    //## 参数：
    //##    无
    //## 返回：
    //##     GTextStyle&
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-5
    GTextStyle& getTextStyle();
    //##ModelId=436C47290148
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
    //##ModelId=436C476400BB
    //##Documentation
    //## 功能：
    //##      取图元内部数据掩码
    //## 参数：
    //##     无
    //## 返回：（uint32）
    //##     返回内部数据掩码
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-5
    uint32 getStyleMask() const;
    //##ModelId=436C47B302AF
    //##Documentation
    //## 功能：
    //##      设置图元内部数据掩码
    //## 参数：
    //##     uint32 mask
    //## 返回：
    //##     无
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-5
    virtual void setStyleMask(uint32 mask);
    //##ModelId=436C6DF600DA
    //##Documentation
    //## 功能：
    //##   实现图元绘制功能，产生图元静态图象
    //## 参数：
    //##   GDC* pDC         //绘制环境
    //##   GRect* pRect     //矩形区域
    //##   GScaleFactor* pSF//缩放因子
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-3
    void onDraw(GDC* pDC, const GRect& rc, GScaleFactor* pSF = NULL);
    //##ModelId=436C781902BF
    //##Documentation
    //## 功能：
    //##   获取图元内部缩放因子操作
    //## 参数：  
    //##   无
    //## 返回：
    //##   const GScaleFactor&
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-5
    FLOAT8 getScale() const;    
    //##ModelId=436C785E0251
    //##Documentation
    //## 功能：
    //##   设置图元内部缩放因子操作
    //## 参数：  
    //##   无
    //## 返回：
    //##   const GScaleFactor&
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-5
    void setScale(FLOAT8 scale);
    //##ModelId=436C970A0213
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
	//##ModelId=4375D0CC01D4
    //##Documentation
    //## 功能：
    //##   测试当前点在图元相关位置
    //## 参数：  
    //##   const GPoint& pt
    //## 返回：
    //##   uint32
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-12
    virtual uint32 hitTest(const GPoint& pt);
    //##ModelId=4375D1AB02CE
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
    //##ModelId=4375DAD7031C
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
    virtual void drawFocus(GDC* pDC, uint32 nFlags = 0);
    //##ModelId=4375DB6B03A9
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
	//##ModelId=437C7800000F
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
	//##ModelId=437AB085004C
    //##Documentation
    //## 功能：
    //##      设置旋转中心点
    //## 参数：
    //##      const GPoint& po
    //## 返回：
    //##      无
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-16
    virtual void setRO(const GPoint& po);
    //##ModelId=4375DC69002E
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
    //##ModelId=4375DCC301B5
    //##Documentation
    //## 功能：
    //##     获取旋转操作点，用于进行旋转操作
    //## 参数：
    //##     无
    //## 返回：
    //##     操作点坐标
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-11-12
    virtual int32 getROPT(GPoint *);
	
	//##ModelId=4377E06900DA
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
    //##ModelId=4377F8980034
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
	 //##ModelId=43793D6C0177
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
    //##ModelId=43788BA2032C
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
    //##ModelId=43793752036B
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
    //##ModelId=43793DC00177
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
	//##ModelId=43771C78002E
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
    //##ModelId=43771C9A02BF
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
	//##ModelId=437B2584031C
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
    //##ModelId=437B261000FA
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
    //##ModelId=437B2676000F
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
    //##ModelId=437B26810290
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
    //##ModelId=437B27590251
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
	//##ModelId=43814F200196
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
	//##ModelId=438690240232
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
    //##ModelId=438690A002FD
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

	//##ModelId=439EA8B601C5
    //##Documentation
    //## 功能：
    //##    插件交换内部数据,内部使用
    //## 参数：
    //##     GFile& fFile
    //##     uint8  bLoad
    //## 返回：
    //##     无
    //## 设计：
    //##     宋永生
    //## 日期：
    //##     2005-12-13
	virtual void transPluginData(GFile& fFile,uint8 bload=1);
	//##ModelId=439FC62A00F3
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
	//##ModelId=43A0C0A70290
    //##Documentation
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
	//##ModelId=439FA91D01E7
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
	//##ModelId=43A0DD2A000F
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
	//##ModelId=43A6035702DE
    //##Documentation
    //## 功能：
    //##   设置保存掩码
    //## 参数：
    //##   uint32 mask
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
    virtual void setMask(uint32 mask);
    //##ModelId=43A603870271
    //##Documentation
    //## 功能：
    //##   清除保存掩码
    //## 参数：
    //##   uint32 mask
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
    virtual void clearMask(uint32 mask=0);
    //##ModelId=43A603AC0280
    //##Documentation
    //## 功能：
    //##   判断是否具有指定的掩码
    //## 参数：
    //##   uint32 mask
    //## 返回：
    //##   if is return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
    virtual uint8 isMask(uint32 mask) const;

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
	virtual void setDevType(uint32 devType) ;

	//##ModelId=43A603AC0280
    //##Documentation
    //## 功能：
    //##   获取设备状态
    //## 参数：
    //##   uint32
    //## 返回：
    //##   uint8 
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
	virtual uint8 isDevStatus(uint32 devStatus) const;
	//##ModelId=43A603AC0280
    //##Documentation
    //## 功能：
    //##   获取设备状态
    //## 参数：
    //##   无
    //## 返回：
    //##   uint32 
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
	virtual uint32 getDevStatus() const ;
	//##ModelId=43A603AC0280
    //##Documentation
    //## 功能：
    //##   设置设备状态
    //## 参数：
    //##   uint32 devStatus
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
	virtual void setDevStatus(uint32 devStatus) ;

	//##ModelId=43A603AC0280
    //##Documentation
    //## 功能：
    //##   增加设备状态
    //## 参数：
    //##   uint32
    //## 返回：
    //##   uint8 
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
	virtual void addDevStatus(uint32 devStatus);

	//##ModelId=43A603AC0280
    //##Documentation
    //## 功能：
    //##   清除设备状态
    //## 参数：
    //##   uint32
    //## 返回：
    //##   uint8 
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
	virtual void clrDevStatus(uint32 devStatus);
	
	//##ModelId=43A603AC0280
    //##Documentation
    //## 功能：
    //##   判断是否允许粘帖
    //## 参数：
    //##   无
    //## 返回：
    //##   if is return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
	virtual uint8 canSnap() const ;

	//##ModelId=43A603AC0280
    //##Documentation
    //## 功能：
    //##   判断指定点是否位于粘帖范围
    //## 参数：
    //##   const GPoint& pt
    //## 返回：
    //##   if is return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
	virtual uint8 hitSnap(const GPoint& pt);

	//##ModelId=43A603AC0280
    //##Documentation
    //## 功能：
    //##   获取闪现标志
    //## 参数：
    //##   无
    //## 返回：
    //##   if is return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
	uint8  getFlash() const ;
	//##ModelId=43A603AC0280
    //##Documentation
    //## 功能：
    //##   设置闪现标志
    //## 参数：
    //##   uint8 f
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
	void   setFlash(uint8 f=1) ;

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
    //##   获取行为
    //## 参数：
    //##   无
    //## 返回：
    //##   LPDA
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
	//LPDA  getAct() const ;
	uint32 countAct() const;
	GPOS   firstAct() const;
	LPDA   nextAct(GPOS& gPos) const;
	GPOS   lastAct( ) const;
	//##ModelId=43FFCF4A009C
    //##Documentation
    //## 功能：
    //##   实现图元定时响应功能
    //## 参数：
    //##   uint32 ymd//年月日=yyyymmdd
    //##   uint32 hms//时分秒=hhmmss
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2006-2-27
    virtual uint8 onTimer(uint32 ymd, uint32 hmsm);
    //##ModelId=43FFCFFC030D
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

	//##ModelId=43FFCFFC030D
    //##Documentation
    //## 功能：
    //##   取图元中的动态数据链表
    //## 参数：
    //##   无
    //## 返回：
    //##   GGraphDynDataList*
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2006-2-27
	virtual GGraphDynDataList* getDynData() const;

	//##ModelId=43FFCFFC030D
    //##Documentation
    //## 功能：
    //##   设置图元中的附加数据指针
    //## 参数：
    //##   GHANDLE 
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2006-2-27
	void setAttachPtr(GHANDLE data);

	//##ModelId=43FFCFFC030D
    //##Documentation
    //## 功能：
    //##   取图元中的附加数据指针
    //## 参数：
    //##   无
    //## 返回：
    //##   GHANDLE
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2006-2-27
	GHANDLE getAttachPtr() const;

	//##ModelId=43FFCFFC030D
    //##Documentation
    //## 功能：
    //##   设置图元中的附加数据
    //## 参数：
    //##   uint32 
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2006-2-27
	void setAttachData(uint32 data);

		//##ModelId=43FFCFFC030D
    //##Documentation
    //## 功能：
    //##   取图元中的附加数据
    //## 参数：
    //##   无
    //## 返回：
    //##   GHANDLE
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2006-2-27
	uint32 getAttachData() const;

	//##ModelId=43A603AC0280
    //##Documentation
    //## 功能：
    //##   获取父图元
    //## 参数：
    //##   无
    //## 返回：
    //##   GBaseObject*
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
	GBaseObject* getParent() const;

	//##ModelId=43A603AC0280
    //##Documentation
    //## 功能：
    //##   设置父图元
    //## 参数：
    //##   GBaseObject*
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
	void setParent(GBaseObject* parent);
	
	//##ModelId=43A6064003B9
    //##Documentation
    //## 功能：
    //##   重新设置端子属主ID
    //## 参数：
    //##   reset 
	//##	 if =0 重置OwnerID
	//##	 else  清除OwnerID
	//##   nID  
	//##	 if =-1 当前ID为子端子的属主
	//##	 else	指定ID为属主
	//##   pObj
	//##	 if =NULL 当前对象为子端子的属主
	//##	 else	  指定对象为属主	
    //## 返回：
    //##   if is return 1
    //##   else return  0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-25
	virtual void setPortOwner(uint8 reset=0,uint32 nID=(uint32)-1,\
		GBaseObject* pObj=NULL);

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

    //##ModelId=433D3DC30000
    //##Documentation
    //## 图元ID号，在图中唯一标识
    uint32 m_nID;

    //##ModelId=433D3DF703B9
    //##Documentation
    //## 图元类型
    uint32 m_nType;

    //##ModelId=433D3FDF02AF
    //##Documentation
    //## 图元范围
    GRect m_boundBox;

    //##ModelId=4340FC7402EE
    //##Documentation
    //## 动态数据列表
	GGraphDynDataList m_lstDynData;
    //##ModelId=434DC6710312
    //##Documentation
    //## 是否显示该图元 1=vis,0=not vis
    uint8 m_vis;
    //##ModelId=43531B9C0213
    //##Documentation
    //## 版本号
    GVersion m_vers;
    //##ModelId=436C42490157
    //##Documentation
    //## 线风格
    GLineStyle m_lineStyle;
    //##ModelId=436C42B1036B
    //##Documentation
    //## 填充风格
    GFillStyle m_fillStyle;
    //##ModelId=436C42C70148
    //##Documentation
    //## 文本风格
    GTextStyle m_textStyle;
    //##ModelId=436C42DC035B
    //##Documentation
    //## 可视范围
    GRange m_range;
    //##ModelId=436C479E0128
	//##Documentation
    //## 存盘掩码
    uint32 m_styleMask;  
	//##ModelId=436C779D00EA
    //##Documentation
    //## 内部比例因子，用于内部数据固化比例
    FLOAT8 m_scale;
    //##ModelId=437AB0600230
    //##Documentation
    //## 旋转中心点
    GPoint m_pointO;
    //##ModelId=4340A28500FA
    //##Documentation
    //## 旋转角度
    FLOAT8 m_angle;
	//##ModelId=4340A28500FA
    //##Documentation
    //## 旋转行为执行
	FLOAT8 m_rotate;
	//##ModelId=43A0C32E036B
    //##Documentation
    //## 内部实时值
    FLOAT8 m_fValue;
	//##ModelId=43A0C32E036B
    //##Documentation
    //## 设备类型
    uint32 m_devType;
	//##ModelId=43A0C32E036B
    //##Documentation
    //## 设备状态
    uint32 m_devStatus;
	
  protected:
    //##ModelId=4340EFE00261
    //##Documentation
    //## 图元所在层
    GGraphLayer* m_curLayer;
    //##ModelId=43471BEA005D
    //##Documentation
    //## 记录当前图元对应层链表索引指针
    GPOS m_curPos;
	//##ModelId=437C11460167
    //##Documentation
    //## 该图元是否为删除状态，如果为删除状态
    //## 存储时不进行存储，绘制时不进行绘制
    uint8 m_del;
	//##ModelId=437C11460167
    //##Documentation
    //## 图元动态行为列表
	//LPDA  m_dynAct ;s
	GDAL  m_dynAct ;
	//##ModelId=437C11460167
    //##Documentation
    //## 闪烁标志
	uint8 m_flash  ;
	//##ModelId=434DCD5D014B
    //##Documentation
    //## 图元连接标志
    uint32 m_topoFlag;
    //##ModelId=434DCD5D014B
    //##Documentation
    //## 图元的着色状态,如果为0时表示该图元为非实时量图元,不需要根据状态来改变颜色
    int32 m_active;
	//##ModelId=437C11460167
    //##Documentation
    //## 附加数据指针
	GHANDLE m_attachDataPtr;
	//##ModelId=437C11460167
    //##Documentation
    //## 附加数据,用于用户信息处理
	uint32  m_attachData;
	//##ModelId=437C11460167
    //##Documentation
    //## 运行期连接标志
	uint32 m_runFlag;
	//##ModelId=43A625C30203
    //##Documentation
	//## 父图元，与父图元交互使用
	GBaseObject* m_parentObj;
	//##ModelId=43A625C30204
	//##Documentation
	//## 父窗体，与父窗体交互使用
	QWidget* m_parentWidget;


	QMutex		m_actionMutex;
	QMutex		m_setValueMutex;

  protected:
	//##ModelId=437C11460167
    //##Documentation
    //## 获取指定调整位置的点位置
	virtual const GPoint getPoint(uint32 size) const;
};

#endif /* GBASEOBJECT_H_HEADER_INCLUDED_BCB8A08D */
