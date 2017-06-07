#ifndef  __GDEDLL_H__
#define  __GDEDLL_H__

#ifdef WIN32
	#if defined( _GDEDLL)
		#pragma message("win32 export class....!\n")
		#define GDE_EXPORT _declspec(dllexport)
	#else
		//#pragma message("win32 import class....!\n")
		#define GDE_EXPORT _declspec(dllimport)
	#endif
#else
	#define GDE_EXPORT
#endif

#ifdef WIN32
#if defined( _GDEDLL_CONF)
#pragma message("win32 export class....!\n")
#define GDE_EXPORT_CONF _declspec(dllexport)
#else
//#pragma message("win32 import class....!\n")
#define GDE_EXPORT_CONF _declspec(dllimport)
#endif
#else
#define GDE_EXPORT_CONF
#endif

#ifdef WIN32
#if defined( _GDEDLL_DBI)
//#pragma message("win32 export class....!\n")
#define GDE_EXPORT_DBI _declspec(dllexport)
#else
//#pragma message("win32 import class....!\n")
#define GDE_EXPORT_DBI _declspec(dllimport)
#endif
#else
#define GDE_EXPORT_DBI
#endif

#ifdef WIN32
#if defined( _GDEDLL_DBO)
#pragma message("win32 export class....!\n")
#define GDE_EXPORT_DBO _declspec(dllexport)
#else
//#pragma message("win32 import class....!\n")
#define GDE_EXPORT_DBO _declspec(dllimport)
#endif
#else
#define GDE_EXPORT_DBO
#endif

#ifdef WIN32
#if defined( _GDEDLL_DEF)
#pragma message("win32 export class....!\n")
#define GDE_EXPORT_DEF _declspec(dllexport)
#else
//#pragma message("win32 import class....!\n")
#define GDE_EXPORT_DEF _declspec(dllimport)
#endif
#else
#define GDE_EXPORT_DEF
#endif

#ifdef WIN32
#if defined( _GDEDLL_DES)
#pragma message("win32 export class....!\n")
#define GDE_EXPORT_DES _declspec(dllexport)
#else
//#pragma message("win32 import class....!\n")
#define GDE_EXPORT_DES _declspec(dllimport)
#endif
#else
#define GDE_EXPORT_DES
#endif

#ifdef WIN32
#if defined( _GDEDLL_DRTDBO)
#pragma message("win32 export class....!\n")
#define GDE_EXPORT_DRTDBO _declspec(dllexport)
#else
//#pragma message("win32 import class....!\n")
#define GDE_EXPORT_DRTDBO _declspec(dllimport)
#endif
#else
#define GDE_EXPORT_DRTDBO
#endif

#ifdef WIN32
#if defined( _GDEDLL_GSCDI)
#pragma message("win32 export class....!\n")
#define GDE_EXPORT_GSCDI _declspec(dllexport)
#else
//#pragma message("win32 import class....!\n")
#define GDE_EXPORT_GSCDI _declspec(dllimport)
#endif
#else
#define GDE_EXPORT_GSCDI
#endif

#ifdef WIN32
#if defined( _GDEDLL_MFC)
#pragma message("win32 export class....!\n")
#define GDE_EXPORT_MFC _declspec(dllexport)
#else
//#pragma message("win32 import class....!\n")
#define GDE_EXPORT_MFC _declspec(dllimport)
#endif
#else
#define GDE_EXPORT_MFC
#endif

#ifdef WIN32
#if defined( _GDEDLL_MFCDBO)
#pragma message("win32 export class....!\n")
#define GDE_EXPORT_MFCDBO _declspec(dllexport)
#else
//#pragma message("win32 import class....!\n")
#define GDE_EXPORT_MFCDBO _declspec(dllimport)
#endif
#else
#define GDE_EXPORT_MFCDBO
#endif

#ifdef WIN32
#if defined( _GDEDLL_MFCGIF)
#pragma message("win32 export class....!\n")
#define GDE_EXPORT_MFCGIF _declspec(dllexport)
#else
//#pragma message("win32 import class....!\n")
#define GDE_EXPORT_MFCGIF _declspec(dllimport)
#endif
#else
#define GDE_EXPORT_MFCGIF
#endif


#ifdef WIN32
#if defined( _GDEDLL_MFCDOC)
#pragma message("win32 export class....!\n")
#define GDE_EXPORT_MFCDOC _declspec(dllexport)
#else
//#pragma message("win32 import class....!\n")
#define GDE_EXPORT_MFCDOC _declspec(dllimport)
#endif
#else
#define GDE_EXPORT_MFCDOC
#endif

#ifdef WIN32
#if defined( _GDEDLL_MFCTOOLS)
#pragma message("win32 export class....!\n")
#define GDE_EXPORT_MFCTOOLS _declspec(dllexport)
#else
//#pragma message("win32 import class....!\n")
#define GDE_EXPORT_MFCTOOLS _declspec(dllimport)
#endif
#else
#define GDE_EXPORT_MFCTOOLS
#endif

#endif
