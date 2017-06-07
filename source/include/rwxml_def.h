/**
@file rwxml_def.h
@brief 

@author chenkai
@version 1.2.0
@date 2017-1-14
*/
#ifndef RWINI_DEF_H
#define RWINI_DEF_H

#ifdef WIN32
#if defined( RWXMLDLL)
#pragma message("win32 export class....!\n")
#define RWXML_EXPORT _declspec(dllexport)
#else
#pragma message("win32 import class....!\n")
#define RWXML_EXPORT _declspec(dllimport)
#endif
#else
#define RWXML_EXPORT
#endif

#endif