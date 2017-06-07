#ifndef _DFLDR_H
#define _DFLDR_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#define DllImport   __declspec( dllimport )
#define DllExport   __declspec( dllexport )
#else
#define DllImport
#define DllExport
#endif

typedef int         FLDR_RETURN;
typedef void*       fhinstance;
typedef void*       fpointer;
typedef signed char fchar;
typedef short       fsint2;
typedef int         fsint4;

#if defined(WIN32) || defined(WINDOWS)
typedef __int64    fsint8;
typedef unsigned __int64 fuint8;
#else
typedef long long int fsint8;
typedef unsigned long long int fuint8;
#endif //DWINDOWS

#define FLDR_SUCCESS            0
#define FLDR_SUCCESS_WITH_INFO  1
#define FLDR_ERROR              (-1)
#define FLDR_INVALID_HANDLE     (-2)
#define FLDR_NO_DATA            100


#define FLDR_SUCCEEDED(rt)    (((rt)&(~1))==0)

#define FLDR_UNINITILIAZE_COMMIT        (0)
#define FLDR_UNINITILIAZE_ROLLBACK      (1)


#define FLDR_TYPE_BIND                  (1)
#define FLDR_TYPE_TEXT_STREAM           (2)
#define FLDR_TYPE_CTRL                  (3)
#define FLDR_TYPE_JNI                   (4)
#define FLDR_TYPE_JNI_FILE              (5)
#define FLDR_TYPE_FODBC                 (6)
#define FLDR_TYPE_DEFAULT               (7)

#define FLDR_DATA_NULL                  (-1)
#define FLDR_DATA_NTS                   (-3)

//binding input c type
#define FLDR_C_BINARY       1
#define FLDR_C_CHAR         2
#define FLDR_C_TINYINT      3
#define FLDR_C_SHORT        4
#define FLDR_C_INT          5
#define FLDR_C_BIGINT       6
#define FLDR_C_FLOAT        7
#define FLDR_C_DOUBLE       8


#define     FLDR_ATTR_SERVER                1
#define     FLDR_ATTR_UID                   2       
#define     FLDR_ATTR_PWD                   3
#define     FLDR_ATTR_PORT                  4
#define     FLDR_ATTR_TABLE                 5
#define     FLDR_ATTR_SET_INDENTITY         6
//#define     FLDR_ATTR_GENERATE_LOG          7     
#define     FLDR_ATTR_DATA_SORTED           8   
#define     FLDR_ATTR_INDEX_OPTION          9      
// #define     FLDR_ATTR_DATEFORMAT            10   
#define     FLDR_ATTR_DATA_SIZE             11  
#define     FLDR_ATTR_INSERT_ROWS           12    
#define     FLDR_ATTR_COMMIT_ROWS           13  
#define     FLDR_ATTR_PROCESS_ROWS          14
#define     FLDR_ATTR_SEND_NODE_NUM         15       
#define     FLDR_ATTR_ERROR_ROWS            16
#define     FLDR_ATTR_BAD_FILE              17  
// #define     FLDR_ATTR_EXACT_ERROR_ROW       18         
#define     FLDR_ATTR_DATA_CHAR_SET         19       
// #define     FLDR_ATTR_FIELD_DELEMITER       20         
// #define     FLDR_ATTR_ROW_TERMINATOR        21        
#define     FLDR_ATTR_CTRL_FILE             22   
// #define     FLDR_ATTR_ASYN_COMMIT           23
#define     FLDR_ATTR_LOG_FILE              24
#define     FLDR_ATTR_SKIP_ROWS             25
#define     FLDR_ATTR_LOAD_ROWS             26
#define     FLDR_ATTR_READ_ROWS             27
#define     FLDR_ATTR_DIRECT_MODE           28
#define     FLDR_ATTR_ROWS_COMMIT           29
#define     FLDR_ATTR_ERRORS_PERMIT         30
#define     FLDR_ATTR_LOAD_MODE             31
#define     FLDR_ATTR_DISPLAY_LOG           32
#define     FLDR_ATTR_MPP_NODES             33
#define     FLDR_ATTR_MPP_CLIENT            34
#define     FLDR_ATTR_SSL_PATH              35
#define     FLDR_ATTR_LOB_DIRECTORY         36
#define     FLDR_ATTR_MPP_LOCAL_FLAG        37
#define     FLDR_ATTR_BUFFER_SIZE           38
#define     FLDR_ATTR_NULL_MODE             39
#define	    FLDR_ATTR_LOCAL_CODE			40
#define	    FLDR_ATTR_SSL_PWD   			41
#define     FLDR_ATTR_TASK_THREAD_NUM       42
//#define     FLDR_ATTR_BUFFER_NUM            43
// #define     FLDR_ATTR_SERVER_PARAL          44
// #define     FLDR_ATTR_SERVER_BUFFER_SIZE    45
// #define     FLDR_ATTR_SERVER_BUFFER_NUM     46
#define     FLDR_ATTR_SERVER_BLDR_NUM 	    47
#define     FLDR_ATTR_BDTA_SIZE             48
#define     FLDR_ATTR_COMMPRESS_FLAG        49
#define     FLDR_ATTR_BLOB_TYPE             50
#define     FLDR_ATTR_OCI_DIRECTORY         51

