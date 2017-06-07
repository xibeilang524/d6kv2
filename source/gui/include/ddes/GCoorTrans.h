#ifndef GCOORTRANS_H_HEADER_INCLUDED_BC9176DE
#define GCOORTRANS_H_HEADER_INCLUDED_BC9176DE
#include "ddef/ddes_comm.h"
#include "ddef/GPoint.h"
#include "ddef/GRect.h"
#include "GObjectCommon.h"

class GDE_EXPORT_DES GCoorTrans
{
  public:
    virtual uint8 _LP2DP(FLOAT8 ix, FLOAT8 iy, FLOAT8& ox, FLOAT8& oy) const = 0;
    virtual uint8 _DP2LP(FLOAT8 ix, FLOAT8 iy, FLOAT8& ox, FLOAT8& oy) const = 0;
    virtual FLOAT8 DP2LP_Unit(uint8 flag=COOR_SCALE) const = 0;
    virtual FLOAT8 LP2DP_Unit(uint8 flag=COOR_SCALE) const = 0;
    GPoint DP2LP(const GPoint& pt) const;
    GPoint LP2DP(const GPoint& pt) const;
    FLOAT8 LP2DP(FLOAT8 size,uint8 flag=COOR_SCALE) const;
    FLOAT8 DP2LP(FLOAT8 size,uint8 flag=COOR_SCALE) const;
    GRect DP2LP(const GRect& rc) const;
    GRect LP2DP(const GRect& rc) const;
    int32 DP2LP(GPoint* pts, int32 nums) const;
    int32 LP2DP(GPoint* pts, int32 nums) const;
    GPoint DP2LP_Size(const GPoint& pt) const;
    GPoint LP2DP_Size(const GPoint& pt) const;
    virtual GPoint DO() const = 0;
	virtual GPoint Offset() const = 0;
    virtual GPoint LO() const = 0;
    uint8 DP2LP(FLOAT8 ix, FLOAT8 iy, FLOAT8& ox, FLOAT8& oy) const;
    uint8 LP2DP(FLOAT8 ix, FLOAT8 iy, FLOAT8& ox, FLOAT8& oy) const;	
    void setRect(const GRect& rc);
    const GRect& getRect() const;
    GRect& getRect();
	uint8 canViewDP(const GPoint& pt) const;
    uint8 canViewDR(const GRect& rc) const;
    uint8 canViewLP(const GPoint& pt) const;
    uint8 canViewLR(const GRect& rc) const;
  protected:
    GRect m_lvRect;

};

#endif
