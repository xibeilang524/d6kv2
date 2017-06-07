#include <QtCore>
#include <QModelIndex>

#include <QTreeWidgetItem>
#include <QDate>
#include <QStringList>
#include <QDateTime>
#include <QtAlgorithms>
#include <QMessageBox>


#include <QDebug>

#include "hisdatamodel.h"
#include "datacurveapp.h"

#include "mainwindow.h"
#include "datacurvecommand.h"
#include "uitools/dmessagebox.h"


HisDataModel::HisDataModel(QObject *parent/*=0*/ )
:QAbstractTableModel(parent)
{
	currentCurveSelector=static_cast<DataCurveApp*>(qApp)->selcurvepara;
	//notice : current not init static value in model
	curveBeginRow=0;
}

HisDataModel::~HisDataModel()
{

}

int HisDataModel::rowCount( const QModelIndex & parent /*= QModelIndex()*/ ) const
{
	Q_UNUSED(parent);
	if(parent.isValid())
		return 0;
	//notice :  current not init static value in model 
	if(modelCurveData.size()==0)
		return 0;
	else
	{
		return modelCurveData[0].mTimeSeq.size() + modelCurveData[0].mStatSeq.size();
	}
}


int HisDataModel::columnCount( const QModelIndex & parent /*= QModelIndex() */ ) const
{
	Q_UNUSED(parent);
	//one curve contain time and value  
	return modelCurveData.size()*2;
}

QModelIndex HisDataModel::index( int row, int column, const QModelIndex & parent /*= QModelIndex() */ ) const
{
	return QAbstractTableModel::index(row,column,parent);
}

QVariant HisDataModel::headerData( int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole */ ) const
{
	DataCurveApp *myApp = static_cast<DataCurveApp*>(qApp);
	if(modelCurveData.size()==0 && modelCurveData.size() >= section/2)
		return QAbstractItemModel::headerData(section, orientation, role) ;
	if(orientation==Qt::Horizontal && role==Qt::UserRole)  // return section mapping name(代码) (not return timeseq mapping)
	{
		return QVariant(modelCurveData[section/2].keyName);
	}
	else if(orientation==Qt::Horizontal && role==Qt::UserRole+1)// return this column stat value count
	{
		return QVariant(modelCurveData[section/2].mStatSeq.size());
	}
	else if(orientation==Qt::Horizontal && role==Qt::DisplayRole) // return section mapping table head and scd desc(描述)
	{
		if(section % 2 ==0)
		{
			return QVariant(tr("日期和时间"));
		}
		else
		{
			if(modelCurveData.size() < section/2)
			{
				qDebug()<<"model data size error!!!";
				return QAbstractItemModel::headerData(section, orientation, role) ;
			}
			if(lastSelector.dataSrc==YcDataSrc)
			{
				YcParas::ConstIterator itFind=findItem<YcParas>(myApp->ycparas,modelCurveData[section/2].keyName);
				if(itFind!=myApp->ycparas.constEnd())
				{
					if(modelCurveData[section/2].mDataSeq.count()>0 && (modelCurveData[section/2].mDataSeq[0].state & plantime_data))
						return QVariant(tr("计划值曲线"));
					else
						return QVariant((*itFind)->describe);
				}
			}
			else
			{
				KwhParas::ConstIterator itFind=findItem<KwhParas>(myApp->kwhparas,modelCurveData[section/2].keyName);
				if(itFind!=myApp->kwhparas.constEnd())
				{
					return QVariant((*itFind)->describe);
				}
			}
		}
	}
	return QAbstractItemModel::headerData(section, orientation, role);
}

