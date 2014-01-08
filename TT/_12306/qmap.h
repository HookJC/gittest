
#ifndef __Q_MAP_H__
#define __Q_MAP_H__

#include <map>
#include <string>

using std::map;
using std::string;

typedef map<string, string> QStrMap;

class CQMap
{
public:
	CQMap()
	{
		m_map.clear();
	};
	
	~CQMap()
	{
		m_map.clear();
	};
	
public:
	int parse(const string& strsrc)
	{
		int icount = 0;
		size_t ulpos = 0;
		string strkey;
		string strval;
		string strmap;
		string strdata = strsrc;
		
		m_map.clear();
		
		while (!strdata.empty())
		{
			if ((ulpos = strdata.find("&")) == string::npos)
			{
				strmap = strdata;
				strdata = "";
			}
			else
			{
				strmap = strdata.substr(0, ulpos);
				strdata = strdata.substr(ulpos + 1);
			}
			
			// get the key and value
			ulpos = strmap.find("=");
			if (ulpos == string::npos)
			{
				continue;
			}
			strkey = strmap.substr(0, ulpos);
			strval = strmap.substr(ulpos + 1);
			if (strkey.empty() || strval.empty())
			{
				continue;
			}
			m_map[strkey] = strval;
		}
		return icount;
	};
	
	string& operator[](const string& strkey)
	{
		string strval;
		if (getval(strkey, strval) == 0)
		{
			return m_map[strkey];
		}
		m_map[strkey] = "";
		return m_map[strkey];		
	}; 
	
	void cmd(const string& strcmd)
	{
		m_map["cmd"] = strcmd;
	};
	
	void cmd(int icmd)
	{
		char szval[20] = {
			0
		};
		itoa(icmd, szval, 10);
		m_map["cmd"] = szval;
	}; 
	
	int cmd()
	{
		QStrMap::const_iterator it = m_map.find("cmd");
		if (it == m_map.end())
		{
			return 0;
		}
		return atoi(it->second.c_str());
	};
	
	int append(const string& strkey, const string& strval)
	{
		QStrMap::const_iterator it = m_map.find(strkey);
		if (it != m_map.end())
		{
			return -1;
		}		
		m_map[strkey] = strval;
		return 0;
	};	
	
	int append(const string& strkey, int ival)
	{
		QStrMap::const_iterator it = m_map.find(strkey);
		if (it != m_map.end())
		{
			return -1;
		}
		char szval[20] = {
			0
		};
		itoa(ival, szval, 10);	
		m_map[strkey] = szval;
		return 0;
	};
	
	void set(const string& strkey, const string& strval)
	{
		m_map[strkey] = strval;
	};
	
	void set(const string& strkey, int ival)
	{
		char szval[20] = {
			0
		};
		itoa(ival, szval, 10);	
		m_map[strkey] = szval;
	};

	string getstr(const string& imkeyval, const string& imvalkey)
	{
		string strret;
		QStrMap::const_iterator it = m_map.begin();
		while (it != m_map.end())
		{
			if (it->first != "cmd")
			{
				strret += imvalkey;
				strret += it->first;
				strret += imkeyval;
				strret += it->second;
			}			
			++it;
		}
		strret = strret.substr(imvalkey.size());
		return strret;
	}

	void add(const QStrMap& mapnew)
	{
		QStrMap::const_iterator it = mapnew.begin();
		while (it != mapnew.end())
		{			
			m_map[it->first] = it->second;
			++it;
		}
	}

	void clear()
	{
		m_map.clear();
	}
	
private:
	int getval(const string& strkey, string& strval)
	{
		QStrMap::const_iterator it = m_map.find(strkey);
		if (it == m_map.end())
		{
			return -1;
		}
		strval = it->second;
		return 0;
	}
	
private:
	QStrMap m_map;
	
};

#endif
