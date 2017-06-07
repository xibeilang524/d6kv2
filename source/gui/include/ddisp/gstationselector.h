#ifndef GSELECTORDS_H_HEADER_INCLUDED
#define GSELECTORDS_H_HEADER_INCLUDED


#ifdef WIN32
#ifdef PLASTATION_SELECTOR_DLL
#define	PLASTATION_SELECTOR_EXPORT __declspec(dllexport) 
#else
#define	PLASTATION_SELECTOR_EXPORT __declspec(dllimport) 
#endif
#else
#define	PLASTATION_SELECTOR_EXPORT
#endif

#include <QtCore/QString>

class PLASTATION_SELECTOR_EXPORT Gstation_selector
{
		
  public:
	Gstation_selector();
	~Gstation_selector();

public:
	QString get_code();
	QString get_desc();

private:
	QString station_code;
	QString station_desc;		
};

#endif
