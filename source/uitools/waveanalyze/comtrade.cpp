#include <QList>

#include "funutils.h"
// #include "constutils.h"
#include "globaldef.h"
#include "comtrade.h"
#include <math.h>


/* QChannel */

Channel::Channel(Comtrade* owner)
{
	m_pOwner = owner;
	m_nChannelNo = -1;
	m_strChannelName = "";
	m_strDevice = "";
	m_strPhase = "";
	m_pData = NULL;
	m_strClassName = CHANNELCLASS[0];
}

Channel::~Channel()
{
	if (m_pData) 
	{
		delete []m_pData;
		m_pData = NULL;
	}	
}

Channel& Channel::operator=(const Channel &c)
{
//	m_strChannelName = c.ChannelName();
//	m_strDevice = c.Device();
//	m_strPhase = c.Phase();
	return *this;
}

void Channel::copy_data(Channel* src, QPoint range)
{

}

int Channel::get_all_switch(STATUS* pSta)
{
	return 0;
}

double* Channel::get_analog_data(int minPos, int maxPos, int& count)
{
	return NULL;
}

int Channel::channel_phase()
{
	int n = -1;
	for (int i = 0; i < 5; i++)
	{
		if (m_strPhase == PHASES[i])
		{
			n = i;
			break;
		}
	}
	return n;
}

STATUS* Channel::get_digtal_data(int& initData, int& count)
{
	return NULL;
}

int Channel::switch_num()
{
	return 0;
}

QString Channel::channel_name()
{
	return m_strChannelName;
}

int Channel::channel_no()
{
	return m_nChannelNo;
}

void Channel::SetChannelNo( int n )
{
	m_nChannelNo = n;
}

double* Channel::get_data()
{
	return m_pData;
}

QString Channel::get_device()
{
	return m_strDevice;
}

QString Channel::Phase()
{
	return m_strPhase;
}

/* QAnalog */

Analog::Analog(Comtrade* owner) : Channel(owner)
{
	m_strChannelName = CHANNELCLASS[1];
	m_strDataUnit = "";
	m_fScale = 1;
	m_fOffset = 0;
	m_fSkew = 0;
	m_fMinValue = 0;
	m_fMaxValue = 0;
	m_fPrimary = 1;
	m_fSecodary = 1;
	m_strPs = "s";
}

Analog::~Analog()
{
}

// 模拟量类型 0-电流 1-电压 2-频率 3-功率
uint Analog::AnaType()
{
	return 0;
}

double* Analog::get_analog_data(int minPos, int maxPos, int& count)
{
	if (minPos < 0 || minPos >= m_pOwner->data_count()) minPos = 0;
	if (maxPos < 0 || maxPos >= m_pOwner->data_count()) maxPos = m_pOwner->data_count();
	count = maxPos - minPos;
	double* p = m_pData;
	return p + minPos;
}

QColor Analog::GetColor()
{
	int n = channel_phase();
	if (n < 0 || n > 5)
		n = channel_no() % 4;
	return COLORTABS[n];
}

double Analog::MaxValue()
{
	double fResult(1.0);
	for (int i = 0; i < m_pOwner->data_count(); i++)
	{
		if (fResult < fabs(m_pData[i]))
			fResult = fabs(m_pData[i]);
	}
	return fResult;
}

double Analog::Value(int n)
{
	Q_ASSERT(n > -1 && n < m_pOwner->data_count());
	return m_pData[n];
}

Analog& Analog::operator = (const Analog &c)
{

	return *this;
}

/* QDigtal */

Digtal::Digtal(Comtrade* owner) : Channel(owner)
{
	m_strClassName = CHANNELCLASS[2];
	m_nSwitchNum = 0;
}

Digtal::~Digtal()
{
}

Digtal& Digtal::operator = (const Digtal &c)
{
	return *this;
}

