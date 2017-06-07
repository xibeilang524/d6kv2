#include <qsplitter.h>
#include <qtabwidget.h>
#include <qcursor.h>
#include <qcolordialog.h>

#include "constutils.h"
#include "funutils.h"
#include "vectorsimpl.h"

QDlgVectorImpl::QDlgVectorImpl(QWidget* parent, Qt::WindowFlags f)
	: QWidget(parent, f)
{
	Init();
}

QDlgVectorImpl::~QDlgVectorImpl()
{
	delete m_Vector;
	delete m_VectorLabel;
	delete m_Harm;
	delete m_HarmLabel;
}

void QDlgVectorImpl::Init()
{
	m_Analyze = NULL;
	m_clrDirect = Qt::gray;
	m_clrOdd = Qt::cyan;
	m_clrEven = Qt::yellow;
	m_tabVector->removeTab(0);
	QSplitter* s1 = new QSplitter(QSplitter::Horizontal, m_tabVector);
	m_tabVector->addTab(s1, tr("矢量图"));
	m_Vector = new QVectorGraph(s1);
	m_Vector->setMinimumSize(QSize(100, 100));
	m_VectorLabel = new QLabelGraph(s1);
	connect(m_Vector, SIGNAL(VectorMenu()), SLOT(VectorMenu()));

	QSplitter* s2 = new QSplitter(QSplitter::Horizontal, m_tabVector);
	m_tabVector->addTab(s2, tr("谐波棒图"));
	m_Harm = new QHarmGraph(s2);
	m_HarmLabel = new QLabelGraph(s2);
	connect(m_Harm, SIGNAL(HarmMenu()), SLOT(HarmMenu()));
}

void QDlgVectorImpl::VectorMenu()
{
	QMenu menu(this);

	// 颜色
	QMenu* colorMenu = new QMenu(this);
	menu.insertItem(tr("颜色设置"), colorMenu, 1);
	int COLOR_ITEM[3];
	COLOR_ITEM[0] = colorMenu->insertItem(tr("背景颜色"));
	COLOR_ITEM[1] = colorMenu->insertItem(tr("刻度颜色"));
	COLOR_ITEM[2] = colorMenu->insertItem(tr("角度颜色"));
	menu.insertSeparator();

	int checkIndex = -1;

	// 刻度
	QMenu* scaleMenu = new QMenu(this);
	menu.insertItem(tr("刻度分格"), scaleMenu, 2);
	int SCALE_ITEM[5];
	SCALE_ITEM[4] = scaleMenu->insertItem(tr("5格"));
	SCALE_ITEM[3] = scaleMenu->insertItem(tr("4格"));
	SCALE_ITEM[2] = scaleMenu->insertItem(tr("3格"));
	SCALE_ITEM[1] = scaleMenu->insertItem(tr("2格"));
	SCALE_ITEM[0] = scaleMenu->insertItem(tr("无"));
	checkIndex = scaleMenu->count() - m_Vector->ScaleCount();
	scaleMenu->setItemChecked(scaleMenu->idAt(checkIndex), true);

	// 角度
	QMenu* angleMenu = new QMenu(this);
	menu.insertItem(tr("角度分格"), angleMenu, 3);
	int ANGLE_ITEM[3];
	ANGLE_ITEM[2] = angleMenu->insertItem(tr("45度"));
	ANGLE_ITEM[1] = angleMenu->insertItem(tr("30度"));
	ANGLE_ITEM[0] = angleMenu->insertItem(tr("无"));
	checkIndex = angleMenu->count() - m_Vector->AngleCount() - 1;
	angleMenu->setItemChecked(angleMenu->idAt(checkIndex), true);
	menu.insertSeparator();

	int NAME_ITEM = menu.insertItem(tr("名称标注"));
	if (m_Vector->IsShowIndex())
		menu.setItemChecked(NAME_ITEM, true);
	menu.insertSeparator();

	// 基准
	QMenu* baseMenu = new QMenu(this);
	menu.insertItem(tr("基准矢量"), baseMenu, 4);
	if (m_Vector->Count() > 0)
	{
		for (int i = 0; i < m_Vector->Count(); i++)
			baseMenu->insertItem(m_Vector->Items()->Item(i)->Caption());
		baseMenu->insertItem(NOVECTORSTR);
		int nCheck = m_Vector->ItemIndex();
		if (nCheck == -1)
			nCheck = baseMenu->count() - 1;
		baseMenu->setItemChecked(baseMenu->idAt(nCheck), true);
	}
	menu.insertSeparator();
	int ADD_ITEM = menu.insertItem(tr("添加矢量")); 
	
	int id = menu.exec(QCursor::pos());
	if (id == COLOR_ITEM[0])
		VectorColorSet(0);
	else if (id == COLOR_ITEM[1])
		VectorColorSet(1);
	else if (id == COLOR_ITEM[2])
		VectorColorSet(2);
	else if (id == SCALE_ITEM[0])
		VectorScaleSet(1);
	else if (id == SCALE_ITEM[1])
		VectorScaleSet(2);
	else if (id == SCALE_ITEM[2])
		VectorScaleSet(3);
	else if (id == SCALE_ITEM[3])
		VectorScaleSet(4);
	else if (id == SCALE_ITEM[4])
		VectorScaleSet(5);
	else if (id == ANGLE_ITEM[0])
		VectorAngleSet(0);
	else if (id == ANGLE_ITEM[1])
		VectorAngleSet(1);
	else if (id == ANGLE_ITEM[2])
		VectorAngleSet(2);
	else if (id == NAME_ITEM)
		VectorNameSet();
	else if (id == ADD_ITEM)
		VectorAdd();
	else		// 基准矢量
		SetBaseVector(baseMenu->indexOf(id));		
}

