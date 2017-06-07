#ifndef GSCDIBASE_H_HEADER_INCLUDED_BB0439D4
#define GSCDIBASE_H_HEADER_INCLUDED_BB0439D4

#include "ddef/ddes_comm.h"

//#include <qlibrary.h>	//lbh20130117
#include <QtCore/QLibrary>

typedef void* (*GDSFB_PROC)();

class GDE_EXPORT_GSCDI DscdiBase
{
  public:
    DscdiBase();
    virtual ~DscdiBase();

    virtual void clearImpObject();
    virtual uint8 createImpObject();

    void clearImpSrc();
    void setImpSrc(const QString& pluginsrc);

    void* newImpObject(const QString& pluginsrc);

  protected:

    QString m_impSrc;

  private:

    QLibrary* m_hLibrary;

};

#endif
