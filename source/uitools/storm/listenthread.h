#ifndef LINTENTHREAD_H_
#define LINTENTHREAD_H_

#include <QThread>

class QListenThread: public QThread
{
public:
	QListenThread();
	void stop(void);
	//virtual ~QListenThread();

private:
	void run(void);
};


#endif