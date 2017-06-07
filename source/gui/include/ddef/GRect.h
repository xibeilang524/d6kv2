#ifndef GRECT_H_HEADER_INCLUDED_BCB8DC85
#define GRECT_H_HEADER_INCLUDED_BCB8DC85

#include "ddes_comm.h"
#include "GPoint.h"
//##ModelId=4339FF4B032C
//##Documentation
//## 功能：
//##       计算过程无数据丢失的矩形区域
//## 设计：
//##       宋永生
//## 日期：
//##       2005-9-30
class GDE_EXPORT_DEF GRect
{
  public:    

    //##ModelId=433A9B800251
    //##Documentation
    //## 功能：
    //##       构造空矩形  
    //## 参数：
    //##      无
    //## 返回：
    //##      无
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    GRect();
	//##ModelId=437883BD01E4
    //##Documentation
    //## 功能：
    //##       设定矩形      
    //## 参数：
    //##      const GPoint& lt //左上角
    //##      const GPoint& rb //右下角
    //## 返回：
    //##      const GRect&
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    GRect(const GPoint& tl, const GPoint& rb);
    //##ModelId=433A9BA403C8
    //##Documentation
    //## 功能：
    //##       构造指定矩形   
    //## 参数：
    //##      FLOAT8 left  //左上角x坐标
    //##      FLOAT8 top  //左上角y坐标
    //##      FLOAT8 right //右下角x坐标
    //##      FLOAT8 bottom//右下角y坐标
    //## 返回：
    //##      无
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    GRect(FLOAT8 l, FLOAT8 t, FLOAT8 r, FLOAT8 b);
	//##ModelId=433A069202AF
    //##Documentation
    //## 功能：
    //##      测试指定点是否在当前矩形内    
    //## 参数：
    //##      const GPoint& point//测试点
    //## 返回：（uint8）
    //##      if point in curr rect return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    uint8 container(const GPoint& point) const;

    //##ModelId=433A0745006D
    //##Documentation
    //## 功能：
    //##      测试指定矩形是否在当前矩形内     
    //## 参数：
    //##      const GRect& rect//测试矩形
    //## 返回：（uint8）     
    //##       if rect in curr rect return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    uint8 container(const GRect& rect) const;

    //##ModelId=433A9ACA00CB
    //##Documentation
    //## 功能：
    //##      测试指定点是否在当前矩形内     
    //## 参数：
    //##      FLOAT8  x
    //##      FLOAT8  y//测试点
    //## 返回：（uint8）
    //##      if point in curr rect return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    uint8 container(FLOAT8 x, FLOAT8 y) const;
    //##ModelId=433A9BF70128
    //##Documentation
    //## 功能：
    //##       测试两矩形是否相交      
    //## 参数：
    //##       const GRect& rect
    //## 返回： （uint8）
    //##       if success return 1
    //##       else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    uint8 overlap(const GRect& rect) const;

    //##ModelId=433A9C3B0186
    //##Documentation
    //## 功能：
    //##       重新设定矩形
    //## 参数：
    //##      FLOAT8 left  //左上角x坐标
    //##      FLOAT8 top  //左上角y坐标
    //##      FLOAT8 right //右下角x坐标
    //##      FLOAT8 bottom//右下角y坐标      
    //## 返回：
    //##      const GRect&
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GRect& set(FLOAT8 l, FLOAT8 t, FLOAT8 r, FLOAT8 b);

    //##ModelId=433A9C9700DA
    //##Documentation
    //## 功能：
    //##       重新设定矩形      
    //## 参数：
    //##      const GPoint lt //左上角
    //##      const GPoint rb//右下角
    //## 返回：
    //##      const GRect&
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GRect& set(const GPoint& lt, const GPoint& rb);

