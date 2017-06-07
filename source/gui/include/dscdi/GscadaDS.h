#ifndef GSCADADS_H_HEADER_INCLUDED_BB0425D1
#define GSCADADS_H_HEADER_INCLUDED_BB0425D1

#include "gscdi/GtoolsIMP.h"

class GscadaDS : public GtoolsIMP
{
  public:
    GscadaDS();

    ~GscadaDS();

    virtual uint32 sendCMD(
        uint32 cmd,
        uint32 inp=0,
        uint32 outp=0);

    virtual uint32 getCMD(uint32** cmd);
    virtual const char* getCMDDesc(uint32 cmd);

};

extern "C" GDE_EXPORT GtoolsIMP* gdsFactoryBuild();

#endif
