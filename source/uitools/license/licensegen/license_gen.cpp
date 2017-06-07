#include "license_gen.h"
#include "license_gen_def.h"
#include "AES.h"
#include "rwxml.h"

#include <QNetworkInterface>
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>

DLicense_gen::DLicense_gen( QWidget* parent /*= 0 */ ) : QMainWindow( parent )
{
	setupUi( this ) ;
	ini_main_window() ;

	connect( m_pSelectList , SIGNAL( currentRowChanged( int ) ) , this , SLOT( on_change_widget( int ) ) ) ;
	connect( m_pOkBtn , SIGNAL( clicked() ) , this , SLOT( on_ok_btn() ) ) ;
	connect( m_pCancelBtn , SIGNAL( clicked() ) , this , SLOT( on_cancel_btn() ) ) ;
	connect( m_pMacBtn , SIGNAL( clicked() ) , this , SLOT( on_get_macaddr() ) ) ;
	connect( m_pGenPathBtn , SIGNAL( clicked() ) , this , SLOT( on_get_genpath() ) ) ;
	connect( m_pCheckPathBtn , SIGNAL( clicked() ) , this , SLOT( on_get_checkpath() ) ) ;
}

void DLicense_gen::ini_main_window( void )
{
	m_pSelectList->addItem( new QListWidgetItem( QIcon( ":/res/point.png" ), tr( "生成" ) ) ) ;
	m_pSelectList->addItem( new QListWidgetItem( QIcon( ":/res/point.png" ), tr( "查看" ) ) ) ;
	m_pSelectList->item( 0 )->setSelected( true ) ;

	m_pTimeComboBox->addItem( tr( "三  天" ) , 3 ) ;
	m_pTimeComboBox->addItem( tr( "一个月" ) , 30 ) ;
	m_pTimeComboBox->addItem( tr( "三个月" ) , 90 ) ;
	m_pTimeComboBox->addItem( tr( "半  年" ) , 180 ) ;
	m_pTimeComboBox->addItem( tr( "一  年" ) , 365 ) ;
	m_pTimeComboBox->addItem( tr( "无限期" ) , -1 ) ;

	m_pHsplitter->setStretchFactor( 0 , 1 ) ;
	m_pHsplitter->setStretchFactor( 1 , 3 ) ;
}

void DLicense_gen::on_change_widget( int list_index )
{
	if ( list_index == 0 )
	{
		m_pSelectList->item( 0 )->setIcon( QIcon( ":/res/star.png" ) ) ;
		m_pSelectList->item( 1 )->setIcon( QIcon( ":/res/point.png" ) ) ;
		m_pGenLicFrame->show() ;
		m_pCheckLicFrame->hide() ;
	}
	else if ( list_index == 1 )
	{
		m_pSelectList->item( 0 )->setIcon( QIcon( ":/res/point.png" ) ) ;
		m_pSelectList->item( 1 )->setIcon( QIcon( ":/res/star.png" ) ) ;
		m_pGenLicFrame->hide() ;
		m_pCheckLicFrame->show() ;
	}
}

void DLicense_gen::on_ok_btn()
{
	if ( m_pSelectList->currentRow() == 0 )
	{
		gen_license() ;
	}
	else
	{
		check_license() ;
	}
}

