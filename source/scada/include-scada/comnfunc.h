/********************************************************************
*																	*
*					公用库函数	gepeng 2002									*
*																	*
*	01. 冒泡排序(CommSort).											*
*	02. 快速排序(CommQSort).										*
*	03. 二分法查找(CommBinFind).									*
*	04. 注册位于DS3100\DLL\目录下的控件(CommRegisterOcx)			*
*	05. 产生LRC校验码(纵向偶校验)(ComLRC)							*
*																	*
********************************************************************/
#if !defined(_025_3343030_COMMFUNC)
#define _025_3343030_COMMFUNC

#if !defined(COMFACE_DLL)
#define _COMFACE_PORT _declspec(dllimport)
#else
#define _COMFACE_PORT _declspec(dllexport)
#endif


/////////////////////////////////////////////////////////////////////
//冒泡排序，将无序数组变成有序数组。
//pObj数组首地址, objNum数组大小
//调用此函数须重载操作符 "<", ">"
template <class T>
inline void CommSort(T * pObj, int objNum)
{
	int i,j,k,m;
	T obj;
	k = 0; m = objNum-1;
	if (pObj==NULL || objNum <= 0)
		return;
	while(k<m)
	{
		//从前到后扫描序列，比较相邻项目的大小
		//发现逆序则交换，使最大者换到序列的最后
		j = m-1; m = 0;
		for(i=k;i<=j;i++)
		{
			if(pObj[i] > pObj[i+1])
			{	
				obj = pObj[i];
				pObj[i] = pObj[i+1];
				pObj[i+1] = obj;
				m=i;
			}
		}
		//从后到前扫描剩下的序列，比较相邻项目的大小
		//发现逆序则交换，使最小者换到序列的最前
		j=k+1;k=0;
		for(i=m; i>= j; i--)
		{
			if(pObj[i-1] > pObj[i])
			{
				obj = pObj[i];
				pObj[i] = pObj[i-1];
				pObj[i-1] = obj;
				k=i;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////
//将无序数组变成有序数组，相对于前面冒泡排序速度要快。适合比较大的数组
//pObj数组首地址, objNum数组大小
//调用此函数给结构数组排序, 须重载操作符 ">", ">=", "<", "<="
template <class T>
inline void CommQSort(T * pObj, int objNum)
{
	int i0, *i, m;
	T *s;
	i=&i0;
	if(objNum>10)//数组大于10，分割后再排序
	{
		CommSplit(pObj, objNum, i);//分割函数
		m=i0;
		CommQSort(pObj, m);
		s = pObj + (i0 + 1);
		m = objNum - (i0 + 1);
		CommQSort(s, m);
	}
	else		//否则直接用冒泡法排序
		CommSort(pObj, objNum);
}

template <class T>
inline void CommSplit(T * p, int n, int *m)
{
	int i, j, k, l;
	T t;
	i = 0; j = n-1;
	k = (i+j)/2;
	if ((p[i] >= p[j]) && (p[j] >= p[k])) l = j;
	else if ((p[i] >= p[k]) && (p[k] >= p[j])) l = k;
	else l = i;
	t = p[l]; p[l] = p[i];
	while(i!=j)
	{
		while((i<j)&&(p[j]>=t))j=j-1;
		if(i<j)
		{
			p[i]=p[j];
			i=i+1;
			while((i<j)&&(p[i]<=t)) i=i+1;
			if(i<j)
			{
				p[j]=p[i];
				j=j-1;
			}
		}
	}
	p[i] = t;
	*m = i;
}

///////////////////////////////////////////////////////////
//二分法查指定对象在数组中的位置
//obj指定对象, pObj对象数组, objNum数组大小
//返回值 < 0 对象未找到, 否则返回对象在数组中的位置
template <class T>
inline int CommBinFind(T obj, T * pObj, int objNum)
{
	int	wObjNum = objNum, wTempNum = 0,
		wMatchNo = -1, wObjNo = 0;
	if (pObj == NULL || objNum < 1) 
		return -1;
	do
	{
		wTempNum = wObjNum;

		int wTempNo = wObjNo + wObjNum/2;
		T tmpObj = pObj[wTempNo];
		if (tmpObj == obj)
		{
			wMatchNo = wTempNo;
		}
		else if (tmpObj > obj)
		{
			wTempNum = wTempNo - wObjNo;
		}
		else if (obj < tmpObj)
		{
			wTempNum = wObjNum - (wTempNo - wObjNo);
			wObjNo = wTempNo;
		}

		if (wTempNum == wObjNum) break;
		wObjNum = wTempNum;
	}while (wObjNum > 0);

	return wMatchNo;
}

/////////////////////////////////////////////////////////////////////
//注册位于DS3100\DLL\目录下的控件
//参数：OcxName 控件名
//返回值：< 0 不成功 否则成功 
_COMFACE_PORT int CommRegisterOcx(char *OcxName);

////////////////////////////////////////////////////////////////////
// 产生LRC校验码(纵向偶校验)		
// BufLen:报文大小
// Buf:报文内容
// 返回值:一字节校验码
_COMFACE_PORT BYTE CommLRC(int BufLen, BYTE * Buf);

#endif