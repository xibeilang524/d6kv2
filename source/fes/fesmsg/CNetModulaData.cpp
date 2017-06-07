#include "CNetModulaData.h"
#include "string.h"
#include "stdio.h"
#include "Protocol_IEC104.h"
#include "Protocol_CDT92.h"
#include "msgview_string.h"
#include "Protocol_process.h"
#include "msgviewwnd.h"
//#include "vld.h"

#include <QtCore/QCoreApplication>
#include <QString>
#include <QLatin1String>
#include <QByteArray>
#include <QObject>
#include <QDebug>
#include <QList>
#include <QListIterator>
#include <QMutableListIterator>


CNetModulaData::CNetModulaData()
{
	m_pData = NULL;
	//m_exData = NULL;
	m_pKeyFlag = NULL;
	m_nLength = 0;
	m_nType = -1;
	m_exchange = "\0";

	cdt_frame_count = 0;
	protocol_type = "";   //0703添加
	//checkbox_type = 0;
}

CNetModulaData::~CNetModulaData()
{
	if(m_pData)
		delete[] m_pData;

	if(m_pKeyFlag)
		delete[] m_pKeyFlag;


}

//报文存储
bool CNetModulaData::SetData(char* pData, int nLength)
{
	if(m_pData)
	{
		delete[] m_pData;
		m_pData = NULL;
		m_nLength = 0;
	}



	if(m_pKeyFlag)
	{
		delete[] m_pKeyFlag;
		m_pKeyFlag = NULL;
	}

	m_pData = new char[nLength];
	if(!m_pData)
	{
		return false;
	}

	//m_exData = new char[10240];
	//if(!m_exData)
	//{
	//	return false;
	//}

	m_pKeyFlag = new uchar[nLength];
	if(!m_pKeyFlag)
	{
		delete[] m_pData;
		m_pData = NULL;
		return false;
	}
	
	//数据存储，拷贝到m_pData数组中
	memcpy(m_pData, pData, nLength); 
	m_nLength = nLength;




 
	//根据规约类型选择解析规约
	if (protocol_type == "iec104" )   //104规约
	{
//		analyze_iec104_asc(m_pData, m_nLength);
	}

	if (protocol_type == "CDT" )    //CDT规约
	{			
		cdt_rebuild(m_pData, m_nLength, m_nType);
	}



	
	//char存储到QString里
	//for (int i = 0; i < GetexDataLength(); i++)
	//{	
	//	char tmp[5];
	//	sprintf(tmp, "%02X", (unsigned char)*m_exData);


	//	m_exchange += tmp;
	//	//m_exchange += " ";
	//	m_exData++;
	//}

	//m_pData = m_tmp_pData;

	//for (int i = 0; i < m_nLength; i++)
	//{    
	//	bool ok;
	//	QString m_tmp = QString::number((unsigned char)*m_pData,16); 


	//	//接收字符为0x00，变字符串为00
	//	if (m_tmp.toInt(&ok,10) == 0)
	//	{
	//		m_tmp = ("00");
	//	}
	//	//接收字符为0x04,0x07等，补足0
	//	if ((m_tmp.toInt(&ok,10) < 10) && (m_tmp.toInt(&ok,10) > 0))
	//	{
	//		m_tmp = m_tmp.insert(0, "0");
	//	}

	//	m_exchange.append(m_tmp);
	//	m_exchange.append(" ");
	//	m_pData++;
	//	m_tmp.clear();
	//}




	memset(m_pKeyFlag, 0, nLength*sizeof(uchar));

	return true;
}
//
//bool CNetModulaData::Exchange(char* m_pData)
//{
//	QString m_exchange = QString(QLatin1String(m_pData));
//}

