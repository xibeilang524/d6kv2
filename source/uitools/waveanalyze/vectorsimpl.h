#ifndef VECTORSIMPL_H
#define VECTORSIMPL_H

#include <QMenu>

#include "labelgraph.h"
#include "vectorgraph.h"
#include "harmgraph.h"
#include "analyzeutils.h"

#include "ui_vectors.h"


class QDlgVectorImpl : public QWidget,public Ui::QDlgVector
{
    Q_OBJECT

public:
    QDlgVectorImpl( QWidget* parent = 0, Qt::WindowFlags f= 0);
    ~QDlgVectorImpl();

	void ClearVector();
	void AddVector(const QString& str, const QColor& clr);
	void FreshVector(int idx, Complex c, const QString& str);
	void ShowPage(int page);
	void Fresh(int pos);
	int ActivePage();
	void SetAnalyze(QAnalyze* analyze);
	QAnalyze* Analyze()			{ return m_Analyze; }

protected:
	void Init();
	
	void VectorColorSet(int idx);
	void VectorScaleSet(int idx);
	void VectorAngleSet(int idx);
	void VectorAdd();
	void VectorNameSet();

	void HarmColorSet(int idx);
	void HarmStyleSet(int idx);
	void HarmValueSet(int idx);
	void HarmMore();
	void HarmAdd();

	void UpdateVector();
	void FreshVector1(int pos);
	void UpdateHarm();
	void FreshHarm(int pos);
	void SetBaseVector(int index);
	void AddHarmItems(QVectorMgr* Items, bool label);
	int GetHarmIndex(int idx);
	
protected slots:
	void VectorMenu();
	void HarmMenu();

private:
	QVectorGraph*	m_Vector;
	QLabelGraph*	m_VectorLabel;
	QHarmGraph*		m_Harm;
	QLabelGraph*	m_HarmLabel;

	QAnalyze*	m_Analyze;
	QColor	m_clrDirect;	// 直流颜色
	QColor	m_clrOdd;		// 奇次颜色
	QColor	m_clrEven;		// 偶次颜色
	int		m_nHarmValue;	// 数值显示方式

};

#endif