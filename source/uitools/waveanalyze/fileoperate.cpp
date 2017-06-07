#include <qfile.h>
#include <qtextstream.h>
#include <qdatastream.h>
#include <qapplication.h>
#include <qtextcodec.h>

#include "fileoperate.h"
#include "funutils.h"

/* QFileOperate */

typedef unsigned short WORD;

bool File_operate::load_file(const QString& fileName, int fileType)
{
	m_strFileName = fileName;
	m_pComtrade->SetFaultFile(fileName);
	switch(fileType)
	{
	case 0:
		return read_comtrade();
		break;
	default:
		return false;
		break;
	}
}

bool File_operate::read_comtrade()
{
	QString cfg = change_file_ext(m_strFileName, ".cfg");
	if (!QFile::exists(cfg)) return false;
	QString dat = change_file_ext(m_strFileName, ".dat");
	if (!QFile::exists(dat)) return false;
	if (!read_cfg(cfg)) return false;
	QString s[2];
	s[0] = "ASCII";
	s[1] = "BINARY";
	QString ss = m_pComtrade->m_strFileType.toUpper();
	int nError;
	if (ss == s[0])
		return read_ascii(dat);
	else if (ss == s[1])
		return read_binary(dat, nError);
	else 
		return false;
}

bool File_operate::read_cfg(const QString& cfg)
{
	QStringList ss;
	QFile file(cfg);
	if (file.open(QIODevice::ReadOnly))
	{
		QTextStream stream(&file);
// 		stream.setEncoding(QTextStream::Latin1);
		stream.setCodec(QTextCodec::codecForName("GBK"));
		QString line;
		while (!stream.atEnd())
		{
			line = stream.readLine();
			ss += line;
		}
		file.close();
	}
	uint nCount(0);
	m_pComtrade->m_strStation = cut_char(ss[0], 0);
	m_pComtrade->m_strRecorder = cut_char(ss[0], 1);
	m_pComtrade->m_strRevYear = cut_char(ss[0], 2);
	nCount++;

	m_pComtrade->m_nChannelNum = cut_char_to_int(ss[1], 0);
	QString s = cut_char(ss[1], 1);
	s = s.left(s.length() - 1);
	m_pComtrade->m_nAnaChannelNum = s.toInt();
	s = cut_char(ss[1], 2);
	s = s.left(s.length() - 1);
	m_pComtrade->m_nDigChannelNum = s.toInt();
	nCount++;
	if (m_pComtrade->m_nChannelNum != m_pComtrade->m_nAnaChannelNum + m_pComtrade->m_nDigChannelNum) return false;
	uint i;
	for (i = 0; i < m_pComtrade->m_nAnaChannelNum; i++)
	{
		Analog* ana = (Analog*)m_pComtrade->Add();
		set_analog(ss[nCount], ana);
		nCount++;
	}
	
	for (i = 0; i < m_pComtrade->m_nDigChannelNum; i++)
	{
		Digtal* dig = (Digtal*)m_pComtrade->Add(false);
		set_digtal(ss[nCount], dig);
		nCount++;
	}

	m_pComtrade->m_fSysFrequency = ss[nCount++].toFloat();
	m_pComtrade->m_nFrequencyNum = ss[nCount++].toInt();
	m_pComtrade->m_pSampleInfo = new SAMPLE[m_pComtrade->m_nFrequencyNum];
	for (i = 0; i < m_pComtrade->m_nFrequencyNum; i++)
		m_pComtrade->m_pSampleInfo[i] = get_sample_info(ss[nCount++]);
	
	m_pComtrade->m_strStartTime = ss[nCount++];
	m_pComtrade->m_strTriggerTime = ss[nCount++];
	m_pComtrade->m_strFileType = ss[nCount++];
	if (nCount < ss.count())
		m_pComtrade->m_strTimeMult = ss[nCount];

	return true;
}

