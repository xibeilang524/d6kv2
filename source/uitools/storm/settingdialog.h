#ifndef SETTINGDIALOG_H_
#define SETTINGDIALOG_H_

#include <QtGui>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>

class QSetDialog: public QDialog
{
	Q_OBJECT
public:
	QSetDialog(QWidget* parent = NULL);

private slots:
	void onOK(void);
	void onCancel(void);
private:
	QLabel*	 m_pClientNumLabel;
	QLabel*	 m_pClientYxLabel;
	QLabel*	 m_pClientYcLabel;
	QLabel*	 m_pClientChgYXLabel;
	QLabel*	 m_pClientChgYCLabel;
	QLabel*	 m_pSendGapLabel;
	QLabel*	 m_pStartPortLabel;
	QLabel*	 m_pSendCountLabel;

	QLineEdit*	 m_pClientNum;
	QLineEdit*	 m_pClientYxNum;
	QLineEdit*	 m_pClientYcNum;
	QLineEdit*	 m_pClientChgYxNum;
	QLineEdit*	 m_pClientChgYcNum;
	QLineEdit*	 m_pSendGap;
	QLineEdit*	 m_pStartPort;
	QLineEdit*	 m_pSendCount;

	QPushButton*  m_pOK;
	QPushButton*  m_pCancel;  

	QGridLayout*  m_pGridlayout;
};
#endif