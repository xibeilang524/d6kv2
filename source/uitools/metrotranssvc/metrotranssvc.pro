TEMPLATE = app
TARGET = metrotranssvc
INCLUDEPATH += $(NBENV)/source/include \
				$(NBENV)/source/include/afert \
				$(NBENV)/source/include/db \
				$(NBENV)/source/include/net \
				$(NBENV)/source/include/rdb \
				$(NBENV)/source/include/scd \
				$(SYS_DB_INC)
DESTDIR = $(NBENV)/bin
LIBS=-L$(NBENV)/lib -lscdnet -ldbsql -lscdrdb -lscdipc -lscddev -lverapi -lscdclient -lscdserver 
DEFINE += _CRT_SECURE_NO_WARNINGS

# Input
HEADERS += fileinfo.h \
			manageevent.h \
			mainsvc.h \
			globaldef.h
SOURCES += main.cpp \
           fileinfo.cpp \
           manageevent.cpp \
           mainsvc.cpp 
win32 {
	RC_FILE += metrotranssvc.rc
}
