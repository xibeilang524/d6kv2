#include <QApplication>
#include "fesoper.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
	
	QFesOper *pFepOper;
	pFepOper = new QFesOper(NULL);
	pFepOper->show();
	
	return app.exec();
}