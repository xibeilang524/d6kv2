#ifdef WIN32
	#define DllImport   __declspec( dllimport )
	#define DllExport   __declspec( dllexport )

	#ifdef DLL_GRP2SVG
	#define EXPORT DllExport
	#else
	#define EXPORT DllImport
	#endif
#else
	#define EXPORT 
#endif

bool EXPORT txtGrp2Svg(char * txt_file, char * svg_file);