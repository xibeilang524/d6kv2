/**
@file feskeyinfo.h
@brief 密钥信息对话框

@author chenkai 
@version 1.0.0
@date 2015-11-29
*/

#ifndef FESKEYINFO_H_
#define FESKEYINFO_H_

#include <QDialog>
#include <QtGui/QComboBox>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>

#include "fes/fesshm.h"
#include "fes/femailstruct.h"

class QFesKeyInfo : public QDialog
{
	Q_OBJECT

public:
	enum KeyOperType
	{
		keyCheck,//检查公钥
		keyRefresh,//更新公钥
		keyExport,//导出公钥
	};

	QFesKeyInfo( KeyOperType oper_type , ENCRYPT_KEYNO encry_key_info , QWidget *parent = 0 ) ;
	~QFesKeyInfo() ;

public:
	bool getKeyInfo( ENCRYPT_KEYNO &keyInfo ) ;

private slots:
	void key_no_changed( int index ) ;
	void key_no_in_card_changed( int index ) ;

private:
	QHBoxLayout m_key_info_layout ;
	QHBoxLayout m_btn_layout ;
	QVBoxLayout m_main_layout ;

	QLabel m_key_no_label ;
	QLabel m_key_no_in_card_label ;
	QComboBox m_key_no_combo ;
	QComboBox m_key_no_in_card_combo ;
	QPushButton m_ok_btn ;
	QPushButton m_cancel_btn ;

	KeyOperType m_key_oper_type ;

	ENCRYPT_KEYNO m_encry_key_info ;
};

#endif // FESKEYINFO_H_