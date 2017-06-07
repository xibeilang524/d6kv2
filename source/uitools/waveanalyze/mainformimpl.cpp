#include <QTabWidget>
#include <QFileInfo>
#include <QMessageBox>
#include <QSplitter>
#include <QAction>
#include <QMenu>
#include <QScrollArea>
#include <QComboBox>
#include <QApplication>
#include <QLayout>
#include <QFileDialog>
#include <QTimer>

#include "optionsimpl.h"
#include "channelpropertyimpl.h"
#include "comandwave.h"
#include "fileoperate.h"
#include "manageutils.h"
// #include "constutils.h"
#include "globaldef.h"
#include "filepropertyimpl.h"
#include "wavepropertyimpl.h"
#include "vectorsimpl.h"
#include "mainformimpl.h"
#include "wavepix.h"
#include "fileviewimpl.h"
#include "ftpwindow.h"

extern QWidget* getMainWnd();


// 当前数据对象
Gain_data* DataObject()
{
	Main_form_impl* mainform = (Main_form_impl*)(getMainWnd());
	return mainform->Data();
}

extern Propertys* g_Propertys;

/* QMainFormImpl */

Main_form_impl::Main_form_impl( QWidget* parent )
    : QMainWindow( parent)
{
	setupUi(this);
	m_tab_wave = new QWaveTabWidget(widget);
	QMainFormLayout = new QVBoxLayout (widget);
	QMainFormLayout->addWidget(m_tab_wave);

	m_dock_value = create_dock_widget(_C2Q("曲线信息"),this);
	m_dock_value->setObjectName(QString::fromUtf8("dockValue"));
	m_dock_value->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable|QDockWidget::NoDockWidgetFeatures);
	m_dock_value->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	m_value = new Dlg_value_impl(m_dock_value);
	m_dock_value->setWidget(m_value);
	addDockWidget(Qt::RightDockWidgetArea, m_dock_value);

	m_dock_vector = create_dock_widget(_C2Q("矢量信息"),this);

	g_Propertys = new Propertys;
	m_auto_play_interval = 500;

// 	EditZoom = new QComboBox(EditToolbar);
// 	EditZoom->setEditable(true);	
//  	EditToolbar->addWidget(EditZoom);
	m_x_edit_zoom = new QSpinBox(EditToolbar);
	m_x_edit_zoom->setMaximum(1000);	//最大值1000
	m_x_edit_zoom->setSingleStep(100);	
	m_x_edit_zoom->setSuffix(_C2Q("%"));	//后缀 %
	EditToolbar->addWidget(m_x_edit_zoom);

	m_timer_ptr = new QTimer();
	g_GraphMgr = new QGraphMgr;
	g_ComtradeMgr = new QComtradeMgr;
	m_status_bar = new QWaveBar(statusBar());
// 	m_pVectors = new QDlgVectorImpl(m_pDockVector);
// 	m_pDockVector->setWidget(m_pVectors);
// 	m_pix_lock[0] = new QPixmap((const char**)Line_lock);
// 	m_pix_lock[1] = new QPixmap((const char**)Line_Unlock);
	m_pix_lock[0] = new QPixmap(":/res/images/unlock.png");
	m_pix_lock[1] = new QPixmap(":/res/images/lock.png");

	create_graph();
	show_math_type();
	set_action_enable(false);