void CNetModulaData::analyze_iec104_asc( char* m_pData, int nLength)
{
     char temp[2];   //原始char数组
     int pos = 0;   //  pos/2是字节数
     int cur_position = 0;  //
     int cnt = 0;  //帧数目（报文数）
     memset(temp,0,2);
	 int i;
	 int frame_length;  //frame_length + 2	
	 int frame_len = 0;
	 int ms = 0;
	 bool Is_wrong = false;
	 int byte_num = 0;
	 byte_num = nLength;



	 int sendnum = 0;
	 int recvnum = 0;

	// m_exchange = new QString;   //0717




	 while (cur_position < nLength/2) // pos/2是字节数
	 {
		 memset(temp,0,2);


	 //报文头不对
	 if((BYTE)m_pData[cur_position] != FIRST)  
	 {
		 frame_length = (BYTE)m_pData[cur_position+1];
		 for (cur_position = 0; cur_position < frame_length + 2; cur_position++)
		 {
			 sprintf(temp, "%02X", (BYTE)m_pData[cur_position]);
			 m_exchange += temp;
			 m_exchange += remind_space;
		 }
		 m_exchange += remind_first_error; 
		 cur_position = 0;
		 continue;
	 }

	 if ((byte_num -cur_position)%6!=0)
	 {
		 if(((BYTE)m_pData[cur_position + 7] & 0x7f) == 0)
		 {
			 sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);
			 m_exchange += temp;
			 m_exchange += remind_first_error;
			 cur_position++;
			 continue;
		 }
		 else if((((BYTE)m_pData[cur_position + 1] - 10) % ((BYTE)m_pData[cur_position + 7] & 0x7f)) != 0 
			 && (((BYTE)m_pData[cur_position + 1] - 13) % ((BYTE)m_pData[cur_position + 7] & 0x7f)) != 0 )
		 {

			 sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);
			 m_exchange += temp;
			 m_exchange += remind_first_error;
			 cur_position++;
			 continue;
		 }
	 }


	 //报文长度不对
	 if((BYTE)m_pData[cur_position+1] < S_LEN)  
	 {
		 frame_length = (BYTE)m_pData[cur_position+1];
		 for (cur_position = 0; cur_position < frame_length + 2; cur_position++)
		 {
			 sprintf(temp, "%02X", (BYTE)m_pData[cur_position]);
			 m_exchange += temp;
			 m_exchange += remind_space;
		 }
		 m_exchange += remind_second_error; 
		 cur_position = 0;
		continue;
	 }

	 if (((BYTE)m_pData[(BYTE)m_pData[cur_position+1] + cur_position + 2] != FIRST  //报文头
		 &&(BYTE)m_pData[cur_position+1] + cur_position + 2 < byte_num)             //报文长度
		 || (BYTE)m_pData[cur_position+1] + cur_position + 2 > byte_num)            //报文长度
	 {
		 int tem_position = cur_position + 1;
		 Is_wrong = true;
		 /*寻找下一帧报文的报头位置*/

		 while(tem_position < byte_num/2)
		 {

			 if((BYTE)m_pData[tem_position] == FIRST)
			 {
				 if(((BYTE)m_pData[tem_position + 7] & 0x7f) != 0)
				 {
					 if((((BYTE)m_pData[tem_position + 1] - 10) % ((BYTE)m_pData[tem_position + 7] & 0x7f)) == 0 
						 || (((BYTE)m_pData[tem_position + 1] - 13) % ((BYTE)m_pData[tem_position + 7] & 0x7f)) == 0)
					 {
						 break;
					 }
				 }
			 }
			 tem_position++;
		 }
		 frame_len = tem_position - cur_position;
	 }

	 if(Is_wrong)
	 {
		 if(frame_len < 12)
		 {
			 cnt++;
			 cur_position+=frame_len;
			 continue;
		 }
	 }


	 /*****************************正常报文处理部分********************************/


	 //报文头
     sprintf(temp, "%02X", (BYTE)m_pData[cur_position]);
	 m_exchange += temp;
	 m_exchange += remind_first;


	 //数据长度
	 sprintf(temp, "%02X", (BYTE)m_pData[cur_position+1]);
	 m_exchange += temp;
	 m_exchange += remind_len;
	 sprintf(temp,"%d",(BYTE)m_pData[cur_position+1]);
	 m_exchange += temp;
	 m_exchange += remind_rightkuohao;
	 m_exchange += remind_space;

	 apdu_104=(P104_APDU*)&m_pData[cur_position];
	 cur_position += 2;   //移到控制域部分


	 
	 //控制域,判断帧类型
     switch (m_pData[cur_position]&0x03)
	 {
	  case 0x00:   //I帧 68 16 0e 00 04 00 01 03 03 00 02 00 01 00 00 01 03 00 00 01 02 00 00 01 
	  case 0x02:
		 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  // 0E
		 m_exchange += temp;
		 m_exchange += remind_second_type_i;

		 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //00
		 m_exchange += temp;
		 m_exchange += remind_leftkuohao;
		 m_exchange += remind_send_no;
		 sendnum = (apdu_104->apci.type_i.ns.num&0x00ff)>>1;
		 sendnum += apdu_104->apci.type_i.ns.num&0xff00;     //0E 00 发送序列号:7
		 sprintf(temp,"%d",sendnum);
		 m_exchange += temp;
		 m_exchange += remind_rightkuohao;
		 m_exchange += remind_space;

		 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //04
		 m_exchange += temp;
		 m_exchange += remind_space;
		 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //00
		 m_exchange += temp;
		 m_exchange += remind_leftkuohao;
		 m_exchange += remind_recv_no;
		 recvnum = (apdu_104->apci.type_i.nr.num&0x00ff)>>1;
		 recvnum += apdu_104->apci.type_i.nr.num&0xff00;
		 sprintf(temp,"%d",recvnum);   //04 00 接收序列号 2
		 m_exchange += temp;
		 m_exchange += remind_rightkuohao;
		 m_exchange += remind_space;
		 break;

	 case 0x01:   //S帧 68 04 01 00 02 00
		 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  // 04
		 m_exchange += temp;
		 m_exchange += remind_second_type_s;   //S帧

		 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  // 01
		 m_exchange += temp;
		 m_exchange += remind_control_s; 

		 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  // 02
		 m_exchange += temp;
		 m_exchange += remind_space;

		 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  // 00
		 m_exchange += temp;
		 m_exchange += remind_leftkuohao;
		 m_exchange += remind_recv_no;
		 recvnum = (apdu_104->apci.type_i.nr.num&0x00ff)>>1;
		 recvnum += apdu_104->apci.type_i.nr.num&0xff00;
		 sprintf(temp,"%d",recvnum);   //02 00 接收序列号 1
		 m_exchange += temp;
		 m_exchange += remind_rightkuohao;
		 m_exchange += remind_space;
		 return;
	 case 0x03:  //U帧 68 04 07 00 00 00
		 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  // 07
		 m_exchange += temp;
		 m_exchange += remind_second_type_u;   //U帧
		

		 switch(apdu_104->apci.type_u.u_type) //控制域类型判断
		 {
		 case STARTDT_ACT:  //主站启动激活
			 m_exchange += remind_start_act;
			 break;
		 case STARTDT_CON:  //子站确认激活
			 m_exchange += remind_start_con;
			 break;
		 case STOPDT_ACT:
			 m_exchange += remind_stop_act;
			 break;
		 case STOPDT_CON:
			 m_exchange += remind_stop_con;
			 break;
		 case TESTFR_ACT:
			 m_exchange += remind_test_act;
			 break;
		 case TESTFR_CON:
			 m_exchange += remind_test_con;
			 break;
		 default:
			 break;
		 }
         
		  sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  // 00
		  m_exchange += temp;
		  m_exchange += remind_space;

		  sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  // 00
		  m_exchange += temp;
		  m_exchange += remind_space;

		  sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  // 00
		  m_exchange += temp;
		  m_exchange += remind_control_u;
		  return;
	 default:
		 break;

	 }

	 /*****************************************ASDU部分**********************************************/
	 //68 1E 04 00 02 00 03 05 14 00 01 00 01 00 00 02 06 00 00 02 0A 00 00 01 0B 00 00 02 0C 00 00 01
	 //ASDU->TYPE
	 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]); //类型标识 03
	 m_exchange += temp;
	 m_exchange += remind_leftkuohao;
	 m_exchange += remind_type;
	 sprintf(temp,"%d",apdu_104->asdu.type);
	 m_exchange += temp;
	 m_exchange += remind_space;

	 switch(apdu_104->asdu.type)
	 {
	 case SP_NA_1:
		 m_exchange += remind_no_time_single_signal;
		 break;
	 case DP_NA_3:
		  m_exchange += remind_no_time_double_signal;
		 break;
	 case ME_NA_9:
		  m_exchange += remind_measuring;
		 break;
	 case ME_ND_21:
		 m_exchange += remind_no_time_measuring;
		 break;
	 case SP_TB_30:
		 m_exchange += remind_time_single_signal;
		 break;
	 case DP_TB_31:
		 m_exchange += remind_time_double_signal;
		 break;
	 case ME_TD_34:
		 m_exchange += remind_time_quality_measuring;
		 break;
	 case SC_NA_45:
		 m_exchange += remind_single_command;
		 break;
	 case DC_NA_46:
		 m_exchange += remind_double_command;
		 break;
	 case SE_NA_48:
		 m_exchange += remind_no_time_control_measuring;
		 break;
	 case SC_NA_58:
		 m_exchange += remind_time_single_command;
		 break;
	 case DC_NA_59:
		 m_exchange += remind_time_double_command;
		 break;
	 case SE_TA_61:
		 m_exchange += remind_time_control_measuring;
		 break;
	 case IC_NA_100:
		 m_exchange += remind_total_call;
		 break;
	 case CI_NA_101:
		 m_exchange += remind_energy_call;
		 break;
	 case CS_NA_103:
		 m_exchange += remind_clock_sync;
		 break;
	 case M_EI_NA_70:
		 m_exchange += remind_initialization;
		 break;
	 default:
		 m_exchange += _C2Q("未知报文类型");
		 break;
	 }

	 m_exchange += remind_rightkuohao;
	 m_exchange += remind_space;

	 //ASDU->VSQ
	 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]); //类型标识 05
	 m_exchange += temp;
	 m_exchange += remind_leftkuohao;
	 m_exchange += remind_num;
	 sprintf(temp,"%d ",(apdu_104->asdu.vsq)&0x7F);
	 m_exchange += temp;
	 m_exchange += remind_space;

	 if ((apdu_104->asdu.vsq&0x80) == 0)
	 {
		 m_exchange += remind_nonsequenced;
	 }
	 else
	 {
		  m_exchange += remind_sequenced;
	 }

	 m_exchange += remind_rightkuohao;
	 m_exchange += remind_space;

     //传送原因
     sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  //14
	 m_exchange += temp;
	 m_exchange += remind_space;

	 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  //00
	 m_exchange += temp;
	 m_exchange += remind_leftkuohao;
	 m_exchange += remind_transmit_reason;
	 sprintf(temp,"%d ",apdu_104->asdu.cot);    //cot

	 switch(apdu_104->asdu.cot&0x00ff)
	 {
	 case COT_SPONT:  //突发
		 m_exchange += remind_transmit_reason_burst_3;
		 break;
	 case COT_ACT:   //激活
		 m_exchange += remind_transmit_reason_activation_6;
		 break;
	 case COT_ACTCON:    //激活确认
		 m_exchange += remind_transmit_reason_activation_confirm_7;
		 break;
	 case COT_DEACT:   //停止激活
		 m_exchange += remind_transmit_reason_activation_stop_8;
		 break;
	 case COT_DEACTCON:  //停止激活确认
		 m_exchange += remind_transmit_reason_activation_stop_confirm_9;
		 break;
	 case COT_ACTTERM:   //激活终止
		 m_exchange += remind_transmit_reason_activation_teminate_10;
		 break; 
	 case COT_INRO:   //相应站召唤
		 m_exchange += remind_transmit_reason_respond_station_call_20;
		 break;
	 case COT_UNKOWN_TYPE:
		 break;
	 case COT_UNKOWN_COT:  
		 break;
	 case COT_UNKOWN_ADDR: 
		 break;
	 case COT_UNKOWN_INDEX:
		 break;
	 case ACTIVATION:
		 m_exchange += remind_transmit_reason_activation_6;
		 break;
	 case ACTIVATION_CONFIRM:  
		 m_exchange += remind_transmit_reason_activation_confirm_7;
		 break;
	 default:
		 if ((apdu_104->asdu.cot&0x00ff)>20 && (apdu_104->asdu.cot&0x00ff)<37)
		 {
			 m_exchange += _C2Q("响应第");
			 sprintf(temp,"%d",(apdu_104->asdu.cot&0x00ff)-20);
			 m_exchange += temp;
			 m_exchange += _C2Q("组召唤");
		 }
		 else
		 {
			 m_exchange += _C2Q("未知原因");
		 }
		 break;
	 }

	 m_exchange += remind_rightkuohao;
	 m_exchange += remind_space;

	 //终端地址
	 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  // 02
	 m_exchange += temp;
	 m_exchange += remind_space;

	 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  // 00
	 m_exchange += temp;
	 m_exchange += remind_leftkuohao;
     m_exchange += remind_transmit_addr;
	 sprintf(temp,"%d",apdu_104->asdu.addr);
	 m_exchange += temp;
     m_exchange += remind_rightkuohao;
	 m_exchange += remind_space;

	 //数据部分
	 int info_addr = 0;
	 int info_num = apdu_104->asdu.vsq&0x7F;  //信息个数
	 int unit_byte = 0;
	 int des_len = 12;

	 /*根据报文类型进行不同的处理*/
	 switch(apdu_104->asdu.type)
	 {
		case SP_NA_1://单点遥信

			if(Is_wrong)
			{
				unit_byte = 4;
				if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
				{
					info_num = (frame_len - des_len)/unit_byte;
				}

			}

			//ASDU DATA部分
			//01 00 00 01
			//03 00 00 01
			//02 00 00 01
		if ((apdu_104->asdu.vsq&0x80) == 0)//判断是否为顺序，信息地址和信息一起顺序排放
		{

			for(int i = 0; i < info_num; i++)
			{
				info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) + 1;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  // 01
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  // 00 
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  // 00
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_yx_addr;
				sprintf(temp,"%d",info_addr);
				m_exchange += temp;    //信息地址
				m_exchange += remind_rightkuohao;   
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);    //信息体信息 01
				m_exchange += temp;
				m_exchange += remind_leftkuohao;

				m_exchange += remind_siq;
				if((m_pData[cur_position]&0x01) == 0x00)
				{
					m_exchange += remind_open;   //遥信分位
				}
				else
				{
					m_exchange += remind_close;   //遥信合位
				}

				m_exchange += remind_space;

				if ((m_pData[cur_position]&0x10) == 0x00)
				{
					m_exchange += remind_unblock;     //解锁
				}
				else
				{
					m_exchange += remind_block;  //闭锁
				}

				m_exchange += remind_space;

				if ((m_pData[cur_position]&0x20) == 0x00)
				{
					m_exchange += remind_no_replace;  //未取代
				}
				else
				{
					m_exchange += remind_replace;  //取代
				}

				m_exchange += remind_space;

				if ((m_pData[cur_position]&0x40) == 0x00)
				{ 
					m_exchange += remind_current_value;  //当前值
				}
				else
				{
					m_exchange += remind_no_current_value;  //非当前值
				}

				m_exchange += remind_space;

				if (((BYTE)m_pData[cur_position]&0x80) == 0x00)
				{
					m_exchange += remind_valid;   //有效
				}
				else
				{
					m_exchange += remind_invalid;  //无效
				}
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;
			}

		}    //LINE 711

		else    //只送信息起始地址，信息顺序排放
		{
			info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) + 1;  

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //01
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //00 
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //00
			m_exchange += temp;
			m_exchange += remind_space;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_yx_addr;
			sprintf(temp,"%d",info_addr);
			m_exchange += temp;    //信息地址
			m_exchange += remind_rightkuohao;   
			m_exchange += remind_space;

			if(Is_wrong)
			{
				unit_byte = 1;
				des_len += 3;
				if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
				{
					info_num = (frame_len - des_len)/unit_byte;
				}
			}

			for(int i = 0; i < info_num; i++)
			{

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);    //信息体信息 01
				m_exchange += temp;
				m_exchange += remind_leftkuohao;

				m_exchange += remind_siq;
				if((m_pData[cur_position]&0x01) == 0x00)
				{
					m_exchange += remind_open;   //遥信分位
				}
				else
				{
					m_exchange += remind_close;   //遥信合位
				}

				m_exchange += remind_space;

				if ((m_pData[cur_position]&0x10) == 0x00)
				{
					m_exchange += remind_unblock;     //解锁
				}
				else
				{
					m_exchange += remind_block;  //闭锁
				}

				m_exchange += remind_space;

				if ((m_pData[cur_position]&0x20) == 0x00)
				{
					m_exchange += remind_no_replace;  //未取代
				}
				else
				{
					m_exchange += remind_replace;  //取代
				}

				m_exchange += remind_space;

				if ((m_pData[cur_position]&0x40) == 0x00)
				{ 
					m_exchange += remind_current_value;  //当前值
				}
				else
				{
					m_exchange += remind_no_current_value;  //非当前值
				}

				m_exchange += remind_space;

				if (((BYTE)m_pData[cur_position]&0x80) == 0x00)
				{
					m_exchange += remind_valid;   //有效
				}
				else
				{
					m_exchange += remind_invalid;  //无效
				}
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
			    cur_position++;
			}

		}
		break;

	case DP_NA_3://双点遥信
		//68 1E 04 00 02 00 03 05 14 00 01 00 01 00 00 02 06 00 00 02 0A 00 00 01 0B 00 00 02 0C 00 00 01
	    if ((apdu_104->asdu.vsq&0x80) == 0)
		{	
				if(Is_wrong)
				{
					unit_byte = 4;
					if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
					{
						info_num = (frame_len - des_len)/unit_byte;
					}
				}

				for(int i = 0; i < info_num; i++)
				{
					info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) + 1;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //01
					m_exchange += temp;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  //00
					m_exchange += temp;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //00 以上3位为地址
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_yx_addr;
					sprintf(temp,"%d",info_addr);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //数据
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_diq;

					switch(m_pData[cur_position]&0x03) 
					{
					case 0:
						m_exchange += remind_uncertainty_double;
						break;
					case 1:
						m_exchange += remind_open_double;
						break;
					case 2:
						m_exchange += remind_close_double;
						break;
					case 3:
						m_exchange += remind_uncertainty_double;
						break;
					default:
						break;
					}

					m_exchange += remind_space;

					if ((m_pData[cur_position]&0x10) == 0x00)
					{
						m_exchange += remind_unblock;
					}
					else
					{
						m_exchange += remind_block;
					}

					m_exchange += remind_space;

					if ((m_pData[cur_position]&0x20) == 0x00)
					{
						m_exchange += remind_no_replace;
					}
					else
					{
						m_exchange += remind_replace;
					}

					m_exchange += remind_space;

					if ((m_pData[cur_position]&0x40) == 0x00)
					{
						m_exchange += remind_current_value;
					}
					else
					{
						m_exchange += remind_no_current_value;
					}

					m_exchange += remind_space;

					if (((BYTE)m_pData[cur_position]&0x80) == 0x00)
					{
						m_exchange += remind_valid;
					}
					else
					{
						m_exchange += remind_invalid;
					}

					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;
					cur_position++;
				}
		 }

		 else
		 {
			 info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) + 1;

			 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			 m_exchange += temp;
			 m_exchange += remind_space;

			 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			 m_exchange += temp;
			 m_exchange += remind_space;

			 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			 m_exchange += temp;
			 m_exchange += remind_space;
			 m_exchange += remind_leftkuohao;
			 m_exchange += remind_yx_addr;
			 sprintf(temp,"%d",info_addr);
			 m_exchange += temp;    //信息地址
			 m_exchange += remind_rightkuohao;   
			 m_exchange += remind_space;


			 if(Is_wrong)
			 {
				 unit_byte = 1;
				 des_len += 3;
				 if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
				 {
					 info_num = (frame_len - des_len)/unit_byte;
				 }
			 }

			 for(int i = 0; i < info_num; i++)
			 {
				 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
				 m_exchange += temp;
				 m_exchange += remind_leftkuohao;
				 m_exchange += remind_diq;

				 switch(m_pData[cur_position]&0x03) 
					{
				 case 0:
					 m_exchange += remind_uncertainty_double;
					 break;
				 case 1:
					 m_exchange += remind_open_double;
					 break;
				 case 2:
					 m_exchange += remind_close_double;
					 break;
				 case 3:
					 m_exchange += remind_uncertainty_double;
					 break;
				 default:
					 break;
					}

				 m_exchange += remind_space;

				 if ((m_pData[cur_position]&0x10) == 0x00)
					{
						m_exchange += remind_unblock;
					}
				 else
					{
						m_exchange += remind_block;
					}

				 m_exchange += remind_space;

				 if ((m_pData[cur_position]&0x20) == 0x00)
					{
						m_exchange += remind_no_replace;
					}
				 else
					{
						m_exchange += remind_replace;
					}

				 m_exchange += remind_space;

				 if ((m_pData[cur_position]&0x40) == 0x00)
					{
						m_exchange += remind_current_value;
					}
				 else
					{
						m_exchange += remind_no_current_value;
					}

				 m_exchange += remind_space;

				 if (((BYTE)m_pData[cur_position]&0x80) == 0x00)
					{
						m_exchange += remind_valid;
					}
				 else
					{
						m_exchange += remind_invalid;
					}

				 m_exchange += remind_rightkuohao;
				 m_exchange += remind_space;
				 cur_position++;
			 }
		}	
		break;

	case ME_NA_9://测量值，归一化值

		if ((apdu_104->asdu.vsq&0x80) == 0)
	  {
		  if(Is_wrong)
		  {
			  unit_byte = 6;
			  if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
			  {
				  info_num = (frame_len - des_len)/unit_byte;
			  }
		  }

		  for(int i = 0; i < info_num; i++)
		  {
			  info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 16384;

			  sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  //地址1字节
			  m_exchange += temp;
			  m_exchange += remind_space;

			  sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //地址2字节
			  m_exchange += temp;
			  m_exchange += remind_space;

			  sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //地址3字节
			  m_exchange += temp;
			  m_exchange += remind_leftkuohao;
			  m_exchange += remind_yc_addr;
			  sprintf(temp, "%d", info_addr);
			  m_exchange += temp;
			  m_exchange += remind_rightkuohao;
			  m_exchange += remind_space;                   

			  int mea_data = sghextoint_104((BYTE*)&m_pData[cur_position], 2);

			  sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			  m_exchange += temp;                                     //遥测数据字节1
			  m_exchange += remind_space;

			  sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			  m_exchange += temp;                                     //遥测数据字节2 + 解析
			  m_exchange += remind_leftkuohao;
			  m_exchange += remind_me_na;
			  sprintf(temp,"%d",mea_data);   //遥测值
			  m_exchange += temp;
			  m_exchange += remind_rightkuohao;
			  m_exchange += remind_space;

			  sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);     //遥测品质因数
			  m_exchange += temp; 
			  m_exchange += remind_leftkuohao;
			  m_exchange += remind_qds;

			  if((m_pData[cur_position]&0x01) == 0)
			  {
				  m_exchange += remind_no_overflow;
			  }
			  else
			  {
				  m_exchange += remind_overflow;
			  }

			  m_exchange += remind_space;

			  if((m_pData[cur_position]&0x10) == 0)
			  {
				  m_exchange += remind_unblock;
			  }
			  else
			  {
				  m_exchange += remind_block;
			  }

			  m_exchange += remind_space;

			  if((m_pData[cur_position]&0x20) == 0)
			  {
				  m_exchange += remind_no_replace;
			  }
			  else
			  {
				  m_exchange += remind_replace;
			  }

			  m_exchange += remind_space;

			  if((m_pData[cur_position]&0x40) == 0)
			  {
				  m_exchange += remind_current_value;
			  }
			  else
			  {
				  m_exchange += remind_no_current_value;
			  }

			  m_exchange += remind_space;

			  if(((BYTE)m_pData[cur_position++]&0x80) == 0)   //++ 移到下一组数据，两字节数据+一字节品质
			  {
				  m_exchange += remind_valid;
			  }
			  else
			  {
				  m_exchange += remind_invalid;
			  }	

			  m_exchange += remind_rightkuohao;
			  m_exchange += remind_space;
		  }

	  }

	else
	{
		info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 16384;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  //地址1字节
		m_exchange += temp;
		m_exchange += remind_space;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  //地址2字节
		m_exchange += temp;
		m_exchange += remind_space;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //地址3字节
		m_exchange += temp;
		m_exchange += remind_leftkuohao;
		m_exchange += info_addr;
		sprintf(temp, "%d", info_addr);
		m_exchange += temp;
		m_exchange += remind_rightkuohao;
		m_exchange += remind_space; 


		if(Is_wrong)
		{
			unit_byte = 3;
			des_len += 3;

			if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
			{
				info_num = (frame_len - des_len)/unit_byte;
			}
		}

	for(int i = 0; i < info_num; i++)
	{
			int mea_data = sghextoint_104((BYTE*)&m_pData[cur_position], 2);

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //遥测数据字节1
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //遥测数据字节2 + 解析
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_me_na;
			sprintf(temp,"%d",mea_data);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //品质因数
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_qds;

			if((m_pData[cur_position]&0x01) == 0)
			{
				m_exchange += remind_no_overflow;
			}
			else
			{
				m_exchange += remind_overflow;
			}

			if((m_pData[cur_position]&0x10) == 0)
			{
				m_exchange += remind_unblock;
			}
			else
			{
				m_exchange += remind_block;
			}
			if((m_pData[cur_position]&0x20) == 0)
			{
				m_exchange += remind_no_replace;
			}
			else
			{
				m_exchange += remind_replace;
			}
			if((m_pData[cur_position]&0x40) == 0)
			{
				m_exchange +=  remind_current_value;
			}
			else
			{
				m_exchange += remind_no_current_value;
			}
			  if(((BYTE)m_pData[cur_position++]&0x80) == 0)    //原始
			{
				m_exchange += remind_valid;
			}
			else
			{
				m_exchange += remind_invalid;
			}

			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

		}
	}
				
		  break;