void File_operate::set_analog(const QString& anaStr, Analog* ana)
{
	ana->m_strChannelName = cut_char(anaStr, 1);
	ana->m_strPhase = cut_char(anaStr, 2);
	if (ana->m_strPhase.length() == 0)
		ana->m_strPhase = possible_phase(ana->m_strChannelName);
	ana->m_strDevice = cut_char(anaStr, 3);
	ana->m_strDataUnit = cut_char(anaStr, 4);
	ana->m_fScale = cut_char_to_float(anaStr, 5);
	ana->m_fOffset = cut_char_to_float(anaStr, 6);
	ana->m_fSkew = cut_char_to_float(anaStr, 7);
	ana->m_fMinValue = cut_char_to_float(anaStr, 8);
	ana->m_fMaxValue = cut_char_to_float(anaStr, 9);
	ana->m_fPrimary = cut_char_to_float(anaStr, 10);
	ana->m_fSecodary = cut_char_to_float(anaStr, 11);
	ana->m_strPs = cut_char(anaStr, 12);
}

void File_operate::set_digtal(const QString& digStr, Digtal* dig)
{
	dig->m_strChannelName = cut_char(digStr, 1);
	dig->m_strPhase = cut_char(digStr, 2);
	dig->m_strDevice = cut_char(digStr, 3);
	dig->m_nDefaultState = cut_char_to_int(digStr, 4);
}

SAMPLE File_operate::get_sample_info(const QString& samStr)
{
	SAMPLE sam;
	sam.frequency = cut_char_to_float(samStr, 0);
	sam.count = cut_char_to_int(samStr, 1);
	return sam;
}

bool File_operate::read_ascii(const QString& dat)
{
	QFile file(dat);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return false;
	}
	QTextStream in(&file);
	QString file_all = in.readAll();
	QStringList fields = file_all.split("\n",QString::SkipEmptyParts);
	
	m_pComtrade->m_nDataCount = fields.count();
	int wave_count = m_pComtrade->m_nChannelNum;
	int analog_wave_count = m_pComtrade->m_nAnaChannelNum;
	int digtal_curve_count = m_pComtrade->m_nDigChannelNum;
	for (int i=0;i<analog_wave_count;i++)
	{
		Analog *ana = m_pComtrade->analog(i);
		ana->m_pData = new double[m_pComtrade->m_nDataCount];
	}
	for (int i=0;i<digtal_curve_count;i++)
	{
		Digtal* dig = m_pComtrade->digtal(i);
		dig->m_pData = new double[m_pComtrade->m_nDataCount];
	}

	bool ok;
	for (int j=0;j<m_pComtrade->m_nDataCount; j++)
	{
		QString linetmp = fields.at(j);
		for (int i=0;i<wave_count;i++)
		{
			if (i<analog_wave_count)
			{
				Analog *ana = m_pComtrade->analog(i);
				ana->m_pData[j] = linetmp.split(",",QString::SkipEmptyParts).at(2+i).toFloat(&ok);	//通道i的值
			}
			else
			{
				Digtal* dig = m_pComtrade->digtal(i-analog_wave_count);
				dig->m_pData[j] = linetmp.split(",",QString::SkipEmptyParts).at(2+i).toFloat(&ok);	//通道i的值
			}
		}
	}
	double *p1, *pd;
	p1 = new double[m_pComtrade->m_nDataCount];

	{
		double ff = 1000000.0 /  m_pComtrade->m_pSampleInfo[0].frequency;
		pd = p1;
		for (int i = 0; i < m_pComtrade->m_nDataCount; i++)
			pd[i] = ff * i;
	}
	m_pComtrade->SetSampleTime(p1);
	
	file.close();
	return true;
}

// 检查文件大小是否正确
// 正确则返回RecCount: 单个采样记录长度 nStart: 采样数据起始位置
bool File_operate::check_data_file(int size, int& nRec, int& nStart)
{	
	int nHasTime = m_pComtrade->m_nAnaChannelNum + ((m_pComtrade->m_nDigChannelNum + 15) / 16) + 4;
	int nNoTime = nHasTime - 2;
	int nCal = m_pComtrade->CalCount();
	if (size == 2 * nHasTime * nCal)
	{
		nRec = nHasTime;
		nStart = 4;
		return true;
	}
	else if (size == 2 * nNoTime * nCal)
	{
		nRec = nNoTime;
		nStart = 2;
		return true;
	}
	else return false;
}