void Digtal::Filtrate()
{
	m_nSwitchNum = 0;
	Q_ASSERT(m_pData != NULL);
	double* p = m_pData;
	m_nDefaultState = p[0];
	//
	m_sChangeArray[m_nSwitchNum].position = 0;
	m_sChangeArray[m_nSwitchNum].change = p[0];
	m_sChangeArray[m_nSwitchNum].endPosition = 0;
	for (int i = 1; i < m_pOwner->data_count(); i++)
	{
		if (p[i-1] == p[i])
		{
			m_sChangeArray[m_nSwitchNum].endPosition = i;	
			continue;
		}
		m_sChangeArray[m_nSwitchNum].endPosition = i;	
		m_nSwitchNum++;
		m_sChangeArray[m_nSwitchNum].position = i;
		m_sChangeArray[m_nSwitchNum].change = (p[i] == 1);
		m_sChangeArray[m_nSwitchNum].endPosition = i;	
		if (m_nSwitchNum >= 255) break;
	}	
	m_nSwitchNum++;
}

int Digtal::get_all_switch(STATUS* pSta)
{
	int n = m_nDefaultState;
	for (uint i = 0; i < m_nSwitchNum; i++)
		pSta[i] = m_sChangeArray[i];
	return n;
}

STATUS* Digtal::get_digtal_data(int& initData, int& count)
{
	initData = m_nDefaultState;
	count = m_nSwitchNum;
	return m_sChangeArray;
}

double Digtal::Value(int n)
{
	Q_ASSERT(n > -1 && n < m_pOwner->data_count());
	if (m_pData == NULL)
	{
		double val = m_nDefaultState;
		if (m_nSwitchNum > 0)
		{
			STATUS* p = m_sChangeArray;
			for (uint i = 1; i < m_nSwitchNum; i++)
			{
				if (n >= p[i-1].position && n < p[i].position)
					val = p[i-1].change;
			}
			if (n >= p[m_nSwitchNum-1].position)
				val = p[m_nSwitchNum-1].change;
		}
		return val;
	}
	else					
	{
		return m_pData[n];
	}
}

/* QComtrade */

Comtrade::Comtrade()
{
	m_bResetTime = false;
	m_strFaultFile = "";
	m_strStation = "";
	m_strRecorder = "";
	m_nChannelNum = 0;
	m_nAnaChannelNum = 0;
	m_nDigChannelNum = 0;
	m_fSysFrequency = 50;
	m_nFrequencyNum = 0;
	m_strFileType = "";
	m_strTimeMult = "";
	m_pSampleTime = NULL;

	m_nDataCount = 0;
	m_strStartTime = "";
	m_strTriggerTime = "";

// 	m_pAnaChannels = new  QList<QAnalog>;
// 	m_pDigChannels = new  QList<QDigtal>;
}

Comtrade::~Comtrade()
{
	if (m_pSampleTime)
		delete []m_pSampleTime;
	Clear();
// 	delete m_pAnaChannels;
// 	delete m_pDigChannels;
}

Channel* Comtrade::AddItem(bool b /* = true */)
{
	int n;
	Channel* p;
	if (b)
	{
		p = new Analog(this);
		n = MaxAnalogNo() + 1;
	}
	else
	{
		p = new Digtal(this);
		n = MaxDigtalNo() + 1;
	}
	p->SetChannelNo(n);
	return p;
}

Channel* Comtrade::Add(bool b /* = true */)
{
	Channel* p = AddItem(b);
	if (b)	m_pAnaChannels.append((Analog*)p);
	else	m_pDigChannels.append((Digtal*)p);

	return p;
}

Channel* Comtrade::Insert(int n, bool b /* = true */)
{
	Channel* p = AddItem(b);
	if (b)	m_pAnaChannels.insert(n, (Analog*)p);
	else	m_pDigChannels.insert(n, (Digtal*)p);
	return p;
}

void Comtrade::Clear()
{
	m_nAnaChannelNum = 0;
	m_nDigChannelNum = 0;
	m_nChannelNum = 0;

// 	 QListIterator<QAnalog> itAna(*m_pAnaChannels);
//     QAnalog* ana;
//     while ((ana = itAna.current()) != 0) 
// 	{
//         ++itAna;
// 		delete ana;
//     }	
	m_pAnaChannels.clear();

// 	 QListIterator<QDigtal> itDig(*m_pDigChannels);
//     QDigtal* dig;
//     while ((dig = itDig.current()) != 0) 
// 	{
//         ++itDig;
// 		delete dig;
//     }	
	m_pDigChannels.clear();
}

