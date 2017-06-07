#include "msgframe.h"
#include "msgviewwnd.h"
#include "verapi.h"
// #include "vld.h"

#include <QFont>

bool g_autoSave = false;
int g_saveDays = 7;


int main(int argc, char *argv[])
{
	//Version
	if(argc > 1)
	{
		for(int i=1; i<argc; i++)
		{
			if(strcmp("--version", argv[i]) == 0)
			{
				printf("fesmsg, 1.2");
				writever("fesmsg,1.2");
				return 0;
			}
		}
	}
	int i = 1;
	char* pSave;
	char cDays[40];
	int iDays;
	bool ok;
	while (i < argc)
	{
		pSave = strstr(argv[i], "-save");

		if (pSave != NULL)
		{
			g_autoSave = true;
		}
		else
		{
			i++;
			continue;
		}

		if (strlen(argv[i]) > strlen("-save"))
		{
			strcpy(cDays, pSave + strlen("-save"));
		}
		
		iDays = _C2Q(cDays).toInt(&ok, 10);
		if (true == ok && iDays > 0)
		{
			g_saveDays = iDays;
		}
		i++;
	}

	QApplication app(argc, argv);
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
	QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());

	QMsgViewWnd* msgViewWnd = new QMsgViewWnd(NULL);
	app.setWindowIcon(QIcon("msgview.ico"));
	
	msgViewWnd->show();
	app.exec();

	return 0;
}
