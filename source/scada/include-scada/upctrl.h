#ifndef _USERPROC_CTRL_H

#define _USERPROC_CTRL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "uplang.h"

#define  uchar   unsigned char

#define  USERPROCOK             0
#define  USERPROCERR             -1

#define  SYM_EOF           127

#define  WT_DIGIT           1
#define  WT_NAME            2
#define  WT_DBFIELD         3
#define  WT_STRING          4
//#define  WT_XXXXXXXX            5
#define  WT_LABEL           6
#define  WT_LBRACKET        7    //    (   
#define  WT_RBRACKET        8    //    )   
#define  WT_LLBRACKET       9    //    {   
#define  WT_RLBRACKET       10   //    }   
#define  WT_COMMA           11   //    ,   
#define  WT_SEMIC           12   //    ;   
#define  WT_NOT             13    //    !   
#define  WT_MOD             14    //    %   
#define  WT_LSHIFT          15    //    <<   
#define  WT_RSHIFT          16   //    >>   
#define  WT_ADD             17   //    +   
#define  WT_SUB             18   //    -   
#define  WT_MUL             19   //    *   
#define  WT_DIV             20    //    /   
#define  WT_MULMUL          21    //    **   
#define  WT_LARGE           22    //    >   
#define  WT_SMALL           23   //    <   
#define  WT_LARGEEQU        24    //    >=   
#define  WT_SMALLEQU        25    //    <=   
#define  WT_NOTEQU          26    //    !=   
#define  WT_EQU             27    //    =   
#define  WT_EQUEQU          28    //    ==   
#define  WT_AND             29    //    &&   
#define  WT_BITAND          30    //    &   
#define  WT_OR              31    //    ||   
#define  WT_BITOR           32    //    |   
#define  WT_XOR             33    //    ^   
#define  WT_BITNOT          34    //    ~   
#define  WT_LMBRACKET       35    //    [
#define  WT_RMBRACKET       36    //    ]
#define  WT_DOT             37    //    .

#define  WT_IF              60   //    IF   
#define  WT_ELSE            61   //    ELSE   
#define  WT_DATAVAR         62   //    DATAVAR   
#define  WT_STRVAR          63   //    STRING  
#define  WT_GOTO            64   //    GOTO   
#define  WT_PROC            65   //    PROC   
#define  WT_RETURN          66   //    RETURN   
#define  WT_FOR             67   //    FOR
#define  WT_BREAK           68   //    break   
#define  WT_CONTINUE        69   //    CONTINUE  

#define  WT_EOF             255


#define  WT_OBJECT_START    ((int)obj_rsql)
#define  WT_OBJECT_END      ((int)obj_endno)
#define  IS_OBJECT_KEY(a)    ((a)>=WT_OBJECT_START && (a)<WT_OBJECT_END)


////////////////////////////////////
#define  WTNAME_OTHER        0             
#define  WTNAME_VAR          1
#define  WTNAME_VARARRAY     2
#define  WTNAME_OBJECT       3
#define  WTNAME_OBJARRAY     4



//////////////////////////////////
#define  USERPROCERR_NULL             0
#define  USERPROCERR_WORD             1
#define  USERPROCERR_SYNX             2
#define  USERPROCERR_OPERATION        3
#define  USERPROCERR_MALLOC           4

/////////////////////////////////

#define  MAXLINELEN       2000


typedef struct {
    char  keyname[30];
    int   wordtype;
  } USERPROCKEYSTRU;
  


enum PARACHKTYPE  {complete,nocheck,morepara};

typedef struct {
    char          funcname[MAXFUNCNAME_LEN];
	int           objtype;
    int           funcno;
	DATATYPE	  resulttype;
    PARACHKTYPE   parachktype;
    CALLPARA      callpara;
    }  FUNCTIONTAB;


#define   VARNAMELEN           30

typedef struct varname_table {
     char          varname[VARNAMELEN];
     VARSTYLE       vartype;
     DATATYPE      datatype;
     int           addr;
	 int           nums;
     struct varname_table *next;
     } VARNAMETAB;

