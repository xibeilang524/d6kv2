#ifndef _GDCOMMON_H
#define _GDCOMMON_H

#include "ddefs.h"
#include "ddef/GPoint.h"
#include "ddef/GList.h"

enum  gdeStatus
{
	e_Idle,
	e_Pan,
	e_Window,	//¿ª´°·Å´ó
	e_Normal,
	e_Select,
	e_Move,
	e_Size ,
	e_SizeH,
	e_SizeV,
	e_SizeB,
	e_SizeF,
	e_Edit,
	e_Rotate,
	e_Mirror,
	e_Loupe,
	e_MoveCard};

enum gdeCursor
{
	ec_Normal,
	ec_Select,
	ec_Pan   ,
	ec_MoveOver,
	ec_MoveExec,
	ec_SizeOver,
	ec_SizeExec,
	ec_SizeH ,
	ec_SizeV ,
	ec_SizeB ,
	ec_SizeF ,
	ec_Edit  ,
	ec_RotateOver,
	ec_RotateExec,
	ec_Loupe,
	ec_MoveCard,
	ec_Nums};

#endif
