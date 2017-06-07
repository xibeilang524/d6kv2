
TEMPLATE = app
TARGET =   scriptedit
DEPENDPATH += . translations tmp\rcc


QT += xml
CONFIG +=

# set path
DESTDIR +=  		$$(NBENV)/bin
UI_DIR  +=  		forms
RCC_DIR +=  		tmp/rcc
MOC_DIR +=  		tmp/moc
OBJECTS_DIR += 	tmp/obj


# Input

HEADERS += appdef.h \
			functionwidget.h \
           mainwindow.h \
           scriptsyntaxhighlighter.h \
           userscriptedit.h \
           rdbparadialog.h \
           customfunctiondialog.h \
           ../hiscurve/dbextension.h \
           upctrl.h
SOURCES += functionwidget.cpp \
           main.cpp \
           mainwindow.cpp \
           scriptsyntaxhighlighter.cpp \
           userscriptedit.cpp \
           rdbparadialog.cpp \
           customfunctiondialog.cpp \
           ../hiscurve/dbextension.cpp \
           upctrl.cxx
           
#define app version,optional param "APP_VER_1_0" and "APP_VER_2_0"           
DEFINES += APP_VER_1_0
#DEFINES += DBG_REGPROC 

win32 {
		CONFIG += DEBUG
		DEFINES += WIN32
		RC_FILE = res/scriptedit.rc 	
		
				
		INCLUDEPATH	+= . ../include $$(NBENV)/include $$(NBENV)/include/fep $$(NBENV)/include/db \
						$$(NBENV)/include/net $$(NBENV)/include/rdb $$(NBENV)/include/scd \
						#..\scada\include_scada \
						$$(NBENV)/source/scada/include-scada 
						
						
		LIBS += -L$$(NBENV)/lib \
						-lscdnet \ 
		        -ldbsql  \
		        -lscdserver \
		        -lscdrdb \
		        -lverapi\
				-ldmessagebox \
		         -lpldstationselect
		     
}   

unix {
		
		INCLUDEPATH	+= . $$(NBENV)/source/include  $$(NBENV)/source/include/afert  $$(NBENV)/source/include/db \
				$$(NBENV)/source/include/net  $$(NBENV)/source/include/rdb  $$(NBENV)/source/include/scd   $$(NBENV)/source/gui/include\
						$$(NBENV)/source/scada/include-scada \
						$$(ORACLE_HOME)/oci/include/
						
		LIBS += 	-L$$(NBENV)/lib \
							-lscdnet \
							-ldbsql  \
							-lscdserver \
							-lverapi \
							-lscdrdb \
							-ldmessagebox \
						 -lpldstationselect 
}
  
RESOURCES += scriptedit.qrc
TRANSLATIONS += translations/scriptedit_zh_CN.ts translations/qt_zh_CN.ts







