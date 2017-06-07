// QEventSound.cpp: implementation of the QEventSound class.
//
//////////////////////////////////////////////////////////////////////
#include "event_sound.h"

#ifdef _WIN32
	#include <mmsystem.h>
#endif

#include "globaldef.h"
#include "sysdef.h"

#ifdef __unix
#include <string.h>
#ifdef _AIX 
#include <termios.h>
#include <sys/ioctl.h>
#elif __linux
#include <termios.h>
#else
#include <sys/termios.h>
#endif
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


#ifdef __linux
void SetAPortMode( int id )
{
	struct  termios tty_termios, tty_tt;
	memset(&tty_termios, 0, sizeof(tty_termios));

	tty_termios.c_cc[VMIN]=1;
	tty_termios.c_cc[VTIME]=0;
	tty_termios.c_cflag |= CLOCAL;
	tty_termios.c_cflag |= CREAD;
	tty_termios.c_cflag |= CS8;
	tty_termios.c_cflag |= CSTOPB;
	tty_termios.c_cflag |= B9600 ;
	
	tcsetattr (id, TCSANOW, &tty_termios);
}
#elif (defined(__sun) || defined(__hpux))
void SetAPortMode( int id )
{
	struct  termios tty_termios, tty_tt;
	memset(&tty_termios, 0, sizeof(tty_termios));

	tty_termios.c_cc[VMIN]=1;
	tty_termios.c_cc[VTIME]=0;
	tty_termios.c_cflag |= CLOCAL;
	tty_termios.c_cflag |= CREAD;
	tty_termios.c_cflag |= CS8;	//	8位数据位
	
	//tty_termios.c_cflag |= CSTOPB;//1位停止位
	//tty_termios.c_cflag |= PARENB;//无校验
	
	tty_termios.c_cflag |= B9600 ;//9600波特率
	
	
	if( ioctl(id,TCSETS,&tty_termios) > 0 )
	{
		printf( "设置串口错误\n" );
	}
}
#elif defined(_AIX) && defined(__unix)
void SetAPortMode ( int id)
{
	struct  sgttyb sgttyb1;
	struct  termios tty_termios, tty_tt;
	int	flags;

	flags = fcntl(id, F_GETFL);
	fcntl(id, F_SETFL, flags |  O_NONBLOCK);

	ioctl(id,TIOCGETP,&sgttyb1);
	sgttyb1.sg_flags |= RAW;
	sgttyb1.sg_flags &= ~ECHO;
	ioctl(id,TIOCSETP,&sgttyb1);
	if ((tcgetattr (id, &tty_termios) == -1)) {
		perror ("tcgetattr() failed");
		exit (1);
	}

	tty_termios.c_cflag |= HUPCL;
	tty_termios.c_cflag |= CREAD;
	tty_termios.c_cflag |= CS8;
	tty_termios.c_cflag |= CLOCAL;
	tty_termios.c_cflag |= B9600 ;
	
	if ((tcsetattr (id, TCSANOW, &tty_termios) == -1))
	{
		perror ("tcsetattr() failed at TCSANOW");
//		printf("Channel %d  dev:%s Para Error\n",chanNo,chanp->devName);
	}
}
#elif defined (WIN32)
void SetAPortMode( int id)
{
}
#endif

// QEventSound::QEventSound()
//  : m_done(FALSE),QThread( 1024*1024 )
// {
// 	//m_eventList.setAutoDelete( true );
// 	m_toneDevStatus = -1;
// }
Event_sound::Event_sound(): m_done(FALSE),QThread()
{
	//m_eventList.setAutoDelete( true );
	setStackSize(1024*1024) ;
	m_tone_dev_status = -1;
}

Event_sound::~Event_sound()
{

}

