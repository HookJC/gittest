

#ifndef __MEMORY_CONF_H__
#define __MEMORY_CONF_H__

#pragma warning(disable:4786)
#pragma warning(disable:4503)

#include <map>
#include <string>

using std::map;
using std::string;

#define MAX_LINE_LEN 1024

typedef map<string, string> CONF_NAME_VALUE;
typedef map<string, CONF_NAME_VALUE> CONF_KEY_LIST;

class CMemory_Conf
{
public:
	int load_conf(const char* pszfile);
	int save_conf();
	
	int load_str(const char* pszstr);
	int get_str(char* pszOutstr, int iOutlen);

	int	get_conf_str(const char* pszkey, const char* pszname, const char* pszdefval, char* pszvalue, int ivallen);
	int get_conf_int(const char* pszkey, const char* pszname, int idefval);

	int write_conf_str(const char* pszkey, const char* pszname, const char* pszvalue);
	int write_conf_int(const char* pszkey, const char* pszname, int ivalue);

	int del_conf_str(const char* pszkey);
	int del_conf_str(const char* pszkey, const char* pszname);

	static CMemory_Conf* instance();
	void test();

private:
	int loadtomap(const string& str);
	string getfrommap();
	
private:
	static CMemory_Conf* instance_;

	string m_strfile;
	string m_strTopNote;
	CONF_KEY_LIST m_maplist;
	
};


#endif

