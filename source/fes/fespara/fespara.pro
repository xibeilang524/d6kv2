######################################################################
# Automatically generated by qmake (2.01a) ??? ?? 17 14:40:31 2011
######################################################################

TEMPLATE	= app
DESTDIR   = $(NBENV)/bin


TARGET = fespara
DEPENDPATH += .
INCLUDEPATH += $(NBENV)/source/include $(ORACLE_HOME)/oci/include

# Input
HEADERS += fespara.h fespara_string.h
SOURCES += fespara.cpp main.cpp

win32{
	Release{
		LIBS = -lACE
	}
	Debug{
		LIBS = -lACEd
	}
}

unix{
	LIBS = -L$(ACE_ROOT)/lib -lACE
}

LIBS += -L$(NBENV)/lib -lscdclient -lverapi -lfesshm -lfesplatform -ldmessagebox

RESOURCES = res.qrc
win32 {
	#CONFIG += console
	RC_FILE = fespara.rc
}