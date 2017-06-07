// #include <qlibrary.h>
// #include <qdir.h>
// #include <qstringlist.h>
// #include <qprocess.h>
// #include <stdio.h>
// #include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <list>

#ifdef WIN32
#include <io.h>
#else

#include <dirent.h>

#endif

#include "GetVer.h"
#include <string.h>

using namespace std;

list<string> enumFiles(const char *chDir,const char *filter = 0);
bool contain_str(string src,const char* str)
{
	unsigned int strFind = src.find(str);
	if (strFind != std::string::npos)
		return true;

	return false;
}

int main( int argc, char ** argv )
{
	enum DebugType
	{
		exe = 0,//可执行文件   "-E"
		library,//库文件	        "-L"
		plugin,//插件           "-P"
	};
	
	bool isDebug = false;
	DebugType debugType;
//	QString fileStr = "";
	char fileStr[ 256 ];

	if (argc > 1)
	{
		isDebug = true;
		if (argc > 2)
		{
			if (strcmp(argv[1],"-E")==0)
			{
				debugType = exe;
			}
			
			if (strcmp(argv[1],"-L")==0)
			{
				debugType = library;
			}

			if (strcmp(argv[1],"-P")==0)
			{
				debugType = plugin;
			}

			//argv[2] 文件名，当用“-A”时表示所有文件
			strcpy( fileStr, argv[2] );

// 			printf("argv[1] : %s\n ",argv[1]);
// 			printf("argv[2] : %s\n ",argv[2]);

		}
		else
		{
			printf("error,need filename\nparams should follow the format '-E|L|P [filename]|-A'\n");
			return -1;
		}
	}

	char pathname[ 256 ];
	char filename[ 256 ];
	sprintf( filename, "%s", getenv( "NBENV" ) );


	int i;
	GetVer getver;
	
	list<string> filelist;
	list<string>::iterator it;
	string tmpStr;

	//遍历bin文件夹
	sprintf( pathname, "%s/bin", filename );

#ifdef WIN32
	filelist = enumFiles(pathname,"*.exe");
#else
	filelist = enumFiles(pathname);
#endif
 
	filelist.sort();

	for( it=filelist.begin();it != filelist.end(); ++it )
	{
		if (isDebug && (exe != debugType) )
		{
			break;
		}

		tmpStr = "";
		tmpStr.append(pathname);
		tmpStr.append("/");
		tmpStr.append(*it);

		if (contain_str(tmpStr,"getversion"))
		{
			continue;
		}
		if (contain_str(tmpStr,"chktime"))
		{
			continue;
		}
		if (contain_str(tmpStr,"dboper"))
		{
			continue;
		}
		if (contain_str(tmpStr,"report"))
		{
			continue;
		}
	
		if (isDebug)
		{
			if (0 == strcmp(fileStr,"-A"))//文件名参数为"*",取目录下所有文件版本号
			{
				printf( "Debug : get file version,filename=%s.\n", tmpStr.c_str()  );
				getver.getExeVersion( tmpStr.c_str() );
			}
			else if (contain_str(tmpStr,fileStr) )
			{
				printf( "Debug : get file version,filename=%s.\n", tmpStr.c_str()  );
				getver.getExeVersion( tmpStr.c_str() );
			}
		}
		else
		{
			getver.getExeVersion( tmpStr.c_str() );
		}

	}


	//遍历dll文件夹
	filelist.clear();

#ifdef WIN32
	sprintf( pathname, "%s/dll", filename );
	filelist = enumFiles(pathname,"*.dll");
#else
	sprintf( pathname, "%s/lib", filename );
	filelist = enumFiles(pathname);
#endif

	filelist.sort();
	for( it=filelist.begin();it != filelist.end(); ++it )
	{
		if (isDebug && (library != debugType) )
		{
			break;
		}

		tmpStr = "";
		tmpStr.append(pathname);
		tmpStr.append("/");
		tmpStr.append(*it);

		if (isDebug)
		{
			if (0 == strcmp(fileStr,"-A"))//文件名参数为"*",取目录下所有文件版本号
			{
				printf( "Debug : get file version,filename=%s.\n", tmpStr.c_str()  );
				getver.getDllVersion( tmpStr.c_str() );
			}
			else if (contain_str(tmpStr,fileStr) )
			{
				printf( "Debug : get file version,filename=%s.\n", tmpStr.c_str()  );
				getver.getDllVersion( tmpStr.c_str() );
			}
		}
		else
		{
			getver.getDllVersion( tmpStr.c_str() );
		}

	}

	//遍历plugin文件夹
	filelist.clear();

	sprintf( pathname, "%s/plugin", filename );

	filelist = enumFiles(pathname);

	filelist.sort();
	for( it=filelist.begin();it != filelist.end(); ++it )
	{
		if (isDebug && (plugin != debugType) )
		{
			break;
		}

		tmpStr = "";
		tmpStr.append(pathname);
		tmpStr.append("/");
		tmpStr.append(*it);

		if (isDebug)
		{
			if (0 == strcmp(fileStr,"-A"))//文件名参数为"*",取目录下所有文件版本号
			{
				printf( "Debug : get file version,filename=%s.\n", tmpStr.c_str()  );
				getver.getDllVersion( tmpStr.c_str() );
			}
			else if (contain_str(tmpStr,fileStr) )
			{
				printf( "Debug : get file version,filename=%s.\n", tmpStr.c_str() );
				getver.getDllVersion( tmpStr.c_str() );
			}
		}
		else
		{
			getver.getDllVersion( tmpStr.c_str() );
		}
	}

	return 1;


}


list<string> enumFiles(const char *chDir,const char *filter)
{
	list<string> listFiles;
	char tmpStr[256];
	strcpy(tmpStr,chDir);

#ifdef WIN32
	
	long Handle;
	struct _finddata_t FileInfo;
	
	if (filter)
	{
		strcat(tmpStr,"/");
		strcat(tmpStr,filter);
	} 
	else
	{
		strcat(tmpStr,"/*.*");
	}
	Handle=_findfirst(tmpStr,&FileInfo);

	if(-1L == Handle)
	{
		printf("find in dir error,dir = %s\n",tmpStr);
	}
	else
	{
	//	printf("%s\n",FileInfo.name);

		if (strcmp(FileInfo.name, "..") && strcmp(FileInfo.name, "."))
		{
			listFiles.push_back(FileInfo.name);
		}
		
		while( _findnext(Handle,&FileInfo)==0)
		{
			if (0==strcmp(FileInfo.name, "..") || 0==strcmp(FileInfo.name, "."))
			{
				continue;
			}

	//		printf("%s\n",FileInfo.name);
			listFiles.push_back(FileInfo.name);
		}
		_findclose(Handle);
	}

#else
	
	DIR *dir_info;
	struct dirent *dir_entry;

	dir_info = opendir(tmpStr);
	if (dir_info)
	{

		while((dir_entry = readdir(dir_info))!= NULL )
		{
 			if (0==strcmp(dir_entry->d_name, "..") || 0==strcmp(dir_entry->d_name, "."))
			{
 				continue;
 			}

	//		printf("%s\n",dir_entry->d_name);
			listFiles.push_back(dir_entry->d_name);
		}

		closedir(dir_info);
	} 
	else
	{
		printf("open dir error,dir = %s\n",tmpStr);
	}

#endif

	return listFiles;
}