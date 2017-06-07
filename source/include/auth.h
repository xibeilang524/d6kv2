#ifndef LIB_AUTU_H_
#define LIB_AUTU_H_

#if defined(WIN32)
#if defined(AUTH_EXPORTDLL)
#define	AUTH_IMEXPORT				__declspec(dllexport) 
#else
#define	AUTH_IMEXPORT				__declspec(dllimport) 
#endif
#else
#define	AUTH_IMEXPORT
#endif

const int HASH_STRING_LEN = 20;///<输出的md5密文的长度

/**
@brief 用户口令校验
@param in const char* user_name 用户名
@param in const char* input_pwd 输入的口令
@param in const char* db_pwd 数据库中读出的口令
@return 校验成功返回0，失败返回-1
*/
int AUTH_IMEXPORT user_auth(const char* user_name, const char* input_pwd, const char* db_pwd);

/**
@brief 把口令转换为经过MD5加密过的字串，输出缓冲区最少
@param in const char* input_pwd 输入的口令
@param out char* out_pwd 数据库中读出的口令
@param in const int& out_lenth 输出缓冲区的长度,不得小于HASH_STRING_LEN
@return 成功返回0，失败返回-1
*/
int AUTH_IMEXPORT convert_md5(const char* input_pwd, char* out_pwd, const int& out_lenth);

/**
@brief 验证口令是否满足复杂度要求
@param in const char* input_pwd 输入的口令
@return 成功返回true，失败返回false
*/
bool AUTH_IMEXPORT verify_password_auth(const char* input_pwd);

/**
@brief 读取函数调用错误的描述
@return 返回错误信息
*/
AUTH_IMEXPORT char* get_error_desc_auth();

/**
@brief 第三方校验

指纹、口令卡等，待扩展
@param in user_name 用户名
@return 成功返回0，失败返回-1
*/
int thirt_party_auth(const char* user_name);


#endif