// 	QFont font = this->font();
// 	font.setFamily(tr("微软雅黑"));
// 	font.setPointSize(12);
// 	this->setFont(font);
// 	setIconSize( QSize( 16 , 16 ) ) ;

	setStyleSheet("QMainWindow{background:white;}"	
					"QMenuBar{font-family:Microsoft YaHei;font-size:14px;color:#3E464E;background: #FAFAFA;\
				  min-width:60px;min-height:32px;border:1px solid #D5D5D5;}"
				  "QMenuBar::item:selected {background:#4A90E2;color:white;}"
				  "QMenu{font-family:Microsoft YaHei;font-size:13px;color:black;background-color:#FAFAFA;}"			
				  "QMenu::item:selected {background:#4A90E2;color:white;}"
				  "QToolBar{spacing:6px;min-height:30px;background:#FAFAFA;}"
				  "QToolBar QToolButton:checked {background: #FFFFFF;border: 1px solid #4A90E2;}"
				  "QSpinBox{	font-family: Microsoft YaHei;	font-size: 13px;color:#4A4A4A;	}"
				  "QSpinBox::up-button{subcontrol-origin: padding;subcontrol-position: right;width: 24px;height: 24px;border-width: 3px;}"
				  "QSpinBox::up-arrow{image: url(:/res/images/spinup.png);width: 18px;height: 18px;}"
				  "QSpinBox::down-button{subcontrol-origin: border;	subcontrol-position: left;width: 24px;height: 24px;border-width: 3px;}"
				  "QSpinBox::down-arrow{image: url(:/res/images/spindown.png);width: 18px;height: 18px;}"
				  "QSplitter{background: #FAFAFA;}"
				  "QTabWidget::pane{border:none;}""QTabWidget::tab-bar{alignment:left;}"
				  "QTabBar::tab{font-family: SimSun;color:#0F1923; font-size: 12px;min-width:60px; min-height:30px;border:0px;}"
				  "QTabBar::tab:hover{background:rgb(255,255, 255, 100);}"
				  "QTabBar::tab:selected{border-color:white;background:white;color:#5D9CE5;border-bottom: 2px solid #4A90E2;}"
				  "QRadioButton::indicator:unchecked{image:url(:/img/images/radio.png);}"
				  "QRadioButton::indicator:checked{image:url(:/img/images/radio_checked.png);}"
				  "QDialog{background:white;}"	
				  "QFrame{background:white;}"	
				  "QLabel{font-family: Microsoft YaHei;font-size: 12px;color: #0F1923;}"
				  "QLineEdit {font-family:Microsoft YaHei;font-size: 12px;color: #4A4A4A;background: #FFFFFF;\
				  border: 1px solid #C6CFD9;border-radius: 2px;min-height:22px;}"		
				  "QGroupBox{font-family: Microsoft YaHei;font-size: 13px;color: #0F1923;}"
				  "QComboBox {font-family: Microsoft YaHei;font-size: 12px;color: #3E464E;\
				  min-height:23px;border: 1px solid #C6CFD9;border-radius: 2px;}"
				  "QComboBox::drop-down {subcontrol-origin: padding;subcontrol-position: top right;width: 28px;\
				  border-top-right-radius: 2px;border-bottom-right-radius: 2px;}"
				  "QComboBox::down-arrow{image: url(:/res/dropdown.png);}"
				  "QPushButton{font-family: Microsoft YaHei;font-size: 12px;color: white;\
				  background:#4A90E2;border-radius: 2px;min-width:60px;min-height:23px;}"
				  "QPushButton:hover{background:#0C2543;}"
				  "QPushButton:disabled{background:#D5D5D5;}"
				  "QCheckBox{font-family: Microsoft YaHei;font-size: 14px;color:#3E464E;}"
				  "QCheckBox::indicator:unchecked{image:url(:/res/images/checkbox_unchecked.png);}"
				  "QCheckBox::indicator:checked{image:url(:/res/images/checkbox_checked.png);}"
				  "QTableWidget {font-family: Microsoft YaHei;font-size: 12px;color:#4A4A4A;background: #FAFAFA;border: 1px solid #E5E5E5;}"
				  "QTableWidget::item{text-align:center;}"
				  "QListView,QTreeView{font-family: Microsoft YaHei;font-size: 13px;color:#4A4A4A;border: 1px solid #E5E5E5;}"
				  "QListView::item:hover,QTreeView::item:hover{background-color: wheat;	}"
				  "QListView::item:selected,QTreeView::item:selected	{	background-color: lightblue;	}"
				  "QListView::indicator:unchecked{image:url(:/res/images/checkbox_unchecked.png);}"
				  "QListView::indicator:checked{image:url(:/res/images/checkbox_checked.png);}"  );

	// zoom's edit
