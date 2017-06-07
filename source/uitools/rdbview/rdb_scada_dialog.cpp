#include "rdb_scada_dialog.h"
#include "data_info.h"

#include "db/dbapi.h"
#include "uitools/dmessagebox.h"
#include "rwxml.h"

#include <QMessageBox>

extern DBApi db;
extern DataInfo Info; 

Rdb_scada_dialog::Rdb_scada_dialog( QWidget* parent /*= 0 */ ) : QDialog( parent )
{
	setupUi( this ) ;
	for ( int i = 0 ; i < Info.RtdbTableNum ; i++ )
	{
		m_pRdbtable_combobox->insertItem( i , tr( Info.Rtdb[ i ].tabledescribe ) ) ;
	}		

	m_pCol_list->setSortingEnabled( false );

	m_pField_combobox->insertItem( 0 , tr( "" ) ) ;
	for ( int i = 0 ; i < Info.Rtdb[ 0 ].colnum ; i++ )
	{
		m_pField_combobox->insertItem( i + 1 , tr( Info.Rtdb[ 0 ].col[ i ].colname ) ) ;
	}
	if ( Info.Rtdb[ 0 ].orderfield == NULL )
	{
		m_pField_combobox->setCurrentIndex( 0 ) ;
	}
	else
	{
		m_pField_combobox->setItemText( 0 , tr( Info.Rtdb[ 0 ].orderfield ) ) ;
	}

	for( int i = 0 ; i < Info.Rtdb[ 0 ].colnum ; i++ )
	{	
		QString desc_str = tr( Info.Rtdb[ 0 ].col[ i ].coldescribe ) ;		
		QListWidgetItem* pItem  =  new QListWidgetItem( desc_str , m_pCol_list ) ;
		pItem->setCheckState(Qt::Unchecked);

		if ( Info.Rtdb[ 0 ].col[ i ].flag )
		{
			pItem->setCheckState(Qt::Checked);
		}
	}

	connect( m_pCancel_button , SIGNAL( clicked() ) , this, SLOT( slot_exit() ) );
	connect( m_pSave_button , SIGNAL( clicked() ) , this , SLOT( slot_save_config() ) );
	connect( m_pRdbtable_combobox , SIGNAL( activated(int) ) , this , SLOT( slot_table_select_change(int) ) );
	connect( m_pSel_all_button , SIGNAL( clicked() ) , this , SLOT( slot_select_all() ) );
	connect( m_pSel_none_button , SIGNAL( clicked() ) , this , SLOT( slot_select_none() ) );
}

void Rdb_scada_dialog::slot_exit( void )
{
	reject() ;
}

void Rdb_scada_dialog::slot_select_all( void )
{
	int sel_index = m_pRdbtable_combobox->currentIndex() ;
	if ( ( sel_index < 0 ) || ( sel_index >= Info.RtdbTableNum ) )
	{
		return ;
	}
	for( int i = 0 ; i < m_pCol_list->count() ; i ++ )
	{
		QListWidgetItem* pItem  =  m_pCol_list->item( i );
		pItem->setCheckState( Qt::Checked );
	}
}

void Rdb_scada_dialog::slot_select_none( void )
{
	int sel_index = m_pRdbtable_combobox->currentIndex() ;
	if ( ( sel_index < 0 ) || ( sel_index >= Info.RtdbTableNum ) )
	{
		return ;
	}
	for( int i = 0 ; i < m_pCol_list->count() ; i ++ )
	{
		QListWidgetItem* pItem  =  m_pCol_list->item( i );
		pItem->setCheckState( Qt::Unchecked );
	}
}

void Rdb_scada_dialog::slot_save_config( void )
{
	int sel_index = m_pRdbtable_combobox->currentIndex() ;
	if ( ( sel_index < 0 ) || ( sel_index >= Info.RtdbTableNum ) )
	{
		return ;
	}

	char file_name[ 256 ];
// 	sprintf( file_name , "%s/ini/rdbview.ini" , getenv("NBENV") ) ;
// 	ini_RW* pIni = new ini_RW( file_name );
	sprintf( file_name , "%s/resource/xml/rdbview.xml" , getenv("NBENV") ) ;
	Xml_RW* pIni = new Xml_RW( file_name );

	for( int i = 0 ; i < m_pCol_list->count() ; i ++ )
	{
		QListWidgetItem* pItem  =  m_pCol_list->item( i );
		Info.Rtdb[ sel_index ].col[ i ].flag  =  pItem->checkState();
		if( SUCCESS  !=  pIni->set_int( Info.Rtdb[sel_index].tabledescribe, Info.Rtdb[sel_index].col[i].coldescribe,  pItem->checkState() /* ==  Qt::Checked*/ ))
		{
			DMessageBox::information( this , tr( "警告" ) , tr( "保存失败！" ) ) ;
			return ;
		}
	}
	//----排序域名处理---start
	strcpy( Info.Rtdb[ sel_index ].orderfield , _Q2C( m_pField_combobox->currentText() ) ) ;

	if( SUCCESS != pIni->set_string( "排序域名设置",  Info.Rtdb[ sel_index ].tabledescribe , Info.Rtdb[ sel_index ].orderfield ) )
	{
		DMessageBox::information( this , tr( "警告" ) , tr( "保存失败！" ) ) ;
		return ;
	}
	//----排序域名处理---end

	if( pIni )
	{
		delete pIni ;
		pIni = NULL;
	}
	DMessageBox::information( this , tr( "提示" ) , tr( "保存成功！" ) ) ;
}

void Rdb_scada_dialog::slot_table_select_change( int nid )
{
	int sel_index = m_pRdbtable_combobox->currentIndex() ;
	if ( ( sel_index < 0 ) || ( sel_index >= Info.RtdbTableNum ) )
	{
		return ;
	}

	m_pCol_list->clear() ;
	m_pCol_list->setSortingEnabled( false ) ;

	m_pField_combobox->clear() ;
	m_pField_combobox->insertItem( 0 , tr( "" ) ) ;
	for ( int i = 0 ; i < Info.Rtdb[ sel_index ].colnum ; i++ )
	{
		m_pField_combobox->insertItem( i + 1 , tr( Info.Rtdb[ sel_index ].col[ i ].colname ) ) ;
	}
	if ( Info.Rtdb[ sel_index ].orderfield == NULL )
	{
		m_pField_combobox->setCurrentIndex( 0 ) ;
	}
	else
	{
		m_pField_combobox->setItemText( 0 , tr( Info.Rtdb[ sel_index ].orderfield ) ) ;
	}

	for( int i = 0 ; i < Info.Rtdb[ sel_index ].colnum ; i++ )
	{
		QString desc_str = tr( Info.Rtdb[ sel_index ].col[ i ].coldescribe ) ;
		QListWidgetItem* pItem  =  new QListWidgetItem( desc_str , m_pCol_list ) ;
		pItem->setCheckState(Qt::Unchecked) ;
		if ( Info.Rtdb[ sel_index ].col[ i ].flag )
		{
			pItem->setCheckState( Qt::Checked ) ;
		}
	}
}