case ME_ND_21://不带品质遥测
	if ((apdu_104->asdu.vsq&0x80) == 0)
	{
		if(Is_wrong)
		{
			unit_byte = 5;
			if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
			{
				info_num = (frame_len - des_len)/unit_byte;
			}
		}

		for(int i = 0; i < info_num; i++)
		{
			info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 16384;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //地址3字节解析
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_yc_addr;
			sprintf(temp,"%d",info_addr);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;


			int mea_data = sghextoint_104((BYTE*)&m_pData[cur_position], 2);

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //遥测字节1
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  //遥测字节2
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_me_na;
			sprintf(temp,"%d",mea_data);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;
		}

	}
	else
	{
		info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 16384;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
		m_exchange += temp;
		m_exchange += remind_space;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
		m_exchange += temp;
		m_exchange += remind_space;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //地址3字节
		m_exchange += temp;
		m_exchange += remind_leftkuohao;
		m_exchange += remind_yc_addr;
		sprintf(temp,"%d",info_addr);
		m_exchange += temp;
		m_exchange += remind_rightkuohao;
		m_exchange += remind_space;

		if(Is_wrong)
		{
			unit_byte = 2;
			des_len += 3;
			if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
			{
				info_num = (frame_len - des_len)/unit_byte;
			}
		}

		for(int i = 0; i < info_num; i++)
		{
			int mea_data = sghextoint_104((BYTE*)&m_pData[cur_position], 2);

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_me_na;
			sprintf(temp,"%d",mea_data);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

		}
	}
	break;

	case SP_TB_30://带时标单点遥信,SOE
		if ((apdu_104->asdu.vsq&0x80) == 0)
		{
			if(Is_wrong)
			{
				unit_byte = 11;
				if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
				{
					info_num = (frame_len - des_len)/unit_byte;
				}
			}

			for(int i = 0; i < info_num; i++)
			{
				info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) + 1;   //信息体地址3字节

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);     //地址字节1
				m_exchange += temp;
			    m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //地址字节2
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //地址字节3
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_yx_addr;
				sprintf(temp,"%d",info_addr);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //信息字节
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_siq;

				if((m_pData[cur_position]&0x01) == 0x00)
				{
					m_exchange += remind_open;
				}
				else
				{
					m_exchange += remind_close;
				}

				m_exchange += remind_space;

				if ((m_pData[cur_position]&0x10) == 0x00)
				{
					m_exchange += remind_unblock;
				}
				else
				{
					m_exchange += remind_block;
				}

				m_exchange += remind_space;

				if ((m_pData[cur_position]&0x20) == 0x00)
				{
					m_exchange += remind_no_replace;
				}
				else
				{
					m_exchange += remind_replace;
				}

				m_exchange += remind_space;

				if ((m_pData[cur_position]&0x40) == 0x00)
				{
					m_exchange += remind_current_value;
				}
				else
				{
					m_exchange += remind_no_current_value;
				}

				m_exchange += remind_space;

				if (((BYTE)m_pData[cur_position]&0x80) == 0x00)
				{
					m_exchange += remind_valid;
				}
				else
				{
					m_exchange += remind_invalid;
				}

				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//SOE时间部分处理
				ms = usghextoint((BYTE*)&m_pData[cur_position], 2);

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //毫秒低字节
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //毫秒高字节 + 解析
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_ms;
				sprintf(temp,"%d",ms);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //分钟
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_minute;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //小时
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_hour;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //日
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_day;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //月
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_month;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //年
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_year;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

			}

		}
		else
		{
			info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) + 1;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);     //地址字节1
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);     //地址字节2
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //地址字节3
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_yx_addr;
			sprintf(temp,"%d",info_addr);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			if(Is_wrong)
			{
				unit_byte = 8;
				des_len += 3;
				if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
				{
					info_num = (frame_len - des_len)/unit_byte;
				}
			}

			for(int i = 0; i < info_num; i++)
			{
				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //信息字节
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_siq;

				if((m_pData[cur_position]&0x01) == 0x00)
				{
					m_exchange += remind_open;
				}
				else
				{
					m_exchange += remind_close;
				}

				m_exchange += remind_space;

				if ((m_pData[cur_position]&0x10) == 0x00)
				{
					m_exchange += remind_unblock;
				}
				else
				{
					m_exchange += remind_block;
				}

				m_exchange += remind_space;

				if ((m_pData[cur_position]&0x20) == 0x00)
				{
					m_exchange += remind_no_replace;
				}
				else
				{
					m_exchange += remind_replace;
				}

				m_exchange += remind_space;

				if ((m_pData[cur_position]&0x40) == 0x00)
				{
					m_exchange += remind_current_value;
				}
				else
				{
					m_exchange += remind_no_current_value;
				}

				m_exchange += remind_space;

				if (((BYTE)m_pData[cur_position]&0x80) == 0x00)
				{
					m_exchange += remind_valid;
				}
				else
				{
					m_exchange += remind_invalid;
				}

				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//SOE时间部分处理
				ms = usghextoint((BYTE*)&m_pData[cur_position], 2);

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //毫秒低字节
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //毫秒高字节 + 解析
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_ms;
				sprintf(temp,"%d",ms);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //分钟
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_minute;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //小时
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_hour;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //日
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_day;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //月
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_month;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //年
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_year;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
			}
		}
		break;


			case DP_TB_31://带时标双点遥信,SOE
				if ((apdu_104->asdu.vsq&0x80) == 0)
				{
					if(Is_wrong)
					{
						unit_byte = 11;
						if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
						{
							info_num = (frame_len - des_len)/unit_byte;
						}
					}

					for(int i = 0; i < info_num; i++)
					{
						info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) + 1;   //信息体地址3字节

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);     //地址字节1
						m_exchange += temp;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //地址字节2
						m_exchange += temp;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //地址字节3
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_yx_addr;
						sprintf(temp,"%d",info_addr);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //信息字节
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_siq;

						switch(m_pData[cur_position]&0x03) 
						{
						case 0:
							m_exchange += remind_uncertainty_double;
							break;
						case 1:
							m_exchange += remind_open_double;
							break;
						case 2:
							m_exchange += remind_close_double;
							break;
						case 3:
							m_exchange += remind_uncertainty_double;
							break;
						default:
							break;
						}

						m_exchange += remind_space;

						if ((m_pData[cur_position]&0x10) == 0x00)
						{
							m_exchange += remind_unblock;
						}
						else
						{
							m_exchange += remind_block;
						}

						m_exchange += remind_space;

						if ((m_pData[cur_position]&0x20) == 0x00)
						{
							m_exchange += remind_no_replace;
						}
						else
						{
							m_exchange += remind_replace;
						}

						m_exchange += remind_space;

						if ((m_pData[cur_position]&0x40) == 0x00)
						{
							m_exchange += remind_current_value;
						}
						else
						{
							m_exchange += remind_no_current_value;
						}

						m_exchange += remind_space;

						if (((BYTE)m_pData[cur_position]&0x80) == 0x00)
						{
							m_exchange += remind_valid;
						}
						else
						{
							m_exchange += remind_invalid;
						}

						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						//SOE时间部分处理
						ms = usghextoint((BYTE*)&m_pData[cur_position], 2);

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //毫秒低字节
						m_exchange += temp;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //毫秒高字节 + 解析
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_ms;
						sprintf(temp,"%d",ms);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //分钟
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_minute;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //小时
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_hour;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //日
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_day;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //月
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_month;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //年
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_year;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
					}

				}
				else
				{
					info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) + 1;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);     //地址字节1
					m_exchange += temp;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);     //地址字节2
					m_exchange += temp;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //地址字节3
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_yx_addr;
					sprintf(temp,"%d",info_addr);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;

					if(Is_wrong)
					{
						unit_byte = 8;
						des_len += 3;
						if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
						{
							info_num = (frame_len - des_len)/unit_byte;
						}
					}

					for(int i = 0; i < (apdu_104->asdu.vsq&0x7F); i++)
					{
						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //信息字节
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_siq;

						switch(m_pData[cur_position]&0x03) 
						{
						case 0:
							m_exchange += remind_uncertainty_double;
							break;
						case 1:
							m_exchange += remind_open_double;
							break;
						case 2:
							m_exchange += remind_close_double;
							break;
						case 3:
							m_exchange += remind_uncertainty_double;
							break;
						default:
							break;
						}

						m_exchange += remind_space;

						if ((m_pData[cur_position]&0x10) == 0x00)
						{
							m_exchange += remind_unblock;
						}
						else
						{
							m_exchange += remind_unblock;
						}

						m_exchange += remind_space;

						if ((m_pData[cur_position]&0x20) == 0x00)
						{
							m_exchange += remind_no_replace;
						}
						else
						{
							m_exchange += remind_replace;
						}

						m_exchange += remind_space;

						if ((m_pData[cur_position]&0x40) == 0x00)
						{
							m_exchange += remind_current_value;
						}
						else
						{
							m_exchange += remind_no_current_value;
						}

						m_exchange += remind_space;

						if (((BYTE)m_pData[cur_position]&0x80) == 0x00)
						{
							m_exchange += remind_valid;
						}
						else
						{
							m_exchange += remind_invalid;
						}


						m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//SOE时间部分处理
				ms = usghextoint((BYTE*)&m_pData[cur_position], 2);

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //毫秒低字节
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //毫秒高字节 + 解析
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_ms;
				sprintf(temp,"%d",ms);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //分钟
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_minute;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //小时
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_hour;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //日
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_day;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //月
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_month;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //年
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_year;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
					}
				}
				break;

