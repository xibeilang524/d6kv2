#include <QLatin1String>
#include <QTextDecoder>
#include <QTranslator>
#include <QLocale>
#include <QFont>
#include <QMessageBox>

#include <QDebug>

#include "datacurveapp.h"
#include "mainwindow.h"

#ifdef APP_HAS_VERSION
#include "verapi.h"
#endif

#include "auth.h"
#include "uitools/dmessagebox.h"

int main(int argc, char **argv) 
{
#ifdef APP_HAS_VERSION
	//Version
	if(argc > 1)
	{
		for(int i=1; i<argc; i++)
		{
			if(strcmp("--version", argv[i]) == 0)
			{
				writever("hiscurve,1.1");
				printf("hiscurve, 1.1");
				return 0;
			}
		}
	}
#endif
	DataCurveApp app(argc, argv);
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());


	QTranslator  translatorMain,translatorQt;
	QString local=QLocale::system().name();
	QString masevn=app.getEnvVar();
	translatorMain.load(masevn+QLatin1String("/resource/hiscurve_")+local);
	translatorQt.load(masevn+QLatin1String("/resource/qt_")+local);
	app.installTranslator(&translatorMain); 
	app.installTranslator(&translatorQt); 
#if defined(Q_OS_WIN32)
	QFont font("simsun",9,QFont::Normal,false);       //宋体
	app.setFont(font);
#elif defined(Q_OS_AIX)
	// 	QFont font("Song Ti",9,QFont::Normal,false);  //宋体
	// 	setFont(font);
#endif
	if(app.dbex.Open(DB_OP_WR,FALSE,TRUE)==0)
	{
		DMessageBox::critical(NULL,QObject::tr("错误提示"),QObject::tr("系统数据库连接失败，\r\n请检查数据库本机数据库客户端配置是否正确。"),
			QMessageBox::Ok,QMessageBox::NoButton);
		app.exit(0);
		return 0;
	}
	app.initUserRightPara();

	//added  [12/9/2012 qj]
	QString strUsername;
	QString strPassword;
	QString strSys_event;//事件对象名0（SYS_EVENT结构）
	QString strDate;//日期

	if (argc > 1)//有参数传入
	{
		QString strParam;
		for (int i=1;i<argc;i++)
		{
			strParam = C2G(argv[i]);
			if ('-' == argv[i][0])
			{
				if ('U' == argv[i][1])//username
				{
					strUsername = strParam.mid(2);
				} 
				else if ('P' == argv[i][1])//password
				{
					strPassword = strParam.mid(2);
				} 
				else if ('R' == argv[i][1])//event name
				{
					strSys_event = strParam.mid(2);
				} 
				else if ('D' == argv[i][1])//date
				{
					strDate = strParam.mid(2);
				}
			}
			qDebug()<<strParam<<"\n";
		}

		bool bAccept = false;
		foreach(consumerRight item,app.rightparas)
		{
			if( item.username.compare(strUsername,Qt::CaseInsensitive)==0
				&& (0 == user_auth(G2C(strUsername), G2C(strPassword), G2C(item.password))) )
			{
				app.currentUserRight=item;
				bAccept = true;
				break;
			}
		}
		if (!bAccept)
		{
			qDebug()<<QObject::tr("匿名用户不允许登陆。")<<"\n";
			return 0;
		}
		else
		{
			if(!app.checkLoginUserRight(BROWER_HISTORY_CURVE,&(app.currentUserRight)))
			{
				DMessageBox::information(NULL,QObject::tr("登录提示"),QObject::tr("登录用户没有浏览历史曲线数据的权限，请向管理员申请或重新登录"),
					QMessageBox::Ok,QMessageBox::NoButton);
				return 0;
			}
		}
	}
	else
	{
		if(!app.doLogin())
		{
			app.exit(0);
			return 0;
		}
		else
		{
			app.dbex.Open(DB_OP_WR,FALSE,TRUE) ;
		}
	}

	app.initCurveTypeDefs();
	app.initStationDb();
	app.initStationAreaDb();
	app.initStationViewDb();
	app.initStationYcDb();
	app.initStationKwnDb();

	MainWindow mainWindow;
	mainWindow.setWindowState(Qt::WindowMaximized);
	mainWindow.show();
	mainWindow.setTreeItemSelect(strSys_event,strDate);

	return app.exec();
}