typedef struct literal_table {
     DATATYPE  literaltype;
     union {
        double literaldouble;
        char   literalstr[MAXSTRINGLEN];
        } literal;
     int   addr;
     int   len;
     struct literal_table *next;
     } LITERALTAB;


typedef struct inpara_table {
     int       pointaddr;
     int       valueaddr;
     DATATYPE  datatype;
     } INPARATAB;

                       
#define MAXPREDICTWORDNUMS            3


typedef struct {
   DATATYPE  datatype;
   ADDRTYPE  addrtype;
   int       addr;
   } SYNXRECORD;
   
#define MAXSYNXRECORD    300
#define TEMPDOUBLENUMS   200
#define TEMPSTRINGNUMS   40

class UserProc {
  FILE   *srcfp;
  FILE   *outfp;
  PROC_HEAD  prochead;
  char   srcfilename[256];
  char   outfilename[256];
  int    lineno;
  int    instruno;
  int    errornums;
  int    expoint;
  int    wordpoint;
  int          dataunitp;
  int          literalp;
  int          tempdoublenums;
  int          tempdoubleoff;
  int          tempstrnums;
  int          tempstringoff;
  int          heaplen;
  int          codeoffset;
  int          literaloffset;
  VARNAMETAB  *vartabroot;
  LITERALTAB *literaltabroot;
  int            inparanums;
  struct {
     int          stackp;
     INPARATAB   inpararcd[100];
     } inparastack;


   int  userprocerror(int  errtype,char *context);

  struct {
     int          stackp;
     ADDRTYPE  addrtype[100];
     int       paraaddr[100];
     } funcparastack;

  struct {
    int    putpoint;
    int    getpoint;
    int    predictflag[MAXPREDICTWORDNUMS];
    int    predicttype[MAXPREDICTWORDNUMS];
    char   predictword[MAXPREDICTWORDNUMS][MAXWORDLEN];
    }    predict;
   
  int    tokentype;
  char   takeword[MAXWORDLEN];
  char   linestr[MAXLINELEN];
  struct {
     int          stackp;
     SYNXRECORD   synxrecord[MAXSYNXRECORD];
    } synxstack;
  uchar  tempdoubleflag[TEMPDOUBLENUMS];
  uchar  tempstringflag[TEMPSTRINGNUMS];
  uchar  errorflag;

