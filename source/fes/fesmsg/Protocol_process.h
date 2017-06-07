#ifndef PROTOCOL_PROCESS_H_
#define PROTOCOL_PROCESS_H_
#include "math.h"


/*将字节数组计算成无符号数值*/
double usghextoint(const BYTE* hex, const int& len)
{   
	int res = 0;
	int i = 0;
	for (i = 0; i < len; i++)
	{
		res += hex[i] * pow(base_num, i);
	}
	return res;
}

/*将字节数组计算成有符号数值-104规约*/
double sghextoint_104(const BYTE* hex, const int& len)
{   
	int res = 0;  //返回值
	int tem = 0;
	int i = 0;
	BYTE hex_tem[5];
	memcpy(hex_tem,hex,len);
	for (i = 0; i < len; i++)
	{

		if (i == (len-1))
		{
			tem = hex_tem[i]&0x80;
			hex_tem[i] = hex_tem[i]&0x7f;
		}
		res += hex_tem[i] * pow(base_num, i);

	}
	if(tem)
	{
		res = -res;
	}
	return res;
}

/*将字节数组计算成有符号数值-CDT规约*/
double sghextoint_cdt(const BYTE* hex, const int& len)
{   
	int res = 0;  //返回值
	int tem = 0;
	int i = 0;
	BYTE hex_tem[5];
	memcpy(hex_tem,hex,len);
	for (i = 0; i < len; i++)
	{

		if (i == (len-1))
		{
			tem = hex_tem[i]&0x08;
			hex_tem[i] = hex_tem[i]&0x07;
		}
		res += hex_tem[i] * pow(base_num, i);

	}
	if(tem)
	{
		res = -res;
	}
	return res;
}


#endif