bool File_operate::read_binary(const QString& dat, int& nError)
{
	nError = 0;
	QFile file(dat);
	if (file.open(QIODevice::ReadOnly))
	{
		int nRec(0), nStart(0);
		if (!check_data_file(file.size(), nRec, nStart)) 
		{
			nError = 1;
			return false;
		}
		m_pComtrade->m_nDataCount = file.size() / (2 * nRec);
		
		char* buffer = new char[file.size()];
		if (!buffer) 
		{
			nError = 2;
			delete []buffer;
			return false;
		}
		uint len(file.size());
		len = file.read(buffer, len);
		if (len != file.size())
		{
			nError = 3;
			delete []buffer;
			return false;
		}
		
		double* pf;
		ushort* p = (ushort*)buffer;
		uint i(0), j(0);
		for (i = 0; i < m_pComtrade->m_nAnaChannelNum; i++)
		{
			Analog* ana = m_pComtrade->analog(i);
			ana->m_pData = new double[m_pComtrade->m_nDataCount];
			pf = ana->m_pData;
			for (j = 0; j < m_pComtrade->m_nDataCount; j++)
			{
#ifdef WIN32
				short ss = (short)p[nStart + i + nRec * j];
#else
				short ss = BigToLitter(p, nStart + i + nRec * j);
#endif
				pf[j] = ana->m_fScale * ss + ana->m_fOffset;
			}
		}
		for (i = 0; i < m_pComtrade->m_nDigChannelNum; i++)
		{
			Digtal* dig = m_pComtrade->digtal(i);
			dig->m_pData = new double[m_pComtrade->m_nDataCount];
			pf = dig->m_pData;
			int delta =	nStart + i / 16 + m_pComtrade->m_nAnaChannelNum;
			for (j = 0; j < m_pComtrade->m_nDataCount; j++)
			{
#ifdef WIN32
				short ss = (short)p[delta + j * nRec];
#else
				short ss = BigToLitter(p, delta + j * nRec);
#endif
				pf[j] = is_bit_on(ss, i % 16);
			}
		}
		
		double *p1, *pd;
		p1 = new double[m_pComtrade->m_nDataCount];
/*
		if (nStart == 4)
		{
			pd = p1;
			p = (ushort*)buffer;
			p += 2;
			int counter = 0;
			for (i = 0; i < m_pComtrade->m_nDataCount; i++)
			{
				ushort dw;		// 取dw来获得时标
				ushort* p3 = &dw;
				*(p3++) = *(p++);
				*p3 = *p;

				p1[counter++] = (double)dw;
				*(pd++) = dw;
				p += nRec - 1;
			}
		}
		else
*/
		{
			double ff = 1000000.0 /  m_pComtrade->m_pSampleInfo[0].frequency;
			pd = p1;
			for (i = 0; i < m_pComtrade->m_nDataCount; i++)
				pd[i] = ff * i;
		}
		m_pComtrade->SetSampleTime(p1);
		
		delete []buffer;
		file.close();
		return true;
	}
	return false;
}

// 要求：fileName是ASCII编码
bool File_operate::save_as_comtrade(const QString& fileName)
{
	QString cfgFile = change_file_ext(fileName, ".cfg");
	QString datFile = change_file_ext(fileName, ".dat");
	if (! save_cfg_file(cfgFile))
		return false;
	if (! save_dat_file(datFile))
		return false;
	return true;
}