case ME_TD_34://带时标的测量值，归一化值
	if ((apdu_104->asdu.vsq&0x80) == 0)
	{
		if(Is_wrong)
		{
			unit_byte = 13;
			if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
			{
				info_num = (frame_len - des_len)/unit_byte;
			}
		}

		for(int i = 0; i < info_num; i++)
		{
			info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 16384;
			
			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //地址字节1
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //地址字节2
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //地址字节3+解析
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_yc_addr;
			sprintf(temp,"%d",info_addr);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;
			
			int mea_data = sghextoint_104((BYTE*)&m_pData[cur_position], 2);   //遥测数据2字节

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_me_na;
			sprintf(temp,"%d",mea_data);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;


			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //品质因数
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_qds;


			if((m_pData[cur_position]&0x01) == 0)
			{
				m_exchange += remind_no_overflow;
			}
			else
			{
				m_exchange += remind_overflow;
			}

			m_exchange += remind_space;

			if((m_pData[cur_position]&0x10) == 0)
			{
				m_exchange += remind_unblock;
			}
			else
			{
				m_exchange += remind_block;
			}

			m_exchange += remind_space;

			if((m_pData[cur_position]&0x20) == 0)
			{
				m_exchange += remind_no_replace;
			}
			else
			{
				m_exchange += remind_replace;
			}

			m_exchange += remind_space;

			if((m_pData[cur_position]&0x40) == 0)
			{
				m_exchange += remind_current_value;
			}
			else
			{
				m_exchange += remind_no_current_value;
			}

			m_exchange += remind_space;

			if(((BYTE)m_pData[cur_position++]&0x80) == 0)
			{
				m_exchange += remind_valid;
			}
			else
			{
				m_exchange += remind_invalid;
			}	

			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			//SOE时间部分处理
			ms = usghextoint((BYTE*)&m_pData[cur_position], 2);

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //毫秒低字节
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //毫秒高字节 + 解析
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_ms;
			sprintf(temp,"%d",ms);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //分钟
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_minute;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //小时
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_hour;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //日
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_day;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //月
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_month;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //年
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_year;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;
		}

	}
	else
	{
		info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 16384;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //地址字节1
		m_exchange += temp;
		m_exchange += remind_space;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //地址字节2
		m_exchange += temp;
		m_exchange += remind_space;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //地址字节3+解析
		m_exchange += temp;
		m_exchange += remind_leftkuohao;
		m_exchange += remind_yc_addr;
		sprintf(temp,"%d",info_addr);
		m_exchange += temp;
		m_exchange += remind_rightkuohao;
		m_exchange += remind_space;

		if(Is_wrong)
		{
			unit_byte = 10;
			des_len += 3;
			if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
			{
				info_num = (frame_len - des_len)/unit_byte;
			}
		}

		for(int i = 0; i < info_num; i++)
		{
			int mea_data = sghextoint_104((BYTE*)&m_pData[cur_position], 2);   //遥测数据2字节

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_me_na;
			sprintf(temp,"%d",mea_data);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			if((m_pData[cur_position]&0x01) == 0)
			{
				m_exchange += remind_no_overflow;
			}
			else
			{
				m_exchange += remind_overflow;
			}

			m_exchange += remind_space;

			if((m_pData[cur_position]&0x10) == 0)
			{
				m_exchange += remind_unblock;
			}
			else
			{
				m_exchange += remind_block;
			}

			m_exchange += remind_space;

			if((m_pData[cur_position]&0x20) == 0)
			{
				m_exchange += remind_no_replace;
			}
			else
			{
				m_exchange += remind_replace;
			}

			m_exchange += remind_space;

			if((m_pData[cur_position]&0x40) == 0)
			{
				m_exchange += remind_current_value;
			}
			else
			{
				m_exchange += remind_no_current_value;
			}

			m_exchange += remind_space;

			if(((BYTE)m_pData[cur_position++]&0x80) == 0)
			{
				m_exchange += remind_valid;
			}
			else
			{
				m_exchange += remind_invalid;
			}	

			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			//SOE时间部分处理
			ms = usghextoint((BYTE*)&m_pData[cur_position], 2);

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //毫秒低字节
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //毫秒高字节 + 解析
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_ms;
			sprintf(temp,"%d",ms);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //分钟
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_minute;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //小时
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_hour;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //日
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_day;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //月
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_month;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //年
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_year;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;
		}
	}
	break;


	//遥控命令
			case SC_NA_45://单点遥控
				if ((apdu_104->asdu.vsq&0x80) == 0)
				{
					if(Is_wrong)
					{
						unit_byte = 4;
						if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
						{
							info_num = (frame_len - des_len)/unit_byte;
						}
					}

					for(int i = 0; i < info_num; i++)
					{
						info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 24576;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //地址3字节
						m_exchange += temp;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_yk_addr;
						sprintf(temp,"%d",info_addr);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);    //遥控命令
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_sco;

						if((m_pData[cur_position]&0x01) == 0)
						{
							m_exchange += remind_open_ctl;
						}
						else
						{
							m_exchange += remind_close_ctl;
						}

						m_exchange += remind_space;

						int bit_tem = (m_pData[cur_position]&0x7C);
						bit_tem>>=2;

						switch(bit_tem)
						{
						case 0:
							m_exchange += remind_no_other;
							break;
						case 1:
							m_exchange += remind_short_pulse;
							break;
						case 2:
							m_exchange += remind_long_pulse;
							break;
						case 3:
							m_exchange += remind_sustained_output;
							break;
						default:
							m_exchange += remind_reserve;
							break;
						}

						m_exchange += remind_space;

						if (((BYTE)m_pData[cur_position]&0x80) == 0)
						{
							m_exchange += remind_execute;
						}
						else
						{
							m_exchange += remind_select;
						}

						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;
					}

				}
				else
				{
					info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 24576;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //地址3字节
					m_exchange += temp;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
					m_exchange += temp;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_yk_addr;
					sprintf(temp,"%d",info_addr);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;


					if(Is_wrong)
					{
						unit_byte = 1;
						des_len += 3;
						if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
						{
							info_num = (frame_len - des_len)/unit_byte;
						}
					}

					for(int i = 0; i < info_num; i++)
					{
						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);    //遥控命令
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_sco;

						int bit_tem = (m_pData[cur_position]&0x7C);
						bit_tem>>=2;

						switch(bit_tem)
						{
						case 0:
							m_exchange += remind_no_other;
							break;
						case 1:
							m_exchange += remind_short_pulse;
							break;
						case 2:
							m_exchange += remind_long_pulse;
							break;
						case 3:
							m_exchange += remind_sustained_output;
							break;
						default:
							m_exchange += remind_reserve;
							break;
						}

						m_exchange += remind_space;

						if ((m_pData[cur_position]&0x80) == 0)
						{
							m_exchange += remind_execute;
						}
						else
						{
							m_exchange += remind_select;
						}

						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;
					}
				}
				break;


	case DC_NA_46://双点遥控
		if ((apdu_104->asdu.vsq&0x80) == 0)
		{
			if(Is_wrong)
			{
				unit_byte = 4;
				if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
				{
					info_num = (frame_len - des_len)/unit_byte;
				}
			}

			for(int i = 0; i < info_num; i++)
			{
				info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 24576;  //地址3字节

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
			    m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_yk_addr;
				sprintf(temp,"%d",info_addr);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_dco;

				switch(m_pData[cur_position]&0x03)
				{
				case 0:
					m_exchange += remind_no_allowed;
					break;
				case 1:
					m_exchange += remind_open_ctl;
					break;
				case 2:
					m_exchange += remind_close_ctl;
					break;
				case 3:
					m_exchange += remind_no_allowed;
					break;
				default:
					m_exchange += remind_reserve;
					break;

				}

				m_exchange += remind_space;

				int bit_tem = m_pData[cur_position]&0x7C;
				bit_tem>>=2;

				switch(bit_tem)
				{
				case 0:
					m_exchange += remind_no_other;
					break;
				case 1:
					m_exchange += remind_short_pulse;
					break;
				case 2:
					m_exchange += remind_long_pulse;
					break;
				case 3:
					m_exchange += remind_sustained_output;
					break;
				default:
					m_exchange += remind_reserve;
					break;
				}

				m_exchange += remind_space;


				if ((m_pData[cur_position]&0x80) == 0)
				{
					m_exchange += remind_execute;
				}
				else
				{
					m_exchange += remind_select;
				}

				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;
			}

		}
		else
		{
			info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 24576;  //地址3字节

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_yk_addr;
			sprintf(temp,"%d",info_addr);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			if(Is_wrong)
			{
				unit_byte = 1;
				des_len += 3;
				if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
				{
					info_num = (frame_len - des_len)/unit_byte;
				}
			}

			for(int i = 0; i < info_num; i++)
			{
				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_dco;

				switch(m_pData[cur_position]&0x03)
				{
				case 0:
					m_exchange += remind_no_allowed;
					break;
				case 1:
					m_exchange += remind_open_ctl;
					break;
				case 2:
					m_exchange += remind_close_ctl;
					break;
				case 3:
					m_exchange += remind_no_allowed;
					break;
				default:
					m_exchange += remind_reserve;
					break;

				}

				m_exchange += remind_space;

				int bit_tem = m_pData[cur_position]&0x7C;
				bit_tem>>=2;

				switch(bit_tem)
				{
				case 0:
					m_exchange += remind_no_other;
					break;
				case 1:
					m_exchange += remind_short_pulse;
					break;
				case 2:
					m_exchange += remind_long_pulse;
					break;
				case 3:
					m_exchange += remind_sustained_output;
					break;
				default:
					m_exchange += remind_reserve;
					break;
				}

				m_exchange += remind_space;


				if ((m_pData[cur_position]&0x80) == 0)
				{
					m_exchange += remind_execute;
				}
				else
				{
					m_exchange += remind_select;
				}

				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;
			}
		}
		break;

