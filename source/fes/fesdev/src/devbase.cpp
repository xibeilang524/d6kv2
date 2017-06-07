#include "fes/fesdev.h"

Dev_base::~Dev_base()
{

}

char* Dev_base::get_error(void)
{
	return ACE_OS::strerror(errno);
}
