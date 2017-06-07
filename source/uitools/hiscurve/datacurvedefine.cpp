#include <QString>
#include <QStringList>
#include <QMutex>
#include <QMutexLocker>
#include <QLatin1String>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QVector>
#include <QVariant>
#include <QtAlgorithms>
#include <QMapIterator>
#include <QDebug>


#include "appdef.h"
#include "datacurveapp.h"
#include "datacurvedefine.h"






//////////////////////////////////////////////////////////////////////////
////
////
//////////////////////////////////////////////////////////////////////////


DataCurveDefine::DataCurveDefine()
{
}

DataCurveDefine::~DataCurveDefine()
{

}


int DataCurveDefine::convertDateToDays( const QDate& date ) const
{
	DataCurveApp *appSettings=static_cast<DataCurveApp*>(qApp);
	QDateTime dttmp(date,QTime(0,0,0,0),appSettings->timeSpecValue);
	return dttmp.toTime_t()/(24*60*60);
}

int DataCurveDefine::ConvertDateToMons(const QDate& date ) const
{
	return (date.year()*12+date.month())-(1970*12+1);

}

QDate DataCurveDefine::convertDaysToDate( int days ) const
{
	DataCurveApp *appSettings=static_cast<DataCurveApp*>(qApp);
	uint sec=days*24*60*60;  //设置为0点。
	QDateTime dttmp;
	dttmp.setTimeSpec(appSettings->timeSpecValue);
	dttmp.setTime_t(sec);
	return dttmp.date();
}


bool DataCurveDefine::convertDateTimeToDaysAndMinutes( const QDateTime& datetime,bool isbegin,int mincycle,int& SDATE,int& TIME ) const
{
	DataCurveApp *appSettings=static_cast<DataCurveApp*>(qApp);
	QDateTime dttmp(datetime.date(),QTime(0,0,0,0),appSettings->timeSpecValue);
	uint sec=dttmp.toTime_t();
	uint minutes;
	SDATE=sec/(24*60*60);

	if(mincycle<=0) mincycle=appSettings->defaultMinCycle;
	if(datetime.time().hour()==0 && datetime.time().minute()==0)
	{
		SDATE-=1;
		TIME=1440;
	}
	else if(isbegin)
	{
		minutes=datetime.time().hour()*24+((datetime.time().minute()/mincycle+1)*mincycle);
		if(minutes>1440)
		{
			TIME=1440;
		}
		else
			TIME=datetime.time().hour()*24+(datetime.time().minute()/mincycle*mincycle);
	}
	else
	{
		TIME=datetime.time().hour()*24+(datetime.time().minute()/mincycle*mincycle);
	}
	return true;
}

