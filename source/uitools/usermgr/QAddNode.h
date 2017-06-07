#ifndef ADDNODE_H
#define ADDNODE_H

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <qgridlayout.h>


class QAddNodeDlg :public QDialog
{
	Q_OBJECT
public:
	QAddNodeDlg(QWidget* parent = 0);
private:
	QLabel* m_Label;
    QComboBox* m_NodeName;

	QGridLayout* pLayout;

	QPushButton* m_btnOK;
    QPushButton* m_btnCancel;
public:
	void OnSetNodename(QStringList& nodename);
	void OnGetNodename(QString &szname);
};

#endif