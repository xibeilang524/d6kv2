// GetVer.cpp: implementation of the GetVer class.
//
//////////////////////////////////////////////////////////////////////
// #include <qprocess.h>
// #include <qstringlist.h>
// #include <qlibrary.h>

#include "GetVer.h"
#include "net/netapi.h"

#ifdef WIN32
#include <Windows.h>
#elif __hpux
#include <dlfcn.h>
#include <dl.h>
#else
#include <dlfcn.h>
#endif

typedef void* (*GETVER_PROC)();

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GetVer::GetVer()
{
}

GetVer::~GetVer()
{

}

void GetVer::getExeVersion(const char* filename )
{
/*
	QString tmpstr;
	QStringList processArgs;
	

	processArgs << _C2Q( "--version" );

	m_proc.execute( filename, processArgs );
*/

	char strFile[256];
	sprintf(strFile,"%s  --version",filename);


	try
	{
		system(strFile);
	}
	catch (...)
	{
		printf( "exception catched£¬filename=%s\n",  filename );
	}


/*
#if defined(WIN32)

	WinExec(strFile,SW_HIDE);

#else

	if (fork()==0)
	{
		execl(filename,filename,"--version",NULL);
	}

#endif
*/

}

void GetVer::getDllVersion(const char* filename )
{

	GETVER_PROC gProc = NULL;

#ifdef WIN32
	HMODULE	 handle;

	handle = ::LoadLibrary((wchar_t*)filename);
	if (NULL == handle)
	{
		printf( "load lib error£¬filename=%s\n",  filename );
		return;
	}

	try
	{
		gProc = (GETVER_PROC)::GetProcAddress(handle , "getVersion");
		if ( !gProc )
		{
			return;
		}
		gProc();
		::FreeLibrary(handle);
	}
	catch (...)
	{
		printf( "exception catched£¬filename=%s\n",  filename );
	}

#elif __hpux
	shl_t handle;

	handle = shl_load(filename, BIND_DEFERRED | BIND_NONFATAL | DYNAMIC_PATH, 0)
	if (NULL == handle)
	{
		printf( "load lib error£¬filename=%s\n",  filename );
		return;
	}

	try
	{
		if (shl_findsym(&handle, "getVersion", TYPE_UNDEFINED, &gProc) < 0) 
		{
			return;
		}

		gProc();
		shl_unload(handle);
	}
	catch (...)
	{
		printf( "exception catched£¬filename=%s\n",  filename );
	}

#else	//AIXºÍlinux

	void * handle = NULL;

	handle = dlopen( filename, RTLD_LAZY);
	if (NULL == handle)
	{
		printf( "load lib error£¬filename=%s\n",  filename );
		return;
	}

	try
	{
		gProc = (GETVER_PROC)dlsym(handle, "getVersion");
		if ( !gProc )
		{
			return;
		}
		gProc();
		dlclose(handle);
	}
	catch (...)
	{
		printf( "exception catched£¬filename=%s\n",  filename );
	}

#endif

}
