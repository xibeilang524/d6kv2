// QMySound.h: interface for the QMySound class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QMYSOUND_H__5757579B_F38B_4450_82D6_F56552860E7C__INCLUDED_)
#define AFX_QMYSOUND_H__5757579B_F38B_4450_82D6_F56552860E7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class QMySound  
{
public:
	QMySound();
	virtual ~QMySound();

	void play( const char *fname );

private:
	void *m_aud;
	int   m_volume,
        m_infoflag,
        m_playflag;

};

#endif // !defined(AFX_QMYSOUND_H__5757579B_F38B_4450_82D6_F56552860E7C__INCLUDED_)
