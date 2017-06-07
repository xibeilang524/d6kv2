/********************************************************************
file: 
    cpt.c
Notes:
    capture interface
History:
    Date         Who         RefDoc			Memo
    2012-11-06	 xh.wng    					moved	
********************************************************************/
#ifndef _DMCPT_DLL_H
#define _DMCPT_DLL_H

#include "DPI.h"

#define CPT_LANGUAGE_CN			    0	/**<p>中文</p>*/ 
#define CPT_LANGUAGE_EN				1	/**<p>英文</p>*/

#define DM_MAX_CPT_ERROR_INFO_LEN	512
typedef struct cpt_error_info_struct cpt_error_info_t;
struct cpt_error_info_struct
{
	sdint4		error_no;
	sdbyte		error_info[DM_MAX_CPT_ERROR_INFO_LEN];
};

typedef struct cpt_col_def_struct cpt_col_def_t;
struct cpt_col_def_struct 
{
    sdbyte			name[129];		//--字段名称，必须与捕获对象字段一致
	sdbyte			col_define[129];		//--字段类型，该类型只有在表达式不为空时指定才有效，在指定col_define时，要指定完整的定义，如int，varchar(200)，numeric(10,2)
	sdbyte			expr[1024];		//--字段表达式，如果为空串，则不做计算
};

#define CPT_WITH_API	1

/**
* <p>功能说明：数据捕获的创建接口</p>
* <p>算法描述：无</p>
* <p>参数列表：<br><ul>
* <li>con_hdbc, --输入，数据库的connection句柄</li>
* <li>schname,  --输入，捕获对象所属模式</li>
* <li>tvname,  --输入，捕获表/视图名</li>
* <li>wheres,  --输入，捕获条件，条件中需明确指定新值或旧值作为条件，如'OLD:ID < 10 AND NEW:ID > 10' 若没有条件则设为"";</li>
* <li>col_defs, --输入，字段描述</li>
* <li>n_cols,  --输入，捕获字段数</li>
* <li>cdc_id,  --输出，捕获请求标识，对应变化表名</li>
* <li>cdc_blob_id, --输入，标识缓冲区长度，建议需大于等于129 </li>
* <li>cdc_blob_id, --输出，捕获请求标识，对应变化大字段表表名</li>
* <li>cdc_blob_id_len,--输入，大字段标识缓冲区长度，建议需大于等于129   </li>
* <li>error_info, --输出，操作产生错误的原因</li>
* </ul></p>
* <p>返回值: udint4, 是否成功, {TRUE, FALSE}</p>
*/
DllExport
udint4
dm_create_change_data_capture(
#if CPT_WITH_API
	dhcon con_hdbc,
#else
	void *pidao_connection,               //--输入，IDAO_Connection接口指针
#endif   
   sdbyte*		schname,			
   sdbyte*		tvname,				
   sdbyte*		wheres,				
   cpt_col_def_t*	col_defs,	        
   udint2		n_cols,				
   sdbyte*		cdc_id,				
   udint4		cdc_id_len,		    
   sdbyte*		cdc_blob_id,		
   udint4		cdc_blob_id_len,	    
   cpt_error_info_t* error_info			
);

/**
* <p>功能说明：数据捕获的删除接口</p>
* <p>算法描述：无</p>
* <p>参数列表：<br><ul>
* <li>con_hdbc, --输入，数据库的connection句柄</li>
* <li>cdc_id,  --输入，捕获请求标识，对应变化表名</li>
* <li>error_info, --输出，操作产生错误的原因</li>
* </ul></p>
* <p>返回值: udint4, 是否成功, {TRUE, FALSE}</p>
*/
DllExport
udint4
dm_drop_change_data_capture(
#if CPT_WITH_API
	dhcon con_hdbc,
#else
	void *pidao_connection,               //--输入，IDAO_Connection接口指针
#endif
   sdbyte*		cdc_id,
   cpt_error_info_t* error_info
);

/**
* <p>功能说明：数据捕获的语言接口</p>
* <p>算法描述：无</p>
* <p>参数列表：<br><ul>
* <li>code_id, --输入，数据捕获模块编码类型(预留)</li>
* <li>lang_id, --输入，数据捕获模块语言类型</li>
* </ul></p>
* <p>返回值: dmbool, 是否成功, {TRUE, FALSE}</p>
*/
DllExport
void
dm_cpt_set_local_code(
    sdint4      code_id, /* 暂时无效 */
    sdint4      lang_id  
);

#endif
