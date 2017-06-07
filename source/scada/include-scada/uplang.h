#ifndef  _USERPROC_LANG_H
#define  _USERPROC_LANG_H


#define  MAXWORDLEN       256
#define  MAXSTRINGLEN     MAXWORDLEN    



//////////////////  指令定义  //////////////////////////////////////////
#define  INST_RETURN            0    //   return addr1
#define  INST_ADD               2    //   addr1 +  addr2->addr3
#define  INST_SUB               3    //   addr1 -  addr2->addr3
#define  INST_MUL               4    //   addr1 *  addr2->addr3
#define  INST_DIV               5    //   addr1 /  addr2->addr3
#define  INST_MOD               6    //   addr1 %  addr2->addr3
#define  INST_MULMUL            7    //   addr1 ** addr2->addr3
#define  INST_AND               8    //   addr1 && addr2->addr3
#define  INST_OR                9    //   addr1 || addr2->addr3
#define  INST_LARGE             10   //   addr1 >  addr2->addr3
#define  INST_LARGEEQU          11   //   addr1 >= addr2->addr3
#define  INST_SMALL             12   //   addr1 <  addr2->addr3
#define  INST_SMALLEQU          13   //   addr1 <= addr2->addr3
#define  INST_NOTEQU            14   //   addr1 != addr2->addr3
#define  INST_EQUEQU            15   //   addr1 == addr2->addr3
#define  INST_BITAND            16   //   addr1 &  addr2->addr3
#define  INST_BITOR             17   //   addr1 |  addr2->addr3
#define  INST_XOR               18   //   addr1 ^  addr2->addr3
#define  INST_LSHIFT            19   //   addr1 << addr2->addr3
#define  INST_RSHIFT            20   //   addr1 >> addr2->addr3
#define  INST_FU                21   //   -addr1        ->addr3
#define  INST_BITNOT            22   //   ~addr1        ->addr3
#define  INST_PUSH              23   //   addr1的实际地址 -> stack
#define  INST_POP               24   //   stack -> addr3
#define  INST_JMP               25   //   JMP addr3
#define  INST_JMPZERO           26   //   IF(addr1==0) JMP addr3
#define  INST_CALLINPROC        27   //   调用addr1指向的子过程
#define  INST_INPROCRET         28   //   子过程返回主过程

#define  INST_MOVADDR           40   //   addr1 ->  addr3的内容作为实际地址
#define  INST_ADDRMOV           41   //   addr1的内容作为实际地址 -> addr3
#define  INST_MOVSTRADDR        42   //   strcpy(addr3的内容作为实际地址,addr1);
#define  INST_ADDRMOVSTR        43   //   strcpy(addr3,addr1的内容作为实际地址);

#define  INST_MOV               44   //   addr1 -> addr3
#define  INST_MOVOFF            45   //   addr1 -> addr3的内容作为偏移
#define  INST_OFFMOV            46   //   addr1的内容作为偏移 -> addr3
#define  INST_MOVSTR            47   //   strcpy(addr3,add1)
#define  INST_MOVSTROFF         48   //   strcpy(addr3的内容作为偏移, addr1)
#define  INST_OFFMOVSTR         49   //   strcpy(addr3, addr1的内容作为偏移)
#define  INST_ARRAYCHK          50   //   addr1为数组定义的单元个数，
									 //	  addr2为当前数组的下标


#define  INST_ADDSTR            60   //   strcat(addr3,addr1);strcat(addr3,addr2); 
#define  INST_STREQUEQU         61   //   if(strcmp(addr1,addr2)==0) addr3=1;
                                     //   else addr3=0;
#define  INST_STRNOTEQU         62   //   if(strcmp(addr1,addr2)!=0) addr3=1;
                                     //   else addr3=0;                             
#define  INST_STRLARGE          63   //   if(strcmp(addr1,addr2)>0) addr3=1;
                                     //   else addr3=0;                             
#define  INST_STRLARGEEQU       64   //   if(strcmp(addr1,addr2)>=0)  addr3=1;
                                     //   else addr3=0;                                       
