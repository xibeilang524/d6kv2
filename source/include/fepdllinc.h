#ifndef  __PROTOCOLDLL_H__
#define  __PROTOCOLDLL_H__

#ifdef WIN32
	#if defined( _PROTOCOLDLL)
		#pragma message("win32 export class....!\n")
		#define PROTOCOL_EXPORT _declspec(dllexport)
	#else
		#pragma message("win32 import class....!\n")
		#define PROTOCOL_EXPORT _declspec(dllimport)
	#endif
#else
	#define PROTOCOL_EXPORT
#endif

#endif
