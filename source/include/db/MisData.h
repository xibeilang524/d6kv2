// MisData.h: interface for the MisData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MISDATA_H__C46900D5_C1D3_4E72_A317_437E71835870__INCLUDED_)
#define AFX_MISDATA_H__C46900D5_C1D3_4E72_A317_437E71835870__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dbapi.h"

typedef struct _mis_str{
	unsigned short	date;
	unsigned short  time;
	unsigned short  dbno;
}MisStr;

class MisData  
{
	static MisStr*	m_pmis;
	static int		m_num;
	static int		m_mem;
	static int		m_step;
	
public:
	int FreeMem();
	int DelMis(FUN_HIS &fun,int dbno);
	int AddMis(FUN_HIS &fun,int dbno);
	int IsMis(FUN_HIS &fun,int dbno);
	int IsNewData(FUN_HIS &fun);
	MisData();
	virtual ~MisData();


};

#endif // !defined(AFX_MISDATA_H__C46900D5_C1D3_4E72_A317_437E71835870__INCLUDED_)