#define  INST_STRSMALL          65   //   if(strcmp(addr1,addr2)<0)  addr3=1;
                                     //   else addr3=0;                                         
#define  INST_STRSMALLEQU       66   //   if(strcmp(addr1,addr2)<=0)  addr3=1;
                                     //   else addr3=0;                                        


#define  INST_CALLFUNC          70   //   call(addr1) ->addr3
#define  INST_PARATYPE          71   //   addr1=addrtype


#define  INST_OBJECT            72   //   addr1=object addr,addr2= object type
#define  INST_MALLOCOBJ         73   //   malloc object(addr1的内容作为object type) -> addr3
#define  INST_FREEOBJ           74   //   free object (addr1的内容作为object type,addr2 object addr)

////////////////////////////////////////////////////

#define  MAXFUNCNAME_LEN       30
#define  MAXINPARANUMS         128

#define  INPARA_DOUBLE         0
#define  INPARA_STRING         1
#define  INPARA_OBJECT         2

/////////////////////// 调用函数定义 ///////////////

///// 通用函数 ///////
#define  FUNC_BASIS_START     1
#define  FUNC_SQRT            FUNC_BASIS_START
#define  FUNC_COS             2
#define  FUNC_SIN             3
#define  FUNC_TAN             4
#define  FUNC_ACOS            5
#define  FUNC_ASIN            6
#define  FUNC_ATAN            7
#define  FUNC_EXP             8
#define  FUNC_LOG             9
#define  FUNC_LOG10           10
#define  FUNC_ABS             11
#define  FUNC_POW             12
#define  FUNC_MAX             13
#define  FUNC_MIN             14
#define  FUNC_INT             15
#define  FUNC_INT45           16
#define  FUNC_CBRT            17
#define  FUNC_CALLPROC        18
#define  FUNC_PRINT           19
//#define  FUNC_PRINTSTR        20
#define  FUNC_GETDATE         22
#define  FUNC_GETTIME         23
#define  FUNC_SLEEP           24
#define  FUNC_SLEEPMS         25
#define  FUNC_CALDAYS         26
#define  FUNC_CALSECS         27
#define  FUNC_EXECSHELL       28
#define  FUNC_RAND            29

#define  FUNC_CHKYKCONDITION	30

#define  FUNC_POS				31	//取正数
#define  FUNC_NEG				32	//取负数

#define  FUNC_ATOI				33  //字符串转换成数值
#define  FUNC_ITOA				34  //数值转换成字符串

#define  FUNC_INCDATE			35  // 对日期进行加减
#define  FUNC_GETABSSEC			36  // 取绝对秒 (相对于1970.1.1)
#define  FUNC_CLKTOSEC			37  // 时钟格式换算为绝对秒
#define  FUNC_SECTOCLK			38  // 绝对秒换算为时钟格式

#define  FUNC_ISMAINSERVER		94
#define  FUNC_STARTPDR			95
#define  FUNC_ENCODEOBJTYPE		96
#define  FUNC_ENCODEOBJNAME		97
#define  FUNC_DECODEOBJNAME		98
#define  FUNC_ADDUSERENT		99

#define  FUNC_BASIS_END			100


/////////////
#define  FUNC_GETFIELD_NAME        101
#define  FUNC_GETFIELDSTR_NAME     102
#define  FUNC_PUTFIELD_NAME        103
#define  FUNC_PUTFIELDSTR_NAME     104


////////////
#define  FUNC_OPENMEMTAB           110
#define  FUNC_CLOSEMEMTAB          111
#define  FUNC_COUNTMEMRCD             112
#define  FUNC_BINDMEMFIELD            113
#define  FUNC_UPDMEMFIELD             114 


////////////
#define  FUNC_RSQL_SELFROM                120
#define  FUNC_RSQL_SELFIELD               121
#define  FUNC_RSQL_SELECT                 122
#define  FUNC_RSQL_UPDTAB                 123
#define  FUNC_RSQL_UPDFIELD               124
#define  FUNC_RSQL_UPDATE                 125
#define  FUNC_RSQL_WHERE                  126
#define  FUNC_RSQL_COUNTRCD               127
#define  FUNC_RSQL_BINDFIELD              128

