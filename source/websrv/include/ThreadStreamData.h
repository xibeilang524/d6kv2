// ThreadStreamData.h: interface for the ThreadStreamData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREADSTREAMDATA_H__5776CA4D_97F4_4976_9843_652980078DB5__INCLUDED_)
#define AFX_THREADSTREAMDATA_H__5776CA4D_97F4_4976_9843_652980078DB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdb/rdbdef.h"//实时库的头定义

//线程流数据管理
class CThreadStreamData  
{
public:
	RDB_VALUETYPE_STRU * pvalue;//流数据的值
	int * pdatachangedf;//每个数据的改变标志
	int * pdatatype;//是否已经得到过该数据类型,为加快速度，以免每次都分析数据类型
	int bchangef;//是否有数据被刷新
	int datacount;//流数据的个数
	int * pold_order;//原始传送的数据序号
	CThreadStreamData(int datanums);
	virtual ~CThreadStreamData();

};

#endif // !defined(AFX_THREADSTREAMDATA_H__5776CA4D_97F4_4976_9843_652980078DB5__INCLUDED_)
