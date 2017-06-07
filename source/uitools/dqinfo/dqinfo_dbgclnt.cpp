
#include "dqinfo_dbgclnt.h"


CDbgClnt::CDbgClnt()
{
    m_port = DBGPORT;
    m_netno = NET_NO_NULL;
    dialf = FALSE;
}

CDbgClnt::~CDbgClnt()
{
    Close();
}

int CDbgClnt::GetProcInfo(int host,REMOTE_DBG_PROC*  dbg_proc)
	{
    dialf = FALSE;
    Tcp   tmpTcp;
    tmpTcp.Create();

    m_host = host;
    NetStatus  cfg;

    struct in_addr addr;

    m_netno = cfg.GetHostNetNoAddr(host,NET_TYPE_DBG,&addr);

    if (m_netno==NET_NO_NULL)	return FALSE;

    if (tmpTcp.Connect(addr,m_port)==FALSE)
    {
        m_netno = NET_NO_NULL;
        return FALSE;
    }

    HEAD	txhead;

    txhead.src.user[0] = 0;
    txhead.cmdtype = DebugControlGram;
    txhead.userdata1 = DBG_GRAM_GET_PROCINFO;
    txhead.userdata2 = 0;
    txhead.len = 0;

    if (tmpTcp.Send(&txhead,sizeof(HEAD))!=TRUE) return FALSE;

    HEAD	rxhead;

    if (tmpTcp.Recv(&rxhead,sizeof(HEAD))!=TRUE) return FALSE;

    if (rxhead.cmdtype != DebugControlGram)
    {
        m_netno = NET_NO_NULL;
        return FALSE;
    }

    if (rxhead.userdata1 != DBG_GRAM_RET_PROCINFO)
    {
        m_netno = NET_NO_NULL;
        return FALSE;
    }

    dbg_proc->proc = (DbgProcInfo*)malloc(rxhead.len);

    char* buf = (char*)malloc(rxhead.len);
    char* tempbuf = buf;

    if (tmpTcp.Recv(buf,rxhead.len)==FALSE)
    {
        free(buf);
        return FALSE;
    }

    memcpy(&dbg_proc->proc_num,buf,sizeof(int16u));

    buf += sizeof(int16u);
    memcpy(dbg_proc->proc,buf,rxhead.len-sizeof(int16u));
    free(tempbuf);

    return TRUE;
}

int CDbgClnt::GetProcInfo(char* ip,REMOTE_DBG_PROC*  dbg_proc)
{
    if (ip==0)	return FALSE;

    if (strlen(ip)>39)	return FALSE;

    strcpy(dial_addr,ip);

    Tcp   tmpTcp;

    tmpTcp.Create();

//	NetStatus  cfg;

    struct in_addr addr;

    addr.s_addr= inet_addr(dial_addr);

    if (tmpTcp.Connect(addr,m_port)==FALSE)
    {
        m_netno = NET_NO_NULL;
        return FALSE;
    }

    HEAD	txhead;

    strcpy(txhead.src.user,"dqinfo");
    _get_host_name(txhead.src.host);
    txhead.cmdtype = DebugControlGram;
    txhead.userdata1 = DBG_GRAM_GET_PROCINFO;
    txhead.userdata2 = 0;
    txhead.len = 0;

    if (tmpTcp.Send(&txhead,sizeof(HEAD))!=TRUE)	return FALSE;

    HEAD	rxhead;

    if (tmpTcp.IsReadable(2,0)==FALSE)
    {
        m_netno = NET_NO_NULL;
        return FALSE;
    }

    int ret = tmpTcp.Recv(&rxhead,sizeof(HEAD));

    if (ret!=TRUE)	return FALSE;

    if (rxhead.cmdtype != DebugControlGram)
    {
        m_netno = NET_NO_NULL;
        return FALSE;
    }

    if (rxhead.userdata1 != DBG_GRAM_RET_PROCINFO)
    {
        m_netno = NET_NO_NULL;
        return FALSE;
    }

    dbg_proc->proc = (DbgProcInfo*)malloc(rxhead.len);

    char* buf = (char*)malloc(rxhead.len);
    char* tempbuf = buf;

    if (tmpTcp.Recv(buf,rxhead.len)==FALSE)
    {
        free(buf);
        return FALSE;
    }

    memcpy(&dbg_proc->proc_num,buf,sizeof(int16u));

    buf += sizeof(int16u);
    memcpy(dbg_proc->proc,buf,rxhead.len-sizeof(int16u));
    free(tempbuf);

    dialf = TRUE;

    return TRUE;
}