void DLicense_gen::gen_license( void )
{
	if ( m_pMacLineEdit->text().isEmpty() )
	{
		QMessageBox::warning( this , tr( "告警提示" ) , tr( "请输入Mac地址！" ) ) ;
		return;
	}

	if ( m_pGenPathLineEdit->text().isEmpty() )
	{
		QMessageBox::warning( this , tr( "告警提示" ) , tr( "请选择生成路径！" ) ) ;
		return;
	}

	QString strLicPath = m_pGenPathLineEdit->text() + _C2Q( "/license.lic" ) ;

    QFile f( strLicPath ) ;
    if ( f.exists() )
    {
		int tmp_res = QMessageBox::warning( this , tr("告警提示") , tr("文件存在，是否覆盖？") ,
			QMessageBox::Ok | QMessageBox::Cancel,
			QMessageBox::Ok ) ;
        switch ( tmp_res )
        {
        case QMessageBox::Ok:
            break ;
        case QMessageBox::Cancel:
            return ;
        }
    }

// 	ini_RW* pini = new ini_RW( ( char*) _Q2C( strLicPath ) ) ;
	Xml_RW* pini = new Xml_RW( ( char*) _Q2C( strLicPath ) ) ;

    // AES加密算法初始化key
    AES aes( aes_key ) ;
    unsigned char key[ 64 ] = { 0 } ;
    unsigned char value[ 64 ] = { 0 } ;
	char hexKey[ 128 ] = { 0 } ;
	char hexValue[ 128 ] = { 0 } ;

    // 加密
	QString key_str = tr( "授权周期" ) ;
	memset( key , 0 , 64 ) ;
	sprintf( ( char* )key , _Q2C( key_str ) ) ;
	aes.Cipher( ( void* )key ) ;
	ByteToHexStr( key , hexKey , 32 ) ;

    // 周期value
    int period = m_pTimeComboBox->itemData( m_pTimeComboBox->currentIndex() ).toInt() ;
    memset( value , 0 , 64 ) ;
	sprintf( ( char* )value , "%d" , period ) ;
	aes.Cipher( ( void* )value ) ;
	ByteToHexStr( value , hexValue , 32 ) ;
	pini->set_string( "license" , ( char* )hexKey , ( char* )hexValue ) ;

	key_str = tr( "文件生成时间" ) ;
	memset( key , 0 , 64 ) ;
	sprintf( ( char* )key , _Q2C( key_str ) ) ;
	aes.Cipher( ( void* )key ) ;
	ByteToHexStr( key , hexKey , 32 ) ;

	// 文件生成时间value
	QDateTime file_date_time = QDateTime::currentDateTime() ;
	QString file_date = file_date_time.toString( "yyyy-MM-dd hh:mm:ss" ) ;
	memset( value , 0 , 64 ) ;
	sprintf( ( char* )value , _Q2C( file_date ) ) ;
	aes.Cipher( ( void* )value ) ;
	ByteToHexStr( value , hexValue , 32 ) ;
	pini->set_string( "license" , ( char* )hexKey , ( char* )hexValue ) ;

	key_str = tr( "MAC地址" ) ;
	memset( key , 0 , 64 ) ;
	sprintf( ( char* )key , _Q2C( key_str ) ) ;
	aes.Cipher( ( void* )key ) ;
	ByteToHexStr( key , hexKey , 32 ) ;

	// 文件生成时间value
	memset( value , 0 , 64 ) ;
	sprintf( ( char* )value , _Q2C( m_pMacLineEdit->text() ) ) ;
	aes.Cipher( ( void* )value ) ;
	ByteToHexStr( value , hexValue , 32 ) ;
	pini->set_string( "license" , ( char* )hexKey , ( char* )hexValue ) ;

	key_str = tr( "授权是否已使用" ) ;
	memset( key , 0 , 64 ) ;
	sprintf( ( char* )key , _Q2C( key_str ) ) ;
	aes.Cipher( ( void* )key ) ;
	ByteToHexStr( key , hexKey , 32 ) ;

	// 证书是否已使用value
	memset( value , 0 , 64 ) ;
	sprintf( ( char* )value , "否" ) ;
	aes.Cipher( ( void* )value ) ;
	ByteToHexStr( value , hexValue , 32 ) ;
	pini->set_string( "license" , ( char* )hexKey , ( char* )hexValue ) ;

	if( pini )
	{
		delete( pini ) ;
		pini = NULL ;
	}

    QMessageBox::information( this , tr( "提示" ) , tr( "导出授权文件成功！") ) ;
}

