// QMangerPara.h: interface for the QMangerPara class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QMangerPara_H__37A82C9D_FDFE_4815_AA2D_4AE0420DE405__INCLUDED_)
#define AFX_QMangerPara_H__37A82C9D_FDFE_4815_AA2D_4AE0420DE405__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <qstring.h>
#include <qlist.h>
#include <qmap.h>
#include <qstringlist.h>
#include <qmutex.h>

#include "db/dbapi.h"

#define EVENT_SORT_NUM	12

typedef  unsigned long udword;


enum NodeType
{
	NODETYPE_USER = 0,
	NODETYPE_DEV
};

typedef struct NodePara
{
	udword		zone1;		// modfiy [4/10/2012 zhoujian]
	udword		zone2;
	udword		oldzone1;
	udword		oldzone2;

	QString		code;
	QString		desc;
	NodeType	type;
	uint		devtype;
	QString		sname;
}NODEPARA,*PNODEPARA;

typedef struct UserPara		// shunzi [4/11/2012]
{
	bool        userchanged;

	QString		code;
	QString		desc;
	int		roletype;
	QString		roledesc;
	QString		password;
	bool		groupleader;
}USERPARA,*PUSERPARA;

typedef struct _StationPara
{
	QString	code;		//´úÂë
	QString	desc;		//ÃèÊö
}STATIONPARA,*PSTATIONPARA;

typedef struct _DevTypePara
{
	uint devtype;
	QString devname;
	QString tablename;
}DEVTYPEPARA, *PDEVTYPEPARA;

class QMangerPara
{
public:
	QMangerPara();
	virtual ~QMangerPara();

	bool readPara();
	bool savePara();

	bool AddUserPara(QList<PUSERPARA> *paddlist); // ljs [4/17/2012]
	bool DelUserPara(QList<PUSERPARA> *pdellist); // ljs [4/17/2012]
	bool saveUserCfgPara(QList<PUSERPARA> *pchaglist);	// ljs [4/18/2012]


	bool DelNodePara(QList<PNODEPARA> *pdellist);
	bool AddNodePara(QList<PNODEPARA> *paddlist);
	bool AddRolePara(QList<PNODEPARA> *paddrole);  //libinghong 2012-04-16 
	bool DelRolePara(QList<PNODEPARA> *pdellist);

	QList<NODEPARA *>		m_roleList;	// add [4/9/2012 zhoujian]
	QList<NODEPARA *>		m_userList;
	QList<USERPARA *>		m_usercfgList;
	QList<NODEPARA *>		m_devList;
	QList<DEVTYPEPARA *>	m_devTypeList;

	QMap<QString, QString>	m_hostmap;

private:
	bool readRolePara();	// add [4/9/2012 zhoujian]
	bool readNodeTable();
	bool readUserPara();
	bool readUserCfgPara();

	bool saveUserPara();
	bool saveRolePara();
	bool saveUserCfgPara();

	bool AddRolePara();


private:
	DBApi					m_db;
};

#endif // !defined(AFX_QMangerPara_H__37A82C9D_FDFE_4815_AA2D_4AE0420DE405__INCLUDED_)