// 	connect(EditZoom, SIGNAL(activated(int)), SLOT(ZoomActived(int)));
	connect(m_x_edit_zoom, SIGNAL(valueChanged(int)), SLOT(slot_zoom_actived(int)));

	// timer
	connect(m_timer_ptr, SIGNAL(timeout()), SLOT(slot_time_out()));

	// tabwidget
	connect(m_tab_wave, SIGNAL(currentChanged(QWidget*)), SLOT(slot_table_changed(QWidget*)));
	connect(m_tab_wave->WaveTabbar(), SIGNAL(signal_item_close(int)), SLOT(slot_item_close(int)));
	connect(m_tab_wave->WaveTabbar(), SIGNAL(signal_item_property(int)), SLOT(slot_item_property(int)));

	// file
	connect(FileNewAction, SIGNAL(activated()), SLOT(slot_file_new()));
	connect(FileOpenAction, SIGNAL(activated()), SLOT(slot_file_open()));
	connect(FileCloseAction, SIGNAL(activated()), SLOT(slot_file_close()));
	connect(FilePrintAction, SIGNAL(activated()), SLOT(slot_file_print()));
	connect(FilePropertyAction, SIGNAL(activated()), SLOT(slot_file_property()));
	connect(FileExitAction, SIGNAL(activated()), SLOT(slot_exit()));
	connect(FileViewAction, SIGNAL(activated()), SLOT(slot_file_view()));	
	connect(action_download_file, SIGNAL(activated()), SLOT(slot_down_load_file()));

	// view
	connect(ViewFullScreenAction, SIGNAL(activated()), SLOT(slot_view_fullscreen()));
	connect(ViewShowAllAction, SIGNAL(activated()), SLOT(slot_view_show_all()));
	connect(ViewAutoSetAction, SIGNAL(activated()), SLOT(slot_view_auto_set()));
	connect(ViewDraw1Action, SIGNAL(activated()), SLOT(slot_view_draw1()));
	connect(ViewDraw2Action, SIGNAL(activated()), SLOT(slot_view_draw2()));
	connect(ViewAutoPlayAction, SIGNAL(activated()), SLOT(slot_view_auto_play()));
	connect(ViewToHereAction, SIGNAL(activated()), SLOT(slot_view_to_here()));

	// toolbar
	connect(mnuMainBar, SIGNAL(toggled(bool)), SLOT(slot_view_mainbar(bool)));
	connect(mnuEditBar, SIGNAL(toggled(bool)), SLOT(slot_view_editbar(bool)));
	connect(mnuAnaBar, SIGNAL(toggled(bool)), SLOT(slot_view_anabar(bool)));
	
	connect(MainToolbar, SIGNAL(visibilityChanged(bool)), SLOT(slot_visible_changed1(bool)));
	connect(EditToolbar, SIGNAL(visibilityChanged(bool)), SLOT(slot_visible_changed2(bool)));
	connect(AnalyzeToolbar, SIGNAL(visibilityChanged(bool)), SLOT(slot_visible_changed3(bool)));

	// analyze
	connect(AnaValueAction, SIGNAL(activated()), SLOT(slot_analyze_value()));
	connect(AnaHarmAction, SIGNAL(activated()), SLOT(slot_analyze_harm()));
	connect(AnaVectorAction, SIGNAL(activated()), SLOT(slot_analyze_vector()));
	connect(AnaDeleteAction, SIGNAL(activated()), SLOT(slot_analyze_delete()));
	connect(AnaHarmWaveAction, SIGNAL(activated()), SLOT(slot_analyze_harm_wave()));

	// setting
	connect(SetZeroAction, SIGNAL(activated()), SLOT(slot_set_zero()));
	connect(SetFaultAction, SIGNAL(activated()), SLOT(slot_set_fault()));
	connect(SetPropertyAction, SIGNAL(activated()), SLOT(slot_set_property()));
	connect(SetOptionAction, SIGNAL(activated()), SLOT(slot_set_options()));

	// help
	connect(HelpAboutAction, SIGNAL(activated()), SLOT(slot_help_about()));

    // edit
 	connect(EditZoomYOutAction, SIGNAL(activated()), SLOT(slot_edit_zoom_Yout()));
	connect(EditZoomYInAction, SIGNAL(activated()), SLOT(slot_edit_zoom_YIn()));
	connect(EditLockAction, SIGNAL(activated()), SLOT(slot_edit_lock()));
	connect(EditEndAction, SIGNAL(activated()), SLOT(slot_edit_end()));
	connect(EditPageUpAction, SIGNAL(activated()), SLOT(slot_edit_page_up()));
	connect(EditPageDownAction, SIGNAL(activated()), SLOT(slot_edit_page_down()));
	connect(EditMoveUpAction, SIGNAL(activated()), SLOT(slot_edit_move_up()));
	connect(EditMoveDownAction, SIGNAL(activated()), SLOT(slot_edit_move_down()));
	connect(EditShowAllAction, SIGNAL(activated()), SLOT(slot_edit_show_all()));
	connect(EditHomeAction, SIGNAL(activated()), SLOT(slot_edit_home()));

	// caption menu
 	connect(CaptionDeleteAction, SIGNAL(activated()), SLOT(slot_caption_delete()));
 	connect(CaptionAutoPositonAction, SIGNAL(activated()), SLOT(slot_caption_auto_position()));
 	connect(CaptionAutoScaleAction, SIGNAL(activated()), SLOT(slot_caption_auto_scale()));
 	connect(CaptionHarmAction, SIGNAL(activated()), SLOT(slot_caption_harm()));
 	connect(CaptionHarmWaveAction, SIGNAL(activated()), SLOT(slot_caption_harm_wave()));
 	connect(CaptionPropertyAction, SIGNAL(activated()), SLOT(slot_caption_property()));
 	connect(CaptionSamePositonAction, SIGNAL(activated()), SLOT(slot_caption_same_position()));
 	connect(CaptionSameScaleAction, SIGNAL(activated()), SLOT(slot_caption_same_scale()));

	// dock windows
	connect(m_dock_value, SIGNAL(visibilityChanged(bool)), SLOT(slot_value_dock(bool)));
	connect(m_dock_value, SIGNAL(visibilityChanged(bool)), SLOT(slot_vector_dock(bool)));
}

Main_form_impl::~Main_form_impl()
{
	delete m_value;
// 	delete m_pVectors;		//矢量图
	delete g_ComtradeMgr;
	delete g_GraphMgr;
	delete m_status_bar;
	delete g_Propertys;
	delete m_pix_lock[0];
	delete m_pix_lock[1];
}

QDockWidget* Main_form_impl::create_dock_widget(const QString& str,QMainWindow* mainWin)
{
	QDockWidget* w = NULL;
	w = new QDockWidget(str,mainWin );
	w->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable|QDockWidget::NoDockWidgetFeatures);
	w->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    mainWin->addDockWidget(Qt::RightDockWidgetArea,w);
    w->setWindowTitle(str);
	w->hide();
	return w;
}

// 打开文件，并创建Comtrade对象
Comtrade* Main_form_impl::create_comtrade(const QString& fileName, bool bshow)
{
	QFileInfo fileInfo(fileName);
	if (!fileInfo.exists())
	{
		if (bshow)
		{
			QString str = QString(_C2Q("找不到%1")).arg(fileName);
// 			str.sprintf("找不到%s", fileName);
			QMessageBox::information(this, 
				_C2Q("提示"),	
				str);
		}
		return NULL;
	}

	Comtrade* pData = new Comtrade;
	File_operate fileOperator(pData);
	if (! fileOperator.load_file(fileName, 0))
	{
		delete pData;
		if (bshow)
		{
			QMessageBox::critical(NULL,
                tr("错误"),
                tr("打开文件失败!"));
		}
		return NULL;
	}
	return pData;
}
	
