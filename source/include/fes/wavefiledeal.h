#ifndef WAVEFILEDEAL_H
#define WAVEFILEDEAL_H

class QString;
class Wave_Protocol;

#ifdef WIN32
#ifdef _GDEDLL_FEPWAVE
#define	GDE_EXPORT_FEPWAVE __declspec(dllexport) 
#else
#define	GDE_EXPORT_FEPWAVE __declspec(dllimport) 
#endif
#else
#define	GDE_EXPORT_FEPWAVE
#endif

/************************************************************************/
/* 录波文件处理类                                                                     */
/************************************************************************/

class GDE_EXPORT_FEPWAVE WaveFileDeal
{
public:
	WaveFileDeal(char* filename);
	~WaveFileDeal();

	int DealInputFile(QString filename);
	char* Get_Save_Name();

public:
	Wave_Protocol *m_protocol;
// 	P1801dy *m_p1801dy;
};

#endif

