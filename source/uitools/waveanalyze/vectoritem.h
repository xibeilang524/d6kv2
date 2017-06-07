#ifndef VECTORITEM_H
#define VECTORITEM_H

#include <QColor>
#include <QString>
#include <QList>

class QLabelGraph;
class QVectorGraph;
class QHarmGraph;


// 矢量、谐波数据类
class QVectorItem
{
public:
	QVectorItem()
	{
		m_fRe = 0;
		m_fIm = 0;
		m_nTag = 0;
		m_Color = Qt::red;
		m_strCaption = "Item";
		m_bShow = true;
	}
	QVectorItem(const QVectorItem& src)
	{
		m_fRe = 0;
		m_fIm = 0;
		m_nTag = 0;
		m_Color = Qt::red;
		m_strCaption = "Item";
		m_bShow = true;
	}
	
	void SetRe(double f)	{ m_fRe = f; }
	void SetIm(double f)	{ m_fIm = f; }
	void SetCaption(const QString& str) { m_strCaption = str; }
	double Re()				{ return m_fRe; }
	double Im()				{ return m_fIm; }
	QColor Color()			{ return m_Color; }
	int Tag()				{ return m_nTag; }
	QString Caption()		{ return m_strCaption; }
	bool IsShow()			{ return m_bShow; }

private:
	double	m_fRe;			// 实部
	double	m_fIm;			// 虚部
	int		m_nTag;			//
	QColor	m_Color;		// 颜色
	QString m_strCaption;	// 名称
	bool	m_bShow;		// 显示

	friend class QLabelGraph;
	friend class QVectorGraph;
	friend class QHarmGraph;
	friend class QVectorMgr;

};


// 矢量子项管理类

class QVectorMgr
{
public:
	QVectorMgr();
	~QVectorMgr();

	int Count()					{ return m_lstVector.count(); }
	void Clear()				{ m_lstVector.clear(); }
	void Delete(int index);
	QVectorItem* Item(int index);
	QVectorItem* Add(QColor clr, QString str);
	void Connect(QObject* receiver, const char* member);
	
protected:
	void Change()				{ /*emit m_pSig(); */}

private:
	 QList<QVectorItem*>  m_lstVector;
signals:
// 	 void m_pSig();
	
};


#endif