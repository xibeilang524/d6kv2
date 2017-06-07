#ifndef RWINI_H_
#define RWINI_H_
#pragma warning (disable:4786)
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <algorithm>   
#include <functional>  
#include <iterator>

#include "fepdllinc.h"

 
using namespace std;

#define SUCCESS	 1
#define FAIL	 0
#define MAXSTRINGLEN 256

typedef pair<string, string>	SECTIONKEY;


class PROTOCOL_EXPORT ini_RW
{
public:
	ini_RW(char *fileName);
	~ini_RW();
public:
	int get_bool(char *section, char *key, bool &value);
	int get_int(char *section, char *key, int &value);
	int get_string(char *section, char *key, char *&value);
	int get_string(string section, string key, string &value);
	int set_bool(char *section, char *key, bool value);
	int set_int(char *section, char *key, int value);
	int set_string(char*section, char *key, char *value);
	int set_string(string section, string key, string value);
	map<string, string> list_key(string section);
	map<SECTIONKEY, string> list_all(void);


private:
	int setKeyValue(const string section, const string key, const string value);
	string lTrim(string ss);
	string rTrim(string ss);
	string trim(string ss);

private:
	string	m_fileName;
	fstream m_file;
	bool	m_modifyFlag;
	char	m_strbuf[MAXSTRINGLEN];
	map<SECTIONKEY, string> m_configSet;

protected:
private:
};
#endif