#ifndef GSCALEFACTOR_H_HEADER_INCLUDED_BCB882DE
#define GSCALEFACTOR_H_HEADER_INCLUDED_BCB882DE
#include "ddef/ddes_comm.h"
#include "ddef/GPoint.h"
#include "ddef/GRect.h"

class GBaseObject;
class GFile;

class GDE_EXPORT_DES GScaleFactor
{
  public:

    GScaleFactor();

    GScaleFactor(FLOAT8 sx, FLOAT8 sy, const GPoint& opt);

    const GPoint scalePoint(const GPoint& srcPoint);
    void scaleObject(GBaseObject* obj);
    const GPoint rotatePoint(const GPoint& srcPoint);
    GScaleFactor(FLOAT8 ra, const GPoint& opt);
    void setScale(FLOAT8 sx, FLOAT8 sy, const GPoint& opt);
    void setRotate(FLOAT8 ra, const GPoint& opt);
    const GPoint transPoint(const GPoint& srcPoint);
    void setTrans(FLOAT8 sx, FLOAT8 sy, FLOAT8 ra, const GPoint& opt);
    GScaleFactor(FLOAT8 sx, FLOAT8 sy, FLOAT8 ra, const GPoint& opt);
    GScaleFactor(const GScaleFactor& copy);
    const GScaleFactor& operator=(const GScaleFactor& copy);
    virtual int32 saveFile(GFile& fFile);

    virtual int32 loadFile(GFile& fFile);
    void setScale(FLOAT8 sx, FLOAT8 sy);
    void setRotate(FLOAT8 ra);
    void setTrans(FLOAT8 sx, FLOAT8 sy, FLOAT8 ra);

    FLOAT8 scaleX;
    FLOAT8 scaleY;
    GPoint pointO;
    FLOAT8 rotate;
    uint32 mask;

};

#endif
