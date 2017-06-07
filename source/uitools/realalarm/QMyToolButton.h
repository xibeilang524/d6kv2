#ifndef QMYTOOLBUTTON_H
#define QMYTOOLBUTTON_H

#include <QPushButton>

class QMyToolButton : public QPushButton
{
	Q_OBJECT
public:
	QMyToolButton( QString buttonName , QString buttonDesc , QString buttonImg = 0 , QWidget* parent = 0 ) ;

protected:
	void enterEvent ( QEvent * event ) ;
	void leaveEvent( QEvent * event ) ;
	void mouseReleaseEvent( QMouseEvent *event ) ;

private:
	void iniMyToolButton( void ) ;

private:
	QPushButton* m_pImgBtn ;
	QString  m_buttonName ; 
	QString  m_buttonDesc ;
	QString  m_buttonImgHover ;
	QString  m_buttonImgPress ;
	QString  m_buttonImgNormal ;
	QString  m_pixLabelName ;
};

#endif