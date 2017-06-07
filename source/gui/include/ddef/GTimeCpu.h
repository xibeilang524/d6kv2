#ifndef GTIMECPU_H_HEADER_INCLUDED_BCAB1FEE
#define GTIMECPU_H_HEADER_INCLUDED_BCAB1FEE
#include "ddes_comm.h"
#include <time.h>

//##ModelId=433FC90C00BB
//##Documentation
//## 定义：
//##   运行效率测试类
//## 功能：  
//##   提供运行实效测试功能
//## 设计：
//##   宋永生
//## 日期：
//##   2005-10-2
class GDE_EXPORT_DEF GTimeCpu
{
public:
	//##ModelId=433FCF3F03B9
    //##Documentation
    //## 功能：
    //##      构造函数
    //## 参数：
    //##      uint8 autostart  自动开始计时
	//##	  uint8 outresult  主动输出结果
    //## 返回：
    //##      无
    //## 设计：
    //##       宋永生
    //## 日期：
    //##    2005-10-2
	GTimeCpu(uint8 autostart=0,uint8 outresult=0);
	
	//##ModelId=433FD1E402EE
    //##Documentation
    //## 功能：
    //##    析构函数
    //## 参数：
    //##    无
    //## 返回：
    //##    无
    //## 设计：
    //##    宋永生
    //## 日期：
    //##    2005-10-2
	~GTimeCpu();

	//##ModelId=433FD1E402EE
    //##Documentation
    //## 功能：
    //##    取得CPU时钟间隔
    //## 参数：
    //##    无
    //## 返回：
    //##    clock_t
    //## 设计：
    //##    宋永生
    //## 日期：
    //##    2005-10-2
	clock_t cpuTick() const;

	//##ModelId=433FD1E402EE
    //##Documentation
    //## 功能：
    //##    取得CPU时间（秒）
    //## 参数：
    //##    无
    //## 返回：
    //##    FLOAT8
    //## 设计：
    //##    宋永生
    //## 日期：
    //##    2005-10-2
	FLOAT8 cpuTime() const;

	//##ModelId=433FD1E402EE
    //##Documentation
    //## 功能：
    //##    启动CPU计时
    //## 参数：
    //##    无
    //## 返回：
    //##    clock_t
    //## 设计：
    //##    宋永生
    //## 日期：
    //##    2005-10-2
	clock_t start();

	//##ModelId=433FD1E402EE
    //##Documentation
    //## 功能：
    //##    终止CPU计时
    //## 参数：
    //##    无
    //## 返回：
    //##    clock_t
    //## 设计：
    //##    宋永生
    //## 日期：
    //##    2005-10-2
	clock_t stop();

	//##ModelId=433FD1E402EE
    //##Documentation
    //## 功能：
    //##    重置CPU计时
    //## 参数：
    //##    无
    //## 返回：
    //##    无
    //## 设计：
    //##    宋永生
    //## 日期：
    //##    2005-10-2
	void	reset();

	//##ModelId=433FD1E402EE
    //##Documentation
    //## 功能：
    //##    允许输出调试结果
    //## 参数：
    //##    uint8 enable
    //## 返回：
    //##    无
    //## 设计：
    //##    宋永生
    //## 日期：
    //##    2005-10-2
	void	outEnable(uint8 enable=1);

protected:

	//clock start rec
	clock_t	m_cs;

	//clock end rec
	clock_t m_ce;

	//clock freq
	clock_t m_cf;

	//auto start
	uint8   m_as;

	//out result
	uint8	m_or;

};

#endif
