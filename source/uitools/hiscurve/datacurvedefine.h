#ifndef DATACURVEDEFINE_H
#define DATACURVEDEFINE_H


#include <QMutex>
#include <QDateTime>
#include <QVector>
#include <QPair>
#include <QColor>
#include <QString>

//#include <qwt_symbol.h>
//#include <qwt_plot_curve.h>

#include "appdef.h"

class selectorpara;
QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

enum  CurveDataKind
{
	stat_value,
	curve_value
};

enum  DataState
{
	original_data	=	0x01,
	empty_data		=	0x02,
	modified_data	=   0x04,
	plantime_data   =   0x08,
	illegal_data    =   0x10
};

#define STAT_COLOR      QColor(Qt::lightGray)
#define ORIGINAL_COLOR	QColor(Qt::white)
#define EMPTY_COLOR 	QColor(Qt::yellow)
#define MODIFIED_COLOR	QColor(Qt::red)

 

// typedef QPair<DataState,double>			mCurveDataPair;
// typedef QVector<mCurveDataPair>			mOneCurveData;
// typedef QPair<QString,mOneCurveData>     mOneCurveDataPair;
// typedef QVector<mOneCurveDataPair>       mCurveData;

typedef struct  
{
	QString  desc;         //统计描述
	QString  val;          //统计值文本

}mStatData;

typedef struct
{
	DataState state;            //model data point   state 
	double    val;              //model data current value,original value in struct hisDataValue
}mData;

typedef QVector<QDateTime>				mTimeSequence; 
typedef QVector<mData>					mDataSequence;
typedef QVector<mStatData>              mStatSequence;

typedef struct 
{
	QString					keyName;        //code of key name mapping database key
	mTimeSequence           mTimeSeq;
	mDataSequence           mDataSeq;
	mStatSequence           mStatSeq;       //static value seq,only effect GeneralDate style 
public:						//common operation
	QDateTime               keyDate() const {return mTimeSeq[0];}
}mOneCurveData;

typedef QVector<mOneCurveData>          mCurveData;   //count indicate curve count,elem order mapping DbCurveData order





typedef QVector<hisDataValue>			DbOneCurveData;
typedef QPair<QString,DbOneCurveData>	DbOneCurveDataPair;
typedef QVector<DbOneCurveDataPair>		DbCurveData;


class DataCurveDefine  
{
public:
	DataCurveDefine();
	~DataCurveDefine();
public:
	bool readHisData(selectorpara& selPara,int readParaIdx,DbCurveData &hisCurveData);
	bool readDHisData(QString paraTabPrefix,QString paraName,int minCycle,int dataflag,
		QDateTime beginDate,QDateTime endDate,DbOneCurveData& oneCurveData);
	bool readMHisData(QString paraTabPrefix,QString paraName,int dataflag,
		QDate beginDate,QDate endDate,DbOneCurveData& oneCurveData);
	bool readYHisData(QString paraTabPrefix,QString paraName,int dataflag,
		QDate beginDate,QDate endDate,DbOneCurveData& oneCurveData);
	bool readDStatData(QString name,QDate date,int flag,hisDataValue& hisData);
	bool readMStatData(QString name,QDate date,int flag,hisDataValue& hisData);
	bool readYStatData();
	bool staticsProc(QDate date ,QString pointCode,int staticType);
	bool convertDateTimeToDaysAndMinutes(const QDateTime& datetime,bool isbegin,int mincycle,int& SDATE,int& TIME) const;
	int convertDateToDays(const QDate& date) const;
	int ConvertDateToMons(const QDate& date) const;
	QDate convertDaysToDate(int days) const;
	
	bool insertHisDataValue(selectorpara selpara,hisDataValue& hisData);
	bool updateHisDataValue(selectorpara selpara,hisDataValue& hisData);
	
private:
	QString getHisTabPrefix(CurveDataSrc dataSrc,CurveCycleType cycle,int datalflag=HISDATA_REAL);
	QString getHisTabName(CurveDataSrc dataSrc,CurveCycleType cycle,QDate date);
};

#endif
