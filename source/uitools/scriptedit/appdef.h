#ifndef APPDEF_H
#define APPDEF_H

#define C2G(str) QString::fromLocal8Bit(str)
#define G2C(qs)  (const char*)(qs).toLocal8Bit()

#define Rdb_Server			"SCADA"
#define Rdb_Name			"用户过程参数表"
#define Rdb_UserName        "scada"
#define Rdb_Password        "scada"

#define DefaultGroupNo      1


#define XML_INI_FILE_NAME   C2G("resource/xml/scriptedit.xml")
#define CUSTOM_CLASS_NAME   C2G("用户自定义函数")

#define DOM_CLASS_NODE_TAG				C2G("function_class")
#define DOM_CLASS_CLASS_ATTR			C2G("functionclass")

#define DOM_FUNCTION_NODE_TAG           C2G("func_instruction")
#define DOM_FUNCTION_NODE_DESC_ATTR     C2G("desc")
#define DOM_FUNCTION_NODE_DEF_ATTR		C2G("funcdef")
#define DOM_FUNCTION_NODE_HELP_ATTR		C2G("funchelp")

#define HANDPROC_INTERVAL               2
#define HANDPROC_REMOVEDELAY            3


//#define HANDEXECPROC
#endif