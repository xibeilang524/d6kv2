#ifndef QT_SCOLL_VIEW_H_
#define QT_SCOLL_VIEW_H_


#include <QAbstractScrollArea>
#include <QToolBar>
#include <QAction>
#include <QDateTime>
#include <QPainter>


#include "CNetModula.h"

class CNetLogFile;

class QtScrollView : public QAbstractScrollArea
{
public:
    QtScrollView(QWidget *parent=0);
	~QtScrollView();
	void DrawModulaData(CNetModulaData* pModulaData, QPainter* pPainter);
	void UpdateScroll();
	int CalcTotalLine(); 
	void setUpDown(int upDown) {m_upDown = upDown;};
	//void AutoSaveModulaData(CNetModulaData* pModulaData, QTextStream &output,QString strtime);


public:
	int m_nCurrentLine;             //当前行数
	int m_nTotalLine;               //当前页面总行数
	int m_nChar;                    //m_nChar = 29  一行显示的报文数
	int scrollview_width;           //scrollview部件像素宽度
	int checkbox_state;   //复选框状态，1解析报文，0不解析
	int m_nDescChar;      //未知变量
	CNetModula* m_pCurrentModula;
	QList<unsigned char> m_keyWordList;



private:
	bool matchkeyWord(unsigned char msg);

private:
	int m_nCharWidth;   //字符宽度，m_nCharWidth 为5，字符宽度
	int m_nLineheight;  ////字符高度
	int m_upDown;//0:上下行 1:下行 2:上行

protected:
	 void resizeEvent(QResizeEvent *re);
     void paintEvent(QPaintEvent *pe);  
     void scrollContentsBy(int dx, int dy);

};


#endif