	//##ModelId=433A9C9700DA
    //##Documentation
    //## 功能：
    //##       重新设定矩形      
    //## 参数：
    //##      FLOAT8 width //宽
    //##      FLOAT8 height//高
    //## 返回：
    //##      const GRect&
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GRect& set(FLOAT8 width, FLOAT8 height);

	//##ModelId=433A9C9700DA
    //##Documentation
    //## 功能：
    //##       重新设定矩形      
    //## 参数：
    //##      FLOAT8 width //宽
    //## 返回：
    //##      const GRect&
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GRect& setWidth(FLOAT8 width);
	
	//##ModelId=433A9C9700DA
    //##Documentation
    //## 功能：
    //##       重新设定矩形      
    //## 参数：
    //##      FLOAT8 height//高
    //## 返回：
    //##      const GRect&
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GRect& setHeight(FLOAT8 height);
	
	
    //##ModelId=433A9CE803D8
    //##Documentation
    //## 功能：
    //##       相对平移矩形    
    //## 参数：
    //##       FLOAT8 cx //相对移动x轴距离
    //##       FLOAT8 cy //相对移动y轴距离
    //## 返回：
    //##      const GRect&
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GRect& offset(FLOAT8 cx, FLOAT8 cy);

    //##ModelId=433A9D67032C
    //##Documentation
    //## 功能：
    //##       移动矩形到指定位置      
    //## 参数：
    //##       FLOAT8 x //移动到x轴位置
    //##       FLOAT8 y //移动到y轴位置
    //## 返回：
    //##      const GRect&
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GRect& move(FLOAT8 x, FLOAT8 y);

	//##ModelId=433A9D67032C
    //##Documentation
    //## 功能：
    //##       移动矩形到指定位置      
    //## 参数：
    //##       const GPoint& pt
    //## 返回：
    //##       const GRect&
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GRect& move(const GPoint& pt);

	//##ModelId=433A9D67032C
    //##Documentation
    //## 功能：
    //##      移动矩形中心点到指定位置      
    //## 参数：
    //##      const GPoint& ptc
    //## 返回：
    //##      const GRect&
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GRect& centerPoint(const GPoint& ptc);

	//##ModelId=433A9D67032C
    //##Documentation
    //## 功能：
    //##      移动矩形中心点到指定矩形中心点位置      
    //## 参数：
    //##      const GRect& rc
    //## 返回：
    //##      const GRect&
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GRect& centerRect(const GRect& rc);

    //##ModelId=433A9E5501D4
    //##Documentation
    //## 功能：
    //##       缩放矩形       
    //## 参数：
    //##       FLOAT8 cx //缩小x轴尺寸
    //##       FLOAT8 cy //缩小y轴尺寸
    //## 返回：
    //##      const GRect&
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GRect& deflate(FLOAT8 cx, FLOAT8 cy);

    //##ModelId=433A9EB901D4
    //##Documentation
    //## 功能：
    //##       缩放矩形       
    //## 参数：
    //##       FLOAT8 l //缩小x轴尺寸
    //##       FLOAT8 t //缩小y轴尺寸
    //##       FLOAT8 r //缩小x轴尺寸
    //##       FLOAT8 b //缩小y轴尺寸
    //## 返回：
    //##      const GRect&
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GRect& deflate(FLOAT8 l, FLOAT8 t, FLOAT8 r, FLOAT8 b);

    //##ModelId=433A9F060128
    //##Documentation
    //## 功能：
    //##       缩放矩形       
    //## 参数：
    //##       FLOAT8 xy //缩小xy轴尺寸
    //## 返回：
    //##      const GRect&
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GRect& deflate(FLOAT8 xy);
	//##ModelId=4434A34B0261
    //##Documentation
    //## 功能：
    //##   赋值函数
    //## 参数：
    //##   const GRect&
    //## 返回：
    //##   const GRect&
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-5
    const GRect& operator=(const  GRect& copy);
    //##ModelId=433A9F440399
    //##Documentation
    //## 功能：
    //##       计算两个矩形相交的部分，并存储到本身
    //## 参数：
    //##       const GRect& rect //相交的矩形      
    //## 返回：（const GRect&）
    //##       返回结果矩形
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GRect& operator &=(const GRect& rect);