case SE_NA_48://设点命令，归一化值

	//68 10 00 00 02 00 30 01 07 00 01 00 00 00 00 34 12 80 
	if ((apdu_104->asdu.vsq&0x80) == 0)
	{
		if(Is_wrong)
		{
			unit_byte = 6;
			if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
			{
				info_num = (frame_len - des_len)/unit_byte;
			}
		}

		for(int i = 0; i < info_num; i++)
		{
			info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 16384;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);     //地址信息3字节
		    m_exchange += temp; 
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_yc_addr;
			sprintf(temp,"%d",info_addr);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			int mea_data = sghextoint_104((BYTE*)&m_pData[cur_position], 2);

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //设定遥测2字节
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_me_na;
			sprintf(temp,"%d",mea_data);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_qos;

			if ((m_pData[cur_position]&0x7F) == 0)
			{
				m_exchange += _C2Q("未用");
			}
			
			//m_exchange += remind_space;

			else if ((m_pData[cur_position]&0x7F) <= 15)
			{
				m_exchange += _C2Q("保留（兼容范围");
			}

			//m_exchange += remind_space;

			else if ((m_pData[cur_position]&0x7F) > 15 &&(m_pData[cur_position]&0x7F) < 32 )
			{
				m_exchange += _C2Q("保留（专用范围");
			}

			//m_exchange += remind_space;

			else
			{
				m_exchange += _C2Q("未知");
			}

			m_exchange += remind_space;

			if(((BYTE)m_pData[cur_position]&0x80) == 0)
			{
				m_exchange += remind_execute;
			}
			else
			{
				m_exchange += remind_select;
			}

			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;
			cur_position++;
		}

	}
	else
	{
		info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 16384;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);     //地址信息3字节
		m_exchange += temp; 
		m_exchange += remind_space;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
		m_exchange += temp;
		m_exchange += remind_space;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
		m_exchange += temp;
		m_exchange += remind_leftkuohao;
		m_exchange += remind_yc_addr;
		sprintf(temp,"%d",info_addr);
		m_exchange += temp;
		m_exchange += remind_rightkuohao;
		m_exchange += remind_space;

		if(Is_wrong)
		{
			unit_byte = 3;
			des_len += 3;
			if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
			{
				info_num = (frame_len - des_len)/unit_byte;
			}
		}

		for(int i = 0; i < info_num; i++)
		{
			int mea_data = sghextoint_104((BYTE*)&m_pData[cur_position], 2);

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //设定遥测2字节
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_me_na;
			sprintf(temp,"%d",mea_data);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_qos;

			if ((m_pData[cur_position]&0x7F) == 0)
			{
				m_exchange += _C2Q("未用");
			}

			//m_exchange += remind_space;

			else if ((m_pData[cur_position]&0x7F) <= 15)
			{
				m_exchange += _C2Q("保留（兼容范围");
			}

			//m_exchange += remind_space;

			else if ((m_pData[cur_position]&0x7F) > 15 &&(m_pData[cur_position]&0x7F) < 32 )
			{
				m_exchange += _C2Q("保留（专用范围");
			}

			//m_exchange += remind_space;

			else
			{
				m_exchange += _C2Q("未知");
			}

			m_exchange += remind_space;

			if(((BYTE)m_pData[cur_position]&0x80) == 0)
			{
				m_exchange += remind_execute;
			}
			else
			{
				m_exchange += remind_select;
			}

			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;
			cur_position++;
		}
	}			
	break;


		case SC_NA_58://带时标的单命令
			if ((apdu_104->asdu.vsq&0x80) == 0)
			{
				if(Is_wrong)
				{
					unit_byte = 11;
					if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
					{
						info_num = (frame_len - des_len)/unit_byte;
					}
				}

				for(int i = 0; i < info_num; i++)
				{
					info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 24576;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //地址3字节
					m_exchange += temp;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //地址3字节
					m_exchange += temp;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //地址3字节
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_yk_addr;
					sprintf(temp,"%d",info_addr);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);    //遥控命令
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_sco;

					if((m_pData[cur_position]&0x01) == 0x00)
					{
						m_exchange == remind_open_ctl;
					}
					else
					{
						m_exchange += remind_close_ctl;
					}

					m_exchange += remind_space;

					int bit_tem = m_pData[cur_position]&0x7C;
					bit_tem>>=2;

					switch(bit_tem)
					{
					case 0:
						m_exchange += remind_no_other;
						break;
					case 1:
						m_exchange += remind_short_pulse;
						break;
					case 2:
						m_exchange += remind_long_pulse;
						break;
					case 3:
						m_exchange += remind_sustained_output;
						break;
					default:
						m_exchange += remind_reserve;
						break;
					}

					m_exchange += remind_space;

					if (((BYTE)m_pData[cur_position]&0x80) == 0)
					{
						m_exchange += remind_execute;
					}
					else
					{
						m_exchange += remind_select;
					}
                    
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;
					cur_position++;

					ms = usghextoint((BYTE*)&m_pData[cur_position], 2);

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //毫秒低字节
					m_exchange += temp;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //毫秒高字节 + 解析
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_ms;
					sprintf(temp,"%d",ms);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //分钟
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_minute;
					sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //小时
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_hour;
					sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //日
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_day;
					sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //月
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_month;
					sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //年
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_year;
					sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;
				}

			}
			else
			{
				info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 24576;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //地址3字节
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //地址3字节
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);    //地址3字节
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_yk_addr;
				sprintf(temp,"%d",info_addr);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				if(Is_wrong)
				{
					unit_byte = 8;
					des_len += 3;
					if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
					{
						info_num = (frame_len - des_len)/unit_byte;
					}
				}

				for(int i = 0; i < info_num; i++)
				{
					sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);    //遥控命令
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_sco;

					if((m_pData[cur_position]&0x01) == 0x00)
					{
						m_exchange == remind_open_ctl;
					}
					else
					{
						m_exchange += remind_close_ctl;
					}

					m_exchange += remind_space;

					int bit_tem = m_pData[cur_position]&0x7C;
					bit_tem>>=2;

					switch(bit_tem)
					{
					case 0:
						m_exchange += remind_no_other;
						break;
					case 1:
						m_exchange += remind_short_pulse;
						break;
					case 2:
						m_exchange += remind_long_pulse;
						break;
					case 3:
						m_exchange += remind_sustained_output;
						break;
					default:
						m_exchange += remind_reserve;
						break;
					}

					m_exchange += remind_space;

					if (((BYTE)m_pData[cur_position]&0x80) == 0)
					{
						m_exchange += remind_execute;
					}
					else
					{
						m_exchange += remind_select;
					}

					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;
					cur_position++;

					ms = usghextoint((BYTE*)&m_pData[cur_position], 2);

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //毫秒低字节
					m_exchange += temp;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //毫秒高字节 + 解析
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_ms;
					sprintf(temp,"%d",ms);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //分钟
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_minute;
					sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //小时
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_hour;
					sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //日
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_day;
					sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //月
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_month;
					sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //年
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_year;
					sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;
				}
			}
			break;


			case DC_NA_59://带时标的双命令
				if ((apdu_104->asdu.vsq&0x80) == 0)
				{
					if(Is_wrong)
					{
						unit_byte = 11;
						if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
						{
							info_num = (frame_len - des_len)/unit_byte;
						}
					}

					for(int i = 0; i < info_num; i++)
					{
						info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 24576;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  //地址3字节
						m_exchange += temp;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_yk_addr;
						sprintf(temp,"%d",info_addr);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);    //遥控命令
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_dco;

						switch(m_pData[cur_position]&0x03)
						{
						case 0:
							m_exchange += remind_no_allowed;
							break;
						case 1:
							m_exchange += remind_open_ctl;
							break;
						case 2:
							m_exchange += remind_close_ctl;
							break;
						case 3:
							m_exchange += remind_no_allowed;
							break;
						default:
							m_exchange += remind_reserve;
							break;
						}

						m_exchange += remind_space;

						int bit_tem = m_pData[cur_position]&0x7C;
						bit_tem>>=2;

						switch(bit_tem)
						{
						case 0:
							m_exchange += remind_no_other;
							break;
						case 1:
							m_exchange += remind_short_pulse;
							break;
						case 2:
							m_exchange += remind_long_pulse;
							break;
						case 3:
							m_exchange += remind_sustained_output;
							break;
						default:
							m_exchange += remind_reserve;
							break;

						}

						m_exchange += remind_space;

						if (((BYTE)m_pData[cur_position]&0x80) == 0)
						{
							m_exchange += remind_execute;
						}
						else
						{
							m_exchange += remind_select;
						}

						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						ms = usghextoint((BYTE*)&m_pData[cur_position], 2);

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //毫秒低字节
						m_exchange += temp;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //毫秒高字节 + 解析
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_ms;
						sprintf(temp,"%d",ms);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //分钟
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_minute;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //小时
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_hour;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //日
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_day;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //月
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_month;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //年
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_year;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
					}

				}
				else
				{	
				   info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 24576;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  //地址3字节
					m_exchange += temp;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
					m_exchange += temp;
					m_exchange += remind_space;

					sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
					m_exchange += temp;
					m_exchange += remind_leftkuohao;
					m_exchange += remind_yk_addr;
					sprintf(temp,"%d",info_addr);
					m_exchange += temp;
					m_exchange += remind_rightkuohao;
					m_exchange += remind_space;

					if(Is_wrong)
					{
						unit_byte = 8;
						des_len += 3;
						if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
						{
							info_num = (frame_len - des_len)/unit_byte;
						}
					}

					for(int i = 0; i < info_num; i++)
					{
						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);    //遥控命令
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_dco;

						switch(m_pData[cur_position]&0x03)
						{
						case 0:
							m_exchange += remind_no_allowed;
							break;
						case 1:
							m_exchange += remind_open_ctl;
							break;
						case 2:
							m_exchange += remind_close_ctl;
							break;
						case 3:
							m_exchange += remind_no_allowed;
							break;
						default:
							m_exchange += remind_reserve;
							break;
						}

						m_exchange += remind_space;

						int bit_tem = m_pData[cur_position]&0x7C;
						bit_tem>>=2;

						switch(bit_tem)
						{
						case 0:
							m_exchange += remind_no_other;
							break;
						case 1:
							m_exchange += remind_short_pulse;
							break;
						case 2:
							m_exchange += remind_long_pulse;
							break;
						case 3:
							m_exchange += remind_sustained_output;
							break;
						default:
							m_exchange += remind_reserve;
							break;
						}

						m_exchange += remind_space;

						if (((BYTE)m_pData[cur_position]&0x80) == 0)
						{
							m_exchange += remind_execute;
						}
						else
						{
							m_exchange += remind_select;
						}


						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;


						ms = usghextoint((BYTE*)&m_pData[cur_position], 2);

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //毫秒低字节
						m_exchange += temp;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //毫秒高字节 + 解析
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_ms;
						sprintf(temp,"%d",ms);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //分钟
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_minute;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //小时
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_hour;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //日
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_day;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //月
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_month;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //年
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += remind_year;
						sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
					}
				}
				break;

