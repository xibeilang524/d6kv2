#ifndef FILE_INTERFACE_H
#define FILE_INTERFACE_H

#include <QFile>
#include <QTextStream>
#include <QMutex>

#include "license/AES.h"

#define  NEW_LINE    "\r\n"

typedef struct
{
	QString value;
	qint64 seek;
}VALUE_SEEK;

// QT 文件操作封装
class LICENSELIB_EXPORT CFile
{
public:

	CFile(const char* fileName);
	~CFile();

	// 打开文件
	bool OpenFile(QIODevice::OpenMode ioFlags);

	// 关闭文件
	void CloseFile();

	bool isExist();

	// 读取文件
	QString ReadFile();

	// 从指定的seek位置追加数据
	void InsertData(QString data, qint64 seek);

	// 从指定的seek位置追加加密后的数据
	int InsertData(QString strKey, QString data, AES& aes);

	// 从指定的行追加数据
	void InsertData(void* data, int row);

	// 从文件尾部追加数据
	void AppendData(QString data, bool isNewLine = true);

	// 修改数据
	void ModifyData(QString data, qint64 seek);

	// 删除指定行数
	void DeleteFileData(int row);

	// 删除指定位置、指定大小的数据
	void DeleteFileData(long long seek, long long size);

	// 根据key，查找value
	VALUE_SEEK* SearchValueFromKey(QString key, AES& aes);

	// 设置当前seek位置
	void SetCurrentSeek(qint64 pos);

	// 获取当前seek位置
	qint64 GetCurrentSeek() const;

	// seek复位
	void RecoverySeek();

	// 文件大小
	qint64 FileSize();

	// 刷新
	void Flush();

private:

	// 文件名
	const char* m_fileName;

	// QT 文件操作对象
	QFile* m_pFile;

	// 锁
	QMutex m_mutex;

public:

	// 位置
	qint64 m_seek;

	QTextStream* m_pStream;

};

#endif
