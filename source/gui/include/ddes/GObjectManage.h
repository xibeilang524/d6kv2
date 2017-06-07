#ifndef GOBJECTMANAGE_H_HEADER_INCLUDED_BC868426
#define GOBJECTMANAGE_H_HEADER_INCLUDED_BC868426
#include "GComObject.h"
#include "ddef/ddes_comm.h"
#include "ddef/GPoint.h"
class GBaseObject;
class GPoint;
class GGraphFile;

//##ModelId=43797F090182
//##Documentation
//## 定义：
//##        图元组合类
//## 功能：
//##        用于管理图元，包括
//##        图元添加，删除恢复与重做
//##        剪切，复制，粘贴
//##        旋转，镜像等
//## 设计：
//##       宋永生
//## 日期：
//##       2005-11-15
class GDE_EXPORT_DES GObjectManage : public GComObject
{
  public:
    //##ModelId=4379807D0124
    //##Documentation
    //## 功能：
    //##     构造/析构函数    
    //## 参数：
    //##     无
    //## 返回：
    //##     无
    //## 设计：
    //##      宋永生
    //## 日期：
    //##      2005-11-15
    GObjectManage();
	//##ModelId=43798052029B
    //##Documentation
    //## 功能：
    //##     构造/析构函数    
    //## 参数：
    //##     无
    //## 返回：
    //##     无
    //## 设计：
    //##      宋永生
    //## 日期：
    //##      2005-11-15
    virtual ~GObjectManage();
	//##ModelId=437AB47A0200
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
    //##ModelId=437AB47A025E
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
	virtual void  setAct(LPDA dynAct);

    //##ModelId=4379827C0190
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
    //##ModelId=4379827C0190
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
    //##ModelId=4379827C020D
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
    //##ModelId=4379827C027A
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
    //##ModelId=4379827C02E8
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
    //##ModelId=4379827C0365
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
    //##ModelId=4379827C03D2
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
    //##ModelId=4379827D0067
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
    //##ModelId=4379827D00D4
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
    //##ModelId=4379827D0161
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
    //##ModelId=4379827D01DE
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
    //##ModelId=4379827D025B
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
    virtual void drawFocus(GDC* pDC, uint32 nFlags=0);
    //##ModelId=4379827D02D8
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
    //##ModelId=4379827D0365
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
	//##ModelId=437C7852032C
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
    //##ModelId=4379827D03E2
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
    //##ModelId=4379827E0190
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
    //##ModelId=4379827E022C
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
    //##ModelId=4379827E02B9
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
    //##ModelId=4379827E0345
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
    //##ModelId=4379827F0122
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
    //##ModelId=4379827F01CE
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
    //##ModelId=4379827F026B
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
    //##ModelId=4379827F0307
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
    //##ModelId=437990B2037E
    //##Documentation
    //## 功能：
    //##      设置所属文档
    //## 参数：
    //##      GGraphFile* gFile//指定所属文件
    //## 返回：
    //##      无
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-15
    void setFile(GGraphFile* gFile);
    //##ModelId=437990B30051
    //##Documentation
    //## 功能：
    //##      获取所属文档
    //## 参数：
    //##      无
    //## 返回：
    //##      GGraphFile*
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-15
    GGraphFile* getFile() const;   
    //##ModelId=43799675005E
    //##Documentation
    //## 功能：
    //##      选择对象是否可以提升
    //## 参数：
    //##      无
    //## 返回：
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-15
    uint8 canUp();
    //##ModelId=437996F402C0
    //##Documentation
    //## 功能：
    //##      选择对象是否可以下降
    //## 参数：
    //##      无
    //## 返回：
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-15
    uint8 canDown();
    //##ModelId=4379971A00EA
    //##Documentation
    //## 功能：
    //##      选择对象是否可以置顶
    //## 参数：
    //##      无
    //## 返回：
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-15
    uint8 canTop();
    //##ModelId=4379972001C5
    //##Documentation
    //## 功能：
    //##      选择对象是否可以置尾
    //## 参数：
    //##      无
    //## 返回：
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-15
    uint8 canBottom();
    //##ModelId=4379979D01F4
    //##Documentation
    //## 功能：
    //##      选择对象是否可以对齐
    //## 参数：
    //##      无
    //## 返回：
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-15
    uint8 canAlign();
    //##ModelId=437997B20129
    //##Documentation
    //## 功能：
    //##      选择对象是否可以合并
    //## 参数：
    //##      无
    //## 返回：
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-15
    uint8 canUnion();
    //##ModelId=437997D200AC
    //##Documentation
    //## 功能：
    //##      选择对象是否可以分割
    //## 参数：
    //##      无
    //## 返回：
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-15
    uint8 canSplit();
    //##ModelId=437998250177
    //##Documentation
    //## 功能：
    //##      选择对象是否可以镜像
    //## 参数：
    //##      无
    //## 返回：
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-15
    uint8 canMirror();
    //##ModelId=4379983801F4
    //##Documentation
    //## 功能：
    //##      选择对象是否可以旋转
    //## 参数：
    //##      无
    //## 返回：
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-15
    virtual uint8 canRotate() const;
    //##ModelId=4379985E0281
    //##Documentation
    //## 功能：
    //##      选择对象是否可以复制
    //## 参数：
    //##      无
    //## 返回：
    //##     if success return 1
    //##     else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-15
    uint8 canCopy();
	
	//##ModelId=4379B91901A5
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
    //##ModelId=4387FB14002E
    //##Documentation
    //## 功能：
    //##   图元对齐操作,根据指定功能对齐图元
    //## 参数：
    //##   uint32 op
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
    virtual uint8 alignPos(uint32 op,FLOAT8* rv=NULL);
    //##ModelId=4387FB1400BB
    //##Documentation
    //## 功能：
    //##   图元等大小操作,根据指定功能等大小图元
    //## 参数：
    //##   uint32 op
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
    virtual uint8 sameSize(uint32 op,FLOAT8 *rw=NULL,FLOAT8 *rh=NULL);
	//##ModelId=4388007A0177
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
    //##ModelId=4388007A01F4
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
    void setVis(uint8 visable);

  protected:    
    //##ModelId=437990970264
    GGraphFile* m_graphFile;

};



#endif /* GOBJECTMANAGE_H_HEADER_INCLUDED_BC868426 */
