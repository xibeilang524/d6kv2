#include <qpushbutton.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qradiobutton.h>
#include <qbuttongroup.h> 
#include "optionsimpl.h"

Dlg_option_impl::Dlg_option_impl( QWidget* parent, Qt::WindowFlags f )
	: QDialog(parent, f)
{
	setupUi(this) ;

	for (int i = 0; i < 4; i++)
	{
		QString str;
		str.sprintf("%3.1fs", AUTOPLAYTIME[i]);
		m_cbbAutoPlay->addItem(str);
	}
	connect(m_btnOk, SIGNAL(clicked()), SLOT(accept()));
	connect(m_btnCancel, SIGNAL(clicked()), SLOT(reject()));
	connect(m_btnHarm2, SIGNAL(toggled(bool)), SLOT(slot_harm_button_click(bool)));		
	setStyleSheet("QDialog{background:white;}"	
						"QLabel{font-family: Microsoft YaHei;font-size: 12px;color: #0F1923;}"
						"QLineEdit {font-family:Microsoft YaHei;font-size: 12px;color: #4A4A4A;background: #FFFFFF;\
						border: 1px solid #C6CFD9;border-radius: 2px;min-height:22px;}"		
						"QGroupBox{font-family: Microsoft YaHei;font-size: 13px;color: #0F1923;}"
						"QComboBox {font-family: Microsoft YaHei;font-size: 12px;color: #3E464E;\
						min-height:23px;border: 1px solid #C6CFD9;border-radius: 2px;}"
						"QPushButton{font-family: Microsoft YaHei;font-size: 12px;color: white;\
						background:#4A90E2;border-radius: 2px;min-width:60px;min-height:23px;}"
						"QPushButton:hover{background:#0C2543;}"
						"QPushButton:disabled{background:#D5D5D5;}"
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
				  "QRadioButton::indicator:unchecked{image:url(:/res/images/radio.png);}"
				  "QRadioButton::indicator:checked{image:url(:/res/images/radio_checked.png);}"
				  );
}

Dlg_option_impl::~Dlg_option_impl()
{
}
	
void Dlg_option_impl::slot_harm_button_click(bool Checked)
{
	m_edtHarm->setEnabled(Checked);
}

int Dlg_option_impl::filter()
{
	int n = 0;

	if (m_btnMath1->isChecked())
	{
		n=0;
	}
	else if (m_btnMath2->isChecked())
	{
		n=1;
	}
	else if (m_btnMath3->isChecked())
	{
		n=2;
	}
	else if (m_btnMath4->isChecked())
	{
		n=3;
	}
	return n;
}

void Dlg_option_impl::set_filter(int n)
{
	if (n == 0)
		m_btnMath1->setChecked(true);
	else if (n == 1)
		m_btnMath2->setChecked(true);
	else if (n == 2)
		m_btnMath3->setChecked(true);
	else if (n == 3)
		m_btnMath4->setChecked(true);
}


/* QPropertys */

Propertys* g_Propertys = NULL;

Propertys::Propertys()
{
	m_nFilter = 0;
	m_nDiffValue = 2;
	m_bFixedHarm = false;
	m_nFixedHarmValue = 11;
	m_nFloatFormatValue = 2;
	m_nAutoPlayTime = 1;
}

Propertys::~Propertys()
{
}

bool Propertys::Config()
{
	bool b = false;
	Dlg_option_impl dlg;
	dlg.set_filter(m_nFilter);
	dlg.m_cbbAutoPlay->setCurrentIndex(m_nAutoPlayTime);
	if (dlg.exec() == QDialog::Accepted)
	{
		b = true;
		m_nFilter = dlg.filter();
		m_nAutoPlayTime = dlg.m_cbbAutoPlay->currentIndex();
	}
	return b;
}