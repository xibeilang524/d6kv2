#ifndef G_BAY_OBJECT_H_
#define G_BAY_OBJECT_H_

#include "GComObject.h"

class GScaleFactor;
class GFile;
class GBaseObject;

//间隔类
class GDE_EXPORT_DES GBayObject : public GComObject
{
public:
	GBayObject();
	virtual ~GBayObject();

	//写入文件
	virtual int32 saveFile(GFile& fFile);
	//读取所有数据包含父类
	virtual int32 loadFile(GFile& fFile);
	//绘制图元，产生图元静态图象
	virtual uint8 drawObj(GDC* pDC, GScaleFactor* pSF = NULL);
	//重载读取XML函数
	virtual uint8 readXML(QDomElement& dom);
	//重载保存XML函数
	virtual uint8 saveXML(QDomElement& dom);
	//测试当前点在图元相关位置
	virtual uint32 hitTest(const GPoint& pt);
	//计算图元矩形
	virtual void calcRect();
	////X,Y偏移量
	//virtual void offsetXY(FLOAT8 x, FLOAT8 y, uint32 flags = 0);
	////X,Y调整偏移量
	//virtual void resizeXY(const GPoint& pt, FLOAT8 x, FLOAT8 y, uint32 flags = 0);
	////对图元进行镜像处理
	//virtual void mirrorXY(uint32 op, uint32 flags = 0);
	////以指定原点对图元进行镜像处理
	//virtual void mirrorXY(const GPoint& pto, uint32 op, uint32 flags = 0);
	//按图元内部旋转中心点旋转th度
	virtual void rotateXY(FLOAT8 th, uint32 flags = 0);
	////按指定旋转中心点ptc旋转th度
	//virtual void rotateXY(const GPoint& ptc, FLOAT8 th, uint32 flags = 0);
	//鼠标进入操作
	//virtual int32 onMouseEnter(GDC* pDC, const GPoint& pt, uint32 flags);
	////鼠标在对象中移动操作
	//virtual int32 onMouseMove(GDC* pDC, const GPoint& pt, uint32 flags);
	////鼠标离开操作
	//virtual int32 onMouseLeave(GDC* pDC, const GPoint& pt, uint32 flags);
	////鼠标点击左键弹起操作
	//virtual int32 onLButtonUp(GDC* pDC, const GPoint& pt, uint32 flags);
	////鼠标点击左键操作
	//virtual int32 onLButtonDown(GDC* pDC, const GPoint& pt, uint32 flags);
	////鼠标点击右键弹起操作
	//virtual int32 onRButtonUp(GDC* pDC, const GPoint& pt, uint32 flags);
	////鼠标点击右键操作
	//virtual int32 onRButtonDown(GDC* pDC, const GPoint& pt, uint32 flags);
	////提供对象缩放计算，无返回，直接改变绘制对象
	////默认忽略，不要在该函数中调用pSF->scaleObject
	////virtual void scaleFactor(GScaleFactor* pSF);
	////图元对齐操作,根据指定功能对齐图元
	//virtual void alignPos(uint32 op, FLOAT8 v);
	//图元等大小操作,根据指定功能等大小图元
	virtual void sameSize(uint32 op, FLOAT8 w, FLOAT8 h);
	//////测试该对象是否可以旋转
	////virtual uint8 canRotate() const;
	////设置图元填充风格操作
	//virtual void setFillStyle(const GFillStyle& style);
	////设置图元线风格
	//virtual void setLineStyle(const GLineStyle& style);
	////设置图元文本风格信息
	//virtual void setTextStyle(const GTextStyle& style);
	////重新设置端子属主ID
	////reset为0时重置OwnerID，否则清除OwnerID
	////nID为-1时，当前ID为子端子的属主，否则指定ID为属主
	////pObj为 NULL 当前对象为子端子的属主，否则指定对象为属主	
	////成功返回1，失败返回0
	//virtual void setPortOwner(uint8 reset=0,uint32 nID=(uint32)-1,\
	//	GBaseObject* pObj=NULL);
	////图元定时响应功能
	////ymd 年月日 = yyyymmdd
	////hms 时分秒 = hhmmss
	//virtual uint8 onTimer(uint32 ymd, uint32 hmsm);
	////图元是否允许定时响应功能
	////是返回1，否返回0
	//virtual uint8 canTimer() const;
	//获取控件名称
	//virtual QString getPluginName() const;
	////设置内部实时值
	//virtual void setValue(FLOAT8 value);
	////设置图元当前层
	//virtual void setLayer(GGraphLayer* pLayer);
	//获取链接图元序列
	//uint8 loadElems(const char* libPath,uint32 libID);
	////设置自动删除其中的对象
	//void setAutoDelete(uint8 bAutoDelete);
	////图元是否激活
	//virtual void setActive(uint8 active );
	////判断是否自动删除其中的对象
	//uint8 getAutoDelete() const;

	//一般化，添加间隔模板时，分析组成间隔的各个图元中的动态数据
	//形成模板
	int generalize( QString& msg );
	uint16 keyDeviceType(void) const {return m_keyDevType;};
	QString keyDevicePrefix(void) const {return m_keyDevPrefix;};

protected:
	QString m_keyDevPrefix;		//主设备代码的前缀
	uint16 m_keyDevType;		//主设备的类型
	uint16 m_keyDevVolH;		//主设备电压等级
	uint16 m_keyDevVolM;
	uint16 m_keyDevVolL;

};
#endif