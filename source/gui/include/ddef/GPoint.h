#ifndef GPOINT_H_HEADER_INCLUDED_BCB8A4D3
#define GPOINT_H_HEADER_INCLUDED_BCB8A4D3
#include "ddes_comm.h"
#include "GList.h"

//##ModelId=4339FECE036B
//##Documentation
//## 功能：
//##        计算过程无数据丢失的点定义
//## 设计：
//##       宋永生
//## 日期：
//##       2005-9-30
class GDE_EXPORT_DEF GPoint
{
  public:
    //##ModelId=433A015D0138
    //##Documentation
    //## 功能：
    //##       比较两点是否不相等
    //## 参数：
    //##       const GPoint& point//比较的点
    //## 返回：（uint8）
    //##       if not equ return 1
    //##       else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    uint8 operator !=(const GPoint& point) const;

    //##ModelId=433A01D9003E
    //##Documentation
    //## 功能：
    //##       比较两点是否相等
    //## 参数：
    //##       const GPoint& point//比较的点
    //## 返回：（uint8）
    //##       if equ return 1
    //##       else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-9-28
    uint8 operator==(const GPoint& point) const;

    //##ModelId=433A01F60251
    //##Documentation
    //## 功能：
    //##      重载操作符+=
    //## 参数：
    //##      const GPoint& point//移动的点      
    //## 返回：（const GPoint&）
    //##     返回计算出的点
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-1
    const GPoint& operator+=(const GPoint& point);

    //##ModelId=433A998E038A
    //##Documentation
    //## 功能：
    //##      重载操作符+
    //## 参数：
    //##      const GPoint& point//移动的点      
    //## 返回：（const GPoint）
    //##     返回计算出的点，不改变该点
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-10-1
    const GPoint operator+(const GPoint& point) const;
    //##ModelId=436CA0000399
    //##Documentation
    //## 功能：
    //##   构造函数
    //## 参数：
    //##   无
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-5
    GPoint();

	GPoint(int n);

