#ifndef GTOOLSIMP_H_HEADER_INCLUDED_BAD1AA20
#define GTOOLSIMP_H_HEADER_INCLUDED_BAD1AA20
#include "ddef/ddes_comm.h"

class Gtools;
class DscdiBase;

class GDE_EXPORT_GSCDI GtoolsIMP
{
  public:
    GtoolsIMP();
    virtual ~GtoolsIMP();

	virtual uint32 notifyParent(uint32 cmd,GWPARAM inp=NULL, GLPARAM outp=NULL);
    virtual uint32 sendCMD(uint32 cmd, GWPARAM inp =NULL,GLPARAM outp =NULL);

	virtual uint32 getCMDCount() const;
	virtual uint32 getCMDPos( uint32 nPos) const;	
	virtual const char* getPOSDesc(uint32 nPos) const;
    virtual const char* getCMDDesc(uint32 cmd) const;

    void setParent(DscdiBase* parent);
    DscdiBase* getParent() const;	

  protected:
    Gtools* m_pParent;

};

#endif
