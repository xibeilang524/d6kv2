#ifndef GXYCOORTRANS_H_HEADER_INCLUDED_BC903B02
#define GXYCOORTRANS_H_HEADER_INCLUDED_BC903B02
#include "GCoorTrans.h"
#include "ddef/ddes_comm.h"
#include "ddef/GPoint.h"
#include "ddef/GRect.h"

class GDE_EXPORT_DES GXYCoorTrans : public GCoorTrans
{
  public:
    GXYCoorTrans();
    GXYCoorTrans(const GXYCoorTrans& trans);
    const GXYCoorTrans& operator=(const GXYCoorTrans& trans);
    uint8 _DP2LP(FLOAT8 ix, FLOAT8 iy, FLOAT8& ox, FLOAT8& oy) const;
    uint8 _LP2DP(FLOAT8 ix, FLOAT8 iy, FLOAT8& ox, FLOAT8& oy) const;
    FLOAT8 DP2LP_Unit(uint8 flag=COOR_SCALE) const;
    FLOAT8 LP2DP_Unit(uint8 flag=COOR_SCALE) const;
    GPoint DO() const;
	virtual GPoint Offset() const;
    GPoint LO() const;
    void setLO(const GPoint& opt);
	void setOffset(const GPoint& off);
    void setDO(const GPoint& opt);
    const GPoint& getLO() const;
    GPoint& getLO();
    const GPoint& getDO() const;
    GPoint& getDO();
    void setScale(FLOAT8 scale);
	void setScaleX(FLOAT8 scale);
	void setScaleY(FLOAT8 scale);
    FLOAT8 getScale() const;
	FLOAT8 getScaleX() const;
	FLOAT8 getScaleY() const;    
    GPoint DP2XY(const GPoint& pt) const;
    GPoint XY2DP(const GPoint& pt) const;
    GPoint mirrorX(const GPoint& pt) const;
    GPoint mirrorY(const GPoint& pt) const;
    GPoint mirrorO(const GPoint& pt) const;
    GPoint mirrorXY(const GPoint& pt) const;
    GPoint mirrorNXY(const GPoint& pt) const;
    GRect mirrorX(const GRect& rc) const;
    GRect mirrorY(const GRect& rc) const;
    GRect mirrorO(const GRect& rc) const;
    GRect mirrorXY(const GRect& rc) const;
    GRect mirrorNXY(const GRect& rc) const;
    GPoint mirror(const GPoint& pt, int32 op) const;
    GRect mirror(const GRect& rc, int32 op) const;
  protected:
    GPoint m_loPoint;
	GPoint m_offsetPoint;
    GPoint m_doPoint;
    FLOAT8 m_scaleDL;
	FLOAT8 m_scaleDLX ;
	FLOAT8 m_scaleDLY ;

};

#endif