QVariant HisDataModel::data( const QModelIndex & index, int role /*= Qt::DisplayRole */ ) const
{
	if(!index.isValid())
		return QVariant();
	int col=index.column();											
	int row=index.row();

	if(role==Qt::UserRole)  //return inner curve custom defined value
	{
		//datetime column : both return stat state
		if(col % 2 ==0)  
		{
			if(row<curveBeginRow)
			{
				return QVariant(stat_value);
			}
			else
				return QVariant(curve_value);
		}
		else //return state
		{
			if(row<curveBeginRow)
				return QVariant();
			else
			{
				//qDebug()<< "data: mDataSeq Size : " << modelCurveData[col/2].mDataSeq.size();
				return 	QVariant(modelCurveData[col/2].mDataSeq[row-curveBeginRow].state);
			}
		}
	}
	else if(role == Qt::UserRole+1) //return inner curve data value
	{
		if(col % 2 ==0 )
		{
			if(row<curveBeginRow)
			{
				return QVariant(); 
			}
			else
				return QVariant(modelCurveData[col/2].mTimeSeq[row-curveBeginRow]);
		}
		else
		{
			if(row<curveBeginRow)
				return QVariant(modelCurveData[col/2].mStatSeq[row].val);
			else 
				return QVariant(modelCurveData[col/2].mDataSeq[row-curveBeginRow].val);
		}
	}
	else if(role == Qt::DisplayRole || role == Qt::EditRole)
	{
		if(col%2==0)  //return datetime format text
		{
			if(row<curveBeginRow)
			{
				return QVariant(modelCurveData[col/2].mStatSeq[row].desc);
			}
			else
			{
				if(lastSelector.cycle==DayCycle)
					if(col==0)
						return QVariant(modelCurveData[col/2].mTimeSeq[row-curveBeginRow].toString(tr("yyyy-MM-dd hh:mm")));
					else
					{
						return QVariant(modelCurveData[col/2].mTimeSeq[row-curveBeginRow].toString(tr("yyyy-MM-dd hh:mm")));
					}
					else if(lastSelector.cycle==MonthCycle)
						if(col==0)
							return QVariant(modelCurveData[col/2].mTimeSeq[row-curveBeginRow].toString(tr("yyyy-MM-dd")));
						else
							return QVariant(modelCurveData[col/2].mTimeSeq[row-curveBeginRow].toString(tr("yyyy-MM-dd")));
						else if(lastSelector.cycle==YearCycle)
							if(col==0)
								return QVariant(modelCurveData[col/2].mTimeSeq[row-curveBeginRow].toString(tr("yyyy-MM")));
							else
					return QVariant(modelCurveData[col/2].mTimeSeq[row-curveBeginRow].toString(tr("yyyy-MM")));
			}
		}
		else  //return display text
		{
			if(row<curveBeginRow)
			{
				return QVariant(modelCurveData[col/2].mStatSeq[row].val);
			}
			else
			{
				if((modelCurveData[col/2].mDataSeq[row-curveBeginRow].state & original_data)==original_data)
				{
					return QVariant(QString("%1")
						.arg(modelCurveData[col/2].mDataSeq[row-curveBeginRow].val,0,'f',REAL_PRECISION)
						);
				}
				else if((modelCurveData[col/2].mDataSeq[row-curveBeginRow].state & modified_data)==modified_data)
					return QVariant(QString("%1")
					    .arg(modelCurveData[col/2].mDataSeq[row-curveBeginRow].val,0,'f',REAL_PRECISION)
					);
				else 
				{
					if(role==Qt::EditRole)
						return QVariant(0.0f);
					else
						return QVariant(tr(""));
				}
			}
		}
	}
	else if(role==Qt::BackgroundColorRole)
	{
		if(col % 2 != 0)
		{
			if(row<curveBeginRow)
				return QVariant(STAT_COLOR);
			else
			{
				if(modelCurveData[col/2].mDataSeq[row-curveBeginRow].state & modified_data)
					return QVariant(MODIFIED_COLOR);
				else if(modelCurveData[col/2].mDataSeq[row-curveBeginRow].state & empty_data)
					return QVariant(EMPTY_COLOR);
				else if(modelCurveData[col/2].mDataSeq[row-curveBeginRow].state & original_data)
				return QVariant(ORIGINAL_COLOR);
			}
		}
		else if(row<curveBeginRow)
		{
			return QVariant(STAT_COLOR);
		}
	}
	return QVariant();
}

bool HisDataModel::setData( const QModelIndex &index, const QVariant &value, int role/*=Qt::EditRole*/ )
{
	int mCurveColIdx=index.column()/2;
	int mCurveRowIdx=index.row()-curveBeginRow;
	if(mCurveColIdx >= modelCurveData.size())
		return false;

	mOneCurveData &mOneCurve=modelCurveData[mCurveColIdx];
	if(mCurveRowIdx >= mOneCurve.mDataSeq.size())
		return false;
	
	if(value.userType()==QVariant::Double || 
		value.userType()==QVariant::UInt || 
		value.userType()==QVariant::Int ||
		value.userType() == QVariant::String)
	{
		bool ok;
		double tmpVal=value.toDouble(&ok);
		if(ok && qAbs<double>(value.toDouble()-mOneCurve.mDataSeq[mCurveRowIdx].val)>=EPSILON)
		{
			DataEditCellcommand *cellCmd;
			mData befor,val;
			befor.val =   mOneCurve.mDataSeq[mCurveRowIdx].val;
			befor.state = mOneCurve.mDataSeq[mCurveRowIdx].state;
			val.val= value.toDouble();
			val.state = (DataState)(mOneCurve.mDataSeq[mCurveRowIdx].state | modified_data);
			cellCmd = new DataEditCellcommand(this,index,befor,val,tr("更新数据表项目"));
			mainWindow()->undoStack()->push(cellCmd);

			//mOneCurve.mDataSeq[mCurveRowIdx].val=value.toDouble();
			//mOneCurve.mDataSeq[mCurveRowIdx].state =(DataState)(mOneCurve.mDataSeq[mCurveRowIdx].state | modified_data);
		}
		else
			return false;
	}
	else
	{
		qDebug()<<"HisDataModel::setData fail! value.userType="<<value.userType() ;
	}
//	qDebug()<<"mOneCurve.mDataSeq[mCurveRowIdx].val="<<mOneCurve.mDataSeq[mCurveRowIdx].val;
//	qDebug()<<"mOneCurve.mDataSeq[mCurveRowIdx].state="<<(DataState)(mOneCurve.mDataSeq[mCurveRowIdx].state | modified_data);

/*
 	QModelIndex topLeft(index),bottonRight(index);
 	Q_EMIT(dataChanged(topLeft,bottonRight));
*/
	//temporary replease above code,above code have problem
	reset();  
	return true;
}

Qt::ItemFlags HisDataModel::flags( const QModelIndex &index ) const
{
	DataCurveApp *myApp=static_cast<DataCurveApp*>(qApp);

	if (index.isValid())
	{
		if(index.column() % 2 == 0 || index.row()<curveBeginRow)
			return Qt::NoItemFlags;
		else if(myApp->checkLoginUserRight(MODIFY_HISTORY_CURVE))
			return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
		else
			return Qt::ItemIsEnabled | Qt::ItemIsSelectable ;
	}
	else
		return Qt::ItemIsEnabled;
}