bool DataCurveDefine::readHisData(selectorpara& selPara,int readParaIdx,DbCurveData &hisCurveData )
{
	DataCurveApp *myApp=static_cast<DataCurveApp*>(qApp);
	bool         success=false;
	QString      tableprefix;
	QString      paraname;
	int          savetime=0;
	int          plantime=0;
	int          dataflag;
	DbOneCurveData		  oneCurveData;
	
	hisCurveData.clear();
	if(selPara.paras.size()==0)
		return false;
	
	if(selPara.selType==MutilSelector && readParaIdx < 0)
	{
		foreach(QString paraname,selPara.paras)
		{
			if(selPara.dataSrc==YcDataSrc)
			{
				YcParas::ConstIterator itFind=findItem<YcParas>(myApp->ycparas,paraname);
				if(itFind!=myApp->ycparas.constEnd())
				{
					tableprefix=getHisTabPrefix(selPara.dataSrc,selPara.cycle);
					paraname=(*itFind)->name;
					savetime=(*itFind)->savetime;
					dataflag=selPara.statType;
				}
			}
			else if(selPara.dataSrc==DdDataSrc)
			{
				KwhParas::ConstIterator itFind=findItem<KwhParas>(myApp->kwhparas,paraname);
				if(itFind!=myApp->kwhparas.constEnd())
				{
					tableprefix=getHisTabPrefix(selPara.dataSrc,selPara.cycle);;
					paraname=(*itFind)->name;
					savetime=(*itFind)->savetime;
					dataflag=selPara.statType;
				}
			}
			//读一条曲线数据
			if(selPara.cycle==DayCycle)
			{
				if(readDHisData(tableprefix,paraname,savetime,dataflag
					,QDateTime(selPara.beginDate,selPara.beginTime,((DataCurveApp*)qApp)->timeSpecValue)
					,QDateTime(selPara.endDate,selPara.endTime,((DataCurveApp*)qApp)->timeSpecValue)
					,oneCurveData))
				{
					hisCurveData.append(qMakePair(paraname,oneCurveData));
				}
			}
			else if(selPara.cycle==MonthCycle)
			{
				if(readMHisData(tableprefix,paraname,dataflag
					,selPara.beginDate
					,selPara.endDate
					,oneCurveData))
				{
					hisCurveData.append(qMakePair(paraname,oneCurveData));
				}
			}
			else if(selPara.cycle==YearCycle)
			{
				if(readYHisData(tableprefix,paraname,dataflag
					,selPara.beginDate
					,selPara.endDate
					,oneCurveData))
				{
					hisCurveData.append(qMakePair(paraname,oneCurveData));
				}
			}
			
		}// end foreach paraname
	}//end if mulsel all;
	else if(selPara.isCompType())
	{
		if(selPara.dataSrc==YcDataSrc)
		{
			YcParas::ConstIterator itFind=findItem<YcParas>(myApp->ycparas,selPara.paras[0]);
			if(itFind!=myApp->ycparas.constEnd())
			{
				tableprefix=getHisTabPrefix(selPara.dataSrc,selPara.cycle);
				paraname=(*itFind)->name;
				savetime=(*itFind)->savetime;
				dataflag=selPara.statType;
			}
		}
		else if(selPara.dataSrc==DdDataSrc)
		{
			KwhParas::ConstIterator itFind=findItem<KwhParas>(myApp->kwhparas,selPara.paras[0]);
			if(itFind!=myApp->kwhparas.constEnd())
			{
				tableprefix=getHisTabPrefix(selPara.dataSrc,selPara.cycle);;
				paraname=(*itFind)->name;
				savetime=(*itFind)->savetime;
				dataflag=selPara.statType;
			}
		}
		for(int i=0;i<selPara.compDateCount();i++)
		{
			//read comp curve data
			if(selPara.cycle==DayCycle)
			{
				if(readDHisData(tableprefix,paraname,savetime,dataflag
					,QDateTime(selPara.compBeginDate(i),selPara.compBeginTime(),((DataCurveApp*)qApp)->timeSpecValue)
					,QDateTime(selPara.compEndDate(i),selPara.compEndTime(),((DataCurveApp*)qApp)->timeSpecValue)
					,oneCurveData))
				{
					hisCurveData.append(qMakePair(paraname,oneCurveData));
				}
			}
			else if(selPara.cycle==MonthCycle)
			{
				if(readMHisData(tableprefix,paraname,dataflag
					,selPara.compBeginDate(i)
					,selPara.compEndDate(i)
					,oneCurveData))
				{
					hisCurveData.append(qMakePair(paraname,oneCurveData));
				}
			}
			else if(selPara.cycle==YearCycle)
			{
				if(readYHisData(tableprefix,paraname,dataflag
					,selPara.compBeginDate(i)
					,selPara.compEndDate(i)
					,oneCurveData))
				{
					hisCurveData.append(qMakePair(paraname,oneCurveData));
				}
			}
		}//end for each date
	}
	else 
	{
		if(selPara.dataSrc==YcDataSrc)
		{
			tableprefix=getHisTabPrefix(selPara.dataSrc,selPara.cycle);;
			dataflag=selPara.statType;
			if(selPara.selType==MutilSelector && readParaIdx>=0)
				paraname=selPara.paras[readParaIdx];
			else if(selPara.selType==SingleSelector)
				paraname=selPara.paras[0];
			YcParas::ConstIterator itFind=findItem<YcParas>(myApp->ycparas,paraname);
			if(itFind!=myApp->ycparas.constEnd())
			{
				savetime=(*itFind)->savetime;
				plantime=(*itFind)->plantime;
			}
		}
		else if(selPara.dataSrc==DdDataSrc)
		{
			tableprefix=getHisTabPrefix(selPara.dataSrc,selPara.cycle);;
			dataflag=selPara.statType;
			if(selPara.selType==MutilSelector && readParaIdx>=0)
				paraname=selPara.paras[readParaIdx];
			else if(selPara.selType==SingleSelector)
				paraname=selPara.paras[0];
			KwhParas::ConstIterator itFind=findItem<KwhParas>(myApp->kwhparas,paraname);
			if(itFind!=myApp->kwhparas.constEnd())
				savetime=(*itFind)->savetime;
		}
		//read one curve data
		if(selPara.cycle==DayCycle)
		{
			//读一条曲线数据
			if(readDHisData(tableprefix,paraname,savetime,dataflag
				,QDateTime(selPara.beginDate,selPara.beginTime,((DataCurveApp*)qApp)->timeSpecValue)
				,QDateTime(selPara.endDate,selPara.endTime,((DataCurveApp*)qApp)->timeSpecValue)
				,oneCurveData))
			{
				hisCurveData.append(qMakePair(paraname,oneCurveData));
			}
			//if has plantime then append plancurve data into one curve selector
			if(selPara.isPlanType())
			{
				if(readDHisData(tableprefix,paraname,plantime,HISDATA_PLAN
					,QDateTime(selPara.beginDate,selPara.beginTime,((DataCurveApp*)qApp)->timeSpecValue)
					,QDateTime(selPara.endDate,selPara.endTime,((DataCurveApp*)qApp)->timeSpecValue)
					,oneCurveData))
				{
					hisCurveData.append(qMakePair(paraname,oneCurveData));
				}
			}
		}
		else if(selPara.cycle==MonthCycle)
		{
			if(readMHisData(tableprefix,paraname,dataflag
				,selPara.beginDate
				,selPara.endDate
				,oneCurveData))
			{
				hisCurveData.append(qMakePair(paraname,oneCurveData));
			}
		}
		else if(selPara.cycle==YearCycle)
		{
			if(readYHisData(tableprefix,paraname,dataflag
				,selPara.beginDate
				,selPara.endDate
				,oneCurveData))
			{
				hisCurveData.append(qMakePair(paraname,oneCurveData));
			}
		}
	}// end if mulsel one end if selsel one

	if(hisCurveData.size()>0)
		return true;
	return false;
}


