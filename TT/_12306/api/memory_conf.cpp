
#include "memory_conf.h"

CMemory_Conf* CMemory_Conf::instance_ = NULL;
CMemory_Conf* CMemory_Conf::instance()
{
	if (instance_ == NULL)
	{
		instance_ = new CMemory_Conf();
	}
	return instance_;
}

int CMemory_Conf::loadtomap(const string& str)
{
	size_t ulpos;
	size_t ulequ;
	string strkey;
	string strname;
	string strvalue;
	string strrow;	
	string strdata = str;
	bool bhaskey = false;
	CONF_NAME_VALUE ConfNVal;
	CONF_NAME_VALUE::const_iterator it = NULL;

	// find key	
	while (!strdata.empty())
	{
		ulpos = strdata.find_first_of('\n');
		strrow = strdata.substr(0, ulpos);
		if (ulpos == string::npos)
		{
			strdata = "";
		}
		else
		{
			strdata = strdata.substr(ulpos + 1);
			ulpos = strdata.find("\n");
		}
		
		strrow.erase(0, strrow.find_first_not_of("\r\t\n "));
		
		if (strrow[0] == '#' || strrow[0] == ';')
		{
			if (m_strTopNote.empty())
			{
				m_strTopNote = strrow;
			}
			continue;
		}		

		if (strrow[0] == '[')
		{
			if (ConfNVal.size() > 0 && !strkey.empty())
			{
				it = ConfNVal.begin();
				while (it != ConfNVal.end())
				{
					m_maplist[strkey][it->first] = it->second;
					++it;
				}
				ConfNVal.clear();
			}			
			strkey = strrow.substr(1, strrow.find_first_of(']') - 1);			
			continue;
		}

		ulequ = strrow.find_first_of('=');
		if (ulequ == string::npos)
		{
			continue;
		}

		//remove tail one of in "\r\n\t" and get name,value.
		strrow.erase(strrow.find_last_not_of("\r\n\t ") + 1);
		strname = strrow.substr(0, ulequ);
		strname.erase(strname.find_last_not_of("\t ") + 1);
		strvalue = strrow.substr(ulequ + 1);
		strvalue.erase(0, strvalue.find_first_not_of("\t "));

		ConfNVal[strname] = strvalue;		
	}

	if (ConfNVal.size() > 0 && !strkey.empty())
	{
		it = ConfNVal.begin();
		while (it != ConfNVal.end())
		{
			m_maplist[strkey][it->first] = it->second;
			++it;
		}
		ConfNVal.clear();
	}
	return 0;
}

string CMemory_Conf::getfrommap()
{
	string strtail = "\n";
	string strRet = m_strTopNote;	
	CONF_NAME_VALUE::const_iterator it = NULL;
	CONF_KEY_LIST::const_iterator lit = m_maplist.begin();
	
	while (lit != m_maplist.end())
	{
		strRet += strtail;
		strRet += "[";
		strRet += lit->first;
		strRet += "]";
		strRet += strtail;

		it = lit->second.begin();
		while (it != lit->second.end())
		{
			strRet += it->first;
			strRet += " = ";
			strRet += it->second;
			strRet += strtail;
		
			++it;
		}
		++lit;
	}

	return strRet;
}


int CMemory_Conf::load_conf(const char* pszfile)
{
	m_strfile = pszfile;
	m_maplist.clear();

	FILE* pfile = fopen(pszfile, "rb");
	if (pfile == NULL)
	{
		return -1;
	}
	string strfileconn;
	char szblock[MAX_LINE_LEN + 1] = {0};
	while (fread(szblock, 1, MAX_LINE_LEN, pfile) == MAX_LINE_LEN)
	{
		strfileconn += szblock;
		memset(szblock, 0x0, sizeof(szblock));
	}
	strfileconn += szblock;
	fclose(pfile);	

	return load_str(strfileconn.c_str());
}

int CMemory_Conf::load_str(const char* pszstr)
{
	return loadtomap(pszstr);
}


int CMemory_Conf::get_str(char* pszOutstr, int iOutlen)
{
	string str = getfrommap();
	if (str.empty())
	{
		return -1;
	}
	
	strncpy(pszOutstr, str.c_str(), iOutlen);
	return 0;
}


int CMemory_Conf::save_conf()
{
	string str = getfrommap();
	if (str.empty())
	{
		return -1;
	}
	
	FILE* pfile = fopen(m_strfile.c_str(), "wb");
	if (pfile == NULL)
	{
		return -2;
	}
	fwrite(str.c_str(), 1, str.size(), pfile);
	fclose(pfile);

	return 0;
}


int	CMemory_Conf::get_conf_str(const char* pszkey, const char* pszname, const char* pszdefval, char* pszvalue, int ivallen)
{
	memset(pszvalue, 0x0, ivallen);
	if (strcmp(pszkey, "") == 0 || strcmp(pszname, "") == 0)
	{
		strncpy(pszvalue, pszdefval, ivallen);
		return -1;
	}
	
	CONF_KEY_LIST::const_iterator lit = m_maplist.find(pszkey);
	if (lit == m_maplist.end())
	{
		strncpy(pszvalue, pszdefval, ivallen);		
		return -2;
	}

	CONF_NAME_VALUE::const_iterator it = lit->second.find(pszname);
	if (it == lit->second.end())
	{
		strncpy(pszvalue, pszdefval, ivallen);		
		return -3;
	}

	strncpy(pszvalue, m_maplist[pszkey][pszname].c_str(), ivallen);
	return 0;
}


int CMemory_Conf::get_conf_int(const char* pszkey, const char* pszname, int idefval)
{
	if (strcmp(pszkey, "") == 0 || strcmp(pszname, "") == 0)
	{
		return idefval;
	}
	
	CONF_KEY_LIST::const_iterator lit = m_maplist.find(pszkey);
	if (lit == m_maplist.end())
	{
		return idefval;
	}

	CONF_NAME_VALUE::const_iterator it = lit->second.find(pszname);
	if (it == lit->second.end())
	{			
		return idefval;
	}
	return atoi(m_maplist[pszkey][pszname].c_str());
}

int CMemory_Conf::write_conf_str(const char* pszkey, const char* pszname, const char* pszvalue)
{
	if (strcmp(pszkey, "") == 0 || strcmp(pszname, "") == 0)
	{
		return -1;
	}

	m_maplist[pszkey][pszname] = pszvalue;
	
	return 0;
}

int CMemory_Conf::write_conf_int(const char* pszkey, const char* pszname, int ivalue)
{
	char szvalue[12] = {0};
	sprintf(szvalue, "%d", ivalue);
	return write_conf_str(pszkey, pszname, szvalue);
}

int CMemory_Conf::del_conf_str(const char* pszkey)
{
	CONF_KEY_LIST::const_iterator lit = m_maplist.find(pszkey);
	if (lit == m_maplist.end())
	{		
		return -1;
	}

	m_maplist.erase(pszkey);
	return 0;
}

int CMemory_Conf::del_conf_str(const char* pszkey, const char* pszname)
{
	CONF_KEY_LIST::const_iterator lit = m_maplist.find(pszkey);
	if (lit == m_maplist.end())
	{		
		return -1;
	}
	
	CONF_NAME_VALUE::const_iterator it = lit->second.find(pszname);
	if (it == lit->second.end())
	{			
		return -2;
	}

	m_maplist[pszkey].erase(pszname);
	return 0;
}

void CMemory_Conf::test()
{
	load_conf("test.ini");
	char szTest[100] = {0};
	get_conf_str("Main", "test", "", szTest, 100);
	save_conf();
}



