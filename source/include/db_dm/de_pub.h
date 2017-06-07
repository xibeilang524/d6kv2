#ifndef DE_PUB_H
#define DE_PUB_H

#ifdef __cplusplus
extern "C" {
#endif

typedef   signed short     sdint2;
typedef unsigned short     udint2;

typedef   signed int       sdint4;                   
typedef unsigned int       udint4; 

#ifdef WIN32
typedef __int64            sdint8;
typedef unsigned __int64   udint8;
#else
typedef long long int          sdeint8;
typedef unsigned long long int udeint8;
#endif

typedef signed char         sdbyte;
typedef unsigned char       udbyte;

typedef double              ddouble;

#define NSTR_LOCAL_LEN  48

typedef struct de_str_struct de_str;
struct de_str_struct
{
    udint4            len;    /*字符串长度*/
    udbyte*           data;   /*字符串指针*/
    udbyte            local_space[NSTR_LOCAL_LEN];  /*local_space存储的字符串以'\0'结束，实际只能存储47个字符*/
};


typedef struct de_data_struct de_data;
struct de_data_struct
{
	udint4 null_flag; /*参数是否为空，1表示非空，0表示空*/
	union	
    {
		sdint4   v_int;
		ddouble  v_double;
		de_str   v_str;
	}data;
};


typedef struct de_args_struct de_args;
struct de_args_struct
{
	udint4        n_args;      /*参数个数*/
    de_data*      args;        /*参数列表*/
};

/**
 * <p>功能说明：返回第arg_id参数的为整型值</p>
 * <p>算法描述：无</p>
 * <p>参数列表：<br><ul>
   <li> args：参数列表信息，不可能为空</li>
   <li> arg_id：取出指定id的参数值，起始值为0，不可能为空</li>
* </ul></p>
 * <p>返回值 ：第arg_id参数的整型值</p>
 */
sdint4 
de_get_int(
    de_args* args, 
    udint4   arg_id
);

/**
 * <p>功能说明：返回第arg_id参数的为double类型值</p>
 * <p>算法描述：无</p>
 * <p>参数列表：<br><ul>
   <li> args：参数列表信息，不可能为空</li>
   <li> arg_id：取出指定id的参数值，起始值为0，不可能为空</li>
* </ul></p>
 * <p>返回值 ：第arg_id参数的double类型值</p>
 */
ddouble 
de_get_double(
    de_args*  args, 
    udint4    arg_id
);

/**
 * <p>功能说明：返回第arg_id参数的的字符串类型</p>
 * <p>算法描述：无</p>
 * <p>参数列表：<br><ul>
   <li> args：参数列表信息，不可能为空</li>
   <li> arg_id：取出指定id的参数值，起始值为0，不可能为空</li>
* </ul></p>
 * <p>返回值 ：第arg_id参数的字符串，返回结果以“\0”结尾</p>
 */
udbyte* 
de_get_str(
    de_args* args, 
    udint4   arg_id
);

/**
 * <p>功能说明：返回第arg_id参数的字符串</p>
 * <p>算法描述：无</p>
 * <p>参数列表：<br><ul>
   <li> args：参数列表信息，不可能为空</li>
   <li> arg_id：取出指定id的参数值，起始值为0，不可能为空</li>
	<li> alen：返回字符串的长度</li>
* </ul></p>
 * <p>返回值 ：第arg_id参数的字符串，返回结果不以“\0”结尾</p>
 */
udbyte* 
de_get_str_with_len(
    de_args*    args, 
    udint4      arg_id, 
    udint4*     alen
);

/**
 * <p>功能说明：设置第arg_id参数的整型值</p>
 * <p>算法描述：无</p>
 * <p>参数列表：<br><ul>
   <li> args：参数列表信息，不可能为空</li>
   <li> arg_id：设置指定id的参数值，起始值为0，不可能为空</li>
	<li> ret：第arg_id参数新的整型值 </li>
* </ul></p>
 * <p>返回值 ：无</p>
 */
void 
de_set_int(
    de_args*    args, 
    udint4      arg_id, 
    sdint4      ret
);

/**
 * <p>功能说明：设置第arg_id参数的double类型值</p>
 * <p>算法描述：无</p>
 * <p>参数列表：<br><ul>
   <li> args：参数列表信息，不可能为空</li>
   <li> arg_id：设置指定id的参数值，起始值为0，不可能为空</li>
	<li> ret：第arg_id参数新的double类型值 </li>
* </ul></p>
 * <p>返回值 ：无</p>
 */
void 
de_set_double(
    de_args* args, 
    udint4   arg_id, 
    ddouble  ret
);

/**
 * <p>功能说明：设置第arg_id参数的字符串 </p>
 * <p>算法描述：无</p>
 * <p>参数列表：<br><ul>
   <li> args：参数列表信息，不可能为空</li>
   <li> arg_id：设置指定id的参数值，起始值为0，不可能为空</li>
	<li> ret：第arg_id参数新的字符串，此字符串以“\0”结尾</li>
* </ul></p>
 * <p>返回值 ：无</p>
 */
void de_set_str(
    de_args*    args, 
    udint4      arg_id, 
    udbyte*     ret
);

/**
 * <p>功能说明：设置第arg_id参数的字符串 </p>
 * <p>算法描述：无</p>
 * <p>参数列表：<br><ul>
   <li> args：参数列表信息，不可能为空</li>
   <li> arg_id：设置指定id的参数值，起始值为0，不可能为空</li>
<li> alen：新字符串的长度</li>
	<li> ret：第arg_id参数新的字符串，此字符串不要求以“\0”结尾</li>
* </ul></p>
 * <p>返回值 ：无</p>
 */
void 
de_set_str_with_len(
    de_args*    args, 
    udint4      arg_id, 
    udbyte*     ret, 
    udint4      alen
);

/**
 * <p>功能说明：返回第arg_id参数的整型值</p>
 * <p>算法描述：无</p>
 * <p>参数列表：<br><ul>
   <li> ret：需要返回的整型值</li>
* </ul></p>
 * <p>返回值 ：整型</p>
 */
de_data 
de_return_int(
    sdint4 ret
);

/**
 * <p>功能说明：返回第arg_id参数的double类型值</p>
 * <p>算法描述：无</p>
 * <p>参数列表：<br><ul>
   <li> ret：需要返回的double类型值</li>
* </ul></p>
 * <p>返回值 ：double类型</p>
 */
de_data 
de_return_double(
    ddouble ret
);

/**
 * <p>功能说明：返回第arg_id参数的字符串</p>
 * <p>算法描述：无</p>
 * <p>参数列表：<br><ul>
   <li> ret：需要返回的字符串，该字符串以“\0”结尾</li>
* </ul></p>
 * <p>返回值 ：字符串 </p>
 */
de_data
de_return_str(
    udbyte* ret
);

/**
 * <p>功能说明：返回第arg_id参数的字符串</p>
 * <p>算法描述：无</p>
 * <p>参数列表：<br><ul>
   <li> ret：需要返回的字符串，该字符串不要求以“\0”结尾</li>
  <li> alen：字符串长度</li>
* </ul></p>
 * <p>返回值 ：字符串 </p>
 */
de_data 
de_return_str_with_len(
    udbyte*    ret, 
    udint4     alen
);

/**
 * <p>功能说明：释放空间</p>
 * <p>算法描述：无</p>
 * <p>参数列表：<br><ul>
   <li> str：指向需要释放字符空间的指针</li>
* </ul></p>
 * <p>返回值 ：无 </p>
 */
void 
de_str_free(
    sdbyte* str
);

/**
 * <p>功能说明：判断参数是否为空</p>
 * <p>算法描述：无</p>
 * <p>参数列表：<br><ul>
      <li> args：参数列表信息，不可能为空</li>
      <li> arg_id：设置指定id的参数值，起始值为0，不可能为空</li>
* </ul></p>
 * <p>返回值 ：布尔类型 </p>
 */
udint4 
de_is_null(
    de_args*  args, 
    udint4    arg_id
);

/**
 * <p>功能说明：设置参数为空</p>
 * <p>算法描述：无</p>
 * <p>参数列表：<br><ul>
      <li> args：参数列表信息，不可能为空</li>
      <li> arg_id：设置指定id的参数值，起始值为0，不可能为空</li>
* </ul></p>
 * <p>返回值 ：无 </p>
 */
void  
de_set_null(
    de_args*    args, 
    udint4      arg_id
);

/**
 * <p>功能说明：返回空值</p>
 * <p>算法描述：无</p>
 * <p>参数列表：无<br><ul>
* </ul></p>
 * <p>返回值 ：无 </p>
 */
de_data 
de_return_null();

#ifdef __cplusplus
}
#endif

#endif

