#ifndef GSTREAM_H_HEADER_INCLUDED_BC330BF3
#define GSTREAM_H_HEADER_INCLUDED_BC330BF3

#include "dscdibase.h"

struct GstreamField;
class GGraphLayer;
class GGraphFile;
class GGraphDynData;
class GBaseObject;
class GstreamIMP;
class GBayObject;

class GDE_EXPORT_GSCDI Gstream:public DscdiBase
{
public:

    Gstream();	
    virtual ~Gstream();
	
    virtual uint8 start(GGraphFile* pFile,int32 groupno);	
    virtual void end();
	
    virtual void update();
    virtual void updateObj(GBaseObject* pObj);

	virtual void setValue( int32 objPos , FLOAT8 value);	
	virtual void setValue( int32 objPos , const char* value);
	
    GBaseObject* getObjAt(int32 pos) const;	
    GHANDLE getAttachDataAt(int32 pos) const;
    int32 getDataPosAt(int32 pos) const;
    GstreamField* getParaAt(int32 pos) const;

    GBaseObject** getObj() const;
    GHANDLE* getAttachData() const;
    int32* getDataPos() const;
    GstreamField* getPara() const;
	
    int32 getCount() const;	
    uint8 isValid() const;

	virtual uint8 createImpObject();
    virtual void  clearImpObject();

	virtual GHANDLE getStreamAttachInfo() const;
	virtual void    setStreamAttachInfo(GHANDLE info);
	
protected:

    virtual uint8 addObject(GBaseObject* pObj,int32 groupno);	
    virtual uint8 addLayer(GGraphLayer* pLayer,int32 groupno);
	uint8 addBay(GBaseObject* pBayObj,int32 groupno);
	
    virtual GstreamField addDynData(\
		GBaseObject* pObj,int32 groupno,\
		GGraphDynData* pDynData,int32 nPos=-1);	
    virtual GstreamField addDevData(\
		GBaseObject* pObj,int32 groupno,\
		GGraphDynData* pDynData,int32 nPos=-1);
	virtual GstreamField addCustomData(\
		GBaseObject* pObj,int32 groupno,int32 nPos=-1);	
    virtual uint8 addRdbField(\
		GBaseObject* pObj,GstreamField* pData,\
		GHANDLE pAttach=NULL,int32 nPos=-1);
	
    GstreamIMP*   m_streamIMP;
    GBaseObject** m_objBuffer;
	GHANDLE*	  m_objAttachData;	
	int32*		  m_objDataPos;
    GstreamField* m_streamBuffer;

    uint32		  m_streamNumber;
	GHANDLE		  m_streamAttachInfo;
};

#endif
