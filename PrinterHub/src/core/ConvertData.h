#pragma once
#include <string>
#include <atlstr.h>
class ConvertData
{
public:	ConvertData();
	  ~ConvertData();
	  static std::string CStringToString(const CString& cstr);
	  static int CStringToInt(const CString& cstr);

	  static CString StringToCString(const std::string& str);
	  int GetRandomInt(int min, int);
};

