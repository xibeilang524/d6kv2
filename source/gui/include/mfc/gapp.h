#ifndef _GAPP_H
#define _GAPP_H

#include "mfc/ddefs.h"
#include <QtGui/QApplication>

class GDE_EXPORT_MFC GApp:public QApplication
{
public:
	GApp( int argc , char** argv );
};

#endif