bool DataCurveDefine::readDHisData(QString paraTabPrefix,QString paraName,int minCycle,int dataflag, 
								   QDateTime beginDate,QDateTime endDate,DbOneCurveData& oneCurveData )
{
	DataCurveApp *myApp=static_cast<DataCurveApp*>(qApp);
	int     i,j,colCnt;
	QDate   bDate,eDate,tmpDate;
	int     bSDATE,eSDATE,bTIME,eTIME;
	QString sqlWhere,sqlTable,sqlWord;
	QVector<QVariant> tmpContent;

	if(minCycle<=0) minCycle=myApp->defaultMinCycle;
	oneCurveData.clear();

	bDate=beginDate.date();
	eDate=endDate.date();
	
	convertDateTimeToDaysAndMinutes(beginDate,true,minCycle,bSDATE,bTIME);
	convertDateTimeToDaysAndMinutes(endDate,false,minCycle,eSDATE,eTIME);
	
	if(dataflag==HISDATA_PLAN)  //计划值曲线
		sqlWhere=QString("name='%1' and sdate>=%2 and sdate<=%3 and flag=%4 and mod(time,%5)=0")
		.arg(paraName)
		.arg(bSDATE)
		.arg(eSDATE)
		.arg(dataflag)
		.arg(minCycle);
	else
		sqlWhere=QString("name='%1' and sdate>=%2 and sdate<=%3 and flag<%4 and mod(time,%5)=0")
			.arg(paraName)
			.arg(bSDATE)
			.arg(eSDATE)
			.arg(HISDATA_PLAN)
			.arg(minCycle);
	tmpDate=bDate;
	if(myApp->ycEverdayTable)  //每天一表
	{
		do 
		{
			tmpContent.clear();
			sqlTable=QString("%1%2%3%4")
				.arg(paraTabPrefix)
				.arg(tmpDate.year(),4,10,QLatin1Char('0'))
				.arg(tmpDate.month(),2,10,QLatin1Char('0'))
				.arg(tmpDate.day(),2,10,QLatin1Char('0'));
			sqlWord=QString("select name,sdate,time,flag,data,rawdata from %1 where %2 order by sdate,time")
				.arg(sqlTable).arg(sqlWhere);
			colCnt=myApp->dbex.sqlReadRecorderDir(DB_NAME,sqlWord,tmpContent);
			if(colCnt>0)
			{
				for(i=0;i<tmpContent.size();i+=colCnt)
				{
					j=i;
					hisDataValue tmpData;
					tmpData.name=tmpContent[j++].toString();
					tmpData.date=tmpContent[j++].toUInt();
					tmpData.time=tmpContent[j++].toUInt();
					tmpData.flag=tmpContent[j++].toUInt();
					tmpData.val.ycdata=tmpContent[j++].toDouble();
					tmpData.rawval.ycdata=tmpContent[j++].toUInt();
					if(tmpData.date==bSDATE && tmpData.time<bTIME)
						continue;
					else if(tmpData.date==eSDATE && tmpData.time>eTIME)
						continue;
					oneCurveData.append(tmpData);
				}
			}
			tmpDate=tmpDate.addDays(1);
		} while (eDate>tmpDate);
	}
	else
	{
		do 
		{
			tmpContent.clear();
			sqlTable=QString("%1%2%3")
				.arg(paraTabPrefix)
				.arg(tmpDate.year(),4,10,QLatin1Char('0'))
				.arg(tmpDate.month(),2,10,QLatin1Char('0'));
			sqlWord=QString("select name,sdate,time,flag,data,rawdata from %1 where %2 order by sdate,time")
				.arg(sqlTable).arg(sqlWhere);
			colCnt=myApp->dbex.sqlReadRecorderDir(DB_NAME,sqlWord,tmpContent);
			//qDebug()<<sqlWord;
			if(colCnt>0)
			{
				for(i=0;i<tmpContent.size();i+=colCnt)
				{
					j=i;
					hisDataValue tmpData;
					tmpData.name=tmpContent[j++].toString();
					tmpData.date=tmpContent[j++].toUInt();
					tmpData.time=tmpContent[j++].toUInt();
					tmpData.flag=tmpContent[j++].toUInt();
					tmpData.val.ycdata=tmpContent[j++].toDouble();
					tmpData.rawval.ycdata=tmpContent[j++].toUInt();
					if(tmpData.date==bSDATE && tmpData.time<bTIME)
						continue;
					else if(tmpData.date==eSDATE && tmpData.time>eTIME)
						continue;
					oneCurveData.append(tmpData);
				}
			}
			tmpDate=tmpDate.addMonths(1);
		} while (eDate>tmpDate);
	}
	if(oneCurveData.size()>0)
		return true;
	return false;
}


