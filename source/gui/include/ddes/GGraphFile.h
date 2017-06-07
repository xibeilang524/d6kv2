#ifndef GGRAPHFILE_H_HEADER_INCLUDED_BCB8BEF8
#define GGRAPHFILE_H_HEADER_INCLUDED_BCB8BEF8
#include "ddef/ddes_comm.h"
#include "GGraphLayer.h"
#include "GVersion.h"
#include "GFillStyle.h"
class GFile;
class GRect;
class GScaleFactor;
class GDC;

//##ModelId=433D33F3000F
//##Documentation
//## 定义：
//##        图形存储结构
//## 功能：
//##        图形存储文件头文件结构
//## 设计：
//##       宋永生
//## 日期：
//##       2005-9-30
class GDE_EXPORT_DES GGraphFile
{
  public:
    //##ModelId=433FE4C90290
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
	//##ModelId=433FE4C90290
    //##Documentation
    //## 功能：
    //##      读取头文件信息函数
    //## 参数：
    //##      GFile& fFile
    //## 返回：（int32）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-2
    virtual uint8 loadHead(GFile& fFile);

    //##ModelId=433FE4CC004E
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

    //##ModelId=434103AC02EE
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
    GGraphFile();

    //##ModelId=434103B5005D
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
    virtual ~GGraphFile();

     //##ModelId=434716400271
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

    //##ModelId=434716BE03D8
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

    //##ModelId=434717520203
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

    //##ModelId=4347176C0196
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

    //##ModelId=434717ED0203
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

    //##ModelId=4347185F01E4
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

    //##ModelId=4347189003B9
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

    //##ModelId=43471E0F008C
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

    //##ModelId=4347218D02BF
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
    //##ModelId=434DE57801DF
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
    virtual uint32 getSize();
    //##ModelId=435DA05901C7
    //##Documentation
    //## 功能：
    //##      重载读取函数
    //## 参数：
    //##      const char* strFile
    //## 返回：（uint8）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-25
    virtual uint8 readXML(const char* strFile);
	//##ModelId=435DA05901C7
    //##Documentation
    //## 功能：
    //##      重载读取函数
    //## 参数：
    //##      const char* strFile
    //## 返回：（uint8）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-25
    virtual uint8 readHead(const char* strFile);
    //##ModelId=435D9573021B
    //##Documentation
    //## 功能：
    //##      重载保存函数
    //## 参数：
    //##      const char* strFile
    //## 返回：（uint8）
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-25
    virtual uint8 saveXML(const char* strFile);
    //##ModelId=436C5B9002DE
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
    void onDraw(GDC* pDC, const GRect& rc , GScaleFactor* pSF = NULL);
	//##ModelId=4372DFFC007D
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
    //##ModelId=4372E03B0280
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
    //##ModelId=4372E06A008C
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
    virtual GGraphLayer* prevLayer(GGraphLayer* pLayer) const;
    //##ModelId=4372E0B9035B
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
    virtual GGraphLayer* nextLayer(GGraphLayer* pLayer) const;
	//##ModelId=4372FEA00167
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
	 //##ModelId=437B1CEC001F
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
    //##ModelId=437B1CEC00AB
    //##Documentation
    //## 功能：
    //##      获取当前位置对象，前进到下一个对象
    //## 参数：
    //##      GPOS&
    //## 返回：
    //##      GGraphLayer*
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-15
    virtual GGraphLayer* nextPos(GPOS& gPos) const;
    //##ModelId=437B1CEC0148
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
    //##ModelId=437B1CEC01F4
    //##Documentation
    //## 功能：
    //##      获取当前位置对象，前进到上一个对象
    //## 参数：
    //##      GPOS&
    //## 返回：
    //##      GGraphLayer*
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-15
    virtual GGraphLayer* prevPos(GPOS& gPos) const;
    //##ModelId=437B1CEC0290
    //##Documentation
    //## 功能：
    //##      获取指定对象位置
    //## 参数：
    //##      GGraphLayer* layer
    //## 返回：
    //##      GPOS
    //## 设计：
    //##      宋永生
    //## 日期：
    //##      2005-11-15
    virtual GPOS findPos(GGraphLayer* layer) const;

	//##ModelId=437B1CEC0290
    //##Documentation
    //## 功能：
    //##      执行按钮宏动作
    //## 参数：
    //##      GBaseObject*
    //## 返回：
    //##      void
    //## 设计：
    //##      宋永生
    //## 日期：
    //##      2005-11-15
    virtual void execBtnMacro(GBaseObject* pObj);

	//##ModelId=437B1CEC0290
    //##Documentation
    //## 功能：
    //##      执行按钮命令
    //## 参数：
    //##      GBaseObject*
    //## 返回：
    //##      void
    //## 设计：
    //##      宋永生
    //## 日期：
    //##      2005-11-15
    virtual void execBtnCMD(GBaseObject* pObj);

