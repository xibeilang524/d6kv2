#ifndef ALARMTHREAD_H
#define ALARMTHREAD_H

#include <QThread>
#include "alarmwnd.h"

class AlarmThread : public QThread
{
	Q_OBJECT
public:
	AlarmThread( QObject * parent = 0 ) ;
protected:
	void run() ;

private:
};

#endif