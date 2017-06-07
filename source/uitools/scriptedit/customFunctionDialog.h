#ifndef CUSTOMFUNCTIONDIALOG_H
#define CUSTOMFUNCTIONDIALOG_H

#include <QDialog>
class QAbstractButton;
class QGridLayout;
class QLabel;
class QLineEdit;
class QTextEdit;
class QDialogButtonBox;

class CustomFunctionDialog : public QDialog  
{
	Q_OBJECT
public:
	CustomFunctionDialog(QWidget * parent = 0, Qt::WindowFlags f = 0 );
	virtual ~CustomFunctionDialog();
	//void setEditContext(const QString& funcDesc,const QString& funcDef,const QString& funcHelp);
private slots:
 	void buttonClicked(QAbstractButton* button);
private:
	void init();
public:
	QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *funcNameEdit;
    QLabel *label_2;
    QTextEdit *funcDefEdit;
    QLabel *label_3;
    QTextEdit *funcHelpEdit;
    QDialogButtonBox *buttonBox;
};

#endif
