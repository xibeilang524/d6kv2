// QMySound.cpp: implementation of the QMySound class.
//
//////////////////////////////////////////////////////////////////////

#include "QMySound.h"
#include "net/netapi.h"

#if defined __unix || __linux 
#include <esd.h>
#include <audiofile.h>
#endif

#define   MAX_AUFILE_LEN      (1024 *24)

#include <stdio.h>
#include <globaldef.h>
#include <qfile.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

QMySound::QMySound()
{
	m_aud = NULL;
	m_volume = 100,
	m_infoflag = 0,
	m_playflag = 1;
}

QMySound::~QMySound()
{
/*#ifndef _AIX
	if(m_aud)
		AuCloseServer((AuServer*)m_aud);
#endif*/
}

void QMySound::play( const char *fname )
{
	printf( "play filename=%s\n", fname );
	int ret = esd_play_file( NULL, fname, 0 );
}
