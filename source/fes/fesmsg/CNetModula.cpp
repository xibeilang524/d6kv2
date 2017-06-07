#include "CNetModula.h"
#include "CNetModulaData.h"
//#include "vld.h"

#include <string>
#include <QTextStream>
#include <QFile>
#include <QMessageBox>
#include <QDateTime>
#include <QString>
#include <QDir>
#include <QByteArray>
#include <QCoreApplication>
#include <QDebug>

float g_LogFileSize = 10.0;
int   g_LogFileNum  = 5;

CNetModula::CNetModula()
{
	m_nTotalBytes = 0;
	m_bPause = false;
	m_nMaxBytes = NETVIEW_MAXBYTE;
	m_bSaveToFile = false;
	m_saveFile = NULL;
}

CNetModula::~CNetModula()
{
	ClearListData();
}

void CNetModula::ClearListData()
{
	while(!m_ListData.isEmpty())
	{
		delete m_ListData.takeFirst();
	}

	m_nTotalBytes = 0;
}

//		m_Modula->AddData(buf, sendNum, 2);
bool CNetModula::AddData(char* pData, int nLength, int nType, QString qprotocol, QDateTime save_begintime, QDateTime save_endtime,\
						 int save_checkbox, QString channel_name, FETIME* msgtime)
{
	if(GetSaveToFile())
		TraceToFile(pData,nLength);

	int checkbox_state = save_checkbox;
		
	if(m_bPause)
		return false;

	QDateTime current_msgtime;
	current_msgtime.setDate(QDate(msgtime->year+1900,msgtime->month,msgtime->day));
	current_msgtime.setTime(QTime(msgtime->hour,msgtime->minute,msgtime->second,msgtime->milisecond));

	//定义并初始化数据存储类
	CNetModulaData* pDataObj;
	pDataObj = new CNetModulaData();

	if(!pDataObj)
	{
		return false;
	}

	pDataObj->SetProtolType(qprotocol);   //规约类型
	pDataObj->SetType(nType);        //收发类型
	pDataObj->SetTime(current_msgtime);

	if(!pDataObj->SetData(pData, nLength))
	{
		delete pDataObj;
		return false;
	}

	{
		m_ListData.push_back(pDataObj);  //public m_ListData		
		m_nTotalBytes += nLength;
	}

	if (pDataObj)
	{
		if ((save_checkbox == 1) && (save_begintime < save_endtime))
		{
			char file_path[ 256 ] ;
			memset(file_path,0,256);
			sprintf(file_path,"%s/message/",getenv("NBENV"));
			QDir dir;  
			dir.mkpath(file_path);   //必须和后面路径一致

			QString file_name = _C2Q( file_path ) ;
			file_name += channel_name;
			file_name += "_";
			file_name += save_begintime.toString("hh_mm_ss_zzz");
			file_name += "-";
			file_name += save_endtime.toString("hh_mm_ss_zzz");
			file_name += ".txt";

			QFile file(file_name);  //创建不成功

			if (!file.open(QFile::Append | QFile::WriteOnly | QFile::Text))
			{
				return false;
			}
			QTextStream out(&file);		
	
			if (file.exists())
			{
				if ((current_msgtime >= m_save_begintime) && (current_msgtime <= m_save_endtime))
				{
					AutoSaveMessage(pDataObj, save_begintime, save_endtime, out);
				}
				else
					file.close();
			}
			
		}
	}

	while(m_nTotalBytes > m_nMaxBytes)		//缓存过大时，删除4帧报文
		RemoveData(4);



	return true;
}

void CNetModula::AutoSaveMessage(CNetModulaData* pModulaData, QDateTime save_begintime, QDateTime save_endtime, QTextStream &out)
{
	char filename[256];
	QString strTitle;
	
	if(!pModulaData)
		return;

	switch(pModulaData->GetType())
	{
		case 1:
			strTitle.sprintf("RECV(%d)", pModulaData->GetDataLength());
			strTitle += _C2Q("        当前时间:");
			strTitle += pModulaData->GetStrTime();
			strTitle += _C2Q("\n");
		break;	
		case 2:
			strTitle.sprintf("SEND(%d)", pModulaData->GetDataLength());
			strTitle += _C2Q("        当前时间:");
			strTitle += pModulaData->GetStrTime();
			strTitle += _C2Q("\n");
		break;
		default:
			strTitle.sprintf("UNKNOW(%d)\n", pModulaData->GetDataLength());
		break;
	}

	if (1 == pModulaData->GetType() || 2 == pModulaData->GetType())
	{
		out << strTitle;
	}

	if (1 == pModulaData->GetType() || 2 == pModulaData->GetType())
	{
		QString strTemp;
		int nCount = 0;

		while(nCount < pModulaData->GetDataLength())
		{
			for(int i = 0; i < 16; i++)
			{
				if(nCount + i < pModulaData->GetDataLength())
				{
					strTemp.sprintf("%02X ", (BYTE)(*(pModulaData->GetData()+nCount+i)));
					out << strTemp;
				}
			}
			out << QString("   ");
			out << QString("\n");
			nCount += 16;
		}
	}




}

