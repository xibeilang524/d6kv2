#ifndef GCOMOBJECT_H_HEADER_INCLUDED_BCB20227
#define GCOMOBJECT_H_HEADER_INCLUDED_BCB20227
#include "GBaseObject.h"
#include "gcommobjs.h"

class GScaleFactor;
class GFile;

//##ModelId=4340FF8A008C
//##Documentation
//## 定义：
//##        图元组合类
//## 功能：
//##        具有图元基本属性图元类型为GD_TYPE_COMOBJ
//##        有基本图元（可嵌套组合图元）组成
//## 设计：
//##       宋永生
//## 日期：
//##       2005-9-30
class GDE_EXPORT_DES GComObject : public GBaseObject
{
  public:
    //##ModelId=4341043D0157
    //##Documentation
    //## 功能：
    //##       构造/析构函数    
    //## 参数：
    //##     无
    //## 返回：
    //##     无
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-3
    GComObject();

    //##ModelId=4341045101E4
    //##Documentation
    //## 功能：
    //##       构造/析构函数    
    //## 参数：
    //##     无
    //## 返回：
    //##     无
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-3
    virtual ~GComObject();
	//##ModelId=437C8C9A0203
    //##Documentation
    //## 功能：
    //##   设置当前组件选中对象
    //## 参数：
    //##   GBaseObject* obj
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
    void setObj(GBaseObject* obj);
    //##ModelId=437C8CA50399
    //##Documentation
    //## 功能：
    //##   获取当前组件选中对象
    //## 参数：
    //##   无
    //## 返回：
    //##   GBaseObject* 
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
    GBaseObject* getObj();
	 //##ModelId=43799642008D
    //##Documentation
    //## 功能：
    //##      获取当前对象个数
    //## 参数：
    //##      无
    //## 返回：
    //##      uint32
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-15
    uint32 count() const;

    //##ModelId=4341047200BB
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

    //##ModelId=4341047C007D
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

    //##ModelId=4344FC2102BF
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
    //##ModelId=4354783E032C
    //##Documentation
    //## 功能：
    //##      删除指定图元，包括图元数据
    //## 参数：
    //##      GBaseObject* pObject//指定图元
    //## 返回：(uint8)
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-8
    virtual uint8 delObject(GBaseObject* pObject);


    //##ModelId=43547843008C
    //##Documentation
    //## 功能：
    //##      添加指定图元
    //## 参数：
    //##      GBaseObject* pObject//指定图元
    //## 返回：（GPOS）
    //##      if success return GPOS
    //##      else return NULL
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-8
    virtual GPOS addObject(GBaseObject* pObject);

    //##ModelId=435478490196
    //##ModelId=435478490196
    //##Documentation
    //## 功能：
    //##   提升图元位置
    //## 参数：
    //##   GBaseObject* pObject//指定图元
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-8
    virtual uint8 upObject(GBaseObject* pObject);

    //##Documentation
    //##ModelId=435478500290
    //##Documentation
    //## 功能：
    //##   降低图元位置
    //## 参数：
    //##   GBaseObject* pObject//指定图元
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-8
    virtual uint8 downObject(GBaseObject* pObject);

    //## 图元集合
    //##ModelId=435478640128
    //##Documentation
    //## 功能：
    //##   交互图元位置
    //## 参数：
    //##   GBaseObject* pObject1//指定图元
    //##   GBaseObject* pObject2//指定图元
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-8
    virtual uint8 chgObject(GBaseObject* pObject1, GBaseObject* pObject2);
	//##ModelId=43788ECA002E
    //##Documentation
    //## 功能：
    //##   提升图元位置到顶
    //## 参数：
    //##   GBaseObject* pObject//指定图元
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-8
    virtual uint8 topObject(GBaseObject* pObject);


    //##ModelId=43788ECD032C
    //##Documentation
    //## 功能：
    //##   提升图元位置到尾
    //## 参数：
    //##   GBaseObject* pObject//指定图元
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-8
    virtual uint8 bottomObject(GBaseObject* pObject);

	//##ModelId=4381550900BB
    //##Documentation
    //## 功能：
    //##   取第一个图元
    //## 参数：
    //##   无
    //## 返回：
    //##   GBaseObject*
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-8
    virtual GBaseObject* firstObject() const;

    //##ModelId=43788F02031C
    //##Documentation
    //## 功能：
    //##   取指定图元位置前一图元
    //## 参数：
    //##   GBaseObject* pObject//指定图元
    //## 返回：
    //##   if success return Object
    //##   else return NULL
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-8
    virtual GBaseObject* prevObject(GBaseObject* pObject) const;


    //##ModelId=43788F0A0203
    //##Documentation
    //## 功能：
    //##   取指定图元位置后一图元
    //## 参数：
    //##   GBaseObject* pObject//指定图元
    //## 返回：
    //##   if success return Object
    //##   else return NULL
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-8
    virtual GBaseObject* nextObject(GBaseObject* pObject) const;
    
    //##ModelId=43547871038A
    //##Documentation
    //## 功能：
    //##   删除所有图元，包括图元数据
    //## 参数：
    //##   无
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-8
    virtual uint8 remove();

    
    //##ModelId=4354787703D8
    //##Documentation
    //## 功能：
    //##   删除指定图元，不包括图元数据
    //## 参数：
    //##   GBaseObject* pObject
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-8
    virtual uint8 removeAt(GBaseObject* pObject);

