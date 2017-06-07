#ifndef LICENSE_GEN_DEF_H
#define LICENSE_GEN_DEF_H

#define _Q2C( str ) ( ( const char * )( ( str ).toLocal8Bit(  ).data(  ) ) ) 
#define _C2Q( str ) ( QString::fromLocal8Bit( str ) )

enum
{
	E_THREE_DAY,				// 3天
	E_ONE_MONTH,				// 1个月
	E_THREE_MONTH,        // 3个月
	E_HALF_YEAR,				// 半年
	E_ONE_YEAR,					// 1年
	E_TWO_YEAR,				// 2年
	E_UNLIMITED					// 无限
}TIME_LEVEL;

// AES 加密解密算法key
unsigned char aes_key[] = \
{ \
0x2b, 0x7e, 0x15, 0x16, \
0x28, 0xae, 0xd2, 0xa6, \
0xab, 0xf7, 0x15, 0x88, \
0x09, 0xcf, 0x4f, 0x3c \
};

#endif