void Main_form_impl::new_comtrade_file(const QString& fileName)
{
	if (g_ComtradeMgr->Count() < 1)
	{
		open_comtrade_file(fileName);
		return;
	}

	Comtrade* pData = create_comtrade(fileName);
	if (pData != NULL)
	{
		g_ComtradeMgr->Add(pData);
		QFileInfo fileInfo(fileName);
		Wave_graph* pGraph = create_graph(fileInfo.fileName());
// 		QWidget* w = m_tabWave->widget(m_tabWave->count() - 1);
// 		m_tabWave->setTabLabel(w, fileInfo.fileName());
// 		m_tabWave->showPage(w);
		int tabcount = m_tab_wave->count()-1;
		m_tab_wave->setTabText(tabcount, fileInfo.fileName());
		m_tab_wave->setCurrentIndex(tabcount);
		Wave_caption* pCaption = g_GraphMgr->Caption(tabcount);

		QLinker::AddChannels(pData, pGraph);
		add_lines(pGraph, pData->get_fault_pos(),pCaption);
		slot_table_changed(NULL);
	}
}

void Main_form_impl::open_comtrade_file(const QString& fileName)
{
	m_dock_value->hide();
	m_dock_vector->hide();
	Comtrade* pData = create_comtrade(fileName);	
	int nPage = m_tab_wave->currentIndex();
	if (pData != NULL)
	{
		m_value->set_analyze(NULL);
// 		m_pVectors->SetAnalyze(NULL);
		g_ComtradeMgr->Delete(nPage);
		Wave_graph* pGraph = g_GraphMgr->Graph(nPage);
		pGraph->Clear();
		g_GraphMgr->Caption(nPage)->SetGraph(NULL);

		g_ComtradeMgr->Insert(nPage, pData);
		QLinker::AddChannels(pData, pGraph);
		g_GraphMgr->Caption(nPage)->SetGraph(pGraph);
		Wave_caption* pCaption = g_GraphMgr->Caption(nPage);
		QFileInfo fileInfo(fileName);
		m_tab_wave->setTabText(m_tab_wave->indexOf(m_tab_wave->currentWidget()), fileInfo.fileName());

		set_zoom_box();
		add_lines(pGraph, pData->get_fault_pos(),pCaption);
		update_analyze(pData);
		show_start_time();
		
		set_action_enable(true);
	}
}

Wave_graph* Main_form_impl::create_graph(const QString& caption)
{
	QSplitter* s = new QSplitter(Qt::Horizontal, m_tab_wave);

	QString pageCaption;
	if (caption.isEmpty())
		pageCaption = NEWFILENAME;
	m_tab_wave->addTab(s, pageCaption);
	
	Wave_caption* pCaption = new Wave_caption(s);
	pCaption->setMinimumWidth(80);
// 	pCaption->setMaximumWidth(130);
	
	Wave_graph* pGraph = new Wave_graph(s);
// 	s->setResizeMode(pGraph, QSplitter::Stretch);
	s->setStretchFactor(s->indexOf(pGraph), 1);
	connect(pGraph, SIGNAL(signal_wave_menu()), SLOT(slot_wave_menu()));
	connect(pGraph, SIGNAL(signal_zoom(double)), SLOT(slot_wave_zoom(double)));
	connect(pGraph, SIGNAL(signal_select(int, QColor*)), SLOT(slot_select(int, QColor*)));	
	
	connect(s,SIGNAL(splitterMoved(int,int)),this,SLOT(slot_resize_widget(int,int)));
	pCaption->SetGraph(pGraph);
	connect(pCaption, SIGNAL(CaptionMenu()), SLOT(slot_caption_menu()));
	g_GraphMgr->Add(pGraph, pCaption);
	
	return pGraph;
}

// 设置水平比例缩放下拉框
void Main_form_impl::set_zoom_box()
{
// 	EditZoom->setAutoCompletion(false);
// 	if (Wave() == NULL)
// 		return;
// 
// 	QStringList::iterator it;
// 	for (it = Wave()->ZoomList()->begin(); 
// 		it != Wave()->ZoomList()->end(); ++it)
// 		{
// 			EditZoom->addItem(*it);
// 		}
}

/*
	时间线初始位置:
	0-游标,故障点
	1-相对时标零点,故障点
	2-△t起点,0
*/
void Main_form_impl::add_lines(Wave_graph* graph, int pos,Wave_caption* pCaption)
{
	if (graph->cursor_count() > 0)
		return;

	int n = pos <= 0 ? pos : graph->data_count() / 2;
	for (int i = 0; i < 3; i++)
	{
		Time_line* pLine = graph->add_cursor(i == 0);
		pLine->set_color(TIMELINECOLORS[i]);
		pLine->set_hint(TIMELINECAPTION[i]);
		if (i == 0)
			pLine->set_tag(n + 1);
		else if ( i == 1)
			pLine->set_tag(n);
		else
			pLine->set_tag(10);
		graph->update_line_pos(pLine);
	}
	graph->set_double_click_line(graph->cursors(2));
	connect(graph->cursors(0), SIGNAL(OnCursor(int)), SLOT(slot_SG_time_line1(int)));
	connect(graph->cursors(0),SIGNAL(OnCursor(int)),pCaption,SLOT(slot_cursor_move(int)));
	connect(graph->cursors(1), SIGNAL(OnCursor(int)), SLOT(slot_SG_time_line2(int)));
	connect(graph->cursors(2), SIGNAL(OnCursor(int)), SLOT(slot_SG_time_line3(int)));
}