void QDlgVectorImpl::VectorNameSet()
{
	m_Vector->SetShowIndex(! m_Vector->IsShowIndex());
}

void QDlgVectorImpl::VectorColorSet(int idx)
{
	QColor clr;
	if (idx == 0)
		clr = m_Vector->Color();
	else if (idx == 1)
		clr = m_Vector->ScaleColor();
	else if (idx == 2)
		clr = m_Vector->AngleColor();

	clr = QColorDialog::getColor(clr, this, "hey!");
	if (clr.isValid())
	{
		if (idx == 0)
			m_Vector->SetColor(clr);
		else if (idx == 1)
			m_Vector->SetScaleColor(clr);
		else if (idx == 2)
			m_Vector->SetAngleColor(clr);
	}
}

void QDlgVectorImpl::VectorScaleSet(int idx)
{
	if (idx < 0 || idx > 6)
		return;

	m_Vector->SetScaleCount(idx);
}

void QDlgVectorImpl::VectorAngleSet(int idx)
{
	if (idx < 0 || idx > 3)
		return;
	m_Vector->SetAngleCount(idx);
}

// 添加矢量
void QDlgVectorImpl::VectorAdd()
{
	if (m_Analyze->AddVector())
	{
		UpdateVector();
		FreshVector1(m_Analyze->CursorPos());
	}
}

void QDlgVectorImpl::HarmMenu()
{
	QMenu menu(this);

	// 颜色
	QMenu* colorMenu = new QMenu(this);
	menu.insertItem(tr("颜色设置"), colorMenu, 1);
	int COLOR_ITEM[4];
	COLOR_ITEM[0] = colorMenu->insertItem(tr("背景颜色"));
	COLOR_ITEM[1] = colorMenu->insertItem(tr("直流颜色"));
	COLOR_ITEM[2] = colorMenu->insertItem(tr("奇次谐波"));
	COLOR_ITEM[3] = colorMenu->insertItem(tr("偶次谐波"));
	menu.insertSeparator();

	// 显示
	QMenu* styleMenu = new QMenu(this);
	menu.insertItem(tr("显示模式"), styleMenu, 2);
	int STYLE_ITEM[2];
	STYLE_ITEM[0] = styleMenu->insertItem(tr("水平棒图"));
	STYLE_ITEM[1] = styleMenu->insertItem(tr("垂直棒图"));
	styleMenu->setItemChecked(STYLE_ITEM[m_Analyze->HarmStyle()], true);

	// 数值
	QMenu* valueMenu = new QMenu(this);
	menu.insertItem(tr("谐波数值"), valueMenu, 3);
	int VALUE_ITEM[3];
	VALUE_ITEM[0] = valueMenu->insertItem(tr("数值"));
	VALUE_ITEM[1] = valueMenu->insertItem(tr("百分比"));
	VALUE_ITEM[2] = valueMenu->insertItem(tr("数值+百分比"));
	valueMenu->setItemChecked(VALUE_ITEM[m_Analyze->HarmValueIndex()], true);
	int MORE_ITEM = menu.insertItem(tr("更多次谐波..."));
	menu.setItemEnabled(MORE_ITEM, m_Analyze->HarmChannel() != NULL);

	menu.insertSeparator();
	int ADD_ITEM = menu.insertItem(tr("添加谐波通道")); 
	
	int id = menu.exec(QCursor::pos());
	if (id == COLOR_ITEM[0])
		HarmColorSet(0);
	else if (id == COLOR_ITEM[1])
		HarmColorSet(1);
	else if (id == COLOR_ITEM[2])
		HarmColorSet(2);
	else if (id == COLOR_ITEM[3])
		HarmColorSet(3);
	else if (id == STYLE_ITEM[0])
		HarmStyleSet(0);
	else if (id == STYLE_ITEM[1])
		HarmStyleSet(1);
	else if (id == VALUE_ITEM[0])
		HarmValueSet(0);
	else if (id == VALUE_ITEM[1])
		HarmValueSet(1);
	else if (id == VALUE_ITEM[2])
		HarmValueSet(2);
	else if (id == MORE_ITEM)
		HarmMore();
	else if (id == ADD_ITEM)
		HarmAdd();
}

