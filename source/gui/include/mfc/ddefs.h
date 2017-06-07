#ifndef _GDEFS_H
#define _GDEFS_H

#include "ddef/ddes_comm.h"

#define MB_OK                       0x00000000L
#define MB_OKCANCEL                 0x00000001L
#define MB_ABORTRETRYIGNORE         0x00000002L
#define MB_YESNOCANCEL              0x00000003L
#define MB_YESNO                    0x00000004L
#define MB_RETRYCANCEL              0x00000005L

#define MB_ICONHAND                 0x00000010L
#define MB_ICONQUESTION             0x00000020L
#define MB_ICONEXCLAMATION          0x00000030L
#define MB_ICONASTERISK             0x00000040L

#define MB_ICONWARNING              MB_ICONEXCLAMATION
#define MB_ICONERROR                MB_ICONHAND
#define MB_ICONINFORMATION          MB_ICONASTERISK
#define MB_ICONSTOP                 MB_ICONHAND

#define IDOK                1
#define IDCANCEL            2
#define IDABORT             3
#define IDRETRY             4
#define IDIGNORE            5
#define IDYES               6
#define IDNO                7

enum HoverStatus
{
	H_NORMAL,
	H_PUSHED,
	H_NORMALHOT,
	H_PUSHEDHOT
};

#define ID_SEPARATE		-1
#define MENU_POPUP		0x01
#define TOOL_WIDGET		0x02
#define LARGEICON		0x04

#define CHECKMASK(a,b)	( (uint32)((a)&(b))==(uint32)(b))

#endif
