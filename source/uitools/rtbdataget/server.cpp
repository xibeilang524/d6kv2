#include "soapH.h"
#include "stdsoap2.h"
#include "SOAP.nsmap"

#include "net/netapi.h"
#include "pthread.h"

#include "MyDataSql.h"
#include <sstream>

#if defined (WIN32)
#include <process.h>  
#include <Windows.h>
#endif

#define BACKLOG (100)	// Max. request backlog 
#define MAX_THR (10)	// Max. threads to serve requests 

using namespace std;
//所有new的地方需要改成使用soap_new__，如soap_new___ns1__MeasurementList 否则会造成内存泄露

static ProcMan proc_manager;
int bProcQuit = FALSE;

#if defined (__unix)
void *Checkproc (void*arg)
#elif defined (WIN32)
UINT  Checkproc (void *arg)
#endif
{
	while(true)
	{
		{
			if (proc_manager.CheckParentProc() == FALSE)
			{
				fprintf(stderr, "ParentProc FALSE\n"); 
				bProcQuit = TRUE;
				break;
			}
		}
		_delay_time(1000*1000);
	}
	return NULL;
}

int main(int argc, char **argv) 
{
// 	if ( argc != 1 ) 
// 	{
// 		printf("Usage: %s port/n", argv[0]);
// 		exit(-1);
// 	}

	if (proc_manager.IsProcExist("rtbdataget"))
	{
		exit(0);
	}

	int port = 7790;

	struct soap soap;
	soap_init(&soap);
// 	soap_set_mode(&soap,SOAP_C_MBSTRING);
//  	soap.mode |= SOAP_C_UTFSTRING;
	proc_manager.StartProc("rtbdataget", "实时库数据读取服务进程","");
	proc_manager.RegProc("实时库数据读取服务进程");

#if defined (__unix)
	for(int aa=1;aa<33;aa++)
	{
		if(aa!=SIGUSR1)
			sigset(aa,SIG_IGN);	
	}

	{
		if(setsid()==(pid_t)-1){
			if (setpgrp()==-1)
				printf("\n can't change process group\n");
			else
				printf("\n change process group ok\n");
		}
		else
			printf("\n change process group ok\n");
	}

	pthread_t		idthread;
	if(pthread_create(&idthread,NULL,Checkproc,0)<0) exit(1); 
#elif defined (WIN32)
	 _beginthreadex( NULL, 0, Checkproc, NULL , 0, NULL );
#endif

	soap_omode(&soap, SOAP_XML_DEFAULTNS);

	soap.send_timeout = 60; // 60 seconds 
	soap.recv_timeout = 60; // 60 seconds 

	struct soap *soap_thr[MAX_THR]; // each thread needs a runtime context 
	pthread_t tid[MAX_THR]; 
	SOAP_SOCKET m, s; 
	int i; 
	m = soap_bind(&soap, NULL, port, BACKLOG); 
	if (!soap_valid_socket(m)) 
		exit(1); 
	fprintf(stderr, "Socket connection successful %d\n", m); 
	for (i = 0; i < MAX_THR; i++) 
		soap_thr[i] = NULL; 
	while (bProcQuit == FALSE) 
	{
		for (i = 0; i < MAX_THR; i++) 
		{ 
			s = soap_accept(&soap); 
			if (!soap_valid_socket(s)) 
			{ 
				if (soap.errnum) 
				{ 
					soap_print_fault(&soap, stderr); 
					continue; // retry
				} 
				else
				{ 
					fprintf(stderr, "Server timed out\n"); 
					break; 
				} 
			} 
			fprintf(stderr, "Thread %d accepts socket %d connection from IP %d.%d.%d.%d\n", i, s, (soap.ip >> 24)&0xFF, (soap.ip >> 16)&0xFF, (soap.ip >> 8)&0xFF, soap.ip&0xFF); 
			if (!soap_thr[i]) // first time around 
			{ 
				soap_thr[i] = soap_copy(&soap); 
				if (!soap_thr[i]) 
					exit(1); // could not allocate 
			} 
			else// recycle soap context 
			{ 
				pthread_join(tid[i], NULL); 
				fprintf(stderr, "Thread %d completed\n", i); 
				soap_destroy(soap_thr[i]); // deallocate C++ data of old thread 
				soap_end(soap_thr[i]); // deallocate data of old thread 
			} 
			soap_thr[i]->socket = s; // new socket fd 
			pthread_create(&tid[i], NULL, (void*(*)(void*))soap_serve, (void*)soap_thr[i]); 
		} 
	} 
	for (i = 0; i < MAX_THR; i++) {
		if (soap_thr[i]) 
		{ 
			soap_done(soap_thr[i]); // detach context 
			free(soap_thr[i]); // free up 
		} 
	}

	proc_manager.UnRegProc();
	proc_manager.EndProc();
	return 0; 

	/*int m, s;
	if ((m = soap_bind(&soap, NULL, port, 100)) < 0) 
	{
		soap_print_fault(&soap, stderr);
	}
	else 
	{
		printf("Socket connect successfully: master socket = %d\n", m);
		int i = 0;
		while ( 1 ) 
		{
			if ( (s = soap_accept(&soap)) < 0 ) 
			{
				soap_print_fault(&soap, stderr);
				soap_end(&soap);
				continue;
			}

			printf("Connection %d accepted from IP = %d.%d.%d.%d, slave socket = %d\n", ++i, (soap.ip >> 24) & 0xff, (soap.ip >> 16) & 0xff, (soap.ip >> 8) & 0xff, soap.ip & 0xff, s);
			if ( soap_serve(&soap) != SOAP_OK ) 
			{
				soap_print_fault(&soap, stderr);
				soap_end(&soap);
				continue;
			}
			soap_end(&soap);
		}
	}
	soap_done(&soap);
	proc_manager.UnRegProc();
	proc_manager.EndProc();
	return 0;*/
}