bool DataCurveDefine::readMHisData(QString paraTabPrefix,QString paraName,int dataflag, 
								   QDate beginDate,QDate endDate,DbOneCurveData& oneCurveData )
{
	DataCurveApp *myApp=static_cast<DataCurveApp*>(qApp);
	int     i,j,colCnt;
	QDate   bDate,eDate,tmpDate;
	int     bSDATE,eSDATE;
	QString sqlWhere,sqlTable,sqlWord;
	QVector<QVariant> tmpContent;
	
	oneCurveData.clear();
	
	bDate=beginDate;
	eDate=endDate;
	
	bSDATE=convertDateToDays(bDate);
	eSDATE=convertDateToDays(eDate);
	
	sqlWhere=QString("name='%1' and sdate>=%2 and sdate<=%3 and flag=%4")
		.arg(paraName)
		.arg(bSDATE)
		.arg(eSDATE)
		.arg(dataflag);

	tmpDate=bDate;
	do 
	{
		tmpContent.clear();
		sqlTable=QString("%1%2%3")
			.arg(paraTabPrefix)
			.arg(tmpDate.year(),4,10,QLatin1Char('0'))
			.arg(tmpDate.month(),2,10,QLatin1Char('0'));
		sqlWord=QString("select name,sdate,time,flag,data,rawdata,odate,otime,oflag from %1 where %2 order by sdate")
			.arg(sqlTable).arg(sqlWhere);
		colCnt=myApp->dbex.sqlReadRecorderDir(DB_NAME,sqlWord,tmpContent);
		if(colCnt>0)
		{
			for(i=0;i<tmpContent.size();i+=colCnt)
			{
				j=i;
				hisDataValue tmpData;
				tmpData.name=tmpContent[j++].toString();
				tmpData.date=tmpContent[j++].toUInt();
				tmpData.time=tmpContent[j++].toUInt();
				tmpData.flag=tmpContent[j++].toUInt();
				tmpData.val.ycdata=tmpContent[j++].toDouble();
				tmpData.rawval.ycdata=tmpContent[j++].toUInt();
				tmpData.odate=tmpContent[j++].toUInt();
				tmpData.otime=tmpContent[j++].toUInt();
				tmpData.oflag=tmpContent[j++].toUInt();
				oneCurveData.append(tmpData);
			}
		}
		tmpDate=tmpDate.addMonths(1);
	} while (eDate>tmpDate);
	if(oneCurveData.size()>0)
		return true;
	return false;
}

