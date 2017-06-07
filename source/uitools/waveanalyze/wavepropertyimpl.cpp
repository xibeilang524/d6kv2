#include <QColorDialog>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QKeyEvent>

#include "wavepix.h"
// #include "constutils.h"
#include "wavepropertyimpl.h"
#include "globaldef.h"

Wave_property_impl::Wave_property_impl( QWidget* parent, Qt::WindowFlags f )
	: QDialog(parent, f)
{
	setupUi(this);

	init_controls();	

	this->setStyleSheet(tr("QDialog{background:white;}"	
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
		"QListView,QTreeView{font-family: Microsoft YaHei;font-size: 13px;color:#4A4A4A;border: 1px solid #E5E5E5;}"
		"QListView::item:hover,QTreeView::item:hover{background-color: wheat;	}"
		"QListView::item:selected,QTreeView::item:selected	{	background-color: lightblue;	}"
		"QListView::indicator:unchecked{image:url(:/res/images/checkbox_unchecked.png);}"
		"QListView::indicator:checked{image:url(:/res/images/checkbox_checked.png);}"));
}

Wave_property_impl::~Wave_property_impl()
{
}

void Wave_property_impl::init_controls()
{
	m_cbbCusorStep->setEnabled(false);

// 	m_lvOptions->addColumn("", 150);
// 	m_lvOptions->setColumnWidthMode(0, QListView::Maximum);
// 	m_lvOptions->header()->hide();
// 	m_lvOptions->setSortColumn(-1);

	QListWidgetItem* item = NULL;
	for (int i = 0; i < 3; i++)
	{
		item = new QListWidgetItem(TIMELINECAPTION[i],m_lvOptions);
		item->setCheckState(Qt::Unchecked);
	}

	QPixmap pixmap = QPixmap(":/res/images/clearrect.png");
	m_cbbDigtalKind->addItem(pixmap, tr("空矩形"));
	pixmap = QPixmap(":/res/images/solidrect.png");
	m_cbbDigtalKind->addItem(pixmap, tr("实矩形"));
	m_edtHorzZoom->installEventFilter(this);

	connect(m_btnOk, SIGNAL(clicked()), SLOT(accept()));
	connect(m_btnCancel, SIGNAL(clicked()), SLOT(reject()));
	connect(m_btnNameColor, SIGNAL(clicked()), SLOT(slot_set_color1()));
	connect(m_btnBackColor, SIGNAL(clicked()), SLOT(slot_set_color2()));
	connect(m_btnCusorColor, SIGNAL(clicked()), SLOT(slot_set_color3()));
	connect(m_btnFaultColor, SIGNAL(clicked()), SLOT(slot_set_color4()));
	connect(m_btnZeroColor, SIGNAL(clicked()), SLOT(slot_set_color5()));
}

bool Wave_property_impl::event_filter(QObject* watched, QEvent* e)
{
	if (e->type() == QEvent::KeyPress)
	{
		if (watched == m_edtHorzZoom)
		{
			bool ok(true);
			int key = ((QKeyEvent*)e)->key();
			if ((key >= Qt::Key_0 && key <= Qt::Key_9) ||		// 0..9
				 key == Qt::Key_Backspace ||				
				 key == Qt::Key_Delete || 
				 key == Qt::Key_Period )						// '.'，小数点
				ok = false;
			return ok;
		}
	}

	return QDialog::eventFilter(watched, e);
}

void Wave_property_impl::accept()
{	
	double f = m_edtHorzZoom->text().toFloat();
	if (f <= 1 || f > 10)
	{
		QMessageBox::warning( NULL, QObject::tr("警告"),
			QObject::tr("横向比例微调系数应在1.0~10.0之间!"), QObject::tr("确定"));
		return;
	}
		
	QDialog::accept();
}

QPushButton* Wave_property_impl::get_button(int n)
{
	QPushButton* btn = NULL;
	if (n == 0)
		btn = m_btnNameColor;
	else if (n == 1)
		btn = m_btnBackColor;
	else if (n == 2)
		btn = m_btnCusorColor;
	else if (n == 3)
		btn = m_btnFaultColor;
	else if (n == 4)
		btn = m_btnZeroColor;
	return btn;
}

QColor Wave_property_impl::Colors(int n)
{
// 	return GetButton(n)->paletteBackgroundColor();
	QPalette pal = get_button(n)->palette();
	QColor clr = pal.color(get_button(n)->backgroundRole());
	return clr;
}

void Wave_property_impl::color_set(int n, const QColor& clr)
{
// 	QPalette palette;
// 	palette.setColor(GetButton(n)->backgroundRole(),clr);
// 	GetButton(n)->setPalette(palette);
	get_button(n)->setStyleSheet(QString("background-color:%1").arg(clr.name()));
}

void Wave_property_impl::set_colors(int n)
{
	QColor clr = Colors(n);
	clr = QColorDialog::getColor(clr, this, "hey!");
	if (clr.isValid())
	{
		color_set(n, clr);
	}
}

void Wave_property_impl::slot_set_color1() { set_colors(0); }
void Wave_property_impl::slot_set_color2() { set_colors(1); }
void Wave_property_impl::slot_set_color3() { set_colors(2); }
void Wave_property_impl::slot_set_color4() { set_colors(3); }
void Wave_property_impl::slot_set_color5() { set_colors(4); }

QListWidgetItem* Wave_property_impl::get_item(int n)
{
	QListWidgetItem* item = NULL;
	item = m_lvOptions->item(n);
	return item;
}

bool Wave_property_impl::Checked(int n)
{
	QListWidgetItem* item = get_item(n);
	if (item == NULL)
		return false;
	if (item->checkState() == Qt::Checked)
	{
		return true;
	}
	return false;
}

void Wave_property_impl::set_checked(int n, bool check)
{
	QListWidgetItem* item = get_item(n);
	if (item == NULL)
	{
		return;
	}
	if (check)
	{
		item->setCheckState(Qt::Checked);
	}
	else
	{
		item->setCheckState(Qt::Unchecked);
	}
}

QLineEdit* Wave_property_impl::get_edit(int n)
{
	QLineEdit* edt = NULL;
	if (n == 0)
		edt = m_edtHorzZoom;
	else if (n == 1)
		edt = (QLineEdit*)m_edtLineWidth;
	return edt;
}

void Wave_property_impl::set_edit_data(int n, double val)
{
/*
	QString str;
	if (n == 0)
		str = QString::number(val);
	else if (n == 1)
		str = QString::number((int)val);
	GetEdit(n)->setText(str);
*/
}

double Wave_property_impl::edit_data(int n)
{
	double f = 0.0;
/*
	QLineEdit* edt = GetEdit(n);
	if (edt != NULL)
		f = edt->text().toFloat();
*/
	return f;
}
