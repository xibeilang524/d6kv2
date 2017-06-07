#include "funutils.h"
// #include "constutils.h"
#include "globaldef.h"
#include "complexs.h"
#include "dataserver.h"
#include "analyzeutils.h"
#include "viewintf.h"
#include "selectchannelimpl.h"


QAnalyze::QAnalyze()
{
	m_nCursorPos = 0;
 	m_Comtrade = NULL;
	Init();
}

QAnalyze::QAnalyze(Comtrade* c)
{
	m_nCursorPos = 0;
	m_Comtrade = NULL;
	Init();
	SetComtrade(c);
}

QAnalyze::~QAnalyze()
{
	delete m_pAnaChannels;
	delete m_pDigChannels;
	for (int i = 0; i < 3; i++)
		delete m_pDetails[i];
}

void QAnalyze::Init()
{
	m_pAnaChannels = new Channels;
	m_pDigChannels = new Channels;

	for (int i = 0; i < 3; i++)
		m_pDetails[i] = new QDetails;

	InitDigDetail();
	InitData();
}

void QAnalyze::SetValuePage(int n)
{ 
	m_nValuePage = n; 
}

void QAnalyze::InitData()
{
	// 曲线信息
	m_nValuePage = 0;
	m_bShowAllSwitch = false;
	m_bShowNoSwitch = true;
	m_nAverCount = 1;

	// 矢量信息
	m_nVectorPage = 0;
	m_nVectorIndex = -1;
	m_nHarmStyle = 0;
	m_nHarmValueIndex = 1;
	m_pHarmChannel = NULL;
}

// 最大计算谐波次数
int QAnalyze::MaxHarmValue()
{
	return m_Comtrade->max_cycle_count() / 2 - 1;
}

void QAnalyze::InitAnaDetail()
{
	m_pDetails[0]->Clear();
	int i;
	for (i = 0; i <= 5; i++)
		m_pDetails[0]->Add()->SetCaption(ANAINFOSTR[i]);

	for (i = 2; i <= MaxHarmValue(); i++)
	{
		QString str = QString(ANAINFOSTR[6]).arg(i);
		m_pDetails[0]->Add()->SetCaption(str);
	}
	for (i = 0; i < m_pDetails[0]->Count(); i++)
		m_pDetails[0]->Item(i)->SetData(i + 1);
	for (i = 0; i < 3; i++)
		m_pDetails[0]->Item(i)->SetCheck(true);
}

void QAnalyze::InitDigDetail()
{
	m_pDetails[1]->Clear();
	int sum = 10;	// 最大变位次数暂时设置为10
	for (int i = 0; i < sum; i++)
	{
		QDetail* item = m_pDetails[1]->Add();
		QString str = QString(DIGINFOSTR[0]).arg(i+1);
		item->SetCaption(str);
		if (i < 5)
			item->SetCheck(true);
	}
}

void QAnalyze::SetComtrade(Comtrade* c)
{
	if (m_Comtrade != c)
	{
		m_Comtrade = c;
		if (c == NULL)
		{
			m_pHarmChannel = NULL;
			return;
		}
		// 连接Comtrade的OnDelete信号
		m_pAnaChannels->init_by_comtrade(m_Comtrade, true);
		m_pDigChannels->init_by_comtrade(m_Comtrade, false);
		m_nAverCount = m_Comtrade->max_cycle_count();
		InitAnaDetail();
		InitHarmBarDetail();
	}
}

// 计算模拟量数据
QString QAnalyze::CalAnaData(int ch, int pos, int idx)
{
	m_nCursorPos = pos;
	if (m_pAnaChannels->count() < 1)
		return "";
	QString str;
	Complex data;
	View_data* view = m_pAnaChannels->channel(ch);
	switch (idx)
	{
	case 1:
		str.sprintf("%5.3f", view->Value(pos));
		break;
	case 2:			// 2-基波相量
		data = Data_server::GetVector1(view->channel_no(), pos);
		str = format_vector(data);
		break;
	case 3:			// 3-基波实虚部
		data = Data_server::GetVector1(view->channel_no(), pos);
		data = Data_server::VectorToComplex(data);
		str = format_complex(data);
		break;
	case 4:			// 4-平均值
		data = Data_server::GetAverage(view->channel_no(), pos, m_nAverCount);
		str.sprintf("%5.3f", data.Re);
		break;
	case 5:// 5-频率
		str.sprintf("%5.3f", Data_server::GetFreq(view->channel_no(), pos));
		break;
	case 6:// 6-直流
		data = Data_server::GetDirect(view->channel_no(), pos);
		str.sprintf("%5.3f", data.Re);
		break;
	default:// 谐波
		data = Data_server::GetHarmVector(view->channel_no(), pos, idx - 5);
		str = format_vector(data);
		break;
	}
	return str;
}

