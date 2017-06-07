#ifndef DATACURVEAPP_H
#define DATACURVEAPP_H

#include <QApplication>
#include <QVector>
#include <QMap>
QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE
class LoginDialog;

#include "dbextension.h"
#include "datacurvedefine.h"
#include "appdef.h"
#include "uitools/login_widget.h"

//login para
#define	GRADE_NUM		32
#define USRRIGHT_NUM	42

#define MODIFY_PLAN_CURVE		0
#define MODIFY_HISTORY_CURVE	2
#define BROWER_PLAN_CURVE		8
#define BROWER_HISTORY_CURVE    10

typedef struct
{
	uint	  type;
	QString   typedesc;
	QString   username;
	QString   userdesc;
	QString   password;
	QString   groupname;
	bool	  ismonitor;
}consumer;

typedef struct 
{
	uint	type;
	QString userdesc;
	QString	password;
	
	QString	hostname;
	QString	username;
	bool	right[USRRIGHT_NUM];
	bool	grade[GRADE_NUM];
	
}consumerRight;

typedef QVector<consumer>	    UserPara;		//用户口令参数表
typedef QVector<consumerRight> RightPara;	    //节点权限参数表
//////////////////////////////////////////////////////////////////////////

typedef QVector<terminalpara* >		Terminals;
typedef QVector<StationAreaPara* >		StationAreaParas;
typedef QVector<stationviewpara* >		Stationviewparas;
typedef QVector<ycpara* >			YcParas;
typedef QVector<kwhpara* >			KwhParas;

typedef QVector<curvetypepara>		CurveTypeParas;

template <typename Container>
typename Container::const_iterator findItem(Container &container, const QString &keyOfname)
{
	typename Container::const_iterator it;
	for(it=container.constBegin();it!=container.constEnd();it++)
		if((*it)->name==keyOfname) return it;
	return it;
};

//系统选择器的数据
class selectorpara
{	
public:
	CurveDataSrc			dataSrc;         //数据源（yc,kwh)
	CurveSelectorType		selType;         //曲线的选择类型，如果是单曲，则paras只能有1条曲线.
	
	CurveCycleType			cycle;           //曲线的周期类型,日、月、年..
	CurveSegmentType		segment;         //曲线的分段标志
	QDate					beginDate;       //曲线的开始日期和时间
	QTime					beginTime;
	QDate   				endDate;         //选择的结束日期
	QTime					endTime;
	int						statType;		  //针对月,年等的统计类型
	uint					dayCurveMinCycle; //custom curve cycle(must be greatthan curve min cycle 0:auto)
	QStringList				paras;			  //曲线的参数键，系统对应编码
	
public:	
	selectorpara();
public:
	void debugOut() const;
	bool normalizationDateTime(int cycleMinute);
	bool refreshAll(const selectorpara& cmpPara );
	bool datetimeComp(const selectorpara& cmpPara);
	int  segmentMaxCount(int cycleMinute);
	int  getMinCycle(int parasIdx=0);
	QString getDescript(int parasIdx=0);
	//plan 
	bool  isPlanType() const;
	void  setPlantime(int plantime);
	//comps function
	bool  isCompType() const;
	void  appendCompDate(QDate date);
	bool  removeCompDate(QDate date);
	void  clearCompDate();
	bool  isExistCompDate(QDate& date);
	int   compDateCount() const;
	QDate compBeginDate(int compIdx) const;
	QTime compBeginTime() const;
	QDate compEndDate(int compIdx) const;
	QTime compEndTime() const;
private:
	QDate                   firstbeginDate;
	QTime					firstbeginTime;
	QDate   				firstendDate;         
	QTime					firstendTime;
	QVector<QDate>          comps;              //同比参数,只在单选有效。
	//QDate endDateComp(int compsIdx);          
	int                     plantime;           //计划曲线的时间间隔
};

enum  NullValueStyle{NullValZero,NullValAvg,NullValFix/*,NullValInterpolate*/};

class selectorcurvepara
{
public:
	selectorcurvepara();
public:
	int					     	curvestyle;
	bool						attrAntialiased;
	bool						attrFitted;
	bool						showsymbol;
	bool						showgrid;
	NullValueStyle              nullValueStyle;			//null value style defined(from curve data calc)
	bool						yAxisAutoScale;			//on_off auto y acale
	int                         yAxisScaleMin;			//y scale min value (default from curve data calc)
	int                         yAxisScaleMax;
public:
	
};


class DataCurveApp : public QApplication  
{
	Q_OBJECT
public:
	DataCurveApp( int & argc, char ** argv);
	~DataCurveApp();
public:
	bool doLogin();
	void initUserRightPara();
	void initStationDb();
	void initStationAreaDb();
	void initStationViewDb();
	void initStationYcDb();
	void initStationKwnDb();
	void initCurveTypeDefs();
	QString getEnvVar() const;


	bool checkLoginUserZone(int zone,consumerRight* right=NULL);
	bool checkLoginUserRight(int rightIdx,consumerRight* right=NULL);
	QString loginUser() const;
	QString loginUserStyle() const;
	static DBExtension dbex;
	static DataCurveDefine	dataCurveDef;
	static bool				ycEverdayTable;		//yc is one day one table
private slots:
public:
	UserPara     userparas;
	RightPara	 rightparas; 

	Terminals	 terminals;
	Stationviewparas stationviewparas;
	YcParas		 ycparas;
	KwhParas	 kwhparas;
    StationAreaParas stationAreaParas;
	QMap<int,int> notoareaid;
	CurveTypeParas	curveTypeDefs;
	curvetypepara   curveTypeDef(int type) 
	{
		curvetypepara typeItem;
		foreach(curvetypepara item,curveTypeDefs)
		{
			if(item.Type()==type)
			{
				typeItem=item;
				break;
			}
		}
		return  typeItem;
	}
	
	selectorpara		selpara;
	selectorcurvepara	selcurvepara;

public:
	consumerRight    currentUserRight;

private:
	Login_widget  *loginDlg;
	bool         isSuperMan;
//	consumerRight    currentUserRight;
// 	QString      loginUser;
// 	QString      loginPwd;
// 	int          loginZone;

	QString		 nbenv;
public: //setting;
	int          maxCurveNumber;
	int          maxTableRowNumber;
	int          defaultMinCycle;
	Qt::TimeSpec timeSpecValue; //Qt::UTC
	
	QColor       aPhaseColor;
	QColor       bPhaseColor;
	QColor       cPhaseColor;
	QColor       defaultColor;

	int          curveLineWidth;
	int          curveSymbolStyle;
	QSize        curveSymbolSize;

	QSize        toolsIconSize;  //mainwindow toolbar icon define size
private:
	QSettings    *settings;
	void         readSettings();
	void         saveSettings();
};

#endif