void Event_sound::run()
{
	char filename[ 256 ];
	char env[ 256 ];
	QString soundFile, tmpStr;
	int index;
	QStringList soundFileList;
	uint i;
	Rsys_event *pEvent;

	NetDbg dbg;

	sprintf( env, "%s", getenv("NBENV") );

	if( m_tone_dev.isOpen() )
	{
		m_tone_dev.close();
	}

	printf( "语音设备为%s\n", _Q2C( m_tone_dev_name ) );

	if( !m_tone_dev_name.isEmpty() )
	{
		printf( "打开语音设备\n" );
		m_tone_dev.setFileName( m_tone_dev_name );
// 		if( !m_toneDev.open( IO_Raw | /*IO_Async |*/ IO_ReadWrite ) )
		if( !m_tone_dev.open( QIODevice::Unbuffered | /*IO_Async |*/ QIODevice::ReadWrite ) )
		{
			printf( "open %s failed\n", _Q2C( m_tone_dev_name ) );
		}
		SetAPortMode( m_tone_dev.handle() );
	}

    bool stop = FALSE;
	{
		TRY_LOCK;
		stop = m_done;
	}
	while( !stop )
	{
		//
		// 取第一个事项、语音列表
		//
		if( m_event_list.count() > 0 )
		{
			TRY_LOCK;
			pEvent = m_event_list.first();
			soundFile =  tr( pEvent->tone );
// 			soundFile = soundFile.stripWhiteSpace();
			soundFile = soundFile.trimmed();
			//savePlayFileName( _C2Q( "d:/ds3000/dat/eve2.txt" ), pEvent );
		}
		else
		{
			msleep( 100 );
			TRY_LOCKSTOP;
			stop = m_done;
			continue;
		}

		//
		// 解析语音列表
		//
		if( !m_tone_dev.isOpen() )
		{
			soundFileList.clear();
			while( ( index = soundFile.indexOf( ' ', 0 ) ) != -1 )
			{
				tmpStr = soundFile.left( index );
// 				soundFile = soundFile.right( soundFile.length() - index - 1 ).stripWhiteSpace();
				soundFile = soundFile.right( soundFile.length() - index - 1 ).trimmed();

// 				tmpStr = tmpStr.stripWhiteSpace();
				tmpStr = tmpStr.trimmed();

				sprintf( filename, "%s/resource/wav/%s", env, _Q2C( tmpStr ) );


				if( QFile::exists( tr( filename ) ) )
				{
					//printf( "wav=%s\n", filename );
					soundFileList.append( tr( filename ) );
				}
				else 
					dbg.PutDbg( RALARMDEBUG, "语音文件[%s]不存在！", filename);
			}

			if( !soundFile.isEmpty() )
			{
// 				soundFile = soundFile.stripWhiteSpace();
				soundFile = soundFile.trimmed();
				sprintf( filename, "%s/resource/wav/%s", env, _Q2C( soundFile ) );

				if( QFile::exists( tr( filename ) ) )
				{
					//printf( "wav=%s\n", filename );
					soundFileList.append( tr( filename ) );
				}
				else 
					dbg.PutDbg( RALARMDEBUG, "语音文件[%s]不存在！", filename);
			}

			//
			// 逐段播放语音，并判断事项是否中止
			//
			for( i = 0; i < soundFileList.count(); i ++ )
			{
				bool bRemoved = false;
				{
					TRY_LOCK;
					bRemoved = (m_event_sounds.find( pEvent ) == m_event_sounds.end());
				}
				if( !bRemoved )
				{
					play(  soundFileList[ i ]  );
				}
				else
					break;
			}
		}
		else
		{
			/*readStatus();
			if( m_toneDevStatus )
			{
				msleep( 100 );
				continue;
			}*/
			play_text( tr( pEvent->text ) );
			
		}

		//
		// 本事项语音播报次数处理(-1 循环报本条事项语音)
		//
		{
			TRY_LOCK;
			//if( m_eventSounds.find( pEvent ) != m_eventSounds.end())
			if( m_event_sounds.contains( pEvent ) )
			{
				int nCount = m_event_sounds[ pEvent ];
				if(nCount>1) {
					m_event_sounds[ pEvent ] = nCount - 1;
				}
				else if(nCount>=0) 
				{
// 					m_eventList.remove( pEvent );
					int i = m_event_list.indexOf(pEvent);
					if (i != -1)
						m_event_list.takeAt(i);
					m_event_sounds.remove( pEvent );
				}
				else { // 循环报 -1
				}
			}
		}
		
		{
			TRY_LOCKSTOP;
			stop = m_done;
		}
		msleep( 100 );
	}

	soundFileList.clear();

	if( m_tone_dev.isOpen() )
	{
		m_tone_dev.close();
	}
}

void Event_sound::play( const QString &filename )
{
#ifdef _WIN32
	sndPlaySound ( _Q2C( filename ), SND_SYNC );
#else
	mySound.play( _Q2C( filename ) );
#endif
}

