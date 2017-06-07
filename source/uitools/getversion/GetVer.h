// GetVer.h: interface for the GetVer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GETVER_H__F76CA0C8_4F2E_4CA9_A57A_FC1E6E5A8D7D__INCLUDED_)
#define AFX_GETVER_H__F76CA0C8_4F2E_4CA9_A57A_FC1E6E5A8D7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
#include <qobject.h>
#include <qprocess.h>

#define _Q2C(str) ((const char *)(str.toLocal8Bit().data()))
#define _C2Q(str) (QString::fromLocal8Bit(str))
*/

class GetVer// : public QObject  
{
public:
	GetVer();
	virtual ~GetVer();
	void getExeVersion(const char* filename );
	void getDllVersion(const char* filename );
private:
//	QProcess m_proc;

};

#endif // !defined(AFX_GETVER_H__F76CA0C8_4F2E_4CA9_A57A_FC1E6E5A8D7D__INCLUDED_)
