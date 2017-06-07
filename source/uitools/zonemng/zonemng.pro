
DESTDIR   = $(NBENV)/bin

TARGET    = zonemng

INCLUDEPATH	+= $(NBENV)/source/include $(ORACLE_HOME)/oci/include

CONFIG	+= qt warn_on thread debug

LIBS	+= -L$(NBENV)/lib -ldbsql -lscdnet -lscdclient -lscdrdb -lauth -lverapi -ldlogin -ldmessagebox

win32 {
	RC_FILE = zonemng.rc 
	HEADERS	+= resource.h
	DEFINES -= UNICODE
}

HEADERS	+= QZoneMainWnd.h \
	QZonePara.h \
	globaldef.h \
	QLoginWnd.h
SOURCES	+= main.cpp \
	QZoneMainWnd.cpp \
	QZonePara.cpp \
	QLoginWnd.cpp
FORMS	= qzonewndbase.ui \
	qloginwndbase.ui
RESOURCES = zonemng.qrc

