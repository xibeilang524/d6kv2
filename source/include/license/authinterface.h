#ifndef AUTH_INTERFACE_H
#define AUTH_INTERFACE_H

#include <QObject>
#include <QTimer>
#include <QThread>

#include "license/file_interface.h"

#ifdef AUTH_DLL
# define AUTH_EXPORT Q_DECL_EXPORT
#else
# define AUTH_EXPORT Q_DECL_IMPORT
#endif


class CAuthInterface : public QObject
{
	Q_OBJECT

public:
	CAuthInterface();
	~CAuthInterface();

	// 打开授权。创建线程写入运行时间。
	virtual int OpenAuth(QTimer* timer, QMutex& mutex,QStringList macaddrlist);

	// 关闭授权。
	virtual void CloseAuth();

	// 是否授权到期
	virtual int IsAuthorizedExpired(QStringList macaddrlist);

private:

	// 定时器
	QTimer* m_timer;

	// 授权权限
	bool m_bUnlimited;

	// 打开授权标志
	bool m_bOpenAuthFlag;

	// 告警级别
	int GetWarningLevel();

	// 告警级别
	static int m_nLevel;

	// file
	CFile* m_pFile;

	QMutex* m_mutex;

private:

	void SaveWarningLevel(int nLevel);

	public slots:

		// 定时写入运行时间
		void TimerWriteRunTime();
};

extern "C" AUTH_EXPORT CAuthInterface* GetAuthDll(); //获取类CAuthInterface的对象


#endif