void Event_sound::play_text( const QString &text )
{
	if( !m_tone_dev.isOpen() )
	{
		//m_toneDev.setName( _C2Q( "COM2" ) );
// 		bool ret = m_toneDev.open( IO_Raw | /*IO_Async |*/ IO_ReadWrite );
		bool ret = m_tone_dev.open( QIODevice::Unbuffered | /*IO_Async |*/ QIODevice::ReadWrite );

		if( !ret )
		{
			printf( "串口打开失败!\n" );
		}
		SetAPortMode( m_tone_dev.handle() );
	}

	QString tmptext = text;
	tmptext = tmptext.replace( ' ', ',' );



	char buf[ 1024 ];
	memset( buf, 0, 1024 );
	uint datalen = tmptext.length() * 2 + 2;
	buf[ 0 ] = 0xFD;
	buf[ 1 ] = (datalen & 0xff00) >> 8;
	buf[ 2 ] = datalen & 0xff;
	buf[ 3 ] = 0x01;
	buf[ 4 ] = 0x03;
	
	int i;
	ushort tmpus;
	for( i = 0; i < tmptext.length(); i ++ )
	{
		tmpus = tmptext.at( i ).unicode();
		buf[ 5 + i * 2 ] = tmpus & 0xff;
		buf[ 5 + i * 2 + 1 ] = (tmpus & 0xff00) >> 8;
	}
// 	int ret = m_toneDev.writeBlock( buf, datalen + 3 );
	int ret = m_tone_dev.write( buf, datalen + 3 );
	if( ret <= 0 )
	{
		printf( "语音文字输出失败\n" );
	}
}


void Event_sound::add_sound_event( Rsys_event *eve, int soundcount )
{
	if( soundcount == 0 )
		return;

	//savePlayFileName( _C2Q( "d:/ds3000/dat/eve1.txt" ), eve );
// 	if( !_C2Q( eve->tone ).stripWhiteSpace().isEmpty() ) 
	if( !tr( eve->tone ).trimmed().isEmpty() ) 
	{
		/*Rsys_event * e = new Rsys_event(eve);
		m_eventList.append(e);
		m_eventSounds[ e ] = soundcount;*/
		TRY_LOCK;
		if( soundcount == -1 )
		{
			m_event_list.insert( 0, eve );
		}
		else
		{
			m_event_list.append( eve );
		}
		m_event_sounds[ eve ] = soundcount;

		//savePlayFileName( _C2Q( "d:/ds3000/dat/eve.txt" ), eve );
	}
}

void Event_sound::read_status()
{
	if( !m_tone_dev.isOpen() )
		return;

	char buf[ 1024 ];
	int i;
	buf[ 0 ] = 0xFD;
	buf[ 1 ] = 0x00;
	buf[ 2 ] = 0x01;
	buf[ 3 ] = 0x21;
// 	m_toneDev.writeBlock( buf, 4 );
	m_tone_dev.write( buf, 4 );
// 	int retlen = m_toneDev.readBlock( buf, 1024 );
	int retlen = m_tone_dev.read( buf, 1024 );

	for( i = 0; i < retlen; i ++ )
	{
		if( buf[ i ] == 0x4F )
			m_tone_dev_status = 0;
		else
			m_tone_dev_status = 1;
	}

}

void Event_sound::confirm_event( Rsys_event* eve )
{
	TRY_LOCK;
	if( m_event_list.contains( eve ) )
	{
		//savePlayFileName( _C2Q( "d:/ds3000/dat/eve3.txt" ), eve );
		m_event_list.removeOne( eve );
// 		int i = m_eventList.indexOf(eve);
// 		if (i != -1)
// 			delete m_eventList.takeAt(i);
		m_event_sounds.remove( eve );
	}
}

void Event_sound::stop()
{
	TRY_LOCKSTOP;
	m_done=TRUE;
}

void Event_sound::save_play_filename( const QString &file, const QString &str )
{
	FILE *pfile;

	pfile = fopen( _Q2C( file ), "a+" );

	fprintf( pfile, "%s\n", _Q2C( str ) );

	fclose( pfile );
}

void Event_sound::save_play_filename( const QString &file, Rsys_event *eve )
{
	FILE *pfile;

	pfile = fopen( _Q2C( file ), "a+" );

	ulong lYmd = (ulong)(eve->ymd);
				int Year=(int)(lYmd/(100l*100l));
				int Month=(int)((lYmd%(100l*100l))/100);
				int Day=(int)((lYmd%(100l*100l))%100);

				ulong lTmp = (ulong)(eve->hmsms);
				int Hour = (int)(lTmp/(100l*100l*1000l));
				int Min = (int)((lTmp%(100l*100l*1000l))/(100l*1000l));
				int Sec = (int)(((lTmp%(100l*100l*1000l))%(100l*1000l))/1000l);
				int Ms = (int)(((lTmp%(100l*100l*1000l))%(100l*1000l))%1000l);


	fprintf( pfile, "%04d.%02d.%02d--%02d:%02d:%02d:%03d %s\n", Year,Month,Day,Hour, Min, Sec, Ms, 
		eve->tone );

	fclose( pfile );
}

void Event_sound::confirm_all_event()
{
	TRY_LOCK;
	m_event_list.clear( );
	m_event_sounds.clear( );
}

void Event_sound::set_tone_dev( const QString &dev )
{
	m_tone_dev_name = dev;
}