int CDbgClnt::SetDbgInfo(REMOTE_DBG_SEL* sel)
{
    if (tcp.GetSocket()<0)
    {
        Close();

        NetStatus  cfg;

        struct in_addr addr;

        if (dialf==FALSE)
        {
            m_netno = cfg.GetHostNetNoAddr(m_host,NET_TYPE_DBG,&addr);

            if (m_netno==NET_NO_NULL)	return FALSE;
        }
        else
        {
            if ((addr.s_addr= inet_addr(dial_addr))==INADDR_NONE)	return FALSE;
        }

        if (tcp.Connect(addr,m_port)==FALSE)
        {
            m_netno = NET_NO_NULL;
            return FALSE;
        }
    }

    int size = sizeof(int8u)+sizeof(sel->typenum)+sel->typenum*sizeof(int16s);

    HEAD	txhead;
    txhead.src.user[0] = 0;
    txhead.cmdtype = DebugControlGram;
    txhead.userdata1 = DBG_GRAM_CALL_INFO;
    txhead.userdata2 = 0;
    txhead.len = size;

    if (tcp.Send(&txhead,sizeof(HEAD))==FALSE)	return FALSE;

    char* buf = (char*)malloc(size);

    char* tempbuf = buf;

    memcpy(buf,&(sel->proc_idx),sizeof(int8u));

    buf += sizeof(int8u);

    memcpy(buf,&(sel->typenum),sizeof(int16s));

    buf += sizeof(int16s);

    int typenum = sel->typenum;

    memcpy(buf,sel->type,typenum*sizeof(int16s));

    if (tcp.Send(tempbuf,size)==FALSE)
    {
        free(tempbuf);
        return FALSE;
    }

    free(tempbuf);

    return TRUE;
}

int CDbgClnt::GetDbg(char *type, char *desc)
{
    if (tcp.IsReadable(1,0)==FALSE)					return FALSE;

    HEAD	rxhead;

    if (tcp.Recv(&rxhead,sizeof(HEAD))==FALSE)	return FALSE;

    if (rxhead.cmdtype!=DebugControlGram)		return FALSE;

    if (rxhead.userdata1!=DBG_GRAM_RET_INFO)		return FALSE;

//	if(rxhead.len!=sizeof(REMOTE_DBG_DATA))		return FALSE;

//	if(tcp.IsReadable()==FALSE)					return FALSE;

    MakeBuf  make;

    if (make.ResizeMem(rxhead.len)==0)			return FALSE;

    if (tcp.Recv(make.GetMemPtr(),rxhead.len)==FALSE)		return FALSE;

    int len = 0;

    void* p = make.GetBuf(0,&len);

    ZipData zip;

    REMOTE_DBG_DATA  data;

    if (zip.IsCompressData((char*)p,len)!=COMPRESSED)
    {
        memmove(&data,p,len);
    }
    else
    {
        char* outp = 0;
        int   outlen = 0;
        zip.ExpandData((char*)p,len,&outp,&outlen);
        memmove(&data,outp,outlen);
    }

    strcpy(type,data.data.type);

    strcpy(desc,data.data.desc);

    return TRUE;
}

void CDbgClnt::Close()
{
    tcp.Close();
}