case SE_TA_61://带时标的设点命令，归一化值
	if ((apdu_104->asdu.vsq&0x80) == 0)
	{
		if(Is_wrong)
		{
			unit_byte = 13;
			if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
			{
				info_num = (frame_len - des_len)/unit_byte;
			}
		}

		for(int i = 0; i < info_num; i++)
		{
			info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 16384;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  //地址3字节
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_yc_addr;
			sprintf(temp,"%d",info_addr);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			int mea_data = sghextoint_104((BYTE*)&m_pData[cur_position], 2);   

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //遥测2字节
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_me_na;
			sprintf(temp,"%d",mea_data);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_qos;

			if ((m_pData[cur_position]&0x7F) == 0)
			{
				m_exchange += _C2Q("未用");
			}

			else if ((m_pData[cur_position]&0x7F) <= 15)
			{
				m_exchange += _C2Q("保留（兼容范围");
			}

			else if ((m_pData[cur_position]&0x7F) > 15 &&(m_pData[cur_position]&0x7F) < 32 )
			{
				m_exchange += _C2Q("保留（专用范围");
			}

			else
			{
				m_exchange += _C2Q("未知");
			}

			m_exchange += remind_space;

			if(((BYTE)m_pData[cur_position]&0x80) == 0)
			{
				m_exchange += remind_execute;
			}

			else
			{
				m_exchange += remind_select;
			}

			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;
			cur_position++;


			ms = usghextoint((BYTE*)&m_pData[cur_position], 2);

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //毫秒低字节
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //毫秒高字节 + 解析
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_ms;
			sprintf(temp,"%d",ms);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //分钟
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_minute;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //小时
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_hour;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //日
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_day;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //月
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_month;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //年
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_year;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;
		}

	}
	else
	{
		info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 16384;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  //地址3字节
		m_exchange += temp;
		m_exchange += remind_space;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
		m_exchange += temp;
		m_exchange += remind_space;

		sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
		m_exchange += temp;
		m_exchange += remind_leftkuohao;
		m_exchange += remind_yc_addr;
		sprintf(temp,"%d",info_addr);
		m_exchange += temp;
		m_exchange += remind_rightkuohao;
		m_exchange += remind_space;

		if(Is_wrong)
		{
			unit_byte = 10;
			des_len += 3;
			if(((frame_len - des_len)/unit_byte < (apdu_104->asdu.vsq&0x7F)))
			{
				info_num = (frame_len - des_len)/unit_byte;
			}
		}

		for(int i = 0; i < info_num; i++)
		{
			int mea_data = sghextoint_104((BYTE*)&m_pData[cur_position], 2);   

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //遥测2字节
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_me_na;
			sprintf(temp,"%d",mea_data);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_qos;

			if ((m_pData[cur_position]&0x7F) == 0)
			{
				m_exchange += _C2Q("未用");
			}

			else if ((m_pData[cur_position]&0x7F) <= 15)
			{
				m_exchange += _C2Q("保留（兼容范围");
			}

			else if ((m_pData[cur_position]&0x7F) > 15 &&(m_pData[cur_position]&0x7F) < 32 )
			{
				m_exchange += _C2Q("保留（专用范围");
			}

			else
			{
				m_exchange += _C2Q("未知");
			}

			m_exchange += remind_space;

			if(((BYTE)m_pData[cur_position]&0x80) == 0)
			{
				m_exchange += remind_execute;
			}
			else
			{
				m_exchange += remind_select;
			}

			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;
			cur_position++;

			ms = usghextoint((BYTE*)&m_pData[cur_position], 2);

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //毫秒低字节
			m_exchange += temp;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //毫秒高字节 + 解析
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_ms;
			sprintf(temp,"%d",ms);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //分钟
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_minute;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //小时
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_hour;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //日
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_day;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //月
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_month;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;

			sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //年
			m_exchange += temp;
			m_exchange += remind_leftkuohao;
			m_exchange += remind_year;
			sprintf(temp,"%d",(BYTE)m_pData[cur_position++]);
			m_exchange += temp;
			m_exchange += remind_rightkuohao;
			m_exchange += remind_space;
		}
	}			
	break;

			case IC_NA_100://总召唤命令

				if (Is_wrong)
				{
					continue;
				}

				//info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3);
	
				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //信息体地址3字节
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_info_addr;
				sprintf(temp,"%d",apdu_104->asdu.cs.index_no);   //总召唤地址
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  //召唤限定词QOI
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_call_limit;
				sprintf(temp,"%d",apdu_104->asdu.cs.qoi);
				m_exchange += temp;
				m_exchange += remind_space;

				if(apdu_104->asdu.cs.qoi == 20)
				{
					m_exchange += remind_respond_station_call_20;
				}
				else
				{
					m_exchange += _C2Q("响应第");
					sprintf(temp,"%d",apdu_104->asdu.cs.qoi-20);
					m_exchange += temp;
					m_exchange += _C2Q("组召唤");
				}

				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				break;

			case CI_NA_101://电能脉冲召唤
				if (Is_wrong)
				{
					continue;
				}
				info_addr = usghextoint((BYTE*)&m_pData[cur_position], 3) - 25600;  

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //信息体地址3字节
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_ym_addr;
				sprintf(temp,"%d",info_addr);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);   //QCC
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_energy_limit;
				m_exchange += remind_space;

				switch(m_pData[cur_position]&0xC0) 
				{
				case 0x40:
					m_exchange += remind_energy_fre_no_reset;
					break;
				case 0x80:
					m_exchange += remind_energy_fre_reset;
					break;
				case 0xC0:
					m_exchange += remind_energy_counter_reset;
					break;
				default:
					break;
				}

				m_exchange += remind_space;

				switch(m_pData[cur_position]&0x3F) 
				{
				case 1:
					m_exchange += remind_energy_call_1;
					break;
				case 2:
					m_exchange += remind_energy_call_2;
					break;
				case 3:
					m_exchange += remind_energy_call_3;
					break;
				case 4:
					m_exchange += remind_energy_call_4;
					break;
				case 5:
					m_exchange += remind_energy_call_5;
					break;
				default:
					break;
				}

				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;
				break;

			case CS_NA_103://时钟同步命令

				if (Is_wrong)
				{
					continue;
				}

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //信息体地址3字节
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_info_addr;
				sprintf(temp,"%d",apdu_104->asdu.sts.index_no);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				ms = usghextoint((BYTE*)&m_pData[cur_position], 2);

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);  //毫秒低字节
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //毫秒高字节 + 解析
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_ms;
				sprintf(temp,"%d",ms);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //分钟
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_minute;
				sprintf(temp,"%d",apdu_104->asdu.sts.cp56time.minute);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //小时
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_hour;
				sprintf(temp,"%d",apdu_104->asdu.sts.cp56time.hour);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //天
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_day;
				sprintf(temp,"%d",apdu_104->asdu.sts.cp56time.day);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);     //月
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_month;
				sprintf(temp,"%d",apdu_104->asdu.sts.cp56time.month);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;


				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_year;
				sprintf(temp,"%d",apdu_104->asdu.sts.cp56time.year);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				break;



			case M_EI_NA_70://初始化

				if (Is_wrong)
				{
					continue;
				}

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //地址3字节
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_info_addr;
				sprintf(temp,"%d",apdu_104->asdu.sts.index_no);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				sprintf(temp,"%02X",(BYTE)m_pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += remind_coi;

				switch(m_pData[cur_position]&0x7F)
				{
				case 0:
					m_exchange += remind_coi_0;
					break;
				case 1:
					m_exchange += remind_coi_1;
					break;
				case 2:
					m_exchange += remind_coi_2;
					break;
				default:
					m_exchange += remind_coi_other;
					break;
				}

				m_exchange += remind_space;

				if (((BYTE)m_pData[cur_position++]&0x80) == 0)
				{
					m_exchange += remind_coi_unchanged;
				}
				else
				{
					m_exchange += remind_coi_changed;
				}

				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				break;


	  //case:
		 // break;

	 default:   //剩余类型打印出字符串
		 int left_num = apdu_104->apci.apdu_len - 10 ;  

		 for (i = 0; i < left_num; i++)
		 {
			 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //地址3字节
			 m_exchange += temp;
			 m_exchange += remind_space;
		 }
		 break;




	 }  //while
}  //function





     
     


	
	// if((BYTE)m_pData[cur_position]  == 0x43 || (BYTE)m_pData[cur_position] == 0x07)  //报文长度不对
	// {

	//	 sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]);   //控制域
	//	 m_exchange += temp;
	//	 m_exchange += remind_control_u;
	// }

 //      sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]); 
	//   m_exchange += temp;
	//   m_exchange += remind_space;

	//   sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]); 
	//   m_exchange += temp;
 //      m_exchange += remind_space;

	//   sprintf(temp,"%02X",(BYTE)m_pData[cur_position]); 
	//   m_exchange += temp;
 

}

//CDT报文断帧处理
//void CNetModulaData::cdt_rebuild( char* pData, int nLength, int nType)
//{
//	int pos = 0;   //  pos/2是字节数
//	int cur_position = 0;  //
//	int cnt = 0;  //帧数目（报文数）
//	//memset(temp,0,2);
//	int i;
//	int frame_length;  //frame_length + 2	
//
//	int ms = 0;
//	bool Is_wrong = false;
//	int byte_num = 0;
//	byte_num = nLength;
//
//	int frame_len = nLength;
//
//	//第一帧舍弃，提取EB90部分进缓存区
//	if (cdt_frame_first == true)
//	{
//		for (i = 0; i < nLength; i++)
//		{
//			//判断第一帧里面是否有CDT报文头部分，有的话复制到缓冲区
//			if (((BYTE)pData[i] == 0xEB) && ((BYTE)pData[i+1] == 0x90)
//				&& ((BYTE)pData[i+2] == 0xEB) && ((BYTE)pData[i+3] == 0x90)
//				&& ((BYTE)pData[i+4] == 0xEB) && ((BYTE)pData[i+5] == 0x90))
//			{
//				cdt_len_full = (BYTE)pData[i+8] * 6 + 12;   //完整帧长度
//				
//				//除去前面多余的报文，剩下的刚好是一帧完整的CDT帧
//				if (nLength - i == cdt_len_full)   
//				{
//					memcpy(cdt_complete_buf,&pData[i],cdt_len_full);
//					cdt_frame_finish = true;
//					if (cdt_frame_finish == true)
//					{
//						analyze_cdt_asc((char*)cdt_complete_buf, cdt_len_full, nType);
//					}
//					memset(cdt_complete_buf, 0, cdt_len_full);
//				}
//
//				//包含有前面多余报文，包含有一帧完整的CDT帧，还包含有另外的CDT帧
//				if(nLength - i > cdt_len_full)
//				{
//					memcpy(cdt_complete_buf,&pData[i],cdt_len_full);
//					cdt_frame_finish = true;
//					if (cdt_frame_finish == true)
//					{
//						analyze_cdt_asc((char*)cdt_complete_buf, cdt_len_full, nType);
//					}
//					memset(cdt_complete_buf, 0, cdt_len_full);
//
//				}
//				
//			}
//
//		}
//		cdt_frame_first = false;   //第一帧处理完，不是头帧，进入缓冲区处理
//		return;
//
//	}
//
//	if (cdt_frame_first == false)
//	{
//
//		for (i = 0; i < nLength; i++)
//		{
//			//判断第一帧里面是否有CDT报文头部分，有的话复制到缓冲区
//			if (((BYTE)pData[i] == 0xEB) && ((BYTE)pData[i+1] == 0x90)
//				&& ((BYTE)pData[i+2] == 0xEB) && ((BYTE)pData[i+3] == 0x90)
//				&& ((BYTE)pData[i+4] == 0xEB) && ((BYTE)pData[i+5] == 0x90))
//			{
//				cdt_len_full = (BYTE)pData[i+8] * 6 + 12;   //帧长度
//				if (nLength - i == cdt_len_full)   
//				{
//					memcpy(cdt_complete_buf,&pData[i],cdt_len_full);
//					cdt_frame_finish = true;
//					if (cdt_frame_finish == true)
//					{
//						analyze_cdt_asc((char*)cdt_complete_buf, cdt_len_full, nType);
//					}
//				}
//				memset(cdt_complete_buf, 0, cdt_len_full);
//			}
//
//		}
//		return;
//	}
//
//	//sprintf(temp,"%02X",(BYTE)m_pData[cur_position++]); 
//	//for (i = 0; i < nLength; i++)
//	//{
//	//	if (((BYTE)pData[i] == 0xEB) && ((BYTE)pData[i+1] == 0x90)
//	//		&& ((BYTE)pData[i+2] == 0xEB) && ((BYTE)pData[i+3] == 0x90)
//	//		&& ((BYTE)pData[i+4] == 0xEB) && ((BYTE)pData[i+5] == 0x90))
//	//	{
//	//		cdt_len_full = (BYTE)pData[i+8] * 6 + 12;   //帧长度
//	//		//除去报文头，剩下部分正好是一帧完整的CDT报文,pma模拟
//	//		if (nLength - i == cdt_len_full)   
//	//		{
//	//			memcpy(cdt_complete_buf,&pData[i],cdt_len_full);
//	//			cdt_frame_finish = true;
//	//			return;
//	//		}
//
//	//		//包含有报文头，但是报文不完整，下一帧要继续存储进缓存区
//
//	//		//一帧报文包含有两个报文头
//
//	//		//第一帧，舍弃该报文
//
//
//	//		memcpy(cdt_complete_buf,&pData[i],(BYTE)pData[i]);
//	//		break;
//	//	}
//	//	
//
//}
	
void CNetModulaData::cdt_rebuild( char* pData, int nLength, int nType)
{
		int pos = 0;   //  pos/2是字节数
		int cur_position = 0;  //
		int cnt = 0;  //帧数目（报文数）
		//memset(temp,0,2);
		int i,j;
		int frame_length;  //frame_length + 2	
	
		int ms = 0;
		bool Is_wrong = false;
		int byte_num = 0;
		byte_num = nLength;
		int cdt_frame_length = 0;
		int cdt_len_full = 0;
		int cdt_frame_list_len;
	
		int frame_len = nLength;
		int cdt_frame_append;
		int cdt_begin_position;

		for (i = 0; i < nLength; i++)
		{
			cdt_frame_list << (BYTE)pData[i];		
		}

		cdt_frame_list_len = cdt_frame_list.size();

		if (!cdt_frame_list.isEmpty())
		{
			/*for (i = 0; i < cdt_frame_list_len; i++)
			{*/
			cdt_begin_position = cdt_frame_list.indexOf(0xEB);
			if ((cdt_begin_position >= 0) && (cdt_begin_position + 5 <= cdt_frame_list_len))
			{
				if ((cdt_frame_list.at(cdt_begin_position) == 0xEB) && (cdt_frame_list.at(cdt_begin_position+1) == 0x90)
					&& (cdt_frame_list.at(cdt_begin_position+2) == 0xEB) && (cdt_frame_list.at(cdt_begin_position+3) == 0x90)
					&& (cdt_frame_list.at(cdt_begin_position+4) == 0xEB) && (cdt_frame_list.at(cdt_begin_position+5) == 0x90))
				{
					cdt_frame_append = cdt_begin_position;  //报文头出现位置
					j = cdt_begin_position;


					cdt_len_full = cdt_frame_list.at(cdt_begin_position+8) * 6 + 12;   //完整帧长度
					if (cdt_frame_list_len - cdt_begin_position >= cdt_len_full)
					{
						for (i = 0; i < cdt_len_full; i++,j++)
						{
							cdt_complete_buf[i] = cdt_frame_list.at(j);
						}

						//analyze_cdt_asc((BYTE*)cdt_complete_buf, cdt_len_full, nType);
						analyze_cdt_asc(cdt_complete_buf, cdt_len_full, nType);
						memset(cdt_complete_buf, 0, sizeof(cdt_complete_buf));



						//一帧完整
						if (cdt_frame_list_len == cdt_len_full)
						{
							for (i = 0; i < cdt_frame_list_len; i++)
							{
								//cdt_frame_list.removeAt(i);
								cdt_frame_list.removeFirst();
							}
							return;
						}

						if (cdt_frame_list_len > cdt_len_full)
						{
							if (cdt_frame_list_len - cdt_frame_append -cdt_len_full == 0)
							{
								for (i = 0; i < cdt_frame_list_len; i++)
								{
									cdt_frame_list.removeFirst();
								}
								return;

							}
							if (cdt_frame_list_len - cdt_frame_append -cdt_len_full > 0)
							{
								for (i = 0; i < cdt_frame_append + cdt_len_full; i++)
								{
									cdt_frame_list.removeFirst();
								}
								return;
							}


						}

						if (cdt_frame_list_len < cdt_len_full)
						{
							cdt_untotal_frame(nType);
							return;
						}
						//
					}
				}
				cdt_untotal_frame(nType);
			}
			else
				cdt_untotal_frame(nType);
			    return;

			
			//}   //for

		}
		return;


}
















