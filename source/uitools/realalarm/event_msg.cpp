// QEventMsg.cpp: implementation of the QEventMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "event_msg.h"

#include <QLibrary>
#include <QMessageBox>

#include "globaldef.h"


Event_msg::Event_msg(): m_done(FALSE)
{
// 	m_eventList.setAutoDelete( true );
	QTimer *readTimer = new QTimer( this );
// 	connect( readTimer, SIGNAL(timeout()),this, SLOT(readMycom()) );
// 	readTimer->start( 5000 ,TRUE);
	readTimer->setSingleShot(TRUE);			//modify by chenkai
	readTimer->start( 5000 );
	bool i = readTimer->isActive();
	count = 0;
	m_brcv_msg = false;
}



Event_msg::~Event_msg()
{
}

void Event_msg::run()
{
// 	myCom=new Win_QextSerialPort(m_serial_no); 
	myCom=new QextSerialPort(m_serial_no); 
//     if(!myCom->open(IO_Raw | IO_ReadWrite))
	if(!myCom->open(QIODevice::Unbuffered | QIODevice::ReadWrite))
	{
		printf( "短信串口打开失败!\n" );
		return;
	}
	myCom->setBaudRate(BAUD57600); 
	//波特率设置，我们设置为57600 
    myCom->setDataBits(DATA_8); 
	//数据位设置，我们设置为8位数据位 
	
	myCom->setParity(PAR_NONE); 
	//奇偶校验设置，我们设置为无校验 
	myCom->setStopBits(STOP_1); 
	//停止位设置，我们设置为1位停止位 
	myCom->setFlowControl(FLOW_OFF); 
	//数据流控制设置，我们设置为无数据流控制 
	myCom->setTimeout(500); 
	//延时设置，我们设置为延时500ms,这个在Windows下好像不起作用 
	
	bool isopen = myCom->isOpen();

	printf( "短信设备为%s\n", _Q2C( m_serial_no ) );

	int		year, month, day, hour, min,sec,ms;
	ulong	lYmd,lHmsms;	
	QMap<Rsys_event*,QString>::Iterator curIt;
	uint	i;

	Rsys_event *pEvent;
	int index;
	uint sendfailnum = 0;
	QString phoneno;
	QStringList phonenos;
	char strText[ 1024 ];

    bool stop = FALSE;
	{
		TRY_LOCKSTOP;
		stop = m_done;
	}
	while( !stop )
	{

		if( m_msg_event_list.count() == 0 && m_msg_event_no_list.count() == 0 )
		{
			msleep( 100 );
			TRY_LOCKSTOP;
			stop = m_done;
			continue;
		}
		if(m_msg_event_list.count() > 0)
		{
			TRY_LOCK;
			QMap<Rsys_event*,QString>::Iterator curIt = m_msg_event_list.begin();

			printf( "短信确认时间为!%d\n",m_msgConfirmTime);

			pEvent = curIt.key();
			phoneno = curIt.value();

			lYmd = ( ulong)( pEvent->ymd );
			lHmsms = ( ulong )( pEvent->hmsms );

			phonenos.clear();

			while( ( index = phoneno.indexOf( ',' ) ) != -1 )
			{
// 				phonenos.append( phoneno.left( index ).stripWhiteSpace() );
				phonenos.append( phoneno.left( index ).trimmed() );
// 				phoneno = phoneno.right( phoneno.length() - index - 1 ).stripWhiteSpace();
				phoneno = phoneno.right( phoneno.length() - index - 1 ).trimmed();
			}

			if( !phoneno.isEmpty() )
			{
// 				phonenos.append( phoneno.stripWhiteSpace() );
				phonenos.append( phoneno.trimmed() );
			}

			year = ( int )( lYmd / ( 100l * 100l ) );
			month = ( int )( ( lYmd % ( 100l * 100l ) ) / 100 );
			day = ( int )( ( lYmd % ( 100l * 100l ) ) % 100 );

			hour = ( int )( lHmsms / ( 100l * 100l * 1000l ) );
			min = ( int )( ( lHmsms % ( 100l * 100l * 1000l ) ) / ( 100l * 1000l ) );
			sec = ( int )( ( ( lHmsms % ( 100l * 100l * 1000l ) ) % ( 100l * 1000l ) ) / 1000l );
			ms = ( int )( ( ( lHmsms % ( 100l * 100l * 1000l ) ) % ( 100l * 1000l ) ) % 1000l );

			sprintf( strText, "%04d.%02d.%02d--%02d:%02d:%02d:%03d %s", year, month, day, hour, min, sec, ms, 
				pEvent->text );

// 			m_eventList.remove( pEvent );
// 			int j = m_eventList.indexOf(pEvent);
// 			if (j != -1)
// 				delete m_eventList.takeAt(j);
			m_event_list.removeOne(pEvent) ;
			m_msg_event_list.remove( pEvent );

			for( i = 0; i < phonenos.count();i ++ )
			{
				send_message(phonenos[ i ],tr(strText));
				//建立号码和内容对应关系表
				msleep(8000);
				m_msg_event_no_list[phonenos[ i ]] =tr(strText);
			}
		}
		
		//接受短信
		//删除收到确认的号码和内容对应关系
		if (slot_read_mycom()==2)
		{
			if( m_msg_event_no_list.find( m_rcv_phone_no ) != m_msg_event_no_list.end() )
			{
				m_msg_event_no_list.remove( m_rcv_phone_no );
			}
		}
		
		//检索发送对应发送短信
		if (m_msg_event_no_list.count()>0 && m_brcv_msg)
		{
			msleep(100);
			count ++;
			if (count >=10*m_msgConfirmTime)
			{
				resend_message();
				count = 0;
			}
		}


// 		if( sendfailnum > 30 )
// 		{
// 			(*g_disconn)();
// 			connected = false;
// 		}
		
		msleep( 200 );
		TRY_LOCKSTOP;
		stop = m_done;
	}

// 	if( connected )
// 	{
// 		(*g_disconn)();
// 	}
	TRY_LOCK;
	m_msg_event_list.clear();
}