   int   opensrcfile(char *srcfname);
   int   openoutfile(char *outfname);
   int   writeoutbuff(void *buff,int offset,int len);
   int   readoutbuff(void *buff,int offset,int len);
  void   read_newline(void);
  char   get_nextsymbol(void);
  void     restore_symbol(void);
  void     skip_space(void);
  void     skip_spaceline(void);
  void     skip_comment(void);
  int      scandigit(void);
   int   scanstring(void);
   int   searchkeyname(char *name);
   int    scanname_raw(char *wordbuf);
   int   scanname(void);
   int   get_wordinline(void);
  void   get_nextword(void);
  void   restore_word(void);
   int   match(int  matchtype);
   int   skip_match(int  matchtype,...);
   int	 locate_literaladdr(int instrunums,int literal_startaddr);
   int   get_replaceaddr(int addr,int *realaddr);
   int   locate_replaceaddr(int instrunums);
   int   proc(char *tmpoutfname);
   int	 read_procinpara(void);
   int   procbody(void);
   int   statementlist(void);
   int   statement(void);
   int   simplestatement(void);
   int   assignstatement(void);
   int   expression(void);
   int	 equtal(void);
   int   relation(void);
   int   bitlevel(void);
   int   simplexp(void);
   int   term(void);
   int   factor(void);
   int   primary(void);
   int   funccall(char *funcname,DATATYPE datatype,int objaddr);
   int   proc_array(char *namestr);
   int   varlist(DATATYPE  datatype);
   int	 dbfield_assignstatement(char  *wordbuf);
   int  pushparaop(int optype);
   int  callfunop(int optype);
   int  search_vartab(char *varname,int *varaddr,VARSTYLE *vartype,DATATYPE *datatype,int *nums=NULL,int nodisperr=0);
   int  malloc_vartab(char *varname,VARSTYLE vartype,DATATYPE  datatype,int nums);
   int  free_object_in_vartab(void);
   int  free_vartab(void);
   int  search_literaltab(DATATYPE literaltype,char *literalstr,int *literaladdr);
   int  free_literaltab(void);
   int  writeout_literaltab(int offset);
   int  getcontant_literaltab(int literaladdr,double *value);
   int  init_tempdouble(void);
   int  malloc_tempdouble(void);
   int  free_tempdouble(int  tmpdoubleaddr);
   int  init_tempstring(void);
   int  malloc_tempstring(void);
   int  free_tempstring(int  tmpstraddr);
   int  free_temp_result(void);
   int  free_temp_result(ADDRTYPE freeaddrtype,int freetmpaddr);
   int  push_inparastack(char *inparaname,DATATYPE  datatype);
   int  pop_inparastack(int *pointaddr,int  *valueaddr,DATATYPE  *datatype);
   int  restore_inparastack(int nums);
   int  push_funcparastack(ADDRTYPE  addrtype,int paraaddr);
   int  pop_funcparastack(ADDRTYPE  *addrtype,int *paraaddr);
   int  push_synxstack(DATATYPE datatype,ADDRTYPE addrtype,int addr);
   int  pop_synxstack(DATATYPE *datatype,ADDRTYPE *addrtype,int *addr);
   int  writeinstrument(int  instrucode,int addr1,int addr2,int addr3);
   int  writeinstrument(int inno,int  instrucode,int addr1,int addr2,int addr3);
   int  write_nullinstrument(void);
   int  get_curinstruno(void);
   int  readinstrument(int inno,int  *instrucode,int *addr1,int *addr2,int *addr3);
   int  split_dbfieldname(char *dbfieldname,char *tabname,char *pointname,char *fieldname);
   int  encode_outpara(void);
   int  encode_return(void);
   int  encode_break(void);
   int  encode_continue(void);
   int  encode_inpara(void);
   int  encode_procover(void);
   int  encode_exitproc(void);
   DATATYPE  encode_pushpara(void);
   int  encode_callfunc(DATATYPE  objtype,int objaddr,int callfuncno,CALLPARA  *callparap,DATATYPE resulttype);
   int  encode_ifelse(int resultaddr,int jmpzerono,int jmpno,int elseno,int endifno);
   int  encode_forstatement(int resultaddr,int expstno,int expjmpzerono,int expjmpno,
            int incstno,int incjmpno,int loopstno,int loopjmpno,int forendno);
   int  encode_putfieldvalue(char *dbfieldname,DATATYPE putdatatye);
   int  encode_getfieldvalue(char *dbfieldname,DATATYPE getdatatye);
   int	encode_equtalop(void);
   int  encode_logicalop(int optype);
   int  encode_relationop(int optype);
   int  encode_bitop(int optype);
   int  encode_signop(int optype);
   int  encode_addingop(int optype);
   int  encode_multiplyop(int optype);
   int  encode_mulmulop(int optype);
   int  encode_assignop(void);
   int  encode_array_off(DATATYPE  vardatatype,int varaddr,int nums);
   int  search_functab(char *funcname,DATATYPE datatype,CALLPARA *callpara,DATATYPE *resulttypep);
   int  encode_mallocobject(DATATYPE objtype,int objaddr,int nums);
   int  encode_freeobject(DATATYPE objtype,int objaddr,int nums);
   int  encode_get_arrayvalue(DATATYPE	*datatype,ADDRTYPE	*addrtype,int  *addr);
  public:
   UserProc(void);
   ~UserProc(void);
   int   userprocfile(char *srcfname,int localflag=0);
   int   dispobjfile(char *objfname);
  };

  
  
#endif
  