void DLicense_gen::check_license( void )
{
	if ( m_pCheckPathLineEdit->text().isEmpty() )
	{
		QMessageBox::warning( this , tr( "告警提示" ) , tr("请选择查看文件！" ) ) ;
		return ;
	}

	QString strFilePath = m_pCheckPathLineEdit->text() ;
	QFile f( strFilePath ) ;
	if ( !f.exists() )
	{
		QMessageBox::warning( this , tr( "告警提示" ) , tr("文件不存在！" ) ) ;
		return ;
	}

	m_pDetailTable->setRowCount( 4 ) ;

// 	ini_RW* pini = new ini_RW( ( char*) _Q2C( strFilePath ) ) ;
	Xml_RW* pini = new Xml_RW( ( char*) _Q2C( strFilePath ) ) ;

	AES aes( aes_key ) ;
	unsigned char key[ 64 ] = { 0 } ;
	unsigned char value[ 64 ] = { 0 } ;
	char hexKey[ 128 ] = { 0 } ;
	char *hexValue = NULL ;

	// 加密
	QString key_str = tr( "授权周期" ) ;
	memset( key , 0 , 64 ) ;
	sprintf( ( char* )key , _Q2C( key_str ) ) ;
	aes.Cipher( ( void* )key ) ;
	ByteToHexStr( key , hexKey , 32 ) ;

	if ( SUCCESS != pini->get_string( "license" , ( char* )hexKey , hexValue ) )
	{
		QMessageBox::warning( this , tr( "告警提示" ) , tr("授权文件破损！" ) ) ;
		return ;
	}
	else
	{
		HexStrToByte( hexValue , value , strlen( hexValue ) ) ;
		aes.InvCipher( ( void* )value , strlen( hexValue ) )  ;
	}
	m_pDetailTable->setItem( 0 , 0 , new QTableWidgetItem( _C2Q( "授权周期" ) ) ) ;
	m_pDetailTable->setItem( 0 , 1 , new QTableWidgetItem( _C2Q( ( char* )( value ) ) + _C2Q( "天" ) ) ) ;

	key_str = tr( "文件生成时间" ) ;
	memset( key , 0 , 64 ) ;
	sprintf( ( char* )key , _Q2C( key_str ) ) ;
	aes.Cipher( ( void* )key ) ;
	ByteToHexStr( key , hexKey , 32 ) ;
	hexValue = NULL ;
	if ( SUCCESS != pini->get_string( "license" , ( char* )hexKey , hexValue ) )
	{
		QMessageBox::warning( this , tr( "告警提示" ) , tr("授权文件破损！" ) ) ;
		return ;
	}
	else
	{
		HexStrToByte( hexValue , value , strlen( hexValue ) ) ;
		aes.InvCipher( ( void* )value , strlen( hexValue ) )  ;
	}

	m_pDetailTable->setItem( 1 , 0 , new QTableWidgetItem( _C2Q( "文件生成时间" ) ) ) ;
	m_pDetailTable->setItem( 1 , 1 , new QTableWidgetItem( _C2Q( ( char* )( value ) ) ) ) ;

	key_str = tr( "MAC地址" ) ;
	memset( key , 0 , 64 ) ;
	sprintf( ( char* )key , _Q2C( key_str ) ) ;
	aes.Cipher( ( void* )key ) ;
	ByteToHexStr( key , hexKey , 32 ) ;
	hexValue = NULL ;
	if ( SUCCESS != pini->get_string( "license" , ( char* )hexKey , hexValue ) )
	{
		QMessageBox::warning( this , tr( "告警提示" ) , tr("授权文件破损！" ) ) ;
		return ;
	}
	else
	{
		HexStrToByte( hexValue , value , strlen( hexValue ) ) ;
		aes.InvCipher( ( void* )value , strlen( hexValue ) )  ;
	}
	m_pDetailTable->setItem( 2 , 0 , new QTableWidgetItem( _C2Q( "MAC地址" ) ) ) ;
	m_pDetailTable->setItem( 2 , 1 , new QTableWidgetItem( _C2Q( ( char* )( value ) ) ) ) ;

	key_str = tr( "授权是否已使用" ) ;
	memset( key , 0 , 64 ) ;
	sprintf( ( char* )key , _Q2C( key_str ) ) ;
	aes.Cipher( ( void* )key ) ;
	ByteToHexStr( key , hexKey , 32 ) ;
	hexValue = NULL ;
	if ( SUCCESS != pini->get_string( "license" , ( char* )hexKey , hexValue ) )
	{
		QMessageBox::warning( this , tr( "告警提示" ) , tr("授权文件破损！" ) ) ;
		return ;
	}
	else
	{
		HexStrToByte( hexValue , value , strlen( hexValue ) ) ;
		aes.InvCipher( ( void* )value , strlen( hexValue ) )  ;
	}
	m_pDetailTable->setItem( 3 , 0 , new QTableWidgetItem( _C2Q( "授权是否已使用" ) ) ) ;
	m_pDetailTable->setItem( 3 , 1 , new QTableWidgetItem( _C2Q( ( char* )( value ) ) ) ) ;

	if( pini )
	{
		delete( pini ) ;
		pini = NULL ;
	}
}

void DLicense_gen::on_get_macaddr( void )
{
	QString strMac ;
	bool flag = false ;
	//获取所有网卡信息
	QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces() ;
	for ( int i = 0 ; i < ifaces.count() ; i++ )
	{
		QNetworkInterface iface = ifaces.at( i ) ;
		if ( iface.flags().testFlag(QNetworkInterface::IsUp) &&
			iface.flags().testFlag(QNetworkInterface::IsRunning)
			&& !iface.flags().testFlag(QNetworkInterface::IsLoopBack))
		{ 
			//获取当前有效网卡
			for (int j = 0; j < iface.addressEntries().count(); j++)
			{
				strMac = iface.hardwareAddress();
				flag = true ;
				break ;
			}

			if ( flag )
			{
				break ;
			}
		}
	}

	m_pMacLineEdit->setText(strMac);
}

void DLicense_gen::on_get_genpath( void )
{
	char filename[ 255 ] ;
	sprintf( filename , "%s/log/dat" , getenv("NBENV") ) ;
	QString str = QFileDialog::getExistingDirectory( this , tr( "选择生成文件路径" ) , _C2Q( filename ) ) ;
	m_pGenPathLineEdit->setText( str ) ;
}

void DLicense_gen::on_get_checkpath( void )
{
	char filename[ 255 ] ;
	sprintf( filename , "%s/log/dat" , getenv("NBENV") ) ;
	QString str = QFileDialog::getOpenFileName( this , tr( "打开要查看的文件" ) , _C2Q( filename ) , "license files(*.lic)" ) ;
	m_pCheckPathLineEdit->setText( str ) ;
}

void DLicense_gen::on_cancel_btn( void )
{
	close() ;
}
