#ifndef _COMN_STACK_H
#define _COMN_STACK_H

template <class TYPE>
class  ComnStack {
	int maxunitnums;
	int stockpoint;
	TYPE  *stackdata;
public:
	ComnStack<TYPE>(int unitnums) {
		stackdata=new TYPE[unitnums];
		maxunitnums=unitnums;
		stockpoint=0;
	}
	~ComnStack<TYPE>(void) {
		if(stackdata)delete stackdata;
	}
	int Push(TYPE *data) {
		if(stockpoint<maxunitnums) {
			stackdata[stockpoint]=*data;
			stockpoint++;
			return(1);
		}
		else return(0);
	}
	int Pop(TYPE *data) {
		if(stockpoint>0) {
			if(data)*data=stackdata[stockpoint-1];
			stockpoint--;
			return(1);
		}
		else return(0);
	}
	int Check(TYPE *data) {
		if(stockpoint>0) {
			*data=stackdata[stockpoint-1];
			return(1);
		}
		else return(0);
	}
};


#endif

