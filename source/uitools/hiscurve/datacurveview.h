#ifndef DATACURVEVIEW_H
#define DATACURVEVIEW_H

#include <QAbstractItemView>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>

#include "datacurveapp.h"

class QLineEdit;

class DataCurvePlot;
class QwtPlotCurve;
class QwtPlotMarker;
class selectorpara;
class selectorcurvepara;
class DateTimeScaleDraw;
class DataCurve;
class QwtPlotPicker;

struct curveItemDef
{
public:
	int					dataType();
	QString             describe();
	QString				dataUnit();
	QString             xAxisUnit();
	QPen linePen();
	QwtSymbol::Style	symbolStyle();
	QBrush				symbolBrush();
	QPen				symbolPen();
	QSize symbolSize();
	DataCurve			*curveItem;
	void				showNullMarker();
	void                hideNullMark();
private:
	QPen                _linePen;
	QwtSymbol::Style	_symbolStyle;
	QBrush              _symbolBursh;
	QPen                _symbolPen;
	QString             _nameOfKey;
	int					_dataType;
	QString             _dataUnit;
	QVector<QwtPlotMarker*> _nullValMarkers;
public:
	curveItemDef();
	curveItemDef(QString nameOfKey);
	~curveItemDef();
};


static const QColor curveClrTab[]=
{
		QColor(0xFF,0x00,0x00),
		QColor(0x00,0xFF,0x00),
		QColor(0x00,0x00,0xFF),
		QColor(0xFF,0xFF,0x00),
		QColor(0xFF,0x00,0xFF),
		QColor(0x80,0x80,0x80),
		QColor(0x80,0x80,0x00),
		QColor(0x80,0x00,0x80),
		QColor(0x80,0x00,0x00)
};

class DataCurveView : public QAbstractItemView  
{
	Q_OBJECT
public:
	DataCurveView(QWidget * parent = 0);
	~DataCurveView();
	
	virtual QModelIndex indexAt(const QPoint &point) const {return QModelIndex();}

	virtual void scrollTo ( const QModelIndex & index, ScrollHint hint = EnsureVisible ) {;}
	virtual QRect visualRect ( const QModelIndex & index ) const  {return QRect();}

protected:
	virtual int horizontalOffset () const {return 0;}
	virtual bool isIndexHidden ( const QModelIndex & index ) const {return true;}
	virtual QModelIndex moveCursor ( CursorAction cursorAction, Qt::KeyboardModifiers modifiers ) {return QModelIndex();}
	virtual void setSelection ( const QRect & rect, QItemSelectionModel::SelectionFlags flags ){;}
	virtual int verticalOffset () const  {return 0;}
	virtual QRegion visualRegionForSelection ( const QItemSelection & selection ) const {return QRegion();}
	//virtual void setModel ( QAbstractItemModel * model );
protected slots:
	virtual void dataChanged ( const QModelIndex & topLeft, const QModelIndex & bottomRight );
	virtual void currentChanged ( const QModelIndex & current, const QModelIndex & previous );
public slots:
	virtual void selectAll ();
	virtual void reset ();
	virtual void setRootIndex ( const QModelIndex & index ){;}
//////////////////////////////////////////////////////////////////////////
////my owner
public slots:
	void columnsInserted (const QModelIndex & parent, int start, int end );
	void columnsRemoved ( const QModelIndex & parent, int start, int end );
//////////////////////////////////////////////////////////////////////////
	void selectorChanged(selectorcurvepara& selpara);   //ui curve selector changed slot
	void mouseMoved(const QPoint &);

public:
	void print(QPrinter *printer);
	//DataCurvePlot *getPlot() const {return plot;}
private:
	void resetPlot();
	void updatePlot(int row,int col);
	void updateXAxis(selectorpara& selpara,QVector<uint>& timeSeq);
	void insertCurve(int column);
	void deleteCurve(int column);
	QString getCurveTitle(selectorpara& selpara) const;
	void showStatText(QString text);
	void showInfo(QString strInfo);
	bool findCurveVal(const DataCurve *pCurvData,double x,double &yVal);//取曲线对应时刻的y值
private:
	selectorcurvepara        currentCurveSelector;
	selectorpara             &currentSelector;
	DataCurvePlot			 *plot;
	DateTimeScaleDraw        *scaleDraw;
	QVector<curveItemDef*>   plotCurves;
	QVector<uint>            currentTimeSeq;
	QLineEdit                *statText;
	QwtPlotPicker			*m_qwtPicker;
	
};

#endif
