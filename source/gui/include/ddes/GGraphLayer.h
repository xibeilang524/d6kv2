#ifndef GGRAPHLAYER_H_HEADER_INCLUDED_BCB8F875
#define GGRAPHLAYER_H_HEADER_INCLUDED_BCB8F875

#include "ddef/ddes_comm.h"
#include "ddef/GList.h"
#include "ddef/GRect.h"
#include "GVersion.h"
#include "GRedoundo.h"

class GGraphFile;
class GFile;
class GBaseObject;
class GScaleFactor;

//##ModelId=433D36320000
//##Documentation
//## 定义：
//##        图形存储结构
//## 功能：
//##        图形层定义
//## 设计：
//##       宋永生
//## 日期：
//##       2005-9-30
class GDE_EXPORT_DES GGraphLayer
{
  public:
    //##ModelId=433FE3C40138
    //##Documentation
    //## 功能：
    //##      重载写入函数
    //## 参数：
    //##      GFile& fFile
    //## 返回：（int32）
    //##      if success return write real size
    //##      else return -1
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual int32 saveFile(GFile& fFile);

    //##ModelId=433FE4310261
    //##Documentation
    //## 功能：
    //##      重载读取函数
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

    //##ModelId=4341037302CE
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
    GGraphLayer();

    //##ModelId=4341038E030D
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
    virtual ~GGraphLayer();

    //##ModelId=4347199B001F
    //##Documentation
    //## 功能：
    //##      设置所属文档
    //## 参数：
    //##      GGraphFile* pFile//指定所属文件
    //## 返回：
    //##      无
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-8
    void setFile(GGraphFile* pFile);

    //##ModelId=434719EA0000
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
    //##       2005-10-8
    GGraphFile* getFile();

    //##ModelId=43471A0A0000
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
    uint8 delObject(GBaseObject* pObject);

    //##ModelId=43471A8D038A
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
    GPOS addObject(GBaseObject* pObject);

    //##ModelId=43471ACB006D
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
    uint8 upObject(GBaseObject* pObject);

    //##ModelId=43471AD602EE
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
    uint8 downObject(GBaseObject* pObject);

    //##ModelId=43471ADB0213
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
    uint8 chgObject(GBaseObject* pObject1, GBaseObject* pObject2);

    //##ModelId=43471D09031C
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
    GBaseObject* getPos(GPOS gPos);

    //##ModelId=43471D0D007D
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
     GBaseObject* getPos(int32 nIndex);

    //##ModelId=43471D2001A5
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
    uint8 remove();

	//##ModelId=43471D2001A5
    //##Documentation
    //## 功能：
    //##   删除删除标记图元，包括图元数据
    //## 参数：
    //##   无
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-8
	uint8 removeDels();

    //##ModelId=434720B20213
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
    uint8 removeAt(GBaseObject* pObject);
    //##ModelId=434DE5650123
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
    uint32 getSize();
    //##ModelId=434DF8CF010C
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
    GPOS getPos();


    //##ModelId=434DF8D50060
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
    //##ModelId=435DA06401B8
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


    //##ModelId=435DA068008F
    //##Documentation
    //## 功能：
    //##      重载保存函数
    //## 参数：
    //##      QDomElement&  dom
    //## 返回：（uint8）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-25
    virtual uint8 saveXML(QDomElement& dom);
    //##ModelId=436C6D3902FD
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
	//##ModelId=4372E0FF031C
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
    uint8 topObject(GBaseObject* pObject);
    //##ModelId=4372E10701F4
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
    uint8 bottomObject(GBaseObject* pObject);
	 //##ModelId=43815690001F
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
    GBaseObject* firstObject() const;
    //##ModelId=4372E1620271
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
    GBaseObject* prevObject(GBaseObject* pObject) const;
    //##ModelId=4372E16900DA
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
    GBaseObject* nextObject(GBaseObject* pObject) const;
	    //##ModelId=437BD09E009C
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
    //##ModelId=437BD09E0128
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
    //##ModelId=437BD09E0186
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
    //##ModelId=437BD09E01E4
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
    //##ModelId=437BD09E0232
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
    //##ModelId=437BD0A501B5
    //##Documentation
    //## 功能：
    //##      获取指定对象ID对应位置
    //## 参数：
    //##      uint nID
    //## 返回：
    //##      GPOS
    //## 设计：
    //##      宋永生
    //## 日期：
    //##      2005-11-15
    GPOS findPos(uint32 nID) const;
	//##ModelId=437BD0A501B5
    //##Documentation
    //## 功能：
    //##      获取指定ID对应对象
    //## 参数：
    //##      uint nID
    //## 返回：
    //##      GBaseObject*
    //## 设计：
    //##      宋永生
    //## 日期：
    //##      2005-11-15
    GBaseObject* findObj(uint32 nID) const;

	//##ModelId=437BD0A501B5
    //##Documentation
    //## 功能：
    //##     历史操作记录
    //## 参数：
    //##     无
    //## 返回：
    //##     GCommandHistory&
    //## 设计：
    //##      宋永生
    //## 日期：
    //##      2005-11-15
	//
	GCommandHistory& history();
	//##ModelId=438017F60167
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
    //##ModelId=438017F601F4
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

	//##ModelId=438017F60167
    //##Documentation
    //## 功能：
    //##   是否为显示状态
    //## 参数：
    //##   无
    //## 返回：
    //##   if success return 1
    //##   else return 0
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
    uint8 isVis() const;
    //##ModelId=438017F601F4
    //##Documentation
    //## 功能：
    //##   设置显示状态
    //## 参数：
    //##   uint8 bDel
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-17
    void setVis(uint8 bVis=1);

    //##ModelId=433D369B005D
    //##Documentation
    //## 层类型
    uint32 m_lyrType;

    //##ModelId=433D36C0036B
    //##Documentation
    //## 层名称
    QString m_lyrName;

    //##ModelId=433D36E801C5
    //##Documentation
    //## 层范围
    GRect m_lyrRect;

    //##ModelId=433FC5FC0251
    //##Documentation
    //## 图元列表
    GListOBJECTS m_lstElems;
    //##ModelId=43531B750213
    //##Documentation
    //## 版本号
    GVersion m_vers;
	//##ModelId=4381297B0271
    //##Documentation
    //## 删除图元个数，不存储
    uint32 m_cntDel;
	
  protected:
    //##ModelId=4347197B0128
    //##Documentation
    GGraphFile* m_graphFile;

    //##ModelId=43471CC60000
    //##Documentation
    //## 当前对应层索引指针
    GPOS m_curPos;
	//##ModelId=43471CC60000
    //##Documentation
	//该层所做的历史操作记录
	GCommandHistory m_cmdHistory ;

	//##ModelId=438017DD038A
    //##Documentation
    //## 该图层是否为删除状态，如果为删除状态
    //## 存储时不进行存储，绘制时不进行绘制
    uint8 m_del;

	//##ModelId=438017DD038A
    //##Documentation
    //## 该图层是否为显示状态，用于运行时进行图层切换
    //## 如果为显示状态，绘制时进行绘制，否则不绘制
    uint8 m_vis;

  private:
};


#endif /* GGRAPHLAYER_H_HEADER_INCLUDED_BCB8F875 */
