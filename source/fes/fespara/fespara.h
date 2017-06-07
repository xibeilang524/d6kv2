#ifndef FESPARA_H_
#define FESPARA_H_

#include <QtGui/QDialog>
#include <QtGui/QTableWidget>
#include <QtGui/QTableView>
#include <QtCore/QTimer>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtGui/QIcon>
#include "fes/fesshm.h"

const int REFLESHTIME = 2000;

class QFesPara: public QDialog
{
	Q_OBJECT
public:
	QFesPara( QWidget* parent = NULL );
	~QFesPara(  );

private slots:
	void onTimer( void );
	void onChanClicked( void );
	void onRtuClicked( void );
	void onExit( void );

private:
	bool readProtocol( void );
	void initDevType( void );
	void setChanPara( void );
	void setRtuPara( void );
	void setPara( void );
	void refleshChanPara( void );
	void refleshRtuPara( void );
	int  calcRealChanNum( void );	//计算通道代码不为空的通道总数
	int	 calcRealRtuNum( void );  //计算终端代码不为空的终端总数

private:
	QTableWidget*	m_pChanTable;
	QTableWidget*	m_pRtuTable;
	QTimer*				m_pTimer;
	QPushButton*	m_pRtuButton;
	QPushButton*	m_pChanButton;
	QPushButton*	m_pExitButton;
	QVBoxLayout*	m_pVLayout;
	QHBoxLayout*	m_pHLayout;
	QMap<QString, QString> m_protocol;
	QMap<int ,QString> m_devType;
	Fes_shm&			 m_fes_shm;
};

#endif