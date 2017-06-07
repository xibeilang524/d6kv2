#ifndef GSTREAMIMP_H_HEADER_INCLUDED_BC30E4AC
#define GSTREAMIMP_H_HEADER_INCLUDED_BC30E4AC
#include "ddef/ddes_comm.h"

class GBaseObject;
class Gstream;
class GstreamIMP ;

class GDE_EXPORT_GSCDI GstreamIMP
{
  public:
    GstreamIMP();

    virtual ~GstreamIMP();

    virtual uint8 regStream(Gstream* stream);
    virtual uint8 unregStream();
    virtual void  stopStream();

    virtual void update();
    virtual void updateObj(GBaseObject* pObj);

	virtual void setValue( int32 objPos , FLOAT8 value);
	virtual void setValue( int32 objPos , const char* value);

  protected:
    Gstream* m_pParent;

};

#endif