	//##ModelId=4354787703D8
    //##Documentation
    //## 功能：
    //##   删除指定图元，不包括图元数据
    //## 参数：
    //##   GBaseObject* pObject
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-8
    virtual uint8 removePos(GPOS gPos);

    
    //##ModelId=435478C803B9
    //##Documentation
    //## 功能：
    //##   设置图元索引指针位置
    //## 参数：  
    //##   GPOS gPos//指定图元索引位置
    //## 返回：
    //##   GBaseObject* 
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-8
    virtual GBaseObject* getPos(GPOS gPos) const;
	//##ModelId=435DA0CA0041
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


    //##ModelId=435DA0D001D7
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
    
    //##ModelId=435478CF033C
    //##Documentation
    //## 功能：
    //##   取图元对应索引位置
    //## 参数：
    //##   int32 nIndex//图元
    //## 返回：
    //##   GBaseObject*
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-8
    virtual GBaseObject* getPos(int32 nIndex) const;	   
	//##ModelId=43780174011C
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


    //##ModelId=437801760060
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
	//##ModelId=437938A203A9
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
    //##ModelId=43793E1F032C
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
    //##ModelId=43793E45030D
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


    //##ModelId=43793E48008C
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
	//##ModelId=4379BAAB0128
    //##Documentation
    //## 功能：
    //##      获取第一个位置
    //## 参数：
    //##      无
    //## 返回：
    //##      GPOS
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-15
    virtual GPOS firstPos() const;
    //##ModelId=4379BAAB01D4
    //##Documentation
    //## 功能：
    //##      获取当前位置对象，前进到下一个对象
    //## 参数：
    //##      GPOS&
    //## 返回：
    //##      GBaseObject*
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-15
    virtual GBaseObject* nextPos(GPOS& gPos) const;
    //##ModelId=4379BAAB029F
    //##Documentation
    //## 功能：
    //##      获取最后一个位置
    //## 参数：
    //##      无
    //## 返回：
    //##      GPOS
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-15
    virtual GPOS lastPos() const;
    //##ModelId=4379BAAB034B
    //##Documentation
    //## 功能：
    //##      获取当前位置对象，前进到上一个对象
    //## 参数：
    //##      GPOS&
    //## 返回：
    //##      GBaseObject*
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-15
    virtual GBaseObject* prevPos(GPOS& gPos) const;
    //##ModelId=4379BACE0251
    //##Documentation
    //## 功能：
    //##      获取指定对象位置
    //## 参数：
    //##      GBaseObject* 
    //## 返回：
    //##      GPOS
    //## 设计：
    //##      宋永生
    //## 日期：
    //##      2005-11-15
    virtual GPOS findPos(GBaseObject* obj) const;
	//##ModelId=43771D470157
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


    //##ModelId=43771D49032C
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
    //##ModelId=437B2814000F
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
    //##ModelId=437B281400CB
    //##Documentation
    //## 功能：
    //##   鼠标点击左键谈起操作
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
    //##ModelId=437B28140177
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
    //##ModelId=437B28140251
    //##Documentation
    //## 功能：
    //##   鼠标点击右键谈起操作
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
    //##ModelId=437B2814032C
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
	 //##ModelId=437F17630203
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
     //##ModelId=4387FAE80128
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
    //##ModelId=4387FAE801A5
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
	//##ModelId=43A6064003B9
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
	    //##ModelId=43D313DD034B
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
    //##ModelId=43D313DE009C
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
    //##ModelId=43D313DE0186
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
	//##ModelId=440522BA013A
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
    //##ModelId=440522BA01F5
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
	//##ModelId=43A211A4033C
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
	
	//##ModelId=43A211A4033C
    //##Documentation
    //## 功能：
    //##   获取链接图元序列
    //## 参数：
    //##   libPath 库名
	//##   libID   标识
    //## 返回：
    //##   if success return 1
	//##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-12-17
	uint8 loadElems(const char* libPath,uint32 libID);
	//##ModelId=437984020383
    //##Documentation
    //## 功能：
    //##   设置自动删除其中的对象
    //## 参数：
    //##   uint8 bAutoDelete
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-14
    void setAutoDelete(uint8 bAutoDelete);
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
    //##ModelId=4379844B01CD
    //##Documentation
    //## 功能：
    //##  判断是否自动删除其中的对象
    //## 参数：
    //##   无
    //## 返回：
    //##   if success return 1 
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-14
    uint8 getAutoDelete() const;
	//##ModelId=43A210A70280
    //##Documentation
    //## 该自定义图元名称－仅仅为了显示
    //## 通过getPluginName返回
    QString m_customName;
	//##ModelId=43410072035B
    //##Documentation
    //## 嵌入或链接
	uint8	m_bLink;
	//##ModelId=43410072035B
    //##Documentation
    //## 库路径
	QString	m_libPath;
	//##ModelId=43410072035B
    //##Documentation
    //## 库标识
	uint32 m_libID  ;
    //##ModelId=43410072035B
    //##Documentation
    //## 图元集合
    GListOBJECTS m_objList;
protected:
    //##ModelId=4379AF4F0138
    //##Documentation
    //## 当前操作对象
    GBaseObject* m_curObj;
	
	//##ModelId=437983C40316
	//##Documentation
    //## 当前操作对象
    uint8 m_bAutoDelete;

};


#endif /* GCOMOBJECT_H_HEADER_INCLUDED_BCB20227 */
