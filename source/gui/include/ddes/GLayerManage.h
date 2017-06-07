#ifndef GLAYERMANAGE_H_HEADER_INCLUDED_BC86EA29
#define GLAYERMANAGE_H_HEADER_INCLUDED_BC86EA29
#include "GGraphFile.h"
#include "ddef/ddes_comm.h"
class GGraphLayer;
class GScaleFactor;

//##ModelId=43797F5602BB
//##Documentation
//## 定义：
//##        图层管理类
//## 功能：
//##        用于管理图层，包括图层
//##        添加，删除等的重做与恢复等
//## 设计：
//##       宋永生
//## 日期：
//##       2005-11-15
class GDE_EXPORT_DES GLayerManage : public GGraphFile
{
  public:
    //##ModelId=4379808902CA
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
    GLayerManage();
    //##ModelId=4379809C0337
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
    ~GLayerManage();
    //##ModelId=437982ED01CE
    //##Documentation
    //## 功能：
    //##   添加一层
    //## 参数：
    //##   GGraphLayer* pLayer//指定层
    //## 返回：(GPOS)
    //##   if success return GPOS
    //##   else return NULL
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-8
    virtual GPOS addLayer(GGraphLayer* pLayer);
    //##ModelId=437982ED025B
    //##Documentation
    //## 功能：
    //##   删除指定层，包括层中包含的数据
    //## 参数：
    //##   GGraphLayer* pLayer //指定层
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-8
    virtual uint8 delLayer(GGraphLayer* pLayer);
    //##ModelId=437982ED02C8
    //##Documentation
    //## 功能：
    //##   提升层位置
    //## 参数：
    //##   GGraphLayer* pLayer//提升指定层位置
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-8
    virtual uint8 upLayer(GGraphLayer* pLayer);
    //##ModelId=437982ED0345
    //##Documentation
    //## 功能：
    //##   提升层位置到顶层
    //## 参数：
    //##   GGraphLayer* pLayer//提升指定层位置
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-10
    virtual uint8 topLayer(GGraphLayer* pLayer);
    //##ModelId=437982ED03C2
    //##Documentation
    //## 功能：
    //##   降低层位置
    //## 参数：
    //##   GGraphLayer* pLayer//降低指定层位置
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-8
    virtual uint8 downLayer(GGraphLayer* pLayer);
    //##ModelId=437982EE0057
    //##Documentation
    //## 功能：
    //##   降低层位置到最下层
    //## 参数：
    //##   GGraphLayer* pLayer//提升指定层位置
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-8
    virtual uint8 bottomLayer(GGraphLayer* pLayer);
    //##ModelId=437982EE00E4
    //##Documentation
    //## 功能：
    //##   交互层位置
    //## 参数：
    //##   GGraphLayer* pLayer1//交互层位置
    //##   GGraphLayer* pLayer2//交互层位置
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-8
    virtual uint8 chgLayer(GGraphLayer* pLayer1, GGraphLayer* pLayer2);
    //##ModelId=437982EE0171
    //##Documentation
    //## 功能：
    //##   取当前层
    //## 参数：
    //##   无
    //## 返回：
    //##   GGraphLayer*
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-8
    virtual GGraphLayer* getLayer();
    //##ModelId=437982EE01FD
    //##Documentation
    //## 功能：
    //##   设置当前层
    //## 参数：
    //##   GGraphLayer* pLayer //指定当前层
    //## 返回：  
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-8
    virtual void setLayer(GGraphLayer* pLayer);
    //##ModelId=437982EE027A
    //##Documentation
    //## 功能：
    //##   删除所有层，包括层数据
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
    //##ModelId=437982EE02F7
    //##Documentation
    //## 功能：
    //##       合并删除层图元到指定层
    //## 参数：
    //##      GGraphLayer* pLayer
    //##      GGraphLayer* pRemove
    //## 返回：（uint8 ）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    virtual uint8 mergeLayer(GGraphLayer* pLayer, GGraphLayer* pRemove);
    //##ModelId=437982EE0384
    //##Documentation
    //## 功能：
    //##   实现图元绘制功能，产生图元静态图象
    //## 参数：
    //##   GDC* pDC         //绘制环境
    //##   const GRect& rc  //矩形区域
    //##   GScaleFactor* pSF//缩放因子
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-3
    virtual void onDraw(GDC* pDC, const GRect& rc, GScaleFactor* pSF = NULL);
    //##ModelId=437982EF0019
    //##Documentation
    //## 功能：
    //##   取指定层前一层
    //## 参数：
    //##   GGraphLayer* pLayer//提升指定层位置
    //## 返回：
    //##   if success return Layer
    //##   else return NULL
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-8
    virtual GGraphLayer* prevLayer(GGraphLayer* pLayer);
    //##ModelId=437982EF00A5
    //##Documentation
    //## 功能：
    //##   取指定层后一层
    //## 参数：
    //##   GGraphLayer* pLayer//提升指定层位置
    //## 返回：
    //##   if success return Layer
    //##   else return NULL
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-10
    virtual GGraphLayer* nextLayer(GGraphLayer* pLayer);
    //##ModelId=437982EF0132
    //##Documentation
    //## 功能：
    //##   生成最大标识
    //## 参数：
    //##   无
    //## 返回：
    //##   uint32
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-10
    virtual uint32 maxID();
    //##ModelId=4379831401AF
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
    //##ModelId=4379833800C4
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
    //##ModelId=43798563021A
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
    //##ModelId=4379856302B7
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
  protected:
    //##ModelId=4379856B03A1
    uint8 m_bAutoDelete;
    //##ModelId=437982F4028A
    GGraphFile* m_graphFile;
};

#endif /* GLAYERMANAGE_H_HEADER_INCLUDED_BC86EA29 */
