#ifndef _TOPOAPI_H_
#define _TOPOAPI_H_

#ifdef WIN32
#if defined(_DLLOUT)
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#endif
#else
#define DLLEXPORT
#endif

#include <QString>
#include <QMap>


class  DLLEXPORT TopoApi
{
public:
	int  GetPowerPathFromDev(const char *devName, char ***devList);
	int  GetLoadPathFromDev(const char *devName, int devType,char ***devList);
	int  GetLoadPathFromBefDev(const char *devName, int devType,char ***devList,
		QMap<QString, int> &devActiveMap,
		QMap<QString, int> &devStatusMap );
};

#endif