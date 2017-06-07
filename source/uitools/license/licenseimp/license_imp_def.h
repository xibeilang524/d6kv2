#ifndef LICENSE_IMP_DEF_H
#define LICENSE_IMP_DEF_H

#define _Q2C( str ) ( ( const char * )( ( str ).toLocal8Bit(  ).data(  ) ) ) 
#define _C2Q( str ) ( QString::fromLocal8Bit( str ) )

typedef struct _LICENSE_STRU
{
	short no ;
	short period ;
	char file_gen_time[ 24 ] ;
	char file_imp_time[ 24 ] ;
	char mac_addr[ 24 ] ;
	short expired_flag ;
}LICENSE_STRU ;

#define FillGdbRecord( record,column,item )\
	record[column] = item ;\
	column ++ ;\

#endif