/*
	获得开关量变位信息-两种显示方式：
	1, 显示所有变位; 通道Channel的第Idx次变位;
	2, 显示时标Pos之后的变位; 
*/
QString QAnalyze::CalDigData(int ch, int pos, int idx)
{
	m_nCursorPos = pos;
	int nPos = m_bShowAllSwitch ? 0 : pos;
	View_data* view = m_pDigChannels->channel(ch);
	int sum = view->switch_num();
	if (sum == 0 || idx >= sum)
		return "";

	STATUS* pStatus = new STATUS[sum];
	view->get_all_switch(pStatus);
	if (pos > pStatus[idx].position && (! m_bShowAllSwitch))
		return "";

	QString str;
	str.sprintf("%5.3f ", m_Comtrade->get_sample_time(pStatus[idx].position, nPos));
	return str + switch_state(pStatus[idx].change);
}

// 计算谐波数据 
QString QAnalyze::CalHarmData(int ch, int pos, int idx)
{
	if (m_pAnaChannels->count() < 1)
		return "";
	View_data* view = m_pAnaChannels->channel(ch);
	QString str;
	if (idx == 1)
	{
// 		str.sprintf(_Q2C(GetFormatStr()), view->Value(pos));
// 		str = QString(GetFormatStr()).arg(view->Value(pos));
		str = QString::number(view->Value(pos),'f',2);	//保留小数点后两位
	}
	else	// 谐波相量
	{
		Complex data = Data_server::GetHarmVector(view->channel_no(), pos, view->harm_value());
		str = format_vector(data);
	}
	return str;
}

int QAnalyze::GetDetailIndex(QDetails* det, int idx)
{
	int n = -1;
	for (int i = 0; i < det->Count(); i++)
		if (det->Item(i)->Check())
		{
			if (++n == idx)
			{
				n = i;
				break;
			}
		}
	return n;
}

void QAnalyze::GetSwitchInfo(int count, bool* args)
{
	for (int i = 0; i < m_Comtrade->DigChannelNum(); i++)
		args[i] = m_Comtrade->digtal(i)->switch_num() > 0;
}

QDetails* QAnalyze::Details(int index)
{
	if (index < 0 || index > 2)
		return NULL;
	return m_pDetails[index];
}

// 添加矢量通道
bool QAnalyze::AddVector()
{
	int i;
	bool b = false;
	Dlg_select_channel_impl dlg;
	dlg.setWindowTitle(ADDVECTORSTR);
	for (i = 0; i < m_pAnaChannels->count(); i++)
		dlg.add_item(m_pAnaChannels->channel(i)->channel_name());
	dlg.set_state(true);
	if (dlg.exec() == QDialog::Accepted)
	{
		m_nVectorIndex = -1;
		for (i = 0; i < m_pAnaChannels->count(); i++)
			m_pAnaChannels->set_checked(i, dlg.Checked(i));
		b = true;
	}
	return b;
}

// 获得矢量通道的序号，矢量通道并不是按顺序选择的
int QAnalyze::VectorIndex(int idx)
{
	int index = -1;
	for (int i = 0; i < m_pAnaChannels->count(); i++)
		if (m_pAnaChannels->checked(i))
		{
			index++;
			if (index == idx)
			{
				index = i;
				break;
			}
		}
	return index;
}

// 获得基准矢量的相角值
Complex QAnalyze::VectorValue(int idx, int pos)
{
	if (idx < 0)
		return Complex(0, 0);

	View_data* view = m_pAnaChannels->channel(VectorIndex(idx));
	return Data_server::GetVector1(view->channel_no(), pos);
}

bool QAnalyze::AddHarm()
{
	int idx = SelectOneChannel(ADDHARMSTR);
	if (idx > -1)
	{
		m_pHarmChannel = m_pAnaChannels->channel(idx);
	}
	return m_pHarmChannel != NULL;
}

// 获得谐波数值
double QAnalyze::HarmValue(int idx, int pos)
{
	Complex c(0, 0);
	int n = m_pHarmChannel->channel_no();
	switch (idx)
	{
	case 0:		// 直流
		c = Data_server::GetDirect(n, pos);
		break;
	case 1:		// 基波有效值
		c = Data_server::GetVector(n, pos);
		break;
	default:	// 二次以上谐波
		c = Data_server::GetHarmVector(n, pos, idx);
		break;
	}
	return c.Re;
}

int QAnalyze::SelectOneChannel(const QString& str)
{
	int n = -1;
	Dlg_select_channel_impl dlg;
	dlg.setWindowTitle(str);

	for (int i = 0; i < m_pAnaChannels->count() - 1; i++)
		dlg.add_item(m_pAnaChannels->channel(i)->channel_name());

	dlg.set_multi_check(false);
	dlg.set_state(true);
	if (dlg.exec() == QDialog::Accepted)
	{
		n = dlg.only_check_index();
	}
	return n;
}

// 初始化谐波棒图显示信息
void QAnalyze::AddHarmBar(int idx, const QString& str)
{
	QDetail* item = m_pDetails[2]->Add();
	item->SetCaption(str);
	item->SetCheck(idx <= 11);	// 默认最多添加11次谐波
	item->SetData(idx);
}

void QAnalyze::InitHarmBarDetail()
{
	m_pDetails[2]->Clear();
	AddHarmBar(0, ANAINFOSTR[5]);
	AddHarmBar(1, HARMONESTR);
	for (int i = 2; i <= MaxHarmValue(); i++)
	{
		QString str = QString(ANAINFOSTR[6]).arg(i);

		AddHarmBar(i, str);
	}
}