void Event_msg::add_msg_event( Rsys_event *pEvent, QString phoneno )
{
// 	if( phoneno.stripWhiteSpace().isEmpty() )
	if( phoneno.trimmed().isEmpty() )
		return;

	TRY_LOCK;

	Rsys_event * e = new Rsys_event;
	*e = *pEvent;
	m_event_list.append(e);
	m_msg_event_list[ e ] = phoneno;
}

void Event_msg::stop()
{
	TRY_LOCKSTOP;
	m_done=TRUE;
}

void Event_msg::confirm_event( Rsys_event* eve )
{
	TRY_LOCK;

	if( m_msg_event_list.find( eve ) != m_msg_event_list.end() )
	{
		m_msg_event_list.remove( eve );
	}
}


void Event_msg::send_message(QString &phoneno,const QString &text)
{

	QString phoneNo = phoneno;
	QString tmptext = text;
	// 	QString tmptext = tr( "你好" );

	printf( "开始发送短信,手机号码为:%s短信内容:%s\n", _Q2C( phoneNo ) ,_Q2C( tmptext ));

	char buf[ 1024 ];
	memset( buf, 0, 1024 );
	char phonenolen = phoneNo.length();
	buf[ 0 ] = 0x55;
	buf[ 1 ] = 0xaa;
	buf[ 2 ] = 0x01;
	buf[ 3 ] = phonenolen;

	int i;
	ushort tmpus;
	for( i = 0; i < phonenolen; i ++ )
	{
		tmpus = phoneNo.at( i ).unicode();
		buf[ 4 + i ] = tmpus & 0xff;
	}

	QStringList send_msg_list;
	while(tmptext.length()>70)
	{
		send_msg_list.append(tmptext.left(70));
		tmptext.remove(0,70);
	}

	if (!tmptext.isEmpty())
	{
		send_msg_list.append(tmptext);
	}

	for(int strnum = 0;strnum < send_msg_list.size(); strnum++)
	{
		tmptext.clear();
		tmptext = send_msg_list.at(strnum);
		printf( "开始发送第%d短信,短信内容:%s\n",strnum,_Q2C( tmptext ));
		int textlen = tmptext.length();

		buf[ 4 + phonenolen ] = textlen * 2;
		for( i = 0; i < tmptext.length(); i ++ )
		{
			tmpus = tmptext.at( i ).unicode();
			buf[ 5 + phonenolen + i * 2 ] = (tmpus & 0xff00) >> 8;
			buf[ 5 + phonenolen + i * 2 +1 ] = tmpus & 0xff;
		}



		int datalen = 5 + phonenolen + textlen * 2;

		int msgdata = myCom->write(buf,datalen);
		printf( "短信已发送!%d\n",msgdata);
		msleep( 8000 );
	}
}