void Main_form_impl::close_comtrade_file(int idx, bool ask)
{
	if (g_ComtradeMgr->Count() < 1)
		return;

	if (ask)
	{
		QString str = QString(_C2Q("确定要关闭文件%1吗?")).arg(m_tab_wave->tabText(idx));
		if (QMessageBox::question(this, _C2Q("波形分析工具"), str, 
				QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
			return;
	}
	m_value->set_analyze(NULL);
// 	m_pVectors->SetAnalyze(NULL);
	g_ComtradeMgr->Delete(idx);
	if (m_tab_wave->count() > 1)
	{
		g_GraphMgr->Delete(idx);
		int n;
		if (idx == m_tab_wave->count() - 1)
			n = 0;
		else if (idx == 0)
			n = 2;
		else
			n = 1;
		m_tab_wave->removeTab(idx);
		switch (n)
		{
		case 0:
			m_tab_wave->setCurrentIndex(m_tab_wave->count() - 1);
			break;
		case 1:
			m_tab_wave->setCurrentIndex(idx);
			break;
		case 2:
			m_tab_wave->setCurrentIndex(0);
			break;
		default:
			break;
		}
		slot_table_changed(NULL);
	}
	else
	{
		g_GraphMgr->Graph(0)->Clear();
		g_GraphMgr->Caption(0)->repaint();
		m_tab_wave->setTabText(0 , NEWFILENAME);
		m_dock_value->hide();		// 数值窗口
		m_dock_vector->hide();		// 矢量窗口
		m_status_bar->Clear();		// 清除工具栏
		set_action_enable(false);
	}
}

// 游标
void Main_form_impl::slot_SG_time_line1(int position)
{
	double fTime = current_comtrade()->get_sample_time(position);
	char* str  ;
// 	sprintf(str,"%s%s",TIMESTR[0] , TIMELINEFORMAT);
	char message[256];
// 	sprintf(message,str, fTime, position + 1);		// 游标
	sprintf(message,"游标: %5.2fms, 点数: %d", fTime, position + 1);		// 游标
	m_status_bar->Message(1, _C2Q(message));

	if (! current_wave()->lock_line())
		update_time_delta(position, current_wave()->cursors(2)->tag());
	
	if (m_value != NULL && m_dock_value->isVisible())
		m_value->Fresh(position);
// 	if (m_pVectors != NULL && m_pDockVector->isVisible())
// 		m_pVectors->Fresh(position);
}

// 相对时标零点
void Main_form_impl::slot_SG_time_line2(int position)
{
}

// △t起点
void Main_form_impl::slot_SG_time_line3(int position)
{
	update_time_delta(current_wave()->cursors(0)->tag(), position);
}

void Main_form_impl::update_time_delta(int pos1, int pos2)
{
	double fTime = current_comtrade()->get_sample_time(pos1, pos2);
	char* str  ;
	char message[256];
	sprintf(message,"游标: %5.2fms, 点数: %d", fTime, pos1-pos2);		// 游标

	m_status_bar->Message(2, _C2Q(message));		// 游标与DeltaT之间的差值
}

void Main_form_impl::slot_wave_menu()
{
	if (g_ComtradeMgr->Count() > 0)
	{
		QMenu menu(this);	
		menu.addAction(AnaValueAction);	// 曲线信息
		menu.addSeparator();
		menu.addAction(ViewDraw2Action);	// 压缩/拉伸至一页
		menu.addAction(ViewDraw1Action);	// 按象素绘制
		menu.addSeparator();
		menu.addAction(ViewToHereAction);	// 计时起点至此
		menu.addAction(ViewAutoPlayAction);// 自动演示
		menu.exec(QCursor::pos());
	}
}

void Main_form_impl::slot_caption_menu()
{
	QMenu menu(this);	
	menu.addAction(CaptionPropertyAction);	
	menu.addSeparator();
	menu.addAction(CaptionAutoPositonAction);	
	menu.addAction(CaptionAutoScaleAction);	
	menu.addAction(CaptionSamePositonAction);	
	menu.addAction(CaptionSameScaleAction);	
	bool b = current_caption()->SelCount() > 1;
	CaptionSamePositonAction->setEnabled(b);	
	CaptionSameScaleAction->setEnabled(b && (! current_caption()->HasDigtal()));
	menu.addSeparator();
	menu.addAction(CaptionHarmAction);	
	CaptionHarmAction->setEnabled(false); //谐波矢量分析暂时不使用
	menu.addSeparator();
	menu.addAction(CaptionDeleteAction);	
	menu.exec(QCursor::pos());
}

void Main_form_impl::set_action_enable(bool enable)
{
    FileNewAction->setEnabled(enable);
    FileCloseAction->setEnabled(enable);
//    FilePrintAction->setEnabled(enable);
    FilePropertyAction->setEnabled(enable);
    FileExitAction->setEnabled(enable);

//    ViewFullScreenAction->setEnabled(enable);
    ViewShowAllAction->setEnabled(enable);
    ViewAutoSetAction->setEnabled(enable);
    ViewDraw1Action->setEnabled(enable);
    ViewDraw2Action->setEnabled(enable);
    ViewAutoPlayAction->setEnabled(enable);
    ViewToHereAction->setEnabled(enable);

    AnaValueAction->setEnabled(enable);
	//谐波向量暂时不处理、不显示
//     AnaHarmAction->setEnabled(enable);
//     AnaVectorAction->setEnabled(enable);
    AnaDeleteAction->setEnabled(enable);
    AnaHarmWaveAction->setEnabled(enable);

    SetZeroAction->setEnabled(enable);
    SetFaultAction->setEnabled(enable);
    SetPropertyAction->setEnabled(enable);
    SetOptionAction->setEnabled(enable);

    EditZoomYOutAction->setEnabled(enable);
    EditZoomYInAction->setEnabled(enable);
    EditLockAction->setEnabled(enable);
    EditEndAction->setEnabled(enable);
    EditPageUpAction->setEnabled(enable);
    EditPageDownAction->setEnabled(enable);
    EditMoveUpAction->setEnabled(enable);
    EditMoveDownAction->setEnabled(enable);
    EditShowAllAction->setEnabled(enable);
    EditHomeAction->setEnabled(enable);

	m_x_edit_zoom->setEnabled(enable);
}

void Main_form_impl::slot_file_open()
{
	QString appPath = QCoreApplication::applicationDirPath()+"/wave/" ;

	QString fileName = QFileDialog::getOpenFileName(
							this,
							tr("打开波形文件..."),
							appPath,
							"WaveFile (*.cfg *.dat)");

	if (!fileName.isEmpty())
		open_comtrade_file(fileName);
}

void Main_form_impl::slot_file_new()
{
	QString appPath = QCoreApplication::applicationDirPath()+"/wave/" ;
	QString fileName = QFileDialog::getOpenFileName(
							this,
							tr("打开波形文件..."),
							appPath,
							"WaveFile (*.cfg *.dat)");

	if (!fileName.isEmpty())
		new_comtrade_file(fileName);
}

void Main_form_impl::slot_file_close()
{
	close_comtrade_file(m_tab_wave->currentIndex());
}

void Main_form_impl::slot_file_print()
{
	File_operate op(current_comtrade());
	op.save_as_comtrade("test");
}

void Main_form_impl::slot_exit()
{
	qApp->quit();
}

void Main_form_impl::slot_file_view()
{
	QDlgFileViewImpl dlg(this);
	dlg.exec();
}

void Main_form_impl::slot_file_property()
{
	File_property_impl dlg(current_comtrade(), this);
	dlg.exec();
}

void Main_form_impl::slot_view_fullscreen()
{
}

void Main_form_impl::slot_view_auto_play()
{
	if (m_timer_ptr->isActive())
	{
		m_timer_ptr->stop();
		ViewAutoPlayAction->setText(AUTOSTRING[0]);
		QPixmap pixmap = QPixmap(":/res/images/pdr_nomal.png");
		ViewAutoPlayAction->setIcon(pixmap);
	}
	else
	{
		m_timer_ptr->start(m_auto_play_interval);
		ViewAutoPlayAction->setText(AUTOSTRING[1]);
		QPixmap pixmap = QPixmap(":/res/images/pdr_press.png");
		ViewAutoPlayAction->setIcon(pixmap);
	}
}

void Main_form_impl::view_toolbar(QToolBar* bar, bool b)
{
	if (b)
		bar->show();
	else
		bar->hide();
}

void Main_form_impl::slot_visible_changed1(bool visible)
{
	mnuMainBar->setChecked(visible);
}
void Main_form_impl::slot_visible_changed2(bool visible)
{
	mnuEditBar->setChecked(visible);
}
void Main_form_impl::slot_visible_changed3(bool visible)
{
	mnuAnaBar->setChecked(visible);
}

void Main_form_impl::slot_view_mainbar(bool isToggled)
{
	view_toolbar(MainToolbar, isToggled);
}

void Main_form_impl::slot_view_anabar(bool isToggled)
{
	view_toolbar(AnalyzeToolbar, isToggled);
}

void Main_form_impl::slot_view_editbar(bool isToggled)
{
	view_toolbar(EditToolbar, isToggled);
}

void Main_form_impl::slot_view_to_here()
{
	QPoint pt = current_wave()->mapFromGlobal(QCursor::pos());
	current_wave()->set_line_pos(current_wave()->cursors(2), pt.x());
}

void Main_form_impl::slot_analyze_value()
{
	m_value->set_analyze(current_analyze());
	if (m_dock_value->isVisible())
		m_dock_value->hide();
	else
		m_dock_value->show();
}

void Main_form_impl::view_vectors(int page)
{
// 	m_pVectors->ShowPage(page);
// 	m_pVectors->SetAnalyze(Analyze());
// 	if (m_pDockVector->isVisible())
// 		m_pDockVector->hide();
// 	else
// 		m_pDockVector->show();
}

void Main_form_impl::slot_analyze_harm()
{
	view_vectors(1);
}

void Main_form_impl::slot_analyze_vector()
{
	view_vectors(0);
}

void Main_form_impl::slot_analyze_delete()
{
}

void Main_form_impl::slot_analyze_harm_wave()
{
}

// 相对时标零点
void Main_form_impl::slot_set_zero()
{
	current_wave()->cursors(1)->set_tag(current_wave()->cursors(0)->tag());
	current_wave()->update_line_pos(current_wave()->cursors(1));
}

// 计时起点 
void Main_form_impl::slot_set_fault()
{
	current_wave()->cursors(2)->set_tag(current_wave()->cursors(0)->tag());
	current_wave()->update_line_pos(current_wave()->cursors(2));
}

void Main_form_impl::slot_set_property()
{
	Wave_property_impl dlg(this);
	Wave_graph* w = current_wave();
	Wave_caption* c = current_caption();
	dlg.color_set(0, c->Color());
	dlg.color_set(1, w->get_color());
	dlg.color_set(2, w->cursors(0)->color());
	dlg.color_set(3, w->cursors(1)->color());
	dlg.color_set(4, w->cursors(2)->color());
	dlg.set_checked(0, w->cursors(0)->visible());
	dlg.set_checked(1, w->cursors(1)->visible());
	dlg.set_checked(2, w->cursors(2)->visible());
	dlg.m_edtHorzZoom->setText(QString::number(w->zoom_Xstep()));
	dlg.m_edtLineWidth->setValue(w->line_width());
	dlg.m_chkAutoSet->setChecked(w->auto_set());
	int n = (int)w->digtal_kind();
	dlg.m_cbbDigtalKind->setCurrentIndex(n);

	if (dlg.exec() == QDialog::Accepted)
	{
		c->SetColor(dlg.Colors(0));

		w->lock();

		w->set_color(dlg.Colors(1));
		w->cursors(0)->set_color(dlg.Colors(2));
		w->cursors(1)->set_color(dlg.Colors(3));
		w->cursors(2)->set_color(dlg.Colors(4));
		w->cursors(0)->set_visible(dlg.Checked(0));
		w->cursors(1)->set_visible(dlg.Checked(1));
		w->cursors(2)->set_visible(dlg.Checked(2));
		w->set_zoom_Xstep(dlg.m_edtHorzZoom->text().toFloat());
		w->set_line_width(dlg.m_edtLineWidth->value());
		w->set_auto_set(dlg.m_chkAutoSet->isChecked());
		w->set_digtal_kind((Digtal_Kind)dlg.m_cbbDigtalKind->currentIndex());

		w->unlock();
	}
}

void Main_form_impl::slot_set_options()
{
	if (g_Propertys->Config())
	{
		int n = g_Propertys->AutoPlayTime();
		if (n < 0 || n > 4)
			n = 1;
		m_auto_play_interval = AUTOPLAYTIME[n] * 1000;	 
		show_math_type();
	}
}

void Main_form_impl::slot_help_about()
{
    QMessageBox::about(this, tr("故障录波分析工具"),
			tr("提供波形浏览、数据分析、ftp远程下载录波文件等功能."));
}

void Main_form_impl::slot_down_load_file()
{
	Ftp_window ftpwindow;
	ftpwindow.set_server_name(tr("172.27.27.229"));
	ftpwindow.set_server_port("21");
	ftpwindow.set_user_name("daqo");
	ftpwindow.set_password("");
	ftpwindow.exec();
}

void Main_form_impl::slot_view_show_all()	{ current_wave()->show_all(true); }
void Main_form_impl::slot_view_auto_set()	{ current_wave()->auto_set_rect(); }
void Main_form_impl::slot_view_draw1()		{ current_wave()->expand(); }
void Main_form_impl::slot_view_draw2()		{ current_wave()->fill_in(); }
void Main_form_impl::slot_edit_zoom_YIn()	{ current_wave()->set_show_count(current_wave()->show_count() - 1); }
void Main_form_impl::slot_edit_zoom_Yout()	{ current_wave()->set_show_count(current_wave()->show_count() + 1); }
void Main_form_impl::slot_edit_home()		{ current_wave()->cursor_home(); }
void Main_form_impl::slot_edit_end()		{ current_wave()->cursor_end(); }
void Main_form_impl::slot_edit_move_up()	{ current_wave()->cursor_left();}
void Main_form_impl::slot_edit_move_down()	{ current_wave()->cursor_right(); }
void Main_form_impl::slot_edit_page_up()	{ current_wave()->page_up(); }
void Main_form_impl::slot_edit_page_down()	{ current_wave()->page_down(); }
void Main_form_impl::slot_edit_show_all()	{ current_wave()->set_show_count(current_wave()->analog_count() + 1); }
void Main_form_impl::slot_edit_lock()		
{ 
	current_wave()->lock_lines(); 
	update_lockline(current_wave());
}

void Main_form_impl::update_lockline(Wave_graph* graph)
{
	if (graph == NULL)
		return;
	int n = current_wave()->lock_line() ? 1 : 0;
	EditLockAction->setIcon(*m_pix_lock[n]);		
}

void Main_form_impl::slot_caption_delete()
{
	int n = selected_channel();
	if (n == -1)
		return;
	
	if (QMessageBox::question(this, tr("波形分析工具"), tr("确定要删除本曲线吗?"), 
		QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
			return;
	
	current_wave()->delete_channel(n);
	current_comtrade()->Delete(n);
	update_analyze(current_comtrade());
}

// 更新Data关联的Analyze
void Main_form_impl::update_analyze(Comtrade* c)
{
	QAnalyze* fun = g_ComtradeMgr->Analyze(g_ComtradeMgr->IndexOf(c));
	fun->SetComtrade(NULL);
	fun->SetComtrade(c);

	m_value->set_analyze(NULL);
	m_value->set_analyze(fun);
// 	m_pVectors->SetAnalyze(NULL);
// 	m_pVectors->SetAnalyze(fun);
}

// 自动位置
void Main_form_impl::slot_caption_auto_position()
{
	current_caption()->BeAutoPosition();
}

// 自动比例
void Main_form_impl::slot_caption_auto_scale()
{
	current_caption()->BeAutoSize();
}

void Main_form_impl::slot_caption_harm()
{
	QAnalyze* fun = current_analyze();
	View_data* view = current_comtrade()->analog(selected_channel());
	fun->SetHarmChannel(view);
// 	m_pVectors->SetAnalyze(NULL);
// 	m_pVectors->SetAnalyze(fun);
// 	m_pVectors->ShowPage(1);
	m_dock_vector->show();
}

void Main_form_impl::slot_caption_harm_wave()
{

}

void Main_form_impl::slot_caption_property()
{
	Channel_property_impl	dlg(current_wave(), current_comtrade(), this);
	dlg.select_item(selected_channel());
	dlg.exec();
}

// 处于同一位置
void Main_form_impl::slot_caption_same_position()
{
	current_caption()->BeSamePosition();
}

// 处于同一比例
void Main_form_impl::slot_caption_same_scale()
{
	current_caption()->BeSameScale();
}

int Main_form_impl::selected_channel()
{
	int channelIndex = -1;
	Draw_channel* channel = current_caption()->Channel();
	if (channel != NULL)
	{
		channelIndex = current_wave()->indexOf(channel);
	}
	return channelIndex;
}

Wave_graph* Main_form_impl::current_wave()
{
	return g_GraphMgr->Graph(m_tab_wave->currentIndex());
}

Wave_caption* Main_form_impl::current_caption()
{
	return g_GraphMgr->Caption(m_tab_wave->currentIndex());
}

Gain_data* Main_form_impl::Data()
{
	return (Gain_data*)current_comtrade();
}

QAnalyze* Main_form_impl::current_analyze()
{
	return g_ComtradeMgr->Analyze(m_tab_wave->currentIndex());
}

Comtrade* Main_form_impl::current_comtrade()
{
	return g_ComtradeMgr->comtrade(m_tab_wave->currentIndex());
}

// 执行缩放操作，并切换焦点到波形图控件
void Main_form_impl::slot_zoom_actived(int index)
{
// 	QString strZoom = EditZoom->itemText(index);
// 	strZoom = strZoom.left(strZoom.length() - 1);
// 	Wave()->SetZoomXData(strZoom.toFloat() / 100);
	current_wave()->set_zoom_Xdata((double)index / 100);

//  Windows.SetFocus(CurrentWave.Handle);
}
	
void Main_form_impl::slot_wave_zoom(double f)
{
// 	QString strZoom;
// 	strZoom.sprintf("%d%%", (int)(f * 100));
// 	EditZoom->setEditText(strZoom);
	m_x_edit_zoom->setValue(f*100);
}

void Main_form_impl::slot_time_out()
{
	int n = current_wave()->cursors(0)->tag();
	current_wave()->cursor_right();
	if (n == current_wave()->cursors(0)->tag())
		slot_view_auto_play();
}

void Main_form_impl::slot_table_changed(QWidget* w)
{
	m_timer_ptr->stop();
	update_lockline(current_wave());

	// 切换分析模块
	QAnalyze* fun = current_analyze();
	if (m_value->isVisible())
		m_value->set_analyze(fun);
// 	if (m_pVectors->isVisible())
// 		m_pVectors->SetAnalyze(fun);
}

void Main_form_impl::slot_item_close(int index)
{
	close_comtrade_file(index);
}

void Main_form_impl::slot_item_property(int index)
{
	Comtrade* p = g_ComtradeMgr->comtrade(index);
	if (p != NULL)
	{
		File_property_impl dlg(p, this);
		dlg.exec();
	}
}

void Main_form_impl::show_math_type()
{
	QString str(MATHSTR);	
	str += ARITHMETICSTRING[g_Propertys->Filter()];
	m_status_bar->Message(0, str);
}

void Main_form_impl::show_start_time()
{
	m_status_bar->Message(3, Data()->start_time());
}

// 通道被选择事件
void Main_form_impl::slot_select(int count, QColor* arr)
{
	if (m_value == NULL)
		return;

	m_value->update_grid_color(arr);
}

void Main_form_impl::slot_value_dock(bool visible)
{
	if (m_value->Analyze() == NULL && visible)
		m_value->set_analyze(current_analyze());
}

void Main_form_impl::slot_vector_dock(bool visible)
{
// 	if (m_pVectors->Analyze() == NULL && visible)
// 		m_pVectors->SetAnalyze(Analyze());
}

void Main_form_impl::resizeEvent( QResizeEvent * event )
{
	current_wave()->realloc_rect();
}

void Main_form_impl::slot_resize_widget( int,int )
{
	current_wave()->realloc_rect();
}
