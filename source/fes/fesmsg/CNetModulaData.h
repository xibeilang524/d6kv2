#ifndef CNETMODULADATA_H_
#define CNETMODULADATA_H_


#include <qglobal.h>
#include <QString>
#include <QLatin1String>
#include <QList>
#include <QDateTime>

#include "fes/fesscdshm.h"
#include "Protocol_IEC104.h"



//报文存储类
class CNetModulaData
{
public:
	CNetModulaData();
	~CNetModulaData();
//	bool MatchKeyWord(CNetKeyWord* pKeyWord);

private:
	char* m_pData;
	int m_nLength;
	int m_nType;            //判断报文是上下行
	uchar* m_pKeyFlag;
	QString m_exchange;   //char型转换过后存储到QString里
	//int checkbox_type;
	int cdt_frame_count;  //用"*"分割CDT字符串，用以显示循环长度
	QString protocol_type;   //0703添加
	
	QDateTime msg_time;
public:
	

	bool SetData(char* pData, int nLength);
	void SetType(int nType) { m_nType = nType; };
	void SetProtolType(QString qprotocol){ protocol_type = qprotocol; };
	void SetTime(QDateTime datetime){ msg_time = datetime; };
	QString GetProtolType(){return protocol_type; };
	int GetCDTFramecount(){return cdt_frame_count;};
	char* GetData() { return m_pData; };  //取用出存储的字符串供显示报文
	int GetDataLength() { return m_nLength; };  //获得收发报文长度
	int GetType() { return m_nType; };
	uchar* GetKeyFlag() { return m_pKeyFlag; }
	

	void analyze_iec104_asc( char* pData, int nLength);
	void analyze_cdt_asc( BYTE* pData, int nLength, int nType);
	void cdt_untotal_frame(int nType) {m_exchange += _C2Q("非CDT完整帧，下帧合并处理");};
	void cdt_rebuild( char* pData, int nLength, int nType);
	int GetexDataLength() { return m_exchange.length(); };
    P104_APDU *apdu_104;
	QString GetExchange() { return m_exchange; };  //返回存储的字符串，便于显示

	QString GetStrTime(){return msg_time.toString("yyyy-MM-dd hh:mm:ss.zzz");};
};

#endif