Channel* Comtrade::channel(int n)
{
	if (n < 0 || n >= (int)m_nChannelNum) return NULL;
	bool b = n < (int)m_nAnaChannelNum;
	int n1 = n - m_nAnaChannelNum * (!b);
	if (b)	return analog(n1);
	else	return digtal(n1);
}

double Comtrade::GetTime(int n)
{
	Q_ASSERT(n > -1 && n < (int)m_nDataCount);
	if (!m_pSampleTime)
	{
		m_pSampleTime = new double[m_nDataCount];
		double f = 1.0 / m_pSampleInfo[0].frequency;
		for (uint i = 0; i < m_nDataCount; i++)
			m_pSampleTime[i] = f * 1000000.0 * i;
	}
	return m_pSampleTime[n] / 1000.0;
}

// 获得通过采样记录计算的采样总点数
int Comtrade::CalCount()
{
	int sum(0);
	for (uint i = 0; i < m_nFrequencyNum; i++)
		sum += m_pSampleInfo[i].count;
	return sum;
}

// 设置时标缓冲区指针
void Comtrade::SetSampleTime(double* p)
{
	if (!p) return;
	if (m_pSampleTime)
	{
		delete []m_pSampleTime;
		m_pSampleTime = NULL;
	}
	m_pSampleTime = p;
}

// 根据指定位置获得所在时段索引
int Comtrade::GetSampleIndexByPos(int pos)
{
	ResetSample();
	for (uint i = 0; i < m_nFrequencyNum; i++)
		if (pos >= m_pSampleInfo[i].begin && pos <= m_pSampleInfo[i].end)
			return i;
	return 0;
}

SAMPLE Comtrade::GetSampleByPos(int pos)
{
	return m_pSampleInfo[GetSampleIndexByPos(pos)];
}

// 获得Pos所在时段每周波数据长度
int Comtrade::cycle_count(int pos)
{
	return GetSampleByPos(pos).frequency / 50;
}

double* Comtrade::get_cycle_data(int n, int pos)
{
	SAMPLE sam = GetSampleByPos(pos);
	int ncycle = sam.frequency / 50;
	if ((int)m_nDataCount < ncycle) return NULL;
	bool forward = (pos - ncycle) >= sam.begin;
	int nmax;
	int nmin;
	if (forward)	// 向前取
	{
		nmax = pos;
		nmin = nmax - ncycle + 1;
	}
	else			// 向后取
	{
		nmin = pos;
		nmax = nmin + ncycle;
	}  
	Analog* p = (Analog*)ChannelByNo(n);
	if (!p) return NULL;
	return p->get_analog_data(nmin, nmax, ncycle);
}

double* Comtrade::get_analog_data(int n, int pos, int count)
{
	Analog* p = (Analog*)ChannelByNo(n);
	if (!p) return NULL;
	int ncount(0);
	return p->get_analog_data(pos, pos + count, ncount);
}

QString Comtrade::get_channel_unit(int index)
{
	Analog *p =  (Analog*)ChannelByNo(index);
	if (!p) return NULL;
	QString unit;
	return p->get_channel_unit();
}

void Comtrade::get_full_sample(SAMPLE* sam)
{
	for (uint i = 0; i < m_nFrequencyNum; i++)
		sam[i] = m_pSampleInfo[i];
}

int Comtrade::get_samples(int ns, int ne, SAMPLE* sam)
{
	int n1 = GetSampleIndexByPos(ns);
	int n2 = GetSampleIndexByPos(ne);
	int i(0);
	while (i < n2 - n1 + 1)
	{
		sam[i] = m_pSampleInfo[i + n1];
	}
	sam[0].begin = ns;
	sam[0].count = sam[0].end - sam[0].begin + 1;
	i = n2 - n1;
	sam[i].end = ne;
	sam[i].count = sam[i].end - sam[i].begin + 1;
	return n2 - n1 + 1;
}

int Comtrade::pos(double timeData)
{
	int nResult(-1);
	if (timeData < GetTime(0) && timeData > GetTime(m_nDataCount - 1)) return nResult;
	for (uint i = 0; i < m_nDataCount - 1; i++)
	{
		if (timeData >= m_pSampleTime[i] / 1000.0 && timeData < m_pSampleTime[i + 1] / 1000.0)
		{
			nResult = i;
			break;
		}
	}
	if (fabs(timeData - m_pSampleTime[m_nDataCount - 1] / 1000.0) < 0.001)
		nResult = m_nDataCount - 1;
	return nResult;
}

