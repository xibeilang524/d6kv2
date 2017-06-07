
TEMPLATE =  app
TARGET =   websrv
DEPENDPATH += . ./include


CONFIG    += warn_on thread
CONFIG -= qt 
QT -= gui core

# set path
DESTDIR +=      $$(NBENV)/bin
OBJECTS_DIR += 	tmp/obj

# Input
HEADERS += ./include/db.h \
           ./include/externdefine.h \
           ./include/filestat.h \
           ./include/message.h \
           ./include/ThreadStreamData.h \
           ./include/webconstant.h \
           ./include/webstruct.h \
           ./include/wfa.h 
SOURCES += ./unix/answermsg.cxx \
           ./unix/commfunction.cxx \
           ./unix/DB.cxx \
           ./unix/filestat.cxx \
           ./unix/srvwatchthread.cxx \
           ./unix/ThreadStreamData.cxx \
 
#define app version,optional param "APP_VER_1_0" and "APP_VER_2_0"  
#define APP_HAS_VERSION //use include app version information                  
DEFINES += APP_VER_1_0 

win32 {
#use excelformat export tableview to 
DEFINES += WIN32 _DEBUG _WINDOWS _AFXDLL
DEFINES -= UNICODE 
HEADERS += About.h \
           ClientInfo.h \
           DlgSysConfig.h \
           hyperlink.h \
           MainFrm.h \
           resource.h \
           StdAfx.h \
           WebManage.h \
           WebManageDoc.h \
           WebManageView.h
SOURCES += ClientInfo.cpp \
           DlgSysConfig.cpp \
           HyperLink.cpp \
           MainFrm.cpp \
           StdAfx.cpp \
           WebManage.cpp \
           WebManageDoc.cpp \
           WebManageView.cpp 
CONFIG += DEBUG
RC_FILE = WebManage.rc
			 
INCLUDEPATH	+= . ../include $$(NBENV)/include $$(NBENV)/include/fep $$(NBENV)/include/db \
				$$(NBENV)/include/net $$(NBENV)/include/rdb $$(NBENV)/include/scd \
				./include \
				C:\instantclient_11_2\oci\include
LIBS += -L$$(NBENV)/lib \
		-ldbsql  -lscdnet -lscdrdb 
	contains( DEFINES, APP_HAS_VERSION ) {
	 LIBS += -lverapi
	}

}   

unix {
     
SOURCES += ./unix/webmain.cxx
                 
INCLUDEPATH	+=  .  $$(NBENV)/source/include  $$(NBENV)/source/include/afert  $$(NBENV)/source/include/db \
				$$(NBENV)/source/include/net  $$(NBENV)/source/include/rdb  $$(NBENV)/source/include/scd  \
				./include \
				$$(ORACLE_HOME)/oci/include/
				
LIBS += 	-L$$(NBENV)/lib \
			-lscdnet \
		    -lscdrdb \
			-ldbsql \ 
			-lscdipc
	contains( DEFINES, APP_HAS_VERSION ) {
	 LIBS +=  -lverapi
	}
}