void *process_request(void *soap) 
{ 
	pthread_detach(pthread_self()); 
	soap_serve((struct soap*)soap); 
	soap_destroy((struct soap*)soap); // dealloc C++ data 
	soap_end((struct soap*)soap); // dealloc data and clean up 
	soap_done((struct soap*)soap); // detach soap struct 
	free(soap); 
	return NULL; 
}

#if defined (__unix)	
int __ns1__Request(struct soap* sop, ns2__RequestMessageType *req, ns2__ResponseMessageType *rep)
#else
int __cdecl __ns1__Request(struct soap* sop, ns2__RequestMessageType *req, ns2__ResponseMessageType *rep)
#endif // unix
{
	if (!req->Request->ID || req->Request->ID->empty())
	{
		return SOAP_ERR;
	}

	if (_ns2__HeaderType_Verb__get != req->Header->Verb)
	{
		return SOAP_ERR;
	}

	ID_STR* id = req->Request->ID;		//获取的数据描述 实时表名+代码+字段名 设备的SCD代码 阵列的站所线系代码
// 	for (int i = 0; i < id.size(); i++)
// 	{
// 		printf("request ID %d = %s\n", i, id[i].c_str());
// 	}
	std::string request_type = req->Header->Noun;





	//填充rep
	///////////////////////////////////Responses header填充///////////////////////////////////////
	rep->Header = soap_new_ns2__HeaderType(sop,1);
// 	rep->Header = new ns2__HeaderType;
	rep->Header->Verb = _ns2__HeaderType_Verb__reply;
 	rep->Header->Noun = request_type;
// 	///////////////////////////////////Responses reply填充///////////////////////////////////////
	rep->Reply = soap_new_ns2__ReplyType(sop);
// 	rep->Reply = new ns2__ReplyType;
// 		//返回结果  OK  PARTIAL FAILED?待判断

// 
// 
// 	///////////////////////////////////Responses Payload填充///////////////////////////////////////
	rep->Payload = soap_new_ns2__PayloadType(sop);
	rep->Payload = new ns2__PayloadType;
	rep->Payload->Format = new std::string("getdata");

	//根据需求取数据
	if (request_type == "ArrayValues")
	{
		rep->Payload->__union_PayloadType = SOAP_UNION__ns2__union_PayloadType_ArrayValues;
		rep->Payload->union_PayloadType.ArrayValues = new std::vector<class ns3__ArrayValuesType * >;
		ns3__ArrayValuesType* ptr = new ns3__ArrayValuesType;

	}
	else if (request_type == "DeviceValues")
	{
		rep->Payload->__union_PayloadType = SOAP_UNION__ns2__union_PayloadType_DeviceValues;
		rep->Payload->union_PayloadType.DeviceValues = new std::vector<class ns3__DeviceValuesType * >;
		ns3__DeviceValuesType* ptr = new ns3__DeviceValuesType;

	}
	else if (request_type == "CommonValues" || request_type == "SimpleValues")
	{
		rep->Payload->__union_PayloadType = SOAP_UNION__ns2__union_PayloadType_CommonValues;
		rep->Payload->union_PayloadType.CommonValues = new std::vector<class ns3__CommonValuesType * >;
		ns3__CommonValuesType* ptr = new ns3__CommonValuesType;

	}

		int get_result = 0;
		RdbValueGet_B get_rdbdata(id->size());
	//组织多帧报文
// 	printf("request data_type %s request data: %s\n", request_type.c_str(), id->at(i).__item.c_str());

		get_result = get_rdbdata.Get_Data_From_Database(id,rep->Payload, request_type);

		if (get_result==0)
		{
			rep->Reply->Result = _ns2__ReplyType_Result__FAILED;
		}
		else if (get_result == 3*id->size())
		{
			rep->Reply->Result = _ns2__ReplyType_Result__OK;
		}
		else
		{
			rep->Reply->Result = _ns2__ReplyType_Result__PARTIAL;
		}

	
	return SOAP_OK;

}


#if defined (__unix)
int __ns1__Response(struct soap*, ns2__ResponseMessageType *ns2__ResponseMessage, struct __ns1__ResponseResponse &_param_1)
#else
int __cdecl __ns1__Response(struct soap*, ns2__ResponseMessageType *ns2__ResponseMessage, struct __ns1__ResponseResponse &_param_1)
#endif
{
	return SOAP_OK;

}
#if defined (__unix)
int __ns1__PublishEvent(struct soap *s,class ns2__EventMessageType *req,class ns2__ResponseMessageType *rep)
#else
int __cdecl __ns1__PublishEvent(struct soap *s,class ns2__EventMessageType *req,class ns2__ResponseMessageType *rep)
#endif
{
	return SOAP_OK;
}