void CNetModulaData::analyze_cdt_asc( BYTE* pData, int nLength, int nType)
{
	char temp[2];   //原始char数组
	int pos = 0;   //  pos/2是字节数
	int cur_position = 0;  //
	int cnt = 0;  //帧数目（报文数）
	memset(temp,0,2);
	int i;
	int frame_length;  //frame_length + 2	
	int frame_len = 0;
	int ms = 0;
	bool Is_wrong = false;
	int byte_num = 0;
	byte_num = nLength;
	int sendnum = 0;
	int recvnum = 0;

	
	int cdt_type = 0;   //帧类别
	int cdt_signal_length = 0;  //信息字数
	int cdt_signal_begin = 0;  //信息起始位
	int cdt_signal_seq = 0;
	int cdt_func = 0;   //功能码

	int cdt_di_value = 0;
	int cdt_mea_value = 0;

	//m_exchange = new QString;   //0717

	while (cur_position < nLength/2) // pos/2是字节数
	{
		memset(temp,0,10);

		//报文头不对
		//if((BYTE)m_pData[cur_position] != 0xEB || (BYTE)m_pData[cur_position+1] != 0x90 ||
		//	(BYTE)m_pData[cur_position+2] != 0xEB || (BYTE)m_pData[cur_position+3] != 0x90 ||
		//	(BYTE)m_pData[cur_position+4] != 0xEB || (BYTE)m_pData[cur_position+5] != 0x90)  
		//{
		//	frame_length = (BYTE)m_pData[cur_position+8] * 6 + 12;
		//	for (cur_position = 0; cur_position < nLength; cur_position++)
		//	{
		//		sprintf(temp, "%02X", (BYTE)m_pData[cur_position]);
		//		m_exchange += temp;
		//		m_exchange += remind_space;
		//	}
		//	m_exchange += cdt_header_error; 
		//	cur_position = 0;
		//	//continue;
		//	return;
		//}

		//报文长度不对
		//if(((BYTE)m_pData[cur_position+8] * 6 + 12) != nLength)  
		//{
		//	for (cur_position = 0; cur_position < nLength; cur_position++)
		// {
		//	 sprintf(temp, "%02X", (BYTE)m_pData[cur_position]);
		//	 m_exchange += temp;
		//	 m_exchange += remind_space;
		// }
		//	m_exchange += cdt_len_error; 
		//	cur_position = 0;
		//	//continue;
		//	return;
		//}

		/*****************************正常报文处理部分********************************/


		//报文头6字节
		sprintf(temp, "%02X", (BYTE)pData[cur_position++]);  //EB
		m_exchange += temp;
		m_exchange += remind_space;

		sprintf(temp, "%02X", (BYTE)pData[cur_position++]);  //90
		m_exchange += temp;
		m_exchange += remind_space;

		sprintf(temp, "%02X", (BYTE)pData[cur_position++]);  //EB
		m_exchange += temp;
		m_exchange += remind_space;

		sprintf(temp, "%02X", (BYTE)pData[cur_position++]);  //90
		m_exchange += temp;
		m_exchange += remind_space;

		sprintf(temp, "%02X", (BYTE)pData[cur_position++]);  //EB
		m_exchange += temp;
		m_exchange += remind_space;

		sprintf(temp, "%02X", (BYTE)pData[cur_position++]);  //90
		m_exchange += temp;
		m_exchange += cdt_header;
		m_exchange += remind_space;

		m_exchange += "*";    //插入分隔符,0711
		cdt_frame_count++;

		//控制字1字节
		sprintf(temp, "%02X", (BYTE)pData[cur_position++]);  //71
		m_exchange += temp;
		m_exchange += cdt_control_field;
		m_exchange += remind_space;


		//帧类别
		cdt_type = (BYTE)pData[cur_position];              //帧类别
		sprintf(temp, "%02X", (BYTE)pData[cur_position]);  
		m_exchange += temp;
		m_exchange += remind_leftkuohao;
		m_exchange += cdt_frame_sort;

	

		if (nType == 1)   //RECV
		{
			switch (cdt_type)
			{
			case CDT_FRAME_A:
				m_exchange += cdt_frame_a;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;    //移动到信息字数

				//信息字数
				cdt_signal_length = (BYTE)pData[cur_position];
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_len;        
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//源站地址
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_source_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//目的站地址
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_target_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//校验码
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_crc;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				m_exchange += "*";    //插入分隔符,0711
				cdt_frame_count++;
				cur_position++;

				//信息部分
				
				for (i = 0; i < cdt_signal_length; i++)
				{
					cdt_signal_begin = (BYTE)pData[cur_position];   //信息起始位
					
					if ((cdt_signal_begin >= 0x00) && (cdt_signal_begin  <= 0x7F))  //遥测功能码范围
					{
						//遥测功能码（起始号）
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_func_yc_begin;
						cdt_signal_seq = 2 * (BYTE)pData[cur_position] + 1;    //遥测序号
						sprintf(temp,"%d",cdt_signal_seq);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;
						

						cdt_mea_value = sghextoint_cdt((BYTE*)&pData[cur_position], 2);
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //遥测两字节,第一组
						m_exchange += temp;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += _C2Q("值:");
						sprintf(temp, "%d", cdt_mea_value);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						cdt_mea_value = sghextoint_cdt((BYTE*)&pData[cur_position], 2);
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //遥测两字节,第二组
						m_exchange += temp;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += _C2Q("值:");
						sprintf(temp, "%d", cdt_mea_value);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //校验码
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_frame_crc;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						m_exchange += "*";    //插入分隔符,0711
						cdt_frame_count++;
						cur_position++;
					}

				}
				break;

			case CDT_FRAME_B:
				m_exchange += cdt_frame_b;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;    //移动到信息字数

				//信息字数
				cdt_signal_length = (BYTE)pData[cur_position];
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_len;        
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//源站地址
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_source_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//目的站地址
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_target_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//校验码
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_crc;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				m_exchange += "*";    //插入分隔符,0711
				cdt_frame_count++;
				cur_position++;

				//信息部分

				for (i = 0; i < cdt_signal_length; i++)
				{
					cdt_signal_begin = (BYTE)pData[cur_position];   //信息起始位

					if ((cdt_signal_begin >= 0x00) && (cdt_signal_begin  <= 0x7F))  //遥测功能码范围
					{
						//遥测功能码（起始号）
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_func_yc_begin;
						cdt_signal_seq = 2 * (BYTE)pData[cur_position] + 1;    //遥测序号
						sprintf(temp,"%d",cdt_signal_seq);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;


						cdt_mea_value = sghextoint_cdt((BYTE*)&pData[cur_position], 2);
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //遥测两字节,第一组
						m_exchange += temp;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += _C2Q("值:");
						sprintf(temp, "%d", cdt_mea_value);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						cdt_mea_value = sghextoint_cdt((BYTE*)&pData[cur_position], 2);
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //遥测两字节,第二组
						m_exchange += temp;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += _C2Q("值:");
						sprintf(temp, "%d", cdt_mea_value);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //校验码
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_frame_crc;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						m_exchange += "*";    //插入分隔符,0711
						cdt_frame_count++;
						cur_position++;
					}

				}
				break;

			case CDT_FRAME_C:
				m_exchange += cdt_frame_c;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;    //移动到信息字数

				//信息字数
				cdt_signal_length = (BYTE)pData[cur_position];
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_len;        
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//源站地址
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_source_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//目的站地址
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_target_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//校验码
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_crc;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				m_exchange += "*";    //插入分隔符,0711
				cdt_frame_count++;
				cur_position++;

				//信息部分

				for (i = 0; i < cdt_signal_length; i++)
				{
					cdt_signal_begin = (BYTE)pData[cur_position];   //信息起始位

					if ((cdt_signal_begin >= 0x00) && (cdt_signal_begin  <= 0x7F))  //遥测功能码范围
					{
						//遥测功能码（起始号）
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_func_yc_begin;
						cdt_signal_seq = 2 * (BYTE)pData[cur_position] + 1;    //遥测序号
						sprintf(temp,"%d",cdt_signal_seq);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;


						cdt_mea_value = sghextoint_cdt((BYTE*)&pData[cur_position], 2);
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //遥测两字节,第一组
						m_exchange += temp;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += _C2Q("值:");
						sprintf(temp, "%d", cdt_mea_value);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						cdt_mea_value = sghextoint_cdt((BYTE*)&pData[cur_position], 2);
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //遥测两字节,第二组
						m_exchange += temp;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += _C2Q("值:");
						sprintf(temp, "%d", cdt_mea_value);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //校验码
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_frame_crc;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						m_exchange += "*";    //插入分隔符,0711
						cdt_frame_count++;
						cur_position++;
					}

				}
				break;

			case CDT_FRAME_DI:
				m_exchange += cdt_frame_di;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;    //移动到信息字数


				//信息字数
				cdt_signal_length = (BYTE)pData[cur_position];
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_len;        
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//源站地址
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_source_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//目的站地址
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_target_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//校验码
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_crc;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;


				m_exchange += "*";    //插入分隔符,0711
				cdt_frame_count++;

				//信息部分

				for (i = 0; i < cdt_signal_length; i++)
				{
					cdt_signal_begin = (BYTE)pData[cur_position];   //信息起始位

					if ((cdt_signal_begin >= 0xF0) && (cdt_signal_begin  <= 0xFF))  //遥信功能码范围
					{
						//遥信功能码（起始号）
						//sprintf(temp, "%02X", (BYTE)m_pData[cur_position]);
						//m_exchange += temp;
						//m_exchange += remind_leftkuohao;
						//m_exchange += cdt_func_yx_begin;
						//cdt_signal_seq = 32 * ((BYTE)m_pData[cur_position] - 240) + 1;    //遥信序号
						//sprintf(temp,"%d",cdt_signal_seq);
						//m_exchange += temp;
						//m_exchange += remind_rightkuohao;
						//m_exchange += remind_space;
						//cur_position++;

						//遥信功能码
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);
						m_exchange += temp;
						m_exchange += remind_leftkuohao;

						switch (cdt_signal_begin)
						{
						case 0xF0:
							m_exchange += _C2Q("遥信1-32");
							break;
						case 0xF1:
							m_exchange += _C2Q("遥信33-64");
							break;
						case 0xF2:
							m_exchange += _C2Q("遥信65-96");
							break;
						case 0xF3:
							m_exchange += _C2Q("遥信97-128");
							break;
						case 0xF4:
							m_exchange += _C2Q("遥信129-160");
							break;
						case 0xF5:
							m_exchange += _C2Q("遥信161-192");
							break;
						case 0xF6:
							m_exchange += _C2Q("遥信193-224");
							break;
						case 0xF7:
							m_exchange += _C2Q("遥信225-256");
							break;
						case 0xF8:
							m_exchange += _C2Q("遥信257-288");
							break;
						case 0xF9:
							m_exchange += _C2Q("遥信289-320");
							break;
						case 0xFA:
							m_exchange += _C2Q("遥信321-352");
							break;
						case 0xFB:
							m_exchange += _C2Q("遥信353-384");
							break;
						case 0xFC:
							m_exchange += _C2Q("遥信385-416");
							break;
						case 0xFD:
							m_exchange += _C2Q("遥信417-448");
							break;
						case 0xFE:
							m_exchange += _C2Q("遥信449-480");
							break;
						case 0xFF:
							m_exchange += _C2Q("遥信481-512");
							break;
						default:
							break;
						}

						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;


						cdt_di_value = (BYTE)pData[cur_position];
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //遥信四字节1
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						
						if ((cdt_di_value & 0x01) == 0x01)  //遥信1
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						if ((cdt_di_value & 0x02) == 0x02)  //遥信2
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						if ((cdt_di_value & 0x04) == 0x04)  //遥信3
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						if ((cdt_di_value & 0x08) == 0x08)  //遥信4
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						if ((cdt_di_value & 0x10) == 0x10)  //遥信5
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						if ((cdt_di_value & 0x20) == 0x20)  //遥信6
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						if ((cdt_di_value & 0x40) == 0x40)  //遥信7
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						if ((cdt_di_value & 0x80) == 0x80)  //遥信8
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");
						
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						cdt_di_value = (BYTE)pData[cur_position];
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //遥信四字节2
						m_exchange += temp;
						m_exchange += remind_leftkuohao;

						if ((cdt_di_value & 0x01) == 0x01)  //遥信1
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						if ((cdt_di_value & 0x02) == 0x02)  //遥信2
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						if ((cdt_di_value & 0x04) == 0x04)  //遥信3
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						if ((cdt_di_value & 0x08) == 0x08)  //遥信4
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						if ((cdt_di_value & 0x10) == 0x10)  //遥信5
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						if ((cdt_di_value & 0x20) == 0x20)  //遥信6
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						if ((cdt_di_value & 0x40) == 0x40)  //遥信7
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						if ((cdt_di_value & 0x80) == 0x80)  //遥信8
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						cdt_di_value = (BYTE)pData[cur_position];
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //遥信四字节3
						m_exchange += temp;
						m_exchange += remind_leftkuohao;

						if ((cdt_di_value & 0x01) == 0x01)  //遥信1
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						if ((cdt_di_value & 0x02) == 0x02)  //遥信2
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						if ((cdt_di_value & 0x04) == 0x04)  //遥信3
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						if ((cdt_di_value & 0x08) == 0x08)  //遥信4
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						if ((cdt_di_value & 0x10) == 0x10)  //遥信5
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						if ((cdt_di_value & 0x20) == 0x20)  //遥信6
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						if ((cdt_di_value & 0x40) == 0x40)  //遥信7
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						if ((cdt_di_value & 0x80) == 0x80)  //遥信8
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						cdt_di_value = (BYTE)pData[cur_position];
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //遥信四字节4
						m_exchange += temp;
						m_exchange += remind_leftkuohao;

						if ((cdt_di_value & 0x01) == 0x01)  //遥信1
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						if ((cdt_di_value & 0x02) == 0x02)  //遥信2
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						if ((cdt_di_value & 0x04) == 0x04)  //遥信3
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						if ((cdt_di_value & 0x08) == 0x08)  //遥信4
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						if ((cdt_di_value & 0x10) == 0x10)  //遥信5
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						if ((cdt_di_value & 0x20) == 0x20)  //遥信6
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						if ((cdt_di_value & 0x40) == 0x40)  //遥信7
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						if ((cdt_di_value & 0x80) == 0x80)  //遥信8
						{
							m_exchange += _C2Q("合");
						}
						else
							m_exchange += _C2Q("分");

						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //校验码
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_frame_crc;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						m_exchange += "*";    //插入分隔符,0711
						cdt_frame_count++;
						cur_position++;
					}

				}
			
				break;
			case CDT_FRAME_PS:
				m_exchange += cdt_frame_ps;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;    //移动到信息字数

				//信息字数
				cdt_signal_length = (BYTE)pData[cur_position];
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_len;        
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//源站地址
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_source_addr;
				sprintf(temp,"%d",(BYTE)m_pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//目的站地址
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_target_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//校验码
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_crc;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;

				m_exchange += "*";    //插入分隔符,0711
				cdt_frame_count++;
				cur_position++;

				//信息部分

				for (i = 0; i < cdt_signal_length; i++)
				{
					cdt_signal_begin = (BYTE)pData[cur_position];   //信息起始位

					if ((cdt_signal_begin >= 0xA0) && (cdt_signal_begin  <= 0xDF))  //遥脉功能码范围
					{
						//遥脉功能码（起始号）
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_func_ym_begin;
						cdt_signal_seq = (BYTE)pData[cur_position] - 159;    //遥脉序号，-0xa0 + 1
						sprintf(temp,"%d",cdt_signal_seq);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;



						if (((BYTE)pData[cur_position + 3] & 0x20) == 0x00)   //二进制码，b29 == 0
						{
							cdt_mea_value = usghextoint((BYTE*)&pData[cur_position], 3); 
						}

						if (((BYTE)pData[cur_position + 3] & 0x20) == 0x20)   //BCD码，b29 == 1
						{
							cdt_mea_value = ((BYTE)pData[cur_position] & 0x0F) * 0.01
								             + (((BYTE)pData[cur_position] & 0xF0) >> 4) * 0.1
											 +((BYTE)pData[cur_position+1] & 0x0F) * 1
											 + (((BYTE)pData[cur_position+1] & 0xF0) >> 4) * 10
											 +((BYTE)pData[cur_position+2] & 0x0F) * 100
											 + (((BYTE)pData[cur_position+2] & 0xF0) >> 4) * 1000; 
						}
						 
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);    //脉冲4字节
						m_exchange += temp;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);    //脉冲4字节
						m_exchange += temp;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);    //脉冲4字节
						m_exchange += temp;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);    //脉冲4字节
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += _C2Q("值:");
						sprintf(temp, "%d", cdt_mea_value);
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //校验码
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_frame_crc;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						m_exchange += "*";    //插入分隔符,0711
						cdt_frame_count++;
						cur_position++;
					}

				}				

				break;
			case CDT_FRAME_SOE:
				break;
			case CDT_FRAME_CTRL_CON:
				break;
			default:
				break;
			}
		}

		if (nType == 2)   //SEND
		{
			switch (cdt_type)
			{
			case CDT_FRAME_CTRL_SEL:
				m_exchange += cdt_frame_ctrl_sel;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;    //移动到信息字数


				//信息字数
				cdt_signal_length = (BYTE)pData[cur_position];
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_len;        
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//源站地址
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_source_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//目的站地址
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_target_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//校验码
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_crc;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;


				m_exchange += "*";    //插入分隔符,0711
				cdt_frame_count++;


				//E0 CC 06 CC 06 F8
				//E0 CC 06 CC 06 F8
				//E0 CC 06 CC 06 F8
				for (i = 0; i < cdt_signal_length; i++)
				{
					if ( (BYTE)pData[cur_position] == 0xE0)  //遥控选择
					{
						
						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //E0
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_func_ctrl_sel;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //CC
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						if((BYTE)pData[cur_position] == 0xCC)
						{
							m_exchange += _C2Q("控合");
						}
						if((BYTE)pData[cur_position] == 0x33)
						{
							m_exchange += _C2Q("控分");

						}
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //06
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						sprintf(temp, "%d", (BYTE)pData[cur_position]); 
						m_exchange += _C2Q("遥控号:");
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //CC
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						if((BYTE)pData[cur_position] == 0xCC)
						{
							m_exchange += _C2Q("控合");
						}
						if((BYTE)pData[cur_position] == 0x33)
						{
							m_exchange += _C2Q("控分");

						}
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //06
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						sprintf(temp, "%d", (BYTE)pData[cur_position]); 
						m_exchange += _C2Q("遥控号:");
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //校验码
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_frame_crc;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						m_exchange += "*";    //插入分隔符,0711
						cdt_frame_count++;
						cur_position++;					
					}

				}
			
				
				break;
			case CDT_FRAME_CTRL_EXE:
				m_exchange += cdt_frame_ctrl_exe;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;    //移动到信息字数


				//信息字数
				cdt_signal_length = (BYTE)pData[cur_position];
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_len;        
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//源站地址
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_source_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//目的站地址
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_target_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//校验码
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_crc;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;


				m_exchange += "*";    //插入分隔符,0711
				cdt_frame_count++;


				//E2 AA 06 AA 06 F8
				//E2 AA 06 AA 06 F8
				//E2 AA 06 AA 06 F8
				for (i = 0; i < cdt_signal_length; i++)
				{
					if ( (BYTE)pData[cur_position] == 0xE2)  //遥控执行
					{

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //E2
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_func_ctrl_exe;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //AA
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						if((BYTE)pData[cur_position] == 0xAA)
						{
							m_exchange += _C2Q("执行");
						}
					
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //06
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						sprintf(temp, "%d", (BYTE)pData[cur_position]); 
						m_exchange += _C2Q("遥控号:");
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //AA
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						if((BYTE)pData[cur_position] == 0xAA)
						{
							m_exchange += _C2Q("执行");
						}
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //06
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						sprintf(temp, "%d", (BYTE)pData[cur_position]); 
						m_exchange += _C2Q("遥控号:");
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //校验码
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_frame_crc;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						m_exchange += "*";    //插入分隔符,0711
						cdt_frame_count++;
						cur_position++;					
					}

				}


				break;
			case CDT_FRAME_CTRL_CAN:
				m_exchange += cdt_frame_ctrl_can;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;    //移动到信息字数


				//信息字数
				cdt_signal_length = (BYTE)pData[cur_position];
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_len;        
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//源站地址
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_source_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//目的站地址
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_target_addr;
				sprintf(temp,"%d",(BYTE)pData[cur_position]);
				m_exchange += temp;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;

				//校验码
				sprintf(temp, "%02X", (BYTE)pData[cur_position]); 
				m_exchange += temp;
				m_exchange += remind_leftkuohao;
				m_exchange += cdt_frame_crc;
				m_exchange += remind_rightkuohao;
				m_exchange += remind_space;
				cur_position++;


				m_exchange += "*";    //插入分隔符,0711
				cdt_frame_count++;


				//E3 55 06 55 06 F2
				//E3 55 06 55 06 F2
				//E3 55 06 55 06 F2
				for (i = 0; i < cdt_signal_length; i++)
				{
					if ( (BYTE)pData[cur_position] == 0xE3)  //遥控撤销
					{

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //E3
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_func_ctrl_cle;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //55
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						if((BYTE)pData[cur_position] == 0x55)
						{
							m_exchange += _C2Q("撤销");
						}

						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //06
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						sprintf(temp, "%d", (BYTE)pData[cur_position]); 
						m_exchange += _C2Q("遥控号:");
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //AA
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						if((BYTE)pData[cur_position] == 0x55)
						{
							m_exchange += _C2Q("撤销");
						}
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);  //06
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						sprintf(temp, "%d", (BYTE)pData[cur_position]); 
						m_exchange += _C2Q("遥控号:");
						m_exchange += temp;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;
						cur_position++;

						sprintf(temp, "%02X", (BYTE)pData[cur_position]);   //校验码
						m_exchange += temp;
						m_exchange += remind_leftkuohao;
						m_exchange += cdt_frame_crc;
						m_exchange += remind_rightkuohao;
						m_exchange += remind_space;

						m_exchange += "*";    //插入分隔符,0711
						cdt_frame_count++;
						cur_position++;					
					}

				}
				break;
			case CDT_FRAME_UPDOWN_SEL:	
				break;
			case CDT_FRAME_UPDOWN_EXE:	
				break;
			case CDT_FRAME_UPDOWN_CAN:	
				break;
			case CDT_FRAME_IDI_SET:	
				break;
			case CDT_FRAME_CLOCK_SET:	
				break;
			case CDT_FRAME_CLOCK_CON:	
				break;
			case CDT_FRAME_CLOCK_SUM:	
				break;
			case CDT_FRAME_RESET:	
				break;
			case CDT_FRAME_BROD:	
				break;
			default:
				break;
			
			}
		}


	}



}