    //##ModelId=436CA008036B
    //##Documentation
    //## 功能：
    //##   拷贝构造函数
    //## 参数：
    //##   无
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-5
    GPoint(const GPoint& copy);
    //##ModelId=436CA01F0290
    //##Documentation
    //## 功能：
    //##   拷贝构造函数
    //## 参数：
    //##   FLOAT8 x
    //##   FLOAT8 y
    //## 返回：
    //##   无
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-5
    GPoint(FLOAT8 sx, FLOAT8 sy);	
    //##ModelId=436CA08B006D
    //##Documentation
    //## 功能：
    //##   赋值函数
    //## 参数：
    //##   const GPoint&
    //## 返回：
    //##   const GPoint&
    //## 设计：
    //##   宋永生
    //## 日期：
    //##   2005-11-5
    const GPoint& operator=(const GPoint& copy);
    //##ModelId=436CA7D3033C
    //##Documentation
    //## 功能：
    //##      求点到另一点的距离
    //## 参数：
    //##      const GPoint& point//移动的点      
    //## 返回：
    //##     FLOAT8
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-5
    FLOAT8 distance(const GPoint& point) const;
    //##ModelId=436CA83D0242
    //##Documentation
    //## 功能：
    //##      偏移点
    //## 参数：
    //##      FLOAT8 ox
    //##      FLOAT8 oy
    //## 返回：（const GPoint&）
    //##     返回计算出的点,本地改变
    //## 设计：
    //##      宋永生
    //## 日期：
    //##      2005-11-5
    const GPoint& offset(FLOAT8 ox, FLOAT8 oy);
    //##ModelId=436CAA940280
    //##Documentation
    //## 功能：
    //##      求以本点为定点的角度
    //## 参数：
    //##      const GPoint& pt1//点1
    //##      const GPoint& pt2//点2
    //## 返回：
    //##     FLOAT8
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-5
    FLOAT8 angle(const GPoint& pt1, const GPoint& pt2) const;
    //##ModelId=436CB9B10186
    //##Documentation
    //## 功能：
    //##      重载操作符-
    //## 参数：
    //##      const GPoint& point//移动的点      
    //## 返回：（const GPoint）
    //##     返回计算出的点，不改变该点
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-5
    const GPoint operator-(const GPoint& point) const;
    //##ModelId=436CBA1101A5
    //##Documentation
    //## 功能：
    //##      重载操作符-=
    //## 参数：
    //##      const GPoint& point//移动的点      
    //## 返回：（const GPoint&）
    //##     返回计算出的点，改变该点
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-5
    const GPoint& operator-=(const GPoint& point);
    //##ModelId=436CBC9401C5
    //##Documentation
    //## 功能：
    //##      求该点到另两点决定直线的距离
    //## 参数：
    //##      const GPoint& pt1//点1
    //##      const GPoint& pt2//点2      
    //## 返回：
    //##     FLOAT8
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-5
    FLOAT8 distance(const GPoint& pt1, const GPoint& pt2) const;
    //##ModelId=436CC00B0128
    //##Documentation
    //## 功能：
    //##      求该点与另两点所组成三角形的面积
    //## 参数：
    //##      const GPoint& pt1//点1
    //##      const GPoint& pt2//点2      
    //## 返回：
    //##     FLOAT8
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-5
    FLOAT8 area(const GPoint& pt1, const GPoint& pt2) const;
    //##ModelId=436CCE680222
    //##Documentation
    //## 功能：
    //##      求该点是否在另两点所在的直线上
    //## 参数：
    //##      const  GPoint& pt1//点1
    //##      const  GPoint& pt2//点2
    //##      int32  width      //线宽
    //##      FLOAT8 range      //范围
    //## 返回：
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-5
    uint8 onLine(const GPoint& pt1, const GPoint& pt2, int32 width, FLOAT8 range) const;
    //##ModelId=436CD1920261
    //##Documentation
    //## 功能：
    //##      求该点是否在多边形内
    //## 参数：
    //##      GPoint* pts //多边形区域
    //##      int32   ptn //点个数
    //## 返回：
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-5
    uint8 inPolygon(GPoint* pts, int32 ptn) const;
    //##ModelId=436CD33E01A5
    //##Documentation
    //## 功能：
    //##      求该点是否在多边形内
    //## 参数：
    //##      GPoint* pts //多边形区域
    //##      int32   ptn //点个数
    //##      int32  width//线宽
    //##      FLOAT8 range//范围
    //## 返回：
    //##      if success return edge pos
    //##      else return -1
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-5
    int32 onPolygon(GPoint* pts, int32 ptn, int32 width, FLOAT8 range) const;
    //##ModelId=436CD54F0222
    //##Documentation
    //## 功能：
    //##      求该点是否在椭圆内
    //## 参数：
    //##      const GPoint& c    //椭圆中心点
    //##      FLOAT8 a           //a轴长
    //##      FLOAT8 b           //b轴长
    //##      int32  w           //线宽
    //## 返回：
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-5
    uint8 inEllipse(const GPoint& c, FLOAT8 a, FLOAT8 b, int32 w) const;
    //##ModelId=436CD5560196
    //##Documentation
    //## 功能：
    //##      求该点是否在椭圆上
    //## 参数：
    //##      const GPoint& c    //椭圆中心点
    //##      FLOAT8 a           //a轴长
    //##      FLOAT8 b           //b轴长
    //##      int32  w           //线宽
    //##      FLOAT8 r           //范围
    //## 返回：
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-5
    uint8 onEllipse(const GPoint& c, FLOAT8 a, FLOAT8 b, int32 w, FLOAT8 r) const;
    //##ModelId=436CD8F7009C
    //##Documentation
    //## 功能：
    //##      求以本点与另一点所成直线与X轴夹角->按逻辑坐标系XY轴
    //## 参数：
    //##      const GPoint& pt//点
    //## 返回：
    //##      FLOAT8
    //## 设计：
    //##      宋永生
    //## 日期：
    //##      2005-11-5
    FLOAT8 slopeAngle(const GPoint& pt) const;
    //##ModelId=436D604800EA
    //##Documentation
    //## 功能：
    //##      求该点是否在圆内
    //## 参数：
    //##      const GPoint& c    //圆中心点
    //##      FLOAT8 r            //半径
    //##      int32 w            //线宽
    //## 返回：
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-5
    uint8 inCircle(const GPoint& c, FLOAT8 r, int32 w) const;
    //##ModelId=436D605300BB
    //##Documentation
    //## 功能：
    //##      求该点是否在圆上
    //## 参数：
    //##      const GPoint& c    //圆中心点
    //##      FLOAT8 r           //半径
    //##      int32  w           //线宽
    //##      FLOAT8 range       //范围
    //## 返回：
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-5
    uint8 onCircle(const GPoint& c, FLOAT8 r, int32 w, FLOAT8 range) const;
    //##ModelId=436D64960232
    //##Documentation
    //## 功能：
    //##      求该点是否在矩形内
    //## 参数：
    //##      const GPoint& c    //矩形中心点
    //##      FLOAT8 a           //宽/2
    //##      FLOAT8 b           //高/2
    //##      int32  w           //线宽
    //## 返回：
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-5
    uint8 inRect(const GPoint& c, FLOAT8 a, FLOAT8 b, int32 w) const;
    //##ModelId=436D655202EE
    //##Documentation
    //## 功能：
    //##      求该点是否在矩形上
    //## 参数：
    //##      const GPoint& c    //矩形中心点
    //##      FLOAT8 a           //宽/2
    //##      FLOAT8 b           //高/2
    //##      int32  w           //线宽
    //##      FLOAT8 r           //范围
    //## 返回：
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-5
    uint8 onRect(const GPoint& c, FLOAT8 a, FLOAT8 b, int32 w, FLOAT8 r) const;