    //##ModelId=433A9FD501A5
    //##Documentation
    //## 功能：
    //##       计算能包含两个矩形的最小矩形      
    //## 参数：
    //##       const GRect& rect //相交的矩形      
    //## 返回：（const GRect）
    //##       返回结果矩形
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GRect& operator |=(const GRect& rect);

    //##ModelId=433AA03300EA
    //##Documentation
    //## 功能：
    //##       取两个矩形相交的部分  
    //## 参数：
    //##       const GRect& rect //相交的矩形 
    //## 返回：（const GRect）
    //##       返回结果矩形
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GRect operator &(const GRect& rect);

    //##ModelId=433AA0B2008C
    //##Documentation
    //## 功能：
    //##       矩形的x轴方向向左右扩大Point.x个像素，
    //##       矩形的y轴方向向上下扩大Point.y个像素 
    //## 参数：
    //##       const GPoint& point//点象素
    //## 返回：（const GRect ）
    //##       返回结果矩形
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GRect operator +(const GPoint& point);

    //##ModelId=433AA131029F
    //##Documentation
    //## 功能：
    //##       矩形的x,y轴方向向左右扩大x个像素
    //## 参数：
    //##       FLOAT8 x
    //## 返回：（const GRect &）
    //##       返回结果矩形
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GRect &operator +=(FLOAT8 x);

    //##ModelId=433AA137034B
    //##Documentation
    //## 功能：
    //##       矩形的x,y轴方向向左右扩大1个像素
    //## 参数：
    //##      无
    //## 返回：（const GRect&）
    //##       返回结果矩形
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GRect& operator++(int);

    //##ModelId=433AA224004E
    //##Documentation
    //## 功能：
    //##       矩形的x,y轴方向向左右缩小x个像素，
    //## 参数：
    //##       FLOAT8 x
    //## 返回：（const GRect &）
    //##       返回结果矩形
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GRect& operator-=(FLOAT8 x);

    //##ModelId=433AA2270109
    //##Documentation
    //## 功能：
    //##       矩形的x,y轴方向向左右缩小1个像素
    //## 参数：
    //##      无
    //## 返回：（const GRect&）
    //##       返回结果矩形
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GRect& operator--(int);

    //##ModelId=433AA2D0030D
    //##Documentation
    //## 功能：
    //##       比较两矩形是否相等
    //## 参数：
    //##       const GRect& rect//比较的矩形区域
    //## 返回：（uint8）
    //##       if equ return 1
    //##       else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    uint8 operator==(const GRect& rect) const;

    //##ModelId=433AA3270251
    //##Documentation
    //## 功能：
    //##       比较两矩形是否相等
    //## 参数：
    //##       const GRect& rect//比较的矩形区域
    //## 返回：（uint8）
    //##       if equ return 0
    //##       else return 1
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    uint8 operator !=(const GRect& rect) const;

    //##ModelId=433AA36601A5
    //##Documentation
    //## 功能：
    //##       求矩形宽度
    //## 参数：
    //##     无
    //## 返回：（FLOAT8 ）
    //##       矩形宽度
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    FLOAT8 width() const;

    //##ModelId=433AA36E038A
    //##Documentation
    //## 功能：
    //##       求矩形高度
    //## 参数：
    //##     无
    //## 返回：（FLOAT8 ）
    //##       矩形高度
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    FLOAT8 height() const;

    //##ModelId=433AA37002EE
    //##Documentation
    //## 功能：
    //##       求矩形中心点
    //## 参数：
    //##      无
    //## 返回：（const GPoint）
    //##       矩形中心点
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GPoint center() const;

