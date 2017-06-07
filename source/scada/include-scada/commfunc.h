#ifndef _COMM_FUNC_H
#define _COMM_FUNC_H


#ifdef WIN32
int __declspec(dllexport)
#else
int
#endif
RequireToken(unsigned char *tokenf,int timeout=1500);

#ifdef WIN32
int __declspec(dllexport)
#else
int
#endif
FreeToken(unsigned char *tokenf);


#endif
