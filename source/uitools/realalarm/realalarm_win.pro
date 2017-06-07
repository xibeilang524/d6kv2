
TEMPLATE	= app
DESTDIR   = $(NBENV)/bin

TARGET    = realalarm
DEFINES	-= UNICODE
CONFIG 	 += qt warn_on thread 

INCLUDEPATH	+= . ../include $(NBENV)/source/include $(NBENV)/source/include/db \
				$(NBENV)/source/include/net $(NBENV)/source/include/rdb $(NBENV)/source/include/scd \
				$(SYS_DB_INC)

LIBS			+= -L$(NBENV)/lib -L$(SYS_DB_LIB)  -ldbsql -lscdnet -lscdclient \
			-lscdrdb -ldbreadevt -lscdserver -lauth -ldmfctools -lverapi -ldlogin -ldmessagebox
			
unix {
	LIBS +=  -lesd -laudiofile
	HEADERS	+= QMySound.h
	SOURCES	+= QMySound.cpp
}

win32 {
	RC_FILE = realalarm.rc 
	HEADERS	+= resource.h
	DEFINES -= UNICODE
	LIBS += -lws2_32 -lwinmm
}

HEADERS	+= globaldef.h \ 
	event_cfg.h \
	event_filter.h \
	event_para.h \
	event_print.h \
	event_sound.h \
	event_table.h \
	event_real_wnd.h \
	event_sys_set_dlg.h \
	event_ui_set_dlg.h \
	login_dlg.h \
	event_msg.h \
	list_view_color_item.h \
	read_event_proc.h \
	qextserialport.h \
	qextserialbase.h \
	report_choose.h\
	phone_no_edit.h\
	title_frame.h \
	tool_button.h \
	tool_button_frame.h \
	win_qextserialport.h 
SOURCES	+= main.cpp \
	event_cfg.cpp \
	event_filter.cpp \
	event_para.cpp \
	event_print.cpp \
	event_sound.cpp \
	event_table.cpp \
	event_real_wnd.cpp \
	event_sys_set_dlg.cpp \
	event_ui_set_dlg.cpp \
	login_dlg.cpp \
	event_msg.cpp \
	list_view_color_item.cpp \
	read_event_proc.cpp \
	qextserialport.cpp \
	qextserialbase.cpp \
	report_choose.cpp\
	phone_no_edit.cpp\
	title_frame.cpp \
	tool_button.cpp \
	tool_button_frame.cpp \
    win_qextserialport.cpp 

FORMS	+= qeventsyssetbase.ui \
			qeventuisetbase.ui \
			qloginbase.ui \
			qreportchoosebase.ui \
			qphonenoedit.ui
win32:RC_FILE = realalarm.rc
RESOURCES = realalarm.qrc
QT += xml