bool DataCurveDefine::readYHisData( QString paraTabPrefix,QString paraName,int dataflag, QDate beginDate,QDate endDate,DbOneCurveData& oneCurveData )
{
	DataCurveApp *myApp=static_cast<DataCurveApp*>(qApp);
	int     i,j,colCnt;
	QDate   bDate,eDate,tmpDate;
	int     bSDATE,eSDATE;
	QString sqlWhere,sqlTable,sqlWord;
	QVector<QVariant> tmpContent;
	
	oneCurveData.clear();
	
	bDate=beginDate;
	eDate=endDate;
	
	bSDATE=ConvertDateToMons(bDate);
	eSDATE=ConvertDateToMons(eDate);
	
	sqlWhere=QString("name='%1' and sdate>=%2 and sdate<=%3 and flag=%4")
		.arg(paraName)
		.arg(bSDATE)
		.arg(eSDATE)
		.arg(dataflag);
	
	tmpDate=bDate;
	do 
	{
		tmpContent.clear();
		sqlTable=QString("%1%2")
			.arg(paraTabPrefix)
			.arg(tmpDate.year(),4,10,QLatin1Char('0'));
		sqlWord=QString("select name,sdate,time,flag,data,rawdata,odate,otime,oflag from %1 where %2 order by sdate")
			.arg(sqlTable).arg(sqlWhere);
		colCnt=myApp->dbex.sqlReadRecorderDir(DB_NAME,sqlWord,tmpContent);
		if(colCnt>0)
		{
			for(i=0;i<tmpContent.size();i+=colCnt)
			{
				j=i;
				hisDataValue tmpData;
				tmpData.name=tmpContent[j++].toString();
				tmpData.date=tmpContent[j++].toUInt();
				tmpData.time=tmpContent[j++].toUInt();
				tmpData.flag=tmpContent[j++].toUInt();
				tmpData.val.ycdata=tmpContent[j++].toDouble();
				tmpData.rawval.ycdata=tmpContent[j++].toUInt();
				tmpData.odate=tmpContent[j++].toUInt();
				tmpData.otime=tmpContent[j++].toUInt();
				tmpData.oflag=tmpContent[j++].toUInt();
				oneCurveData.append(tmpData);
			}
		}
		tmpDate=tmpDate.addYears(1);
	} while (eDate>tmpDate);
	if(oneCurveData.size()>0)
		return true;
	return false;
}