void QDlgVectorImpl::HarmColorSet(int idx)
{
	QColor clr;
	if (idx == 0)
		clr = m_Harm->Color();
	else if (idx == 1)
		clr = m_clrDirect;
	else if (idx == 2)
		clr = m_clrOdd;
	else if (idx == 3)
		clr = m_clrEven;
	clr = QColorDialog::getColor(clr, this, "hey!");
	if (clr.isValid())
	{
		if (idx == 0)
			m_Harm->SetColor(clr);
		else if (idx == 1)
			m_clrDirect = clr;
		else if (idx == 2)
			m_clrOdd = clr;
		else if (idx == 3)
			m_clrEven = clr;
		if (idx == 0)
			m_HarmLabel->SetBkColor(clr);
		else
		{
			UpdateHarm();
			FreshHarm(m_Analyze->CursorPos());
		}
		m_Harm->Change();
		m_HarmLabel->Change();
	}
}

void QDlgVectorImpl::HarmStyleSet(int idx)
{
	m_Harm->SetHarmStyle((HARMSTYLE)idx);
	m_Analyze->SetHarmStyle(idx);
}

void QDlgVectorImpl::HarmValueSet(int idx)
{  
	m_nHarmValue = idx;
	m_Analyze->SetHarmValueIndex(idx);
	FreshHarm(m_Analyze->CursorPos());
}

// 显示更多次谐波
void QDlgVectorImpl::HarmMore()
{
	QDlgDetailsImpl dlg(m_Analyze->Details(2), this);
	dlg.UnEnableMoveButton();
	if (dlg.exec() == QDialog::Accepted)
	{
		UpdateHarm();
		FreshHarm(m_Analyze->CursorPos());
	}
}

void QDlgVectorImpl::HarmAdd()
{
	if (m_Analyze->AddHarm())
	{
		UpdateHarm();
		FreshHarm(m_Analyze->CursorPos());
	}
}

void QDlgVectorImpl::SetBaseVector(int index)
{
	int nBase = -1;
	if (index != m_Vector->Count())
		nBase = index;

	m_Vector->SetItemIndex(nBase);
	m_Analyze->SetVectorItemIndex(nBase);
	FreshVector1(m_Analyze->CursorPos());
}

void QDlgVectorImpl::UpdateVector()
{
	if (m_Analyze == NULL)
		return;
	
	ClearVector();
	int n = 0;
	for (int i = 0; i < m_Analyze->AnaChannels()->Count(); i++)
	{
		if (! m_Analyze->AnaChannels()->Checked(i))
			continue;
		IViewData* view = m_Analyze->AnaChannels()->Channel(i);
		AddVector(view->ChannelName(), view->GetColor());
		n++;
	}
	FreshVector1(m_Analyze->CursorPos());
}

void QDlgVectorImpl::FreshVector1(int pos)
{
	Complex base = m_Analyze->VectorValue(m_Vector->ItemIndex(), pos);
	for (int i = 0; i < m_Vector->Count(); i++)
	{
		Complex c = m_Analyze->VectorValue(i, pos);
		c.Im -= base.Im;
		FreshVector(i, c, m_Vector->Items()->Item(i)->Caption());
	}
}

void QDlgVectorImpl::AddHarmItems(QVectorMgr* Items, bool label)
{
	for (int i = 0; i < m_Analyze->Details(2)->Count(); i++)
	{
		QDetail* det = m_Analyze->Details(2)->Item(i);
		if (! det->Check())
			continue;
		QColor clr;
		if (det->Data() == 0)
			clr = m_clrDirect;
		else if (det->Data() % 2 == 0)
			clr = m_clrEven;
		else
			clr = m_clrOdd;

		QString str;
		if (label)
		{
			str = det->Caption();
		}
		else
		{
			if (i == 0)
				str = "直";
			else if (i == 1)
				str = "基";
			else
				str = str.number(i);
		}
		Items->Add(clr, str);
	}
}

