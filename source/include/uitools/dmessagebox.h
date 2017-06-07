#ifndef DMESSAGEBOX_H
#define DMESSAGEBOX_H

#include <QDialog>
#include <QFrame>
#include <QLabel>
#include <QPoint>
#include <QPushButton>
#include <QMessageBox>
#include <QDialogButtonBox>

#ifdef WIN32
#ifdef UI_DMESSAGEBOX_DLL
#define	UI_DMESSAGEBOX_EXPORT __declspec(dllexport) 
#else
#define	UI_DMESSAGEBOX_EXPORT __declspec(dllimport) 
#endif
#else
#define	UI_DMESSAGEBOX_EXPORT
#endif

class UI_DMESSAGEBOX_EXPORT DMessageBox : public QDialog
{
	Q_OBJECT
public:
	DMessageBox( QWidget* parent = 0 , const QString &title = tr( "Tip" ) , const QString &text = "",
		QMessageBox::StandardButtons buttons = QMessageBox::Ok , QMessageBox::StandardButton defaultButton = QMessageBox::NoButton ) ;  

	static QMessageBox::StandardButton information( QWidget* parent , const QString &title , const QString &text , 
		QMessageBox::StandardButtons buttons = QMessageBox::Ok , QMessageBox::StandardButton defaultButton = QMessageBox::NoButton ) ;  
	static QMessageBox::StandardButton question( QWidget* parent , const QString &title , const QString &text , 
		QMessageBox::StandardButtons buttons = QMessageBox::Ok , QMessageBox::StandardButton defaultButton = QMessageBox::NoButton ) ;  
	static QMessageBox::StandardButton warning( QWidget* parent , const QString &title , const QString &text , 
		QMessageBox::StandardButtons buttons = QMessageBox::Ok , QMessageBox::StandardButton defaultButton = QMessageBox::NoButton ) ;  
	static QMessageBox::StandardButton critical( QWidget* parent , const QString &title , const QString &text , 
		QMessageBox::StandardButtons buttons = QMessageBox::Ok , QMessageBox::StandardButton defaultButton = QMessageBox::NoButton ) ;  
	static QMessageBox::StandardButton about( QWidget* parent , const QString &title , const QString &text , const QString &pixmap ="",
		QMessageBox::StandardButtons buttons = QMessageBox::Ok , QMessageBox::StandardButton defaultButton = QMessageBox::NoButton ) ;  

	void set_message_title( QString title ) ;
	void set_message_text( QString text ) ;
	void set_message_pix( QString pixmap , bool flag = false ) ;
	QAbstractButton *clicked_button( void ) const ;
	QMessageBox::StandardButton standard_button( QAbstractButton* pButton ) const ;

// protected:
// 	void mousePressEvent( QMouseEvent* event ) ;
// 	void mouseReleaseEvent( QMouseEvent* event ) ;
// 	void mouseMoveEvent( QMouseEvent* event ) ;

private:
// 	void init_title_frame( void ) ;
	void init_center_frame( void ) ;
	void init_message_frame( void ) ;
	void init_button_frame( QMessageBox::StandardButtons buttons ,  QMessageBox::StandardButton defaultButton ) ;
	void set_default_button( QPushButton* pButton ) ;
	void set_default_button( QMessageBox::StandardButton button ) ;
	int exec_return_code( QAbstractButton* pButton ) ;

private slots:
// 	void slot_close_button( void ) ;
	void slot_button_clicked( QAbstractButton* pButton ) ;

private:
//	QPoint		m_movePoint; //移动的距离
// 	bool				m_mousePress; //按下鼠标左键
// 	QFrame*	m_pTitle_frame ;
	QFrame*	m_pMessage_frame ;
	QFrame*	m_pButton_frame ;
	QFrame*	m_pCenter_frame ;
// 	QLabel*		m_pTitle_text_label ;
// 	QLabel*		m_pTitle_pix_label ;
	QLabel*		m_pPix_label ;
	QLabel*		m_pText_label ;
	QPushButton*	m_pClose_button ;
	QAbstractButton *m_pClicked_button;
	QAbstractButton *m_pDefault_button;
	QDialogButtonBox*	m_pButton_box ;
};

#endif