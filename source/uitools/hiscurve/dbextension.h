#ifndef DBEXTENSION_H
#define DBEXTENSION_H
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QVector>
#include <QMutex>

#include "db/dbapi.h"

class DBExtension : public DBApi
{
public:
	DBExtension();
	~DBExtension();
public:
	int  sqlReadRecorderDir(QString dbName,QString sqlWord,QStringList& content);
	int  ReadRecorders(QString dbName,QString tableName,QStringList& fieldName,QString whereCond,QString orderFn,QStringList& content,bool distinct=true);
	bool InsertRecords(QString dbName,QString tableName,QStringList& fieldname,QStringList& fieldValue,int recNums = 0);
	bool Execute(QString dbName,QString sqlStatement);
	int  sqlReadRecorderDir(QString dbName,QString sqlWord,QVector<QVariant>& content);
private:
	QMutex dbApiLocker;
};

#endif