bool DataCurveDefine::readDStatData( QString name,QDate date,int flag,hisDataValue& hisData )
{
	DBExtension &dbex=static_cast<DataCurveApp*>(qApp)->dbex;
	int     i,j,colCnt;
	QString sqlWhere,sqlTable,sqlWord;
	QVector<QVariant> tmpContent;
	
	int days=convertDateToDays(date);
	sqlWhere=QString("name='%1' and time=0 and sdate=%2 and flag=%3")
		.arg(name)
		.arg(days)
		.arg(flag);
	sqlTable=QString("%1%2")
		.arg(getHisTabPrefix(YcDataSrc,DayCycle,flag))
		.arg(date.toString("yyyyMM"));
	sqlWord=QString("select name,sdate,time,flag,data,rawdata,odate,otime,oflag from %1 where %2")
		.arg(sqlTable).arg(sqlWhere);
	colCnt=dbex.sqlReadRecorderDir(DB_NAME,sqlWord,tmpContent);
	if(colCnt>0)
	{
		for(i=0;i<tmpContent.size();i+=colCnt)
		{
			j=i;
			hisData.name=tmpContent[j++].toString();
			hisData.date=tmpContent[j++].toUInt();
			hisData.time=tmpContent[j++].toUInt();
			hisData.flag=tmpContent[j++].toUInt();
			hisData.val.ycdata=tmpContent[j++].toDouble();
			hisData.rawval.ycdata=tmpContent[j++].toUInt();
			hisData.odate=tmpContent[j++].toUInt();
			hisData.otime=tmpContent[j++].toUInt();
			hisData.oflag=tmpContent[j++].toUInt();
		}
	}
	return colCnt>0;	
}

bool DataCurveDefine::readMStatData( QString name,QDate date,int flag,hisDataValue& hisData )
{
	DBExtension &dbex=static_cast<DataCurveApp*>(qApp)->dbex;
	int     i,j,colCnt;
	QString sqlWhere,sqlTable,sqlWord;
	QVector<QVariant> tmpContent;
	
	int days=ConvertDateToMons(date);
	sqlWhere=QString("name='%1' and time=0 and sdate=%2 and flag=%3")
		.arg(name)
		.arg(days)
		.arg(flag);
	sqlTable=QString("%1%2")
		.arg(getHisTabPrefix(YcDataSrc,MonthCycle,flag))
		.arg(date.toString("yyyy"));
	sqlWord=QString("select name,sdate,time,flag,data,rawdata,odate,otime,oflag from %1 where %2")
		.arg(sqlTable).arg(sqlWhere);
	colCnt=dbex.sqlReadRecorderDir(DB_NAME,sqlWord,tmpContent);
	if(colCnt>0)
	{
		for(i=0;i<tmpContent.size();i+=colCnt)
		{
			j=i;
			hisData.name=tmpContent[j++].toString();
			hisData.date=tmpContent[j++].toUInt();
			hisData.time=tmpContent[j++].toUInt();
			hisData.flag=tmpContent[j++].toUInt();
			hisData.val.ycdata=tmpContent[j++].toDouble();
			hisData.rawval.ycdata=tmpContent[j++].toUInt();
			hisData.odate=tmpContent[j++].toUInt();
			hisData.otime=tmpContent[j++].toUInt();
			hisData.oflag=tmpContent[j++].toUInt();
		}
	}
	return colCnt>0;
}


QString DataCurveDefine::getHisTabPrefix( CurveDataSrc dataSrc,CurveCycleType cycle,int datalflag/*=HISDATA_REAL*/ )
{
	if(dataSrc==YcDataSrc)
	{
		if(cycle==DayCycle && datalflag<HISDATA_REALMAX)
			return YCCURVETABLEPREFIX;
		else if(cycle==DayCycle && datalflag>=HISDATA_REALMAX)
			return YCDAYTABLEPREFIX;
		else if(cycle==MonthCycle)
			return YCDAYTABLEPREFIX;
		else if(cycle==YearCycle)
			return YCMONTABLEPREFIX;
	}
	else if(dataSrc==DdDataSrc)
	{
		if(cycle==DayCycle && datalflag<HISDATA_REALMAX)
			return DDCURVETABLEPREFIX;
		else if(cycle==DayCycle && (datalflag>=HISDATA_DAYKWHSD1 || datalflag<= HISDATA_DAYKWHTTL))
			return DDDAYTABLEPREFIX;
		else if(cycle==MonthCycle)
			return DDDAYTABLEPREFIX;
		else if(cycle==YearCycle)
			return DDMONTABLEPREFIX;
	}
	return C2G("");
}

