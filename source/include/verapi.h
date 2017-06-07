//*********************************************
//
//Created by hongxiang 20160701
//
//*********************************************

/**
@brief 获取版本信息
@param in char * ver appname,version
@return 
*/
#if defined(WIN32)
#if defined(APPFUNC_EXPORTDLL)
void  __declspec(dllexport)  writever( char * ver ); 
#else
void __declspec(dllimport)   writever( char * ver );
#endif
#else
void  writever( char * ver );
#endif


