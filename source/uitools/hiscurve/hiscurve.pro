TEMPLATE = app
TARGET =   hiscurve
DEPENDPATH += . forms translations tmp/rcc


QT += 
CONFIG +=

# set path
DESTDIR +=  		$$(NBENV)/bin
UI_DIR  +=  		forms
RCC_DIR +=  		tmp/rcc
MOC_DIR +=  		tmp/moc
OBJECTS_DIR += 	tmp/obj

# Input
HEADERS += appdef.h \
           datacurveapp.h \
           datacurvedefine.h \
           datacurveplot.h \
           datacurveview.h \
           dataselectortoolbar.h \
           datatableview.h \
           hisdatamodel.h \
           mainwindow.h \
           selectorwidget.h \
           dbextension.h  \
           logindialog.h  \
           datacurvecommand.h \
           variantdatacurveplot.h \
           variantcurveform.h \
		   trendcurveform.h \
		   trenddatacurveplot.h
           
SOURCES += datacurveapp.cpp \
           datacurvedefine.cpp \
           datacurveplot.cpp \
           datacurveview.cpp \
           dataselectortoolbar.cpp \
           datatablevew.cpp \
           hisdatamodel.cpp \
           main.cpp \
           mainwindow.cpp \
           selectorwidget.cpp \
           dbextension.cpp \
           logindialog.cpp \
           datacurvecommand.cpp  \
           variantdatacurveplot.cpp \
           variantcurveform.cpp \
		   trendcurveform.cpp \
		   trenddatacurveplot.cpp
           
#define app version,optional param "APP_VER_1_0" and "APP_VER_2_0"  
#undefine dbg_regproc unuse regproc
#DBG_REGPROC
#define APP_HAS_VERSION //use include app version information         
DEFINES += APP_VER_1_0 DBG_REGPROC


win32 {
		#use excelformat export tableview to 
		#暂时去掉便于写代码
		DEFINES += WIN32 QWT_DLL #EXCELFORMAT 
		contains( DEFINES, EXCELFORMAT ) {
				message("win32 defined macro EXCELFORMAT ")
		    HEADERS += excelformat/BasicExcel.hpp \
		           excelformat/ExcelFormat.h 
				SOURCES += excelformat/BasicExcel.cpp \
		           excelformat/ExcelFormat.cpp
		}
		
		
		CONFIG += DEBUG
		RC_FILE = hiscurve.rc
					 
		INCLUDEPATH	+= . ../include $$(NBENV)/include $$(NBENV)/include/fep $$(NBENV)/include/db \
						$$(NBENV)/include/net $$(NBENV)/include/rdb $$(NBENV)/include/scd \
						./qwt_include 
						
		
		
		LIBS += -L$$(NBENV)/lib \
						-ldbsql \
						-lscdnet \
						-lscdrdb \
						-ldlogin \
						-ldmessagebox \
						-lqwt5 \
						-lauth \        
		        -loci
		contains( DEFINES, APP_HAS_VERSION ) {
			LIBS += -lverapi
		}
}   

unix {
		           
		INCLUDEPATH	+=  .  $$(NBENV)/source/include  $$(NBENV)/source/include/afert  $$(NBENV)/source/include/db \
						$$(NBENV)/source/include/net  $$(NBENV)/source/include/rdb  $$(NBENV)/source/include/scd  \
						./qwt_include \
						$$(ORACLE_HOME)/oci/include/
		LIBS += 	-L$$(NBENV)/lib \
								-ldbsql  \
								-lscdnet \
								-lscdrdb \
								-ldlogin \
								-ldmessagebox \
								-lqwt	\
						  	-lauth
		contains( DEFINES, APP_HAS_VERSION ) {
		LIBS += -lverapi
		}
									
		#./bin 当前路径下的目录
}
        
RESOURCES += hiscurve.qrc
TRANSLATIONS += translations/datacurve_zh_CN.ts translations/qt_zh_CN.ts