QString DataCurveDefine::getHisTabName( CurveDataSrc dataSrc,CurveCycleType cycle,QDate date )
{
	DataCurveApp *appSettings=static_cast<DataCurveApp*>(qApp);
	if(dataSrc==YcDataSrc)
	{
		if(cycle==DayCycle)
			if(appSettings->ycEverdayTable)
				return QString("%1%2").arg(YCCURVETABLEPREFIX).arg(date.toString("yyyyMMdd"));
			else 
				return QString("%1%2").arg(YCCURVETABLEPREFIX).arg(date.toString("yyyyMM"));
		else if(cycle==MonthCycle)
			return QString("%1%2").arg(YCDAYTABLEPREFIX).arg(date.toString("yyyyMM"));
		else if(cycle==YearCycle)
			return QString("%1%2").arg(YCMONTABLEPREFIX).arg(date.toString("yyyy"));
	}
	else if(dataSrc==DdDataSrc)
	{
		if(cycle==DayCycle)
			return QString("%1%2").arg(DDCURVETABLEPREFIX).arg(date.toString("yyyyMM"));
		else if(cycle==MonthCycle)
			return QString("%1%2").arg(DDDAYTABLEPREFIX).arg(date.toString("yyyyMM"));
		else if(cycle==YearCycle)
			return QString("%1%2").arg(DDMONTABLEPREFIX).arg(date.toString("yyyy"));
	}
	return C2G("");
}

bool DataCurveDefine::insertHisDataValue( selectorpara selpara,hisDataValue& hisData )
{
	DBExtension &dbex=static_cast<DataCurveApp*>(qApp)->dbex;
	QString sqlTable,sqlWord;
	sqlTable=getHisTabPrefix(selpara.dataSrc,selpara.cycle);
	QDate date=convertDaysToDate(hisData.date);
	sqlTable=getHisTabName(selpara.dataSrc,selpara.cycle,date);
	sqlWord=QString("insert into  %1 values('%2',%3,%4,%5,%6,%7)")
		.arg(sqlTable)
		.arg(hisData.name)
		.arg(hisData.date)
		.arg(hisData.time)
		.arg(hisData.flag)
		.arg(hisData.val.ycdata)
		.arg(hisData.rawval.ycdata);
	//qDebug()<<"insertHisDataValue SQLWord="<<sqlWord;
	return dbex.Execute(DB_NAME,sqlWord);
}

bool DataCurveDefine::updateHisDataValue(selectorpara selpara,hisDataValue& hisData )
{
	DBExtension &dbex=static_cast<DataCurveApp*>(qApp)->dbex;
	QString sqlWhere,sqlTable,sqlWord;
	sqlWhere=QString("name='%1' and time=%2 and sdate=%3 and flag=%4")
		.arg(hisData.name)
		.arg(hisData.time)
		.arg(hisData.date)
		.arg(hisData.flag);
	sqlTable=getHisTabPrefix(selpara.dataSrc,selpara.cycle);
	QDate date=convertDaysToDate(hisData.date);
	sqlTable=getHisTabName(selpara.dataSrc,selpara.cycle,date);
	int flag = hisData.flag == HISDATA_PLAN ? HISDATA_PLAN : HISDATA_MANUPDATE;
	sqlWord=QString("update %1 set data=%2,flag=%3 where %4")
		.arg(sqlTable)
		.arg(hisData.val.ycdata)
		.arg(flag)
		.arg(sqlWhere);
	//qDebug()<<"updateHisDataValue SQLWord="<<sqlWord;
	return dbex.Execute(DB_NAME,sqlWord);
}

bool DataCurveDefine::staticsProc( QDate date ,QString pointCode,int staticType )
{
	if(pointCode.isNull() || pointCode.isEmpty()) return false;
	DBExtension &dbex = static_cast<DataCurveApp*>(qApp)->dbex;
	QString sqlWord;
	sqlWord=QString("exec data_statics%1%2 %3,0,'%4'")
		.arg(date.toString("yyyy"))
		.arg(date.toString("MM"))
		.arg(date.year()*10000+date.month()*100+date.day())
		.arg(pointCode);
	return dbex.Execute(DB_NAME,sqlWord);
}











