#include "pch.h"
#include "ConvertData.h"
#include<atlstr.h>
#include <random>
// Constructor
ConvertData::ConvertData()
{
}

// Destructor
ConvertData::~ConvertData()
{
}

/**
 * Converts a CString to a std::string
 */
std::string ConvertData::CStringToString(const CString& cstr)
{
    // CT2A converts the CString (which might be wchar_t in Unicode builds) 
    // into a standard ASCII/Ansi string.
    return std::string(CT2A(cstr));
}

int ConvertData::CStringToInt(const CString& cstr) {
   return _ttoi(cstr);
}

/**
 * Converts a std::string to a CString
 */
CString ConvertData::StringToCString(const std::string& str)
{
    // CA2T converts the standard ASCII/Ansi string into the 
    // appropriate TCHAR format used by the current CString configuration.
    return CString(CA2T(str.c_str()));
}

int ConvertData::GetRandomInt(int min, int max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}