void QDlgVectorImpl::UpdateHarm()
{
	m_Harm->Clear();
	m_HarmLabel->Clear();
	if (m_Analyze->HarmChannel() == NULL)
		return;
	IViewData* view = m_Analyze->HarmChannel();
	m_Harm->SetCaption(view->ChannelName());
	m_Harm->SetCaptionColor(view->GetColor());
	AddHarmItems(m_Harm->Items(), false);
	AddHarmItems(m_HarmLabel->Items(), true);
	m_Harm->SetHarmStyle((HARMSTYLE)m_Analyze->HarmStyle());
	m_nHarmValue = m_Analyze->HarmValueIndex();
}

int QDlgVectorImpl::GetHarmIndex(int idx)
{
	int col = 0;
	int index = 1;
	for (int i = 0; i < m_Analyze->Details(2)->Count(); i++)
	{
		if (! m_Analyze->Details(2)->Item(i)->Check())
			continue;
		if (col == idx)
		{
			index = i;
			break;
		}
		col++;
	}
	return index;
}

void QDlgVectorImpl::FreshHarm(int pos)
{
	if (m_Harm->Count() < 1)
		return;
	int i;
	double baseValue = 0;
	for (i = 0; i < m_Analyze->Details(2)->Count(); i++)
	{
		baseValue += fabs(m_Analyze->HarmValue(i, pos));
	}
	double* args = new double[m_Harm->Count()];
	for (i = 0; i < m_Harm->Count(); i++)	// 谐波棒图
	{
		args[i] = m_Analyze->HarmValue(GetHarmIndex(i), pos);
		m_Harm->Items()->Item(i)->SetRe(args[i]);
	}
	m_Harm->SetBaseValue(baseValue);
	m_Harm->Change();
	
	for (i = 0; i < m_Harm->Count(); i++)	// 谐波数值
	{
		QVectorItem* item = m_Harm->Items()->Item(i);
		QString str = "";
		double val1 = item->Re();
		int val2 = (int)(item->Re() / baseValue * 100);
		switch (m_nHarmValue)
		{
		case 0:
			str.sprintf("%5.3f", val1);
			break;
		case 1:
			str.sprintf("%d％", val2);
			break;
		case 2:
			str.sprintf("%5.3f-%d％", val1, val2);
			break;
		}
		QString ss = m_Analyze->Details(2)->Item(GetHarmIndex(i))->Caption();
		m_HarmLabel->Items()->Item(i)->SetCaption(ss + " " + str);
	}
	m_HarmLabel->Change();
}

void QDlgVectorImpl::ClearVector()
{
	m_Vector->Clear();
	m_VectorLabel->Clear();
}

void QDlgVectorImpl::AddVector(const QString& str, const QColor& clr)
{
	if (m_Vector->ItemIndex() > -1)		// 取消矢量基准
	{
		m_Analyze->SetVectorItemIndex(-1);
		m_Vector->SetItemIndex(-1);
	}
	m_Vector->Items()->Add(clr, str);
	m_VectorLabel->Items()->Add(clr, str);
}

void QDlgVectorImpl::FreshVector(int idx, Complex c, const QString& str)
{
	QVectorItem* item = m_Vector->Items()->Item(idx);
	item->SetRe(c.Re);
	item->SetIm(c.Im);
	m_Vector->Change();

	item = m_VectorLabel->Items()->Item(idx);
	item->SetCaption(str + ": " + FormatVector(c));
	m_VectorLabel->Change();
}

void QDlgVectorImpl::ShowPage(int page)
{
	if (page < 0 || page > 1)
		return;
	m_tabVector->setCurrentIndex(page);
}

void QDlgVectorImpl::Fresh(int pos)
{
	if (m_Analyze == NULL)
		return;

	if (m_tabVector->currentIndex() == 0)
		FreshVector1(pos);
	else
		FreshHarm(pos);

	m_Analyze->SetCursorPos(pos);
}

int QDlgVectorImpl::ActivePage()
{
	return m_tabVector->currentIndex();
}

void QDlgVectorImpl::SetAnalyze(QAnalyze* analyze)
{
	if (m_Analyze)
		m_Analyze->SetValuePage(m_tabVector->currentIndex());

	if (m_Analyze != analyze)
	{
		m_Analyze = analyze;
		if (m_Analyze == NULL)
			return;
		UpdateVector();
		UpdateHarm();
		Fresh(m_Analyze->CursorPos());
	}
}