    //##ModelId=433AA41A0399
    //##Documentation
    //## 功能：
    //##       求矩形左上坐标
    //## 参数：
    //##      无
    //## 返回：（const GPoint）
    //##       矩形左上坐标
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GPoint topLeft() const;

    //##ModelId=433AA42E0242
    //##Documentation
    //## 功能：
    //##       求矩形右下坐标
    //## 参数：
    //##      无
    //## 返回：（const GPoint）
    //##       矩形右下坐标
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GPoint bottomRight() const;
    //##ModelId=434BDC370268
    //##Documentation
    //## 功能：
    //##       求矩形右上坐标
    //## 参数：
    //##      无
    //## 返回：（const GPoint）
    //##       矩形左上坐标
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GPoint topRight() const;
    //##ModelId=434BDC5C00D2
    //##Documentation
    //## 功能：
    //##       求矩形左下坐标
    //## 参数：
    //##      无
    //## 返回：（const GPoint）
    //##       矩形右下坐标
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GPoint bottomLeft() const;
	//##ModelId=437E83D002CE
    //##Documentation
    //## 功能：
    //##       求矩形上中坐标
    //## 参数：
    //##      无
    //## 返回：（const GPoint）
    //##       矩形上中坐标
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GPoint topCenter() const;
    //##ModelId=437E83E50232
    //##Documentation
    //## 功能：
    //##       求矩形下中坐标
    //## 参数：
    //##      无
    //## 返回：（const GPoint）
    //##       矩形下中坐标
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GPoint bottomCenter() const;
    //##ModelId=437E83F40242
    //##Documentation
    //## 功能：
    //##       求矩形左中坐标
    //## 参数：
    //##      无
    //## 返回：（const GPoint）
    //##       矩形左中坐标
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GPoint leftCenter() const;
    //##ModelId=437E83FB034B
    //##Documentation
    //## 功能：
    //##       求矩形右中坐标
    //## 参数：
    //##      无
    //## 返回：（const GPoint）
    //##       矩形右中坐标
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GPoint rightCenter() const;
    //##ModelId=434BDD30017D
    //##Documentation
    //## 功能： 
    //##   正规化矩形
    //## 参数：
    //##   无
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-10-11
    void normal();
	//##ModelId=4378865000FA
    //##Documentation
    //## 功能：
    //##      检测矩形是否为空
    //## 参数：
    //##      无
    //## 返回：
    //##       if empty return 1
    //##       else 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    uint8 isEmpty() const;
	//##ModelId=43850C8203A9
    //##Documentation
    //## 功能：
    //##       生成矩形
    //## 参数：
    //##       GPoint* pts 
    //##       int32   npts
    //## 返回：
    //##       const GRect&      
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    const GRect& convFromPoints(GPoint* pts, int32 npts);
	//##ModelId=43850C8203A9
    //##Documentation
    //## 功能：
    //##       生成环抱点
    //## 参数：
    //##       GPoint* pts 
    //##       int32   npts
	//##	   if npts == 2  get topLeft,botRight
	//##	   if npts == 4  get topLeft,topRight,botRight,botLeft
	//##	   if npts == 8  get topLeft,topcenter,
	//##						 topRight,rightcenter
	//##						 botRight,botcenter
	//##						 botLeft,leftcenter
    //## 返回：
    //##       int32    //实际转换的个数
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    int32 convToPoints(GPoint* pts, int32 npts);
    //##ModelId=4378868F001F
    //##Documentation
    //## 功能：
    //##      设置矩形为空
    //## 参数：
    //##      无
    //## 返回：
    //##      无
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    void setEmpty();	

    //##ModelId=4339FF5800DA
    FLOAT8 top;

    //##ModelId=4339FF6A02FD
    FLOAT8 left;

    //##ModelId=4339FF6B02FD
    FLOAT8 bottom;

    //##ModelId=4339FF6C02EE
    FLOAT8 right;

};

#endif /* GRECT_H_HEADER_INCLUDED_BCB8DC85 */
