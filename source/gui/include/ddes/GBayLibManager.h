#ifndef G_BAY_LIB_MANAGER_H_
#define G_BAY_LIB_MANAGER_H_
#include "ddef/ddes_comm.h"
#include "GGraphLayer.h"
#include "GLibManager.h"
class GFile;

class GDE_EXPORT_DES GBayLibManager : public GLibManager
{
public:
	//构造函数   
	GBayLibManager();
	//析构函数   
	~GBayLibManager();
	//重载保存函数
	virtual uint8 saveXML(const char* strFile);
	//重载读取函数
	virtual uint8 readXML(const char* strFile);
	//重载写入函数
	virtual int32 saveFile(GFile& fFile);
	//重载读取函数
	virtual int32 loadFile(GFile& fFile);
	//指定文件是否为图符文件
	static uint8 isGraphLib(const char* strPath);
};



#endif
