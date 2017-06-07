#ifndef LIC_GENR_DEFINE_H
#define LIC_GENR_DEFINE_H

#include <QString>
#include <QTextStream>

#include "AES.h"

#define  LICENSE_FILE_NAME      "license"

// 项目名称


// 授权周期key
#define  AUTHORIZATION_PERIOD           "Authorization period"
// 文件生成时间key
#define  FILE_GENERATION_TIME           "File generation time"
// 系统运行时间key
#define  SYSTEM_RUNNING_TIME            "System running time"
// 本地初次加载时间key
#define  LOCAL_INITIAL_LOADING_TIME     "Local initial loading time"
// 本地当前日期key
#define  LOCAL_CURRENT_DATE             "Local current date"
// Mac地址key
#define  MAC_ADDRESS                    "Mac address"
// 告警级别
#define  WARNING_LEVEL                  "Warning level"

// 初级告警
#define  PRIMARY_ALARM                  "Primary alarm"
// 次级告警
#define  SECONDARY_ALARM                "Secondary alarm"
// 高级告警
#define  ADVANCED_ALARM                 "Advanced alarm"

// 授权模块
// to do:以后新增

// 1天分钟数
#define ONE_DAY     24 * 60

// 无限授权标记
#define UNLIMITED_FLAG       0xFF

// 最大key长度
#define  KEY_MAX_LEN                    32
// 最大value长度
#define  VALUE_MAX_LEN                  32
// 文件key值最大16进制长度
#define  HEX_KEY_MAX_LEN                100
// 文件value值最大16进制长度
#define  HEX_VALUE_MAX_LEN              HEX_KEY_MAX_LEN

#define  THREE_DAY				"Three day"
#define  ONE_MONTH			"One month"
#define  THREE_MONTH		"Three month"
#define  HALF_YEAR				"Half year"
#define  ONE_YEAR				"One year"
#define  TWO_YEAR				"Two year"
#define  UNLIMITED				"Unlimited"

// 无符号char转换成16进制字符串
LICENSELIB_EXPORT void unsigned_char_to_hexString(unsigned char* source, char* dest, unsigned int destLen);

// 16进制转unsigned char*
LICENSELIB_EXPORT void HexStrToByte(const char* source, unsigned char* dest, int sourceLen);

// 查找mac地址
LICENSELIB_EXPORT QString SearchMacAddr();

// license文件key查找
LICENSELIB_EXPORT char* FindKey(const char* src);

// license文件value查找
LICENSELIB_EXPORT char* FindValue(const char* src);

// 判断是否纯数字
LICENSELIB_EXPORT bool isDigitStr(const char* src);

// 20xx-xx-xx xx:xx:xx 标准时间格式检查
LICENSELIB_EXPORT bool TimeCheck(const char* src);

// 文件导入配置检查
LICENSELIB_EXPORT bool FileImportCheck(QTextStream& stream, AES& aes);

// 授权接口文件配置检查
LICENSELIB_EXPORT bool FileCheckForAuthInterface(QTextStream& stream, AES& aes);

// 时间格式（20xx-xx-xx xx:xx:xx）与当前系统时间差值计算 
LICENSELIB_EXPORT int TimeFormatCalc(QString& time);

#endif
