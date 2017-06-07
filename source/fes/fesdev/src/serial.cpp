#include "fes/fesdev.h"

Serial::Serial(const char* dev1, const char* dev2, const int& band_rate, 
			   const int& check_bit, const int& stop_bit, 
			   const int& data_bit, const int& flow)
{
	addr_index = 0;
	valid_addr_num = 0;

	if (dev1)
	{
		dev_addr[valid_addr_num++].set(dev1);
	}

	if (dev2)
	{
		dev_addr[valid_addr_num++].set(dev2);
	}

	param.baudrate = 115200;//band_rate;
	param.stopbits = stop_bit;
	param.databits = data_bit;
	switch (check_bit)
	{
	case 0:
			  param.paritymode = "NONE";
              break;
	case 1:
			 param.paritymode = "EVEN";
		     break;
	case 2:
			 param.paritymode = "ODD";
		     break;
#ifdef WIN32
	case 3:
			 param.paritymode = "MARK";
		     break;
	case 4:
			 param.paritymode = "SPACE";
		     break;
#endif
	default:
		     param.paritymode = "EVEN"; 
			 break;

	}
	if (flow == 0)
	{
		param.xinenb = false;
		param.xoutenb = false;

	}
	else
	{
		param.xinenb = true;
		param.xoutenb = true;
	}

	param.xonlim = 0;
	param.xofflim = 0;
	param.readmincharacters = 0;
	param.readtimeoutmsec = READ_TIME_OUT;		//串口读超时时间10S
	param.ctsenb = false;
	param.rtsenb = 0;
	param.modem = false;
	param.rcvenb = true;
	param.dsrenb = false;
	param.dtrdisable = false;
}

int Serial::open_dev(void)
{
	if (connector.connect(dev, dev_addr[addr_index]) == -1)
	{
		ACE_ERROR_RETURN ((LM_ERROR, "串口打开错误：%s", get_error()), -1);
	}

	if (dev.control(ACE_TTY_IO::SETPARAMS, &param) == -1)
	{
		ACE_ERROR_RETURN ((LM_ERROR, "设置串口参数错误：%s", get_error()), -1);
	}

	return 0;
}

void Serial::close_dev(void)
{
	int ret = -1;
	int retry = CLOSE_RETRY;

	while (retry > 0)
	{
		if (-1 != dev.close())
		{
			ACE_DEBUG((LM_INFO, "关闭串口成功！\n"));
			ret = 0;
			break;
		}
		retry--;
	}

	if (0 != ret)
	{
		ACE_DEBUG((LM_INFO,"关闭串口错误：", get_error()));
	}
}

int Serial::write_dev(unsigned char* buf, int len)
{
	int send_count = dev.send_n(buf, len);
	int ret = 0;

	if (send_count != len)
	{
		ACE_ERROR((LM_ERROR, "发送%d字节数据到串口失败：%s\n", get_error()));
		ret = -1;
	}
	else
		ret = send_count;

	return ret;
}

int Serial::read_dev(unsigned char* buf, int len)
{

	char DataBuf[256];

	int nLen=0;

	int recv_count = 0;//= dev.recv(buf, len);

	for (int nIndex=0;nIndex<5;++nIndex)
	{
		recv_count=dev.recv(DataBuf+nLen,256);

		nLen+=recv_count;

		if (recv_count==0  && len )
		{
			#define min(a,b)   (((a) < (b)) ? (a) : (b))
			memcpy(buf,DataBuf,min(256,nLen));
			len=nLen;
			return len;
		}
		ACE_OS::sleep( ACE_Time_Value( 0 ,1000*10 ) ) ;
	}

	int ret = 0;

	if (recv_count == -1 || recv_count < 0)
	{
		ACE_ERROR((LM_ERROR, "读取串口失败：%s\n", get_error()));
	}
	else
		ret = recv_count;

	return ret;
}

int Serial::get_stat(void)
{
	return dev.get_handle() >= 0 ? 0 : -1;
}