#define     FLDR_CTRL_ROW_ARRAY_SIZE        25

#define     FLDR_DEFAULT_ROW_NUM            5000

DllExport
FLDR_RETURN 
fldr_alloc(
    fhinstance*    instance
);

DllExport
FLDR_RETURN 
fldr_free(
    fhinstance	    instance
);

DllExport
FLDR_RETURN 
fldr_set_attr(
    fhinstance	    instance, 
    fsint4          attrid, 
    fpointer        value, 
    fsint4          length
);

DllExport
FLDR_RETURN 
fldr_get_attr(
    fhinstance	    instance, 
    fsint4          attrid, 
    fpointer        buffer, 
    fsint4          buf_sz,
    fsint4          *length
);

DllExport
FLDR_RETURN 
fldr_initialize(
    fhinstance	    instance, 
    fsint4          type,
    fpointer        conn,
    fchar*          server, 
    fchar*          uid, 
    fchar*          pwd,
    fchar*          table
);

DllExport
FLDR_RETURN 
fldr_uninitialize(
    fhinstance	    instance,
    fsint4          flag
);

DllExport
FLDR_RETURN 
fldr_bind(
    fhinstance	    instance,
    fsint2          col_idx,
    fsint2	        type,
    fchar*          date_fmt,
    fpointer        data,
    fsint4          data_len,
    fsint4*         ind_len
);

DllExport
FLDR_RETURN 
fldr_bind_nth(
    fhinstance	    instance,
    fsint2          col_idx,
    fsint2	        type,
    fchar*          date_fmt,
    fpointer        str_binds,
    fpointer        data,
    fsint2          col_len,
    fsint4          data_len,
    fsint4*         ind_len,
    fsint4          nth
);

DllExport
FLDR_RETURN 
fldr_sendrows(
    fhinstance	    instance,
    fsint4          rows
);

DllExport
FLDR_RETURN 
fldr_sendrows_nth(
    fhinstance	    instance,
    fsint4          rows,
    fsint4          nth,
    fsint4          seq_no
);

DllExport
FLDR_RETURN 
fldr_sendrows_nth_batch(
    fhinstance	    instance,
    fsint4          rows,
    fsint4          nth,
    fsint4          seq_no,
    void**          task_node_io,
    fsint4*         is_send
);

DllExport
FLDR_RETURN
fldr_sendrows_nth_2_over(
    fhinstance	    instance,
    fsint4          seq_no,
    void*           task_node
);

DllExport
FLDR_RETURN 
fldr_control(
    fhinstance	    instance,
    fsint4          attrid,
    fpointer        value, 
    fsint4          length
);

DllExport
FLDR_RETURN 
fldr_col_fmt(
    fhinstance	    instance,
    fsint2          col_idx,
    fpointer        delemiter,
    fsint2          delemiter_len,
    fpointer        encloser,
    fsint2          encloser_len,
    fpointer        line_ender,
    fsint2          line_ender_len
);

DllExport
FLDR_RETURN 
fldr_batch(
    fhinstance	    instance,
    fsint8          *rows
);

DllExport
FLDR_RETURN 
fldr_finish(
    fhinstance	    instance
);

DllExport
FLDR_RETURN 
fldr_get_diag(
    fhinstance	    instance,
    fsint4          rec_num,
    fsint4*         err_code,
    fchar*          err_msg,
    fsint4          buf_sz,
    fsint4*         msg_len
);

DllExport
FLDR_RETURN
fldr_check_cvt(
    fhinstance      inst
);

DllExport
FLDR_RETURN
fldr_exec_ctl(
    fhinstance      instance,
    fchar*          ctl_path
);

DllExport
FLDR_RETURN
fldr_exec_ctl_low(
    fhinstance      instance,
    fchar*          ctl_buffer,
    fsint4          fldr_type,
    fsint8*         row_count
);

DllExport
fsint8
fldr_get_inst_send_rows(
    fhinstance      instance
);

DllExport
FLDR_RETURN
fldr_jni_add_diag(
    fhinstance      instance,
    fsint4          err_code,
    fchar*          err_msg
);

#ifndef _CONSOLE
DllExport
int 
dmfldr2_dll_main(
				 fchar*      para
				 );
#endif

#ifdef __cplusplus
}
#endif



#endif //_DFLDR_H