bool Comtrade::pos_is_correct(double timeData)
{
	int n = pos(timeData);
	return (n >= 0) && (n < (int)m_nDataCount);
}

// 组织采样信息
void Comtrade::ResetSample(bool b /* = false */)
{
	if (m_bResetTime && (!b)) return;
	m_pSampleInfo[0].begin = 0;
	m_pSampleInfo[0].end = m_pSampleInfo[0].count - 1;
	for (uint i = 1; i < m_nFrequencyNum; i++)
	{
		m_pSampleInfo[i].begin = m_pSampleInfo[i-1].end + 1;
		m_pSampleInfo[i].end = m_pSampleInfo[i].begin + m_pSampleInfo[i].count - 1;
	}
	m_bResetTime = true;
}

void Comtrade::AddSample(SAMPLE sam)
{
//	m_nFrequencyNum++; 
//	m_pSampleInfo = new SAMPLE[m_nFrequencyNum];
}

int Comtrade::max_cycle_count()
{
	int nResult(0);
	for (uint i = 0; i < m_nFrequencyNum; i++)
	{
		int n = m_pSampleInfo[i].frequency / 50;
		if (nResult < n)
			nResult = n;
	}
	return nResult;
}

int Comtrade::max_switch_num()
{
	int nResult(0);

// 	QDigtal* dig = NULL;
// 	 QListIterator<QDigtal> it(*m_pDigChannels);
// 	while ((dig = it.current()) != 0)
// 	{
// 		++it;
// 		if (nResult < dig->SwitchNum())
// 			nResult = dig->SwitchNum();
// 	}
	QList<Digtal*>::iterator it;
	for (it=m_pDigChannels.begin();it!=m_pDigChannels.end();++it)
	{
		if (nResult < (*it)->switch_num())
		{
			nResult = (*it)->switch_num();
		}
	}
	return nResult;
}

double Comtrade::get_sample_time(int pos, int zero)
{
	return GetTime(pos) - GetTime(zero);
}

double Comtrade::get_sample_time(int pos)
{
	return GetTime(pos);
}

uint Comtrade::channel_harm_value(int n)
{
	return analog(n)->HarmValue();
}

uint Comtrade::channel_type(int n)
{
	return analog(n)->AnaType();
}

uint Comtrade::channel_phase(int n)
{
//	return GetIndex(Analog(n)->Phase(), PHASES);
	return analog(n)->channel_phase();
}

int Comtrade::Count(int n /* = 2 */)
{
	switch(n)
	{
	case 0:
		return m_pAnaChannels.count();
		break;
	case 1:
		return m_pDigChannels.count();
		break;
	case 2:
		return m_pDigChannels.count() + m_pAnaChannels.count();
		break;
	default:
		return 0;
		break;
	}
}

QString Comtrade::get_info_str(int n)
{
	switch(n)
	{
	case 0:
		return m_strFaultFile; break;
	case 1:
		return m_strRecorder; break;
	case 2:
		return m_strStation; break;
	case 3:
		return m_strStartTime; break;
	default:
		return ""; break;
	}
}

QString Comtrade::file_fype()
{
	return _C2Q("COMTRADE(标准格式)");
}

Analog* Comtrade::analog(int n)
{
	if (n < 0 || n >= (int)m_pAnaChannels.count()) return NULL;
	return m_pAnaChannels.at(n);
}

Digtal* Comtrade::digtal(int n)
{
	if (n < 0 || n >= (int)m_pDigChannels.count()) return NULL;
	return m_pDigChannels.at(n);
}

int Comtrade::IndexOf(Channel* channel)
{
	if (channel->m_strClassName == CHANNELCLASS[1])
		return m_pAnaChannels.indexOf((Analog*)channel);
	else
		return m_pDigChannels.indexOf((Digtal*)channel);
}

// 获得通道从Pos位置开始的数据指针
double* Comtrade::get_data(int n, int pos)
{
	Channel* p = channel(n);
	double* ff = NULL;
	if (p) ff = p->get_data() + pos;
	return ff;
}