#define  FUNC_GETAPEVENT             135
#define  FUNC_CHECKSG                136
#define  FUNC_CHECKOL                137
#define  FUNC_CHECKCHG               138
#define  FUNC_CHECKODD               139


#define  FUNC_YKPRESET                    150
#define  FUNC_WAITYKCHK                   151
#define  FUNC_YKTAKE                      152
#define  FUNC_YKCANCEL                    153
#define  FUNC_WAITYKRES                   154

#define  FUNC_CURTIMETYPE            160
#define  FUNC_TIMETYPE               161


#define  FUNC_READHISDATA                  170
#define  FUNC_BINDHISDATA				   171
#define  FUNC_BINDHISODATA				   172
#define  FUNC_UPDHISDATA                   173
#define  FUNC_DELHISDATA                   174
#define  FUNC_INSERTHISDATA                175
#define  FUNC_READHISMINDATA               176

#define  FUNC_HISTABLE_OPEN                 180
#define  FUNC_HISTABLE_CLOSE                181
#define  FUNC_HISTABLE_COUNT                182
#define  FUNC_HISTABLE_BIND                 183
#define  FUNC_HISTABLE_UPDATE				184
#define  FUNC_HISTABLE_DELETE				185
#define  FUNC_HISTABLE_INSERT				186

#define  FUNC_HISSQLSTATEMENT				190

#define  FUNC_FILEOP_FOPEN					200
#define  FUNC_FILEOP_FCLOSE					201
#define  FUNC_FILEOP_FPRINT					202
#define  FUNC_FILEOP_FGETWORD				203


//////////  对象类型定义  ///////////

#define  OBJECT_NULL             0
#define  OBJECT_RDBTABLE         1
#define  OBJECT_APPEVENT         2
   

/////////////////////
enum  DATATYPE   {nulltype=0,doubledata,stringdata,
				  obj_rsql=1000,
				  obj_appent,
				  obj_rdbtable,
				  obj_ykqueue,
				  obj_hisdata,
				  obj_histable,
				  obj_fileop,
				  obj_endno=3000};
enum  VARSTYLE   {INPUTVAR=0, INTERVAR};
enum  ADDRTYPE   {nulladdr=0,tempvar,tempstr,definevar,literal,tempoffaddr};

#define  DATATYPE_IS_OBJECT(a)   ((a)>=obj_rsql && (a)<obj_endno)

#define  MALLOC_DOUBLEDATA_LEN      sizeof(double)
#define  MALLOC_OBJECT_LEN			sizeof(double)
#define  MALLOC_STRINGDATA_LEN      MAXSTRINGLEN

/////////////////////

typedef struct {
    int   inparanums;
    unsigned  char inparatype[MAXINPARANUMS];
    } CALLPARA;

typedef  struct {
   short   instrucode; //指令代码
   int     addr1;
   int     addr2;
   int     addr3;
   } PCINSTRUMENT;



typedef struct {
	int            len;
	unsigned char  literaltype;
}  LITERAL_CTRL;


#define  UPOUT_FEXNAME      ".upo"
#define  UPSRC_FEXNAME		".ups"
#ifdef __unix
#define  UPOUT_PATHNAME     "procscript/procscript"
#define  UPSRC_PATHNAME		"procscript/usersrc"
#else
#define  UPOUT_PATHNAME     "procscript\\procscript"
#define  UPSRC_PATHNAME		"procscript\\usersrc"
#endif

#define  PC_PLATFORM_WIN32  0
#define  PC_PLATFORM_ALPHA  1
#define  PC_PLATFORM_SUN    2
#define  PC_PLATFORM_AIX    3
#define  PC_PLATFORM_HPUX   4
#define  PC_PLATFORM_LINUX   5

typedef struct {
   unsigned char platform; 
   unsigned char magicnum[3];
   char  procname[128];
   int   inparanums;
   unsigned  char inparatype[128];
   int   heaplen;
   int   codeoffset;
   int   codelen;
   int   literaloffset;
   int   literallen;
   int   exitprocinstruno;
   int   reserve[20];
   } PROC_HEAD;

#endif

