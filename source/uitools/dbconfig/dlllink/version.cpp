#include "verapi.h"

#if defined(WIN32)
extern "C" __declspec(dllexport) void getVersion()
#else
extern "C"  void getVersion()
#endif
{
	writever("dblink,1.11");
}