void Comtrade::FiltrateDigtals()
{
// 	QDigtal* dig = NULL;
// 	 QListIterator<QDigtal> it(*m_pDigChannels);
// 	while ((dig = it.current()) != 0)
// 	{
// 		++it;
// 		dig->Filtrate();
// 	}
	QList<Digtal*>::iterator it;
	for (it=m_pDigChannels.begin();it!=m_pDigChannels.end();++it)
	{
		(*it)->Filtrate();
	}

}

void Comtrade::Delete(int n)
{
	if (n < 0 || (uint)n >= m_nChannelNum)
		return;

	int idx = - 1;
	Channel* channel;
	if ((uint)n < m_nAnaChannelNum)
	{
		idx = n;
		channel = m_pAnaChannels.at(idx);
		Notify((void*)channel, lnDeleted);
		m_pAnaChannels.removeAt(idx);

		m_nAnaChannelNum--;
	}
	else
	{
		idx = n - m_nAnaChannelNum;
// 		pList = ( QList<QChannel*>)m_pDigChannels;
		channel = m_pDigChannels.at(idx);
		m_nDigChannelNum--;
		m_pDigChannels.removeAt(idx);
	}
	m_nChannelNum--;
// 	QChannel* channel = pList.at(idx);
// 	Notify((void*)channel, lnDeleted);
// 	pList.remove(idx);
	delete channel;
}

void Comtrade::DeleteRef(Channel* chl)
{
	int n = IndexOf(chl);
	if (chl->m_strClassName == CHANNELCLASS[2])
		n += m_nAnaChannelNum;
	Delete(n);
}

int Comtrade::get_fault_pos()
{
	double f = time_to_float(m_strTriggerTime) - time_to_float(m_strStartTime);
	return (int)(f * m_pSampleInfo[0].frequency);
}

bool Comtrade::CanAddData(Comtrade& src)
{
	return false;
}

double* Comtrade::get_pos_sample(int pos)
{
	if (pos < 0 || pos >= (int)m_nDataCount) return NULL;
	if (!m_pSampleTime) GetTime(0);
	return m_pSampleTime + pos;
}

int Comtrade::MaxAnalogNo()
{
	int nResult(-1);
	Analog* ana = NULL;
	QList<Analog*>::iterator it;
	for (it = m_pAnaChannels.begin();it !=m_pAnaChannels.end();++it )
	{
		if (nResult < (*it)->channel_no())
		{
			nResult = (*it)->channel_no();
		}
	}
	return nResult;
}

int Comtrade::MaxDigtalNo()
{
	int nResult(-1);
// 	QDigtal* dig = NULL;
// 	 QListIterator<QDigtal> it(*m_pDigChannels);
// 	while ((dig = it.current()) != 0)
// 	{
// 		++it;
// 		if (nResult < dig->ChannelNo())
// 			nResult = dig->ChannelNo();
// 	}
	QList<Digtal*>::iterator it;
	for (it = m_pDigChannels.begin();it !=m_pDigChannels.end();++it )
	{
		if (nResult < (*it)->channel_no())
			nResult = (*it)->channel_no();
	}
	return nResult;
}

Channel* Comtrade::ChannelByNo(int n, bool b /* = true */)
{
	if (b)
	{
		QList<Analog*>::iterator ana;
		for (ana = m_pAnaChannels.begin();ana !=m_pAnaChannels.end();++ana )
		{
			if ( (*ana)->channel_no() == n)
				return *ana;
		}
	}
	else
	{
		QList<Digtal*>::iterator dig;
		for (dig = m_pDigChannels.begin();dig !=m_pDigChannels.end();++dig )
		{
			if ( (*dig)->channel_no() == n)
				return *dig;
		}
	}
	return NULL;
}

void Comtrade::Notify(void* ptr, ListNotify action)
{
	switch(action)
	{
	case lnAdded:
		break;
	case lnDeleted:
		break;
	default:
		break;
	}
}

// 获得通道从pos位置开始的数据指针
STATUS* Comtrade::get_digtal_data(int& initData, int& count, int n)
{
	return NULL;
}

// 是否故障，暂时仅用时间判断
bool Comtrade::IsFault()
{
	if (m_strStartTime.isEmpty())
		return false;
	return m_strStartTime == m_strTriggerTime;
}
