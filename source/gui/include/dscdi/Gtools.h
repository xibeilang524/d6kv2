#ifndef GTOOLS_H_HEADER_INCLUDED_BAD1BD95
#define GTOOLS_H_HEADER_INCLUDED_BAD1BD95

#include "dscdibase.h"

class GtoolsIMP;

class GDE_EXPORT_GSCDI Gtools : public DscdiBase
{
  public:
    Gtools();
    virtual ~Gtools();

    virtual uint8 createImpObject();
    virtual void clearImpObject();

	virtual uint32 childReflect(uint32 cmd,GWPARAM inp=NULL,GLPARAM outp=NULL);
    virtual uint32 sendCMD(uint32 cmd,GWPARAM inp=NULL,GLPARAM outp=NULL);
    
	virtual uint32 getCMDCount() const;
	virtual uint32 getCMDPos( uint32 nPos) const;
    virtual const char* getPOSDesc(uint32 nPos) const;
    virtual const char* getCMDDesc(uint32 cmd) const;

  protected:
    GtoolsIMP* m_toolsIMP;
};

#endif