	//##ModelId=437B1CEC0290
    //##Documentation
    //## 功能：
    //##      执行按钮切换层命令
    //## 参数：
    //##      GBaseObject*
    //## 返回：
    //##      void
    //## 设计：
    //##      宋永生
    //## 日期：
    //##      2005-11-15
    virtual void execBtnLayer(GBaseObject* pObj);

	//##ModelId=437B1CEC0290
    //##Documentation
    //## 功能：
    //##      执行按钮调图命令
    //## 参数：
    //##      GBaseObject*
    //## 返回：
    //##      void
    //## 设计：
    //##      宋永生
    //## 日期：
    //##      2005-11-15
    virtual void execBtnFile(GBaseObject* pObj);

	//##ModelId=437B1CEC0290
    //##Documentation
    //## 功能：
    //##      更新图形
    //## 参数：
    //##      GBaseObject*
    //## 返回：
    //##      void
    //## 设计：
    //##      宋永生
    //## 日期：
    //##      2005-11-15
	virtual void update();
	
	//##ModelId=437B1CEC0290
    //##Documentation
    //## 功能：
    //##      设置指定图元的ID标识
    //## 参数：
    //##      GBaseObject* pObj
    //## 返回：
    //##      无
    //## 设计：
    //##      宋永生
    //## 日期：
    //##      2005-11-15
	void setID(GBaseObject* pObj);

	//##ModelId=437B1CEC0290
    //##Documentation
    //## 功能：
    //##      重置文件中所有图元的ID标识
    //## 参数：
    //##      无
    //## 返回：
    //##      无
    //## 设计：
    //##      宋永生
    //## 日期：
    //##      2005-11-15
	void  resetID();
	
	//##ModelId=437B1CEC0290
    //##Documentation
    //## 功能：
    //##      遍历图中所有图元
    //## 参数：
    //##      无
    //## 返回：
    //##      无
    //## 设计：
    //##      宋永生
    //## 日期：
    //##      2005-11-15
	uint8  travelObj(GTS_FILTER_FUNC filter,void* clientData);

	//##ModelId=437B1CEC0290
    //##Documentation
    //## 功能：
    //##      遍历指定层中所有图元
    //## 参数：
    //##      GGraphLayer* pLayer
    //## 返回：
    //##      无
    //## 设计：
    //##      宋永生
    //## 日期：
    //##      2005-11-15
	uint8  travelObj(GGraphLayer* pLayer,GTS_FILTER_FUNC filter,void* clientData);

	//设置图形安全级
	void setSafeLevel( uint8 level );

	//都到图形安全级
	uint8 safeLevel();

    //##ModelId=433D351E00FA
    //##Documentation
    //## 图形文件标志{bgf}
    char m_gFlag[4];

    //##ModelId=433D3AA80203
    //##Documentation
    //## 缩放规则{0-1:1,1-自动适配,2-宽度分屏,3-高度分屏}
    uint8 m_scaleRule;

    //##ModelId=433D3C590157
    //##Documentation
    //## 图形范围
    GRect m_boundBox;

	//##ModelId=45060BBF035B
    //##Documentation
    //## 图形属主名称
	QString m_owner;
	//##ModelId=45060BDF0242
    //##Documentation
    //## 图形类别
    QString m_type;
    //##ModelId=433FC5A602DE
    //##Documentation
    //## 图层列表
    GListLAYERS m_lstLayers;
    //##ModelId=433D37190271
    //##Documentation
    //## 版本管理类
    GVersion m_vers;
    //##ModelId=436C62D302BF
    //##Documentation
    //## 背景填充风格
    GFillStyle m_fillStyle;
	//##ModelId=43812B3E01E4
    //##Documentation
    //## 删除图层个数，不存储
    uint32 m_cntDel;


	//图形的安全级
	uint8 m_SafeLevel;

	//##ModelId=437B1CEC0290
    //##Documentation
    //## 功能：
    //##      判断文件格式
    //## 参数：
    //##      const char* strPath
    //## 返回：
    //##     
	//##	GT_FAILED		0x00
    //##	GT_BINGRAPH		0x01
    //##	GT_XMLGRAPH		0x02
	//##
    //## 设计：
    //##      宋永生
    //## 日期：
    //##      2005-11-15
	static uint8 isGraphFile(const char* strPath) ;

  protected:
    //##ModelId=434718D80251
    //##Documentation
    //## 操作时使用，提供当前层操作
    GGraphLayer* m_curLayer;
	//##ModelId=4372FE6F0138
    //##Documentation
    //## 当前最大标识
    uint32 m_maxID;
  private:


};

#endif /* GGRAPHFILE_H_HEADER_INCLUDED_BCB8BEF8 */