// bool CNetModulaData::MatchKeyWord(CNetKeyWord* pKeyWord)
// {
// 	QList<int> listMatchIndex;
// 
// 	if(pKeyWord->m_ListSection.isEmpty())
// 		return false;
// 
// 	bool bMatchFixed = false;
// 	int nCurrentIndex = 0;
// 	QListIterator<CNetKeySection*> findIterator(pKeyWord->m_ListSection);
// 	findIterator.toFront();
// 	while(findIterator.hasNext())
// 	{
// 		if(nCurrentIndex >= m_nLength)
// 			return false;
// 
// 		CNetKeySection* pSetction = findIterator.next();
// 		if(pSetction)
// 		{
// 			switch(pSetction->GetType())
// 			{
// 			case KEYSECTION_DATA:
// 				{
// 					if(bMatchFixed)
// 					{
// 						if(pSetction->GetKey() != (uchar)(m_pData[nCurrentIndex]))
// 							return false;
// 						listMatchIndex.push_back(nCurrentIndex);
// 						nCurrentIndex++;
// 					}
// 					else
// 					{
// 						while(nCurrentIndex < m_nLength)
// 						{
// 							if(pSetction->GetKey() == (uchar)(m_pData[nCurrentIndex]))
// 								break;
// 							nCurrentIndex++;
// 						}
// 						if(nCurrentIndex >= m_nLength)
// 							return false;
// 						listMatchIndex.push_back(nCurrentIndex);
// 						nCurrentIndex++;
// 					}
// 					bMatchFixed = true;
// 				}
// 				break;
// 
// 			case KEYSECTION_ANYONE:
// 				bMatchFixed = true;
// 				nCurrentIndex++;
// 				break;
// 
// 			case KEYSECTION_ANYMORE:
// 				bMatchFixed = false;
// 				break;
// 
// 			default:
// 				return false;
// 				break;
// 			}
// 		}
// 	}
// 
// 	if(listMatchIndex.isEmpty())
// 		return false;
// 
// 	QListIterator<int> setIterator(listMatchIndex);
// 	setIterator.toFront();
// 	while(setIterator.hasNext())
// 	{
// 		int nSetIndex = setIterator.next();
// 		if((nSetIndex >= 0) && (nSetIndex < m_nLength))
// 			m_pKeyFlag[nSetIndex] = 1;
// 	}
// 	return true;
// }