void CNetModula::RemoveData(int nTotal)
{
	while(nTotal)
	{
		if(m_ListData.isEmpty())
			break;

		CNetModulaData* pDataObj = m_ListData.takeFirst();
		m_nTotalBytes -= pDataObj->GetDataLength();
		delete pDataObj;
		nTotal--;
	}
}
#define TIMEDATE_FORMAT_MS "yyyy-MM-dd hh:mm:ss.zzz"

void CNetModula::TraceToFile(char* pdata,int nlength)
{
	QDateTime timestamp = QDateTime::currentDateTime();
	if ( timestamp > GetEndTime()) return;
	
	if (!m_saveFile)
	{
		QString filePath = GetSavePath();
		m_saveFile = new QFile(filePath);
		if(!m_saveFile->open(QFile::Append | QFile::WriteOnly | QFile::Text))
		{
			delete m_saveFile;
			m_saveFile = NULL;
			return;
		}
	}
	
	#ifndef ACE_WIN32
	if ( m_saveFile->size() > int(g_LogFileSize*1024*1024 )) //g_LogFileSize Mbyte
	{
		QString str = "";
		m_saveFile->close();
		if (g_LogFileNum > 100)
			g_LogFileNum = 100;
		int j,k;
		QString strj,strk;
		for (int i=0 ; i<=(g_LogFileNum-2) ; i++)
		{	
			 j = g_LogFileNum - i - 2;
			 k = g_LogFileNum - i - 1;
			 strj.sprintf("%d",j);
			 strk.sprintf("%d",k);
			if ( j != 0)
				str+= "mv '" + GetSavePath() + ".old" + strj + "' '" +  GetSavePath() + ".old" + strk +"' ;" ;
			else
				str+= "mv '" + GetSavePath() + "' '" +  GetSavePath() + ".old1' ;";
		}
		/*str = "mv '" + GetSavePath() + ".old4' '" +  GetSavePath() + ".old5' ;";
		str+= "mv '" + GetSavePath() + ".old3' '" +  GetSavePath() + ".old4' ;";
		str+= "mv '" + GetSavePath() + ".old2' '" +  GetSavePath() + ".old3' ;";
		str+= "mv '" + GetSavePath() + ".old1' '" +  GetSavePath() + ".old2' ;";
		str+= "mv '" + GetSavePath() + "' '" +  GetSavePath() + ".old1' ;";*/
		if ( system((const char*)(str.toStdString().c_str())) != -1)
		{	
			if(!m_saveFile->open(QFile::Append | QFile::WriteOnly | QFile::Text))
			{
				delete m_saveFile;
				m_saveFile = NULL;
				return;
			}
		}
		else
		{	
			return;
		}
	}
	#endif
	
	
	QTextStream out(m_saveFile);
	QString timestr = timestamp.toString(TIMEDATE_FORMAT_MS);
	out<<timestr;
	out<<QString(" ");
	char*  buffer = new char[nlength + 1 ];
	memset( buffer, '\0', nlength + 1 );
	memcpy( buffer, pdata, nlength );
	out<<buffer;
	out<<QString("\n");
	delete []buffer;
}

void CNetModula::SetSaveToFile(bool b)
{
	m_bSaveToFile = b;
	if (b)
	{
		if(!m_saveFile)
		{
			QString filePath = GetSavePath();
			m_saveFile = new QFile(filePath);
			if(!m_saveFile->open(QFile::Append | QFile::WriteOnly | QFile::Text))
			{
				delete m_saveFile;
				m_saveFile = NULL;
				return;
			}
		}
	}
	else
	{
		if(m_saveFile != NULL)
		{
			m_saveFile->close();
			delete m_saveFile;
			m_saveFile = NULL;
		}
	}
}