bool HisDataModel::setHeaderData( int section, Qt::Orientation orientation, const QVariant & value, int role /*= Qt::EditRole */ )
{
	return QAbstractTableModel::setHeaderData(section,orientation,value,role);
}



bool HisDataModel::insertRows( int row, int count, const QModelIndex & parent /*= QModelIndex() */ )
{
	Q_UNUSED(parent);
	return true;
}

bool HisDataModel::removeRows( int row, int count, const QModelIndex & parent /*= QModelIndex() */ )
{
	Q_UNUSED(parent);  
	return true;
}

bool HisDataModel::insertColumns( int column, int count, const QModelIndex & parent /*= QModelIndex() */ )
{
	Q_UNUSED(parent);
	return true;
}

bool HisDataModel::removeColumns( int column, int count, const QModelIndex & parent /*= QModelIndex() */ )
{
	Q_UNUSED(parent);
	return true;
}

//////////////////////////////////////////////////////////////////////////
////solts

void HisDataModel::updateModelData(selectorpara& selPara )
{
	if(selPara.cycle==VariantDayCycle) return;
	DataCurveDefine &curveDef = static_cast<DataCurveApp*>(qApp)->dataCurveDef;
	QModelIndex topLeft,bottomRight,parentIndex;
	//if(selPara.paras.size()==0)
		//return;

	if(selPara.selType == MutilSelector)
		CurveMinCycle=selPara.getMinCycle(-1);
	else
		CurveMinCycle=selPara.getMinCycle();  //这里初始化一下最小周期。
	bool mincycleChanged = false;
	

	if(selPara.dayCurveMinCycle==0)
	{
		mincycleChanged = modelSelCycle != CurveMinCycle ? true : false;
		modelSelCycle=CurveMinCycle;
	}
	else if(CurveMinCycle>selPara.dayCurveMinCycle)
	{
		mincycleChanged = modelSelCycle != selPara.dayCurveMinCycle ? true : false;
		modelSelCycle=CurveMinCycle;
	}
	else
	{
		mincycleChanged = modelSelCycle != selPara.dayCurveMinCycle ? true : false;
		modelSelCycle=selPara.dayCurveMinCycle;
	}

	selPara.normalizationDateTime(CurveMinCycle);

	if(lastSelector.refreshAll(selPara) || lastSelector.datetimeComp(selPara) || selPara.selType==SingleSelector ||
		mincycleChanged)
	{
		curveBeginRow=0;
		if(isModifyed(modelCurveData))
		{
// 			if(QMessageBox::question(NULL,tr("提示"),tr("曲线数据被更改，是否存盘?"),QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes)
			if(DMessageBox::question(NULL,tr("提示"),tr("曲线数据被更改，是否存盘?"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No)==QMessageBox::Yes)
			{
				updateModelDataToDB();
			}
			qDebug() << "undoStack clear!!";
			mainWindow()->undoStack()->clear();
		}

		//LiCG 20130325 clear model data
		modelCurveData.clear();
		dbCurveData.clear();
		reset();    //curve comp switch month/year cycle except,add this code is normal;

		bool readit=curveDef.readHisData(selPara,-1,dbCurveData);//全部刷新
		//insert static value
		synchModelData(selPara,modelSelCycle,dbCurveData,modelCurveData,true);

		updateModelNullVal(currentCurveSelector.nullValueStyle,modelCurveData);
		reset();
	}
	else if(selPara.selType==MutilSelector)
	{
		if(lastSelector.paras!=selPara.paras)
		{
			if(lastSelector.paras.size() > selPara.paras.size())
			{
				foreach(QString str,lastSelector.paras)
				{
					if(qFind(selPara.paras,str)==selPara.paras.end())
					{
						if(isModifyed(modelCurveData,str,QDateTime(selPara.beginDate,selPara.beginTime , ((DataCurveApp*)qApp)->timeSpecValue)))
						{
// 							if(QMessageBox::question(NULL, tr("提示"),tr("曲线数据被更改，是否存盘?"),QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes)
							if(DMessageBox::question( NULL, tr("提示"),tr("曲线数据被更改，是否存盘?"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No)==QMessageBox::Yes)
							{
								updateModelDataToDB();
							}
							qDebug() << "undoStack clear!!";
							mainWindow()->undoStack()->clear();
						}
						//del from dbcurvedata;
						DbCurveData::iterator it;
						it=dbCurveData.begin();
						while(it!=dbCurveData.end())
						{
							if((*it).first==str)
							{
								it=dbCurveData.erase(it);
							}
							else
								it++;
						}

						synchModelData(selPara,modelSelCycle,dbCurveData,modelCurveData,false);
						reset();
						//Q_EMIT(dataChanged(topLeft,bottomRight));
					}
				}
			}
			else
			{
				for(int i=0;i<selPara.paras.size();i++)
				{
					if(qFind(lastSelector.paras,selPara.paras[i])==lastSelector.paras.end())
					{
						//add to dbcurvedata
						DbCurveData tmpCurveData;
						if(curveDef.readHisData(selPara,i,tmpCurveData))
						{
							//qCopy(tmpCurveData.begin(),tmpCurveData.end(),dbCurveData.end());
							//qDebug()<<tmpCurveData.size();
 							foreach(DbOneCurveDataPair item,tmpCurveData)
							{
//								//qDebug()<<item.first;
 								dbCurveData.append(item);
							}
						}
						synchModelData(selPara,modelSelCycle,dbCurveData,modelCurveData,false);
						reset();
						//Q_EMIT(dataChanged(topLeft,bottomRight));
					}
				}
			} //end if >
		} //end if !=
	}//end if
	lastSelector=selPara;
}


//////////////////////////////////////////////////////////////////////////
////my implement
bool HisDataModel::findIn( CurveCycleType cycleType,QString keyname,QDateTime findDatetime,int findStatType,
						  const DbCurveData& dbCurveData,hisDataValue& findValue )
{
	DataCurveDefine &curveDef = static_cast<DataCurveApp*>(qApp)->dataCurveDef;
	int sDATE,TIME;

	foreach(DbOneCurveDataPair onePair,dbCurveData)
	{
		if(onePair.first==keyname)
		{
			DbOneCurveData  &dbOneCurveData=onePair.second;
			if(cycleType==DayCycle)	
			{
				sDATE=curveDef.convertDateToDays(findDatetime.date());
				TIME=findDatetime.time().hour()*60+findDatetime.time().minute();
				if(TIME==0)
				{
					TIME=1440;
					sDATE-=1;
				}
				for(int i=0;i<dbOneCurveData.size();i++)
				{
					if(findStatType<0)
					{
						if(dbOneCurveData[i].date==sDATE && dbOneCurveData[i].time==TIME && dbOneCurveData[i].flag<51)
						{
							findValue=dbOneCurveData[i];
							return true;
						}
					}
					else
					{
						if(dbOneCurveData[i].date==sDATE && dbOneCurveData[i].time==TIME && dbOneCurveData[i].flag == findStatType)
						{
							findValue=dbOneCurveData[i];
							return true;
						}
					}
				}
			}
			else if(cycleType==MonthCycle)
			{
				sDATE=curveDef.convertDateToDays(findDatetime.date());
				for(int i=0;i<dbOneCurveData.size();i++)
				{
					if(dbOneCurveData[i].date==sDATE)
					{
						findValue=dbOneCurveData[i];
						return true;
					}
				}
			}
			else if(cycleType==YearCycle)
			{
				sDATE=curveDef.ConvertDateToMons(findDatetime.date());
				for(int i=0;i<dbOneCurveData.size();i++)
				{
					if(dbOneCurveData[i].date==sDATE)
					{
						findValue=dbOneCurveData[i];
						return true;
					}
				}
			}
		}
	}
	return false;
}

/*
void HisDataModel::AddModelData( selectorpara& selpara,int colIdx )
{
	DbOneCurveData dbOneCurveData;
	mOneCurveData  modelOneCurveData;
	QString        selparaname=selpara.paras[colIdx];
	int isfind;
	int findIdx;

	isfind=false;
	foreach(DbOneCurveDataPair dbOnePair,dbCurveData)
	{
		if(dbOnePair.first==selparaname)
		{
			dbOneCurveData=dbOnePair.second;
			isfind=true;
			break;
		}
	}
	if(isfind)
	{
		for(int i=0;i<modelTimeSeq.size();i++)
		{
			findIdx=findIn(selpara,modelTimeSeq[i],dbOneCurveData);
			if(findIdx>=0)
			{
				modelOneCurveData.append(qMakePair(original_data,static_cast<double>(dbOneCurveData[findIdx].val.ycdata)));
			}
			else
			{
				modelOneCurveData.append(qMakePair(empty_data,0.0));
			}
		}
	}
	else 
	{
		for(int i=0;i<modelTimeSeq.size();i++)
			modelOneCurveData.append(qMakePair(empty_data,0.0));
	}
	modelCurveData.append(qMakePair(selparaname,modelOneCurveData));
	return;
}


bool HisDataModel::delModelData( QString nameKey )
{
	for(mCurveData::iterator it=modelCurveData.begin();it!=modelCurveData.end();it++)
	{
		if((*it).first==nameKey)
		{
			modelCurveData.erase(it);
			return true;
		}
	}
	return false;
}

/*
void HisDataModel::initModelTimeSerial(const selectorpara &selpara,int minCycle,mTimeSequence& timeSeq )
{
	//selpara.debugOut();

	QDateTime bDatetime,eDatetime,tmpDatetime;
	timeSeq.clear();
	
	tmpDatetime=QDateTime(selpara.beginDate,selpara.beginTime,((DataCurveApp*)qApp)->timeSpecValue);
	eDatetime=QDateTime(selpara.endDate,selpara.endTime,((DataCurveApp*)qApp)->timeSpecValue);
	do 
	{
		timeSeq.append(tmpDatetime);
		if(selpara.cycle==DayCycle)
			tmpDatetime=tmpDatetime.addSecs(minCycle*60);
		else if(selpara.cycle==MonthCycle)
			tmpDatetime=tmpDatetime.addDays(1);
		else if(selpara.cycle==YearCycle)
			tmpDatetime=tmpDatetime.addMonths(1);
		else
			break;
	} while (tmpDatetime<=eDatetime);
}
*/


void HisDataModel::initModelTimeSeq( CurveCycleType cycleType,QDateTime& begin,QDateTime& end,int minCycle,mTimeSequence& timeSeq )
{
	QDateTime tmpDatetime;
	tmpDatetime=begin;
	timeSeq.clear();
#ifdef DayCycleBeginMinStaticMinute
	bool bSkipFirstZeroPoint=false;
#endif
	do 
	{
#ifdef DayCycleBeginMinStaticMinute
		if(!bSkipFirstZeroPoint && cycleType==DayCycle && lastSelector.segment==GeneralDate 
			&& tmpDatetime.time().hour()==0 && tmpDatetime.time().minute()==0 && tmpDatetime.time().second()==0)
		{
			bSkipFirstZeroPoint=true;
		}
		else
			timeSeq.append(tmpDatetime);
#else
		timeSeq.append(tmpDatetime);
#endif
		if(cycleType==DayCycle)
			tmpDatetime=tmpDatetime.addSecs(minCycle*60);
		else if(cycleType==MonthCycle)
			tmpDatetime=tmpDatetime.addDays(1);
		else if(cycleType==YearCycle)
			tmpDatetime=tmpDatetime.addMonths(1);
		else
			break;
	} while (tmpDatetime<=end);
}

// void HisDataModel::initModelDataSeq(CurveCycleType cycleType,DbCurveData& dbCurveData,mTimeSequence& timeSeq,mDataSequence& dataSeq )
// {
// 	
// }

bool HisDataModel::initOneCurveModelData( QString keyname,CurveCycleType cycle,int curveMinCycle,QDateTime begin,QDateTime end,
										 int curveStatType,	const DbCurveData& dbCurveData,mOneCurveData& modelData )
{
	hisDataValue  dataValue;
	mData         oneData;

	modelData.mDataSeq.clear();
	modelData.mTimeSeq.clear();
	modelData.keyName=keyname;
	
	initModelTimeSeq(cycle,begin,end,curveMinCycle,modelData.mTimeSeq);
	for(int i=0;i<modelData.mTimeSeq.size();i++)
	{
		if(findIn(cycle,keyname,modelData.mTimeSeq[i],curveStatType,dbCurveData,dataValue))
		{
			oneData.state=original_data;
			oneData.val=dataValue.val.ycdata;
		}
		else
		{
			oneData.state=empty_data;
			oneData.val=0;
		}

		if(curveStatType == HISDATA_PLAN)
		{
			oneData.state = (DataState)(oneData.state | plantime_data);
		}
		modelData.mDataSeq.append(oneData);
	}

	if(modelData.mTimeSeq.size()>0) return true;else return false;
}

bool HisDataModel::initOneStatModelData( QString keyname,CurveCycleType cycle,CurveSegmentType segment,QDate date,
										int statType,mOneCurveData& modelData )
{
	DataCurveDefine &curveDef = static_cast<DataCurveApp*>(qApp)->dataCurveDef;
	mStatData       statVal;
// 	if(segment==GeneralDate)//get data from db
// 	{
// 		modelData.mStatSeq.clear();
// 		hisDataValue hisData;
// 		if(cycle==DayCycle)
// 		{
// 			if(curveDef.readDStatData(keyname,date,HISDATA_REALMAX,hisData))
// 			{
// 				statVal.desc=REAL_MAX_DESC;
// 				statVal.val=QString("%1").arg(hisData.val.ycdata,0,'f',REAL_PRECISION);
// 				modelData.mStatSeq.append(statVal);
// 
// 				statVal.desc= STAT_TIME_POINT;
// 				statVal.val=QString("%1 %2:%3")
// 					.arg(curveDef.convertDaysToDate(hisData.date).toString("yyyy-MM-dd"))
// 					.arg(hisData.otime/60)
// 					.arg(hisData.otime%60);
// 				modelData.mStatSeq.append(statVal);
// 			}
// 			else  //calc stat value;
// 			{
// 				
// 			}
// 			if(curveDef.readDStatData(keyname,date,HISDATA_REALMIN,hisData))
// 			{
// 				statVal.desc=REAL_MIN_DESC;
// 				statVal.val=QString("%1").arg(hisData.val.ycdata,0,'f',REAL_PRECISION);
// 				modelData.mStatSeq.append(statVal);
// 				
// 				statVal.desc=tr("发生时间");
// 				statVal.val=QString("%1 %2:%3")
// 					.arg(curveDef.convertDaysToDate(hisData.date).toString("yyyy-MM-dd"))
// 					.arg(hisData.otime/60)
// 					.arg(hisData.otime%60);
// 				modelData.mStatSeq.append(statVal);
// 			}
// 
// 			if(curveDef.readDStatData(keyname,date,HISDATA_AVERAGE,hisData))
// 			{
// 				statVal.desc=tr("实时平均值");
// 				statVal.val=QString("%1").arg(hisData.val.ycdata,0,'f',REAL_PRECISION);
// 				modelData.mStatSeq.append(statVal);
// 			}
// 		}
// 		else if(cycle==MonthCycle)
// 		{
// 			
// 		}
// 	}
	
	bool result=getModelStatVal(cycle,statType,modelData,true);
	curveBeginRow=qMax<int>(modelData.mStatSeq.size(),curveBeginRow);
	if(curveBeginRow == 0)
		qDebug() << "curveBeginRow == 0";
	return result;
}

bool HisDataModel::getModelStatVal( CurveCycleType statCycle,int statType,mOneCurveData& mOneCurve,bool clean/*=false*/ )
{
	DataCurveDefine &curveDef = static_cast<DataCurveApp*>(qApp)->dataCurveDef;
	QString		 strTmp;
	double		 min,max,avg;
	QDateTime    minSdate,maxSdate;
	int			 avgCnt,invalidCnt;
	bool		 bInit;
	mStatData    statVal;
	hisDataValue  dataValue;
	
	if(clean)
		mOneCurve.mStatSeq.clear();  //clean stat

	bInit=false;
	min=max=avg=0;
	avgCnt=invalidCnt=0;

	for(int i=0;i<mOneCurve.mDataSeq.size();i++)
	{
		if((mOneCurve.mDataSeq[i].state & original_data)==original_data && !bInit)
		{
			max=min=mOneCurve.mDataSeq[i].val;
			minSdate=maxSdate=mOneCurve.mTimeSeq[i];
			avg=mOneCurve.mDataSeq[i].val;
			avgCnt++;
			bInit=true;
		}
		else if((mOneCurve.mDataSeq[i].state & original_data)==original_data && bInit)
		{
			if(mOneCurve.mDataSeq[i].val>max)
			{
				max=mOneCurve.mDataSeq[i].val;
				maxSdate=mOneCurve.mTimeSeq[i];
			}
			if(mOneCurve.mDataSeq[i].val<min)
			{
				min=mOneCurve.mDataSeq[i].val;
				minSdate=mOneCurve.mTimeSeq[i];
			}
			avg=avg+mOneCurve.mDataSeq[i].val;
			avgCnt++;
		}

		if(statCycle==DayCycle)
		{
			if(findIn(statCycle,mOneCurve.keyName,mOneCurve.mTimeSeq[i],statType,dbCurveData,dataValue))
			{
				if(dataValue.flag==HISDATA_REAL || dataValue.flag == HISDATA_MANINSERT)
				{
					invalidCnt++;	
				}
			}
		}
	}
	
//	if(bInit)
	{
		statVal.desc=STAT_MAX_DESC;
		statVal.val=QString("%1").arg(max,0,'f',REAL_PRECISION);
		mOneCurve.mStatSeq.append(statVal);

		statVal.desc=STAT_TIME_POINT_DESC;
		if(statCycle==DayCycle)
			statVal.val=QString("%1 %2:%3")
				.arg(maxSdate.toString("yyyy-MM-dd"))
				.arg(maxSdate.toString("hh"))
				.arg(maxSdate.toString("mm"));
		else if(statCycle==MonthCycle)
			statVal.val=QString("%1").arg(maxSdate.toString("yyyy-MM-dd"));
		else if(statCycle==YearCycle)
			statVal.val=QString("%1").arg(maxSdate.toString("yyyy-MM"));
		mOneCurve.mStatSeq.append(statVal);
		
		statVal.desc=STAT_MIN_DESC;
		statVal.val=QString("%1").arg(min,0,'f',REAL_PRECISION);
		mOneCurve.mStatSeq.append(statVal);
		statVal.desc=STAT_TIME_POINT_DESC;
		if(statCycle==DayCycle)
			statVal.val=QString("%1 %2:%3")
			.arg(minSdate.toString("yyyy-MM-dd"))
			.arg(minSdate.toString("hh"))
			.arg(minSdate.toString("mm"));
		else if(statCycle==MonthCycle)
			statVal.val=QString("%1").arg(minSdate.toString("yyyy-MM-dd"));
		else if(statCycle==YearCycle)
			statVal.val=QString("%1").arg(minSdate.toString("yyyy-MM"));
		mOneCurve.mStatSeq.append(statVal);


		statVal.desc=STAT_AVG_DESC;
		statVal.val=QString("%1").arg(avgCnt==0 ? 0: avg/avgCnt,0,'f',REAL_PRECISION);
		mOneCurve.mStatSeq.append(statVal);

		if(statCycle==DayCycle)  //stat invalid point time
		{
			statVal.desc=STAT_DAY_INVALID_COUNT_DESC;
			statVal.val=QString("%1").arg(mOneCurve.mDataSeq.size()-invalidCnt);
			mOneCurve.mStatSeq.append(statVal);

			statVal.desc=STAT_DAY_INVALID_TIME_DESC;
			statVal.val=QString(tr("%1分钟")).arg(qAbs<int>(modelSelCycle*(mOneCurve.mDataSeq.size()-invalidCnt)),0,10);
			mOneCurve.mStatSeq.append(statVal);
		}
	}

	return bInit;
}


void HisDataModel::synchModelData( const selectorpara& selpara,int curveMinCycle,
								   const DbCurveData& dbCurveData,mCurveData& modelCurveData,bool clear/*=true*/ )
{
	int           i,j;
	bool          bfind;
	mOneCurveData oneCurveData;
	QDateTime	  begin,end;
	
	if(clear)
		modelCurveData.clear();
	if(selpara.isCompType()) //compare mode
	{
		//add proc
		for(i=0;i<selpara.compDateCount();i++)
		{
			begin= QDateTime(selpara.compBeginDate(i),selpara.compBeginTime(),((DataCurveApp*)qApp)->timeSpecValue);
			end  = QDateTime(selpara.compEndDate(i),selpara.compEndTime() , ((DataCurveApp*)qApp)->timeSpecValue );
			
			bfind=false;
			for(j=0;j<modelCurveData.size();j++)
			{
				if(modelCurveData[j].keyDate()==begin)
				{
					bfind=true;
					break;
				}
			}
			if(clear || !bfind)
			{
				if(initOneCurveModelData(selpara.paras[0],selpara.cycle,modelSelCycle,begin,end,-1,dbCurveData,oneCurveData))
				{
					//qDebug()<< "oneCurveData key = "<< oneCurveData.keyName << "  oneCurveData count = " << oneCurveData.mTimeSeq.count();
					initOneStatModelData(selpara.paras[0],selpara.cycle,selpara.segment,begin.date(),-1,oneCurveData);
					modelCurveData.append(oneCurveData);
				}
			}
		}
		//del proc
		mCurveData::iterator it;
		it=modelCurveData.begin();
		while(it!=modelCurveData.end())
		{
			bfind=false;
			for(j=0;j<selpara.compDateCount();j++)
			{
				begin= QDateTime(selpara.compBeginDate(j),selpara.compBeginTime() , ((DataCurveApp*)qApp)->timeSpecValue );
#ifdef  DayCycleBeginMinStaticMinute
				begin = QDateTime(begin.date(),QTime(begin.time().hour(),(*it).mTimeSeq[0].time().minute(),(*it).mTimeSeq[0].time().second()) , ((DataCurveApp*)qApp)->timeSpecValue);
#endif
				if(begin==(*it).mTimeSeq[0])
				{
					bfind=true;
					break;
				}
			}
			if(!bfind)
			{
				it=modelCurveData.erase(it);
			}
			else
				it++;
		}
	}
	else
	{
		for(i=0;i<selpara.paras.size();i++)
		{
			begin= QDateTime(selpara.beginDate,selpara.beginTime , ((DataCurveApp*)qApp)->timeSpecValue);
			end  = QDateTime(selpara.endDate,selpara.endTime , ((DataCurveApp*)qApp)->timeSpecValue);
			bfind=false;
			for(j=0;j<modelCurveData.size();j++)
			{
				if(modelCurveData[j].keyName==selpara.paras[i])
				{
					bfind=true;
					break;
				}
			}
			if(clear || !bfind)
			{
				if(initOneCurveModelData(selpara.paras[i],selpara.cycle,modelSelCycle,begin,end,-1,dbCurveData,oneCurveData))
				{
					initOneStatModelData(selpara.paras[i],selpara.cycle,selpara.segment,begin.date(),-1,oneCurveData);
					modelCurveData.append(oneCurveData);
				}
				if(selpara.isPlanType())
				{
					if(initOneCurveModelData(selpara.paras[i],selpara.cycle,modelSelCycle,begin,end,HISDATA_PLAN,dbCurveData,oneCurveData))
					{
						initOneStatModelData(selpara.paras[i],selpara.cycle,selpara.segment,begin.date(),HISDATA_PLAN,oneCurveData);
						modelCurveData.append(oneCurveData);
					}
				}
			}
		}
		//del proc
		mCurveData::iterator it;
		it=modelCurveData.begin();
		while(it!=modelCurveData.end())
		{
			bfind=false;
			for(j=0;j<selpara.paras.size();j++)
			{
				if((*it).keyName==selpara.paras[j])
				{
					bfind=true;
					break;
				}
			}
			if(!bfind)
			{
				it=modelCurveData.erase(it);
			}
			else
				it++;
		}
	}
}



void HisDataModel::updateModelDataToDB()
{
	DataCurveDefine &curveFunc=static_cast<DataCurveApp*>(qApp)->dataCurveDef;
	bool		 bfind;
	hisDataValue hisDataVal;
	
	for(mCurveData::iterator it=modelCurveData.begin();it!=modelCurveData.end();it++)
	{
		mOneCurveData &mOneData=*it;
		for(int i=0;i<mOneData.mTimeSeq.size();i++)
		{
			bool  isPlanVal = lastSelector.isPlanType() && (mOneData.mDataSeq[i].state & plantime_data);
			if(mOneData.mDataSeq[i].state & modified_data) 
			{
				bfind=findIn(lastSelector.cycle,mOneData.keyName,mOneData.mTimeSeq[i],(isPlanVal ? HISDATA_PLAN:-1),dbCurveData,hisDataVal);	
				if(bfind && mOneData.mDataSeq[i].state & original_data)  //has db record and state ok - update it
				{
					hisDataVal.val.ycdata=mOneData.mDataSeq[i].val;
					//hisDataVal.flag=HISDATA_MANUPDATE;
					if(curveFunc.updateHisDataValue(lastSelector,hisDataVal))
					{
						//mOneData.mDataSeq[i].state = (DataState)(mOneData.mDataSeq[i].state & (~modified_data));
						mOneData.mDataSeq[i].state = original_data;
						if(isPlanVal)
						{
							mOneData.mDataSeq[i].state = (DataState)(mOneData.mDataSeq[i].state | plantime_data);
						}
					}
					else
					{
						;//mOneData.mDataSeq[i].state = (DataState)(mOneData.mDataSeq[i].state);
					}
				}
				else if(!bfind && mOneData.mDataSeq[i].state & empty_data)  //has't db record and state ok - insert it
				{
					//problem:state upd but dbcurvedata not update;then don't be imme update data
					hisDataVal.val.ycdata=mOneData.mDataSeq[i].val;	
					hisDataVal.name=mOneData.keyName;
					if(mOneData.mTimeSeq[i].time().hour()==0 && mOneData.mTimeSeq[i].time().minute()==0 && mOneData.mTimeSeq[i].time().second()==0)
					{
						hisDataVal.date=curveFunc.convertDateToDays(mOneData.mTimeSeq[i].date().addDays(-1));
						hisDataVal.time=1440;
					}
					else
					{
						hisDataVal.date=curveFunc.convertDateToDays(mOneData.mTimeSeq[i].date());
						hisDataVal.time=(mOneData.mTimeSeq[i].time()).hour()*60+(mOneData.mTimeSeq[i].time()).minute();
					}
					if(isPlanVal)
						hisDataVal.flag=HISDATA_PLAN;
					else
						hisDataVal.flag=HISDATA_MANINSERT;
					if(curveFunc.insertHisDataValue(lastSelector,hisDataVal))
					{
						mOneData.mDataSeq[i].state = original_data;
						if(isPlanVal)
							mOneData.mDataSeq[i].state = (DataState)(mOneData.mDataSeq[i].state | plantime_data);
					}
					else
					{
						;//mOneData.mDataSeq[i].state = (DataState)(mOneData.mDataSeq[i].state & empty_data);
					}
				}
				
				mainWindow()->undoStack()->clear();
			}// end if modify data
		}// end for each data point
	}//end for each curve
}


bool HisDataModel::curveDataBound( double& min,double& max )
{
	min=max=-1;

	foreach(DbOneCurveDataPair onePair,dbCurveData)
	{
		foreach(hisDataValue hisData,onePair.second)
		{
			if(min==-1)min=hisData.val.ycdata;
			if(max==-1)max=hisData.val.ycdata;
			min=qMin<double>(hisData.val.ycdata,min);
			max=qMax<double>(hisData.val.ycdata,max);
		}
	}
	if(min!=-1 && max!=-1)
		return true;
	return false;
}

void HisDataModel::selectorChanged( selectorpara & selpara )
{
	if(selpara.cycle==DayCycle)
	{
		if(lastSelector.dayCurveMinCycle!=selpara.dayCurveMinCycle)
		{
			//???later modeify this code. inefficient!!
			lastSelector.dayCurveMinCycle=selpara.dayCurveMinCycle;
			updateModelData(selpara); 
		}
	}
	if(selpara.segment==GeneralDate) //auto update model datas
	{
		updateModelData(selpara);
	}
}

void HisDataModel::selectorCurveChanged( selectorcurvepara& selpara )
{
	if(selpara.nullValueStyle!=currentCurveSelector.nullValueStyle)
	{
		updateModelNullVal(selpara.nullValueStyle,modelCurveData);
		//LiCG 20130401 Null selector changed,recursion invoke this
		//reset();
	}
    currentCurveSelector=selpara;
}

void HisDataModel::updateModelNullVal(NullValueStyle nullValStyle,mCurveData& modelData )
{
	if(nullValStyle==NullValZero)
	{
		for(int i=0;i<modelData.size();i++)
		{
			for(int j=0;j<modelData[i].mDataSeq.size();j++)
			{
				if(modelData[i].mDataSeq[j].state & empty_data && !(modelData[i].mDataSeq[j].state & modified_data))
				{
					modelData[i].mDataSeq[j].val=0.0f;
				}
			}
		}
	}
	else if(nullValStyle==NullValAvg)
	{
		int    valCnt;
		double avgVal;
		
		for(int i=0;i<modelData.size();i++)
		{
			valCnt=0;
			avgVal=0;
			for(int j=0;j<modelData[i].mDataSeq.size();j++)
			{
				if(modelData[i].mDataSeq[j].state & original_data)
				{
					avgVal+=modelData[i].mDataSeq[j].val;
					valCnt++;
				}
			}
			valCnt==0 ? avgVal=0 : avgVal=avgVal/valCnt;
			for(int j=0;j<modelData[i].mDataSeq.size();j++)
			{
				if(modelData[i].mDataSeq[j].state & empty_data && !(modelData[i].mDataSeq[j].state & modified_data))
				{
					modelData[i].mDataSeq[j].val=avgVal;
				}
			}
		}
	}
	else if(nullValStyle==NullValFix)
	{
		int    fixValPos;
		double fixVal;

		for(int i=0;i<modelData.size();i++)
		{
			for(int j=0;j<modelData[i].mDataSeq.size();j++) 
			{
				if(modelData[i].mDataSeq[j].state & empty_data && !(modelData[i].mDataSeq[j].state & modified_data))  //
				{
					//find befor this
					fixValPos=-1;
					fixVal=0.0;
					for(int k=j-1;k>=0;k--)
					{
						if(modelData[i].mDataSeq[k].state & original_data)
						{
							fixVal=modelData[i].mDataSeq[k].val;
							fixValPos=k;
							break;
						}
					}
					if(fixValPos>=0)
					{
						modelData[i].mDataSeq[j].val=fixVal;
						continue;
					}
					for(int k=j+1;k<modelData[i].mDataSeq.size();k++)
					{
						if(modelData[i].mDataSeq[k].state & original_data)
						{
							fixVal=modelData[i].mDataSeq[k].val;
							fixValPos=k;
							break;
						}
					}
					if(fixValPos>=0)
					{
						modelData[i].mDataSeq[j].val=fixVal;
						continue;
					}
					// not find in curve set default val 0.0
					modelData[i].mDataSeq[j].val=0.0f;
				}
			} // for each one point in one curve data
		}// for each one model curve data
	}
}

bool HisDataModel::isModifyed( const mCurveData& modelData ) const
{
	foreach(mOneCurveData onedata,modelData)
	{
		if(isModifyed(onedata))
			return true;
	}
	return false;
}

bool HisDataModel::isModifyed( const mOneCurveData& modelOneData ) const
{
	foreach(mData data,modelOneData.mDataSeq)
	{
		if((data.state & modified_data) == modified_data)
		{
			return true;
		}
	}
	return false;
}

bool HisDataModel::isModifyed( const mCurveData& modelData,QString keyname,QDateTime begin ) const
{
	foreach(mOneCurveData onedata,modelData)
	{
		if(onedata.mTimeSeq.size()==0)
			continue;
		if(onedata.keyName==keyname && onedata.mTimeSeq[0]==begin)
			if(isModifyed(onedata))
				return true;
	}
	return false;
}












