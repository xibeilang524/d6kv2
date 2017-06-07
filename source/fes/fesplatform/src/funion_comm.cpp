#include "fes/platformwrapper.h"

static unsigned char tbch0[256];					//BCH 校验码

/////////////////////////////////////////////////////////////////////////
//检测报文头，若满足同步帧返回true，否则返回false
bool Scan_msg_head(unsigned char* buf)	
{
	bool retcode = true;

	for (int loop =0; loop < 3; loop++)
	{
		if (buf[2*loop] != START_CODE || buf[2*loop+1] != SYNC_CODE)
		{
			retcode = false;
			break;
		}
	}

	return retcode;
}

void  MakeBch (unsigned char* bch,unsigned char* buf)
{
	char i, v;

	v = 0;
	for (i=0; i<5; i++)
	{
		v = v ^ buf[i];
		v = tbch0[(unsigned char)v];
	}
	*bch = ~v;

}

void MakeBCHList (void)
{
	int    i, j;
	unsigned char v, val, count;

	count = 0;
	for (i=0; i<256; i++)
	{
		val = count;
		for(j=0; j<8; j++)
		{
			v = val & 0x80;
			val = val << 1;
			if (v == 0x80) val = val ^ 7;
		}
		tbch0[i] = val;
		count++;
	}
}

bool CRC_test( unsigned char *buf , int len , CRC_STATE flag )
{
	unsigned char	Result = 0;
	buf ++ ;
	len -- ;

	for ( int i = 0 ; i < len ; i++ ) //modify by chenkai
	{
		Result +=*buf++;
	}
	if ( flag == CRC_WRITE ) 
	{
		*buf = Result;
	}
	if ( Result == *buf )	
	{
		return true; 
	}
	return	false;
}