	//##ModelId=436D655202EE
    //##Documentation
    //## 功能：
    //##      求指定点决定的圆心坐标，返回值为半径
    //## 参数：
    //##      const GPoint& pts
    //##      const GPoint& ptc
    //##      const GPoint& pte
    //## 返回：
    //##      return 半径
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-5
	FLOAT8  radius( const GPoint& pts , const GPoint& ptc ,const GPoint& pte);

	//##ModelId=436D655202EE
    //##Documentation
    //## 功能：
    //##      计算指定点起始及终止长度
    //## 参数：
    //##      const GPoint& pts
    //##      const GPoint& ptc
    //##      const GPoint& pte
	//##      FLOAT8& as
	//##      FLOAT8& alen
    //## 返回：
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-5
	uint8	anglese(const GPoint& pts , const GPoint& ptc ,const GPoint& pte,\
		FLOAT8& as , FLOAT8& alen );

	//##ModelId=436D655202EE
    //##Documentation
    //## 功能：
    //##      计算直线与指定的水平线的交点X
    //## 参数：
    //##      const GPoint& pts
    //##      const GPoint& pte
	//##      FLOAT8 y0
    //## 返回：
    //##      return x坐标
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-5
	FLOAT8  lineX( const GPoint& pts , const GPoint& pte , FLOAT8 y0 );

	//##ModelId=436D655202EE
    //##Documentation
    //## 功能：
    //##      计算直线与指定的水平线的交点Y
    //## 参数：
    //##      const GPoint& pts
    //##      const GPoint& pte
	//##      FLOAT8 x0
    //## 返回：
    //##      return y坐标
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-5
	FLOAT8  lineY( const GPoint& pts , const GPoint& pte , FLOAT8 x0 );

	//##ModelId=436D655202EE
    //##Documentation
    //## 功能：
    //##      计算两直线的交点
    //## 参数：
    //##      const GPoint& ls1
    //##      const GPoint& le1
	//##      const GPoint& ls2
    //##      const GPoint& le2
    //## 返回：
    //##      if success return 1
    //##      else return 0
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-5
	uint8	intersect(const GPoint& ls1,const GPoint& le1,\
					const GPoint& ls2,const GPoint& le2);

	//##ModelId=436D655202EE
    //##Documentation
    //## 功能：
    //##      求线段中点
    //## 参数：
    //##      const GPoint& pts
    //##      const GPoint& pte
    //## 返回：
    //##      return 中点
    //## 设计：
    //##       宋永生
    //## 日期：
    //##       2005-11-5
	const GPoint& center(const GPoint& pts,const GPoint& pte);

    //##ModelId=4339FEDD0261
    FLOAT8 x;

    //##ModelId=4339FF0302CE
    FLOAT8 y;

};

class GDE_EXPORT_DEF GListPOINTS:public GList<GPoint>{};

#endif /* GPOINT_H_HEADER_INCLUDED_BCB8A4D3 */