void Event_msg::resend_message()
{
	QMap<QString,QString>::Iterator curMsg ;
	for (curMsg=m_msg_event_no_list.begin();curMsg !=m_msg_event_no_list.end();++curMsg)
	{
		QString phoneNo = curMsg.key();
		QString event = curMsg.value();
		send_message(phoneNo,event);
	}
}

int Event_msg::slot_read_mycom()
{
	m_bget_msg = false;
	int nbytewait = myCom->bytesAvailable();
 	if(nbytewait>=15) //如果可用数据大于或等于8字节再读取	
 	{
		printf( "接收到数据0\n");
		int phno_len = 0;
		int loop = 0;
		unsigned char rcv_buf[1024];
		memset( rcv_buf, 0 , 1024 );
		char temp_buf[500];
		ushort temp_buf1[500];
		QChar qchar[500];
		QString rcv_text;
		myCom->flush();
		int rcv_len = myCom->read((char *)rcv_buf,1024);
		printf("%d手机号码是%s\n",rcv_len,rcv_buf);				

		if ((rcv_buf[0] == 0x55) && (rcv_buf[1] == 0xAA))
		{
			if (rcv_buf[2] == 0xf1)
			{ 
				printf( "接收到数据1\n");

				if (0x01 == rcv_buf[15])
				{
					return 1;
				}
				else if (0x02 == rcv_buf[15])
				{
					printf( "send msg failed\n");

				}
			}
			else if (rcv_buf[2] == 0xf2)	//接收到短信的处理
			{
				printf( "接收到数据2\n");
				memset(temp_buf, 0, 500);  
				phno_len = rcv_buf[3];
				// 如果收到短信号码包括86,则去掉86
				if (13==phno_len)
				{
					memcpy(temp_buf, &rcv_buf[4+2], phno_len);
					m_rcv_phone_no = tr(temp_buf);
				}
				else if(11 == phno_len)
				{
					memcpy(temp_buf, &rcv_buf[4], phno_len);
					m_rcv_phone_no = tr(temp_buf);
				}

				for (int i = 0; i<phno_len;i++)
				{
					printf( "%c",rcv_buf[i+4]);
				}

				memset(temp_buf1, 0, 500);
				int textlen = rcv_buf[4 + phno_len];
				for(loop = 0; loop*2 < textlen; loop++) 
				{
					temp_buf1[loop] = rcv_buf[4 + phno_len + 1 + loop*2 + 1] + rcv_buf[4 + phno_len + 1 + loop*2]*0x100;
					qchar[loop]=temp_buf1[loop];
				}
				QString str0(qchar, textlen/2);
				m_rcv_text = str0;
				printf("接受到的数据是%s\n",_Q2C(m_rcv_text));
				m_bget_msg = TRUE;
				return 2;
			}

		} 
 		return 0; 
	} 
		return 0;
}

void Event_msg::set_serial_no( QString no )
{
	m_serial_no = no;
}

void Event_msg::set_msg_confirm_time( uint msgconfirmtime )
{
	m_msgConfirmTime = msgconfirmtime;
}