bool File_operate::save_cfg_file(const QString& cfgFile)
{
	bool b = false;
	QFile file(cfgFile);
	if (file.open(QIODevice::WriteOnly))
	{
		char* comma = ",";
		char* newline = "\n";

        QTextStream stream(&file);
// 		stream.setEncoding(QTextStream::Latin1);
		stream.setCodec(QTextCodec::codecForName("GBK"));
		QString str("");
		// 站名、特征和修改年份
		stream << m_pComtrade->m_strStation << comma 
			   << m_pComtrade->m_strRecorder << comma 
			   << m_pComtrade->m_strRevYear << newline;

		// 通道的数量和类型
		stream << QString::number(m_pComtrade->m_nChannelNum) << comma
			   << QString::number(m_pComtrade->m_nAnaChannelNum) << "A" << comma
			   << QString::number(m_pComtrade->m_nDigChannelNum) << "D" << newline;
		
		uint i = 0;
		// 模拟量
		for (i = 0; i < m_pComtrade->m_nAnaChannelNum; i++)
		{
			Analog* ana = m_pComtrade->analog(i);
			const char* p = ana->m_strChannelName.toLatin1();
			stream << QString::number(i+1) << comma
				   << ana->m_strChannelName.toLatin1() << comma
				   << ana->m_strPhase.toLatin1() << comma
				   << ana->m_strDevice.toLatin1() << comma
				   << ana->m_strDataUnit.toLatin1() << comma
				   << QString::number(ana->m_fScale) << comma
				   << QString::number(ana->m_fOffset) << comma
				   << QString::number(ana->m_fSkew) << comma
				   << QString::number(ana->m_fMaxValue) << comma
				   << QString::number(ana->m_fMinValue) << comma
				   << QString::number(ana->m_fPrimary) << comma
				   << QString::number(ana->m_fSecodary) << comma
				   << ana->m_strPs << newline;
		}

		// 开关量
		for (i = 0; i < m_pComtrade->m_nDigChannelNum; i++)
		{
			Digtal* dig = m_pComtrade->digtal(i);
			stream << QString::number(i+1) << comma
				   << dig->m_strChannelName.toLatin1() << comma
				   << dig->m_strDevice << comma
				   << dig->m_strPhase << newline;
		}
		
		// 系统频率
		stream << QString::number(m_pComtrade->m_fSysFrequency) << newline;
		// 采样信息
		stream << QString::number(m_pComtrade->m_nFrequencyNum) << newline;
		SAMPLE* sam = m_pComtrade->m_pSampleInfo;
		for (i = 0; i < m_pComtrade->m_nFrequencyNum; i++)
		{
			stream << QString::number(sam[i].frequency) << comma
				   << QString::number(sam[i].count) << newline;
		}
		stream << m_pComtrade->m_strStartTime << newline;
		stream << m_pComtrade->m_strTriggerTime << newline;
		stream << "BINARY" << newline;

		file.close();
		b = true;
	}
	return b;    
}

bool File_operate::save_dat_file(const QString& datFile)
{
	bool b = false;
	QFile file(datFile);
	if (file.open(QIODevice::WriteOnly))
	{
		QDataStream stream(&file);

		uint digcount = m_pComtrade->m_nDigChannelNum;
		uint i = digcount % 16 > 0 ? 1 : 0;
		digcount /= 16;
		digcount += i;
		i = 0;
		while (i < m_pComtrade->m_nDataCount)
		{
			unsigned char ch1[4];
			// 序号
			quint32 idx = i;
			memcpy(&ch1, &idx, 4);

			uint j = 0;
			for (j = 0; j < 4; j++)
				stream << ch1[j];

			// 时标
			quint32 ntime = (quint32)m_pComtrade->get_sample_time(i) * 1000;
			memcpy(&ch1, &ntime, 4);
			stream << ch1[3];
			stream << ch1[4];
			stream << ch1[1];
			stream << ch1[2];
/*
			for (j = 0; j < 4; j++)
				stream << ch1[j];
*/

			quint16 data;
			unsigned char ch[2];
			// 模拟量
			for (j = 0; j < m_pComtrade->m_nAnaChannelNum; j++)
			{
				Analog* ana = m_pComtrade->analog(j);
				data = (quint16)(ana->Value(i) / ana->m_fScale);
				memcpy(&ch, &data, 2);
				stream << ch[1];
				stream << ch[0];
			}
			// 开关量
			for (j = 0; j < digcount; j++)
			{
				data = 0;
				for (int k = 0; k < 16; k++)
				{
					uint n = k + j * 16;
					if (n >= m_pComtrade->m_nDigChannelNum)
						break;
					Digtal* dig = m_pComtrade->digtal(n);
					n = (uint)dig->Value(i);
					n <<= k;
					data |= n;
				}
				memcpy(&ch, &data, 2);
				stream << ch[1];
				stream << ch[0];
			}
			i++;
		}
		file.close();
		b = true;
	}
	return b;    
}

bool File_operate::load_config(const QString& fileName)
{
	QString cfg = change_file_ext(fileName, ".cfg");
// 	const char* p = _Q2C(cfg);
	if (!QFile::exists(cfg))
		return false;
	
	return read_cfg(cfg);
}
