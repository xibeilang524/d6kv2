#include "fes/fesdev.h"

Net_dev::Net_dev(const char* dev1, const int& port1, const char* dev2, const int& port2)
{
	valid_addr_num = 0;
	unsigned long ip_addr;
	if (dev1 == NULL)
	{
		addr[valid_addr_num++].set(port1);	
	}
	else if (dev1 && (ip_addr = ACE_OS::inet_addr(dev1)) != 
#ifdef WIN32
		INADDR_NONE
#else 
		((in_addr_t) - 1)
#endif
		)
	{
		addr[valid_addr_num++].set(port1, dev1, 1, AF_INET);
	}


	if (dev2 == NULL)
	{
		addr[valid_addr_num++].set(port2);
	}

	else if (dev2 && (ip_addr = ACE_OS::inet_addr(dev2)) != 
#ifdef WIN32
		INADDR_NONE
#else 
		((in_addr_t) - 1)
#endif
		)
	{

		addr[valid_addr_num++].set(port2, dev2, 1, AF_INET);
	}

	addr_index = valid_addr_num;


}

Net_dev::~Net_dev()
{

}