#include "pch.h"
#include "CsvReader.h"

using namespace PrinterHub::Core;

CsvReader::CsvReader()
    : m_chDelimiter(_T(','))
    , m_bHasHeader(false)
{
}

CsvReader::~CsvReader()
{
}

bool CsvReader::ReadAll(const CString& strFilePath,
    std::vector<std::vector<CString>>& outData)
{
    outData.clear();

    CStdioFile file;
    if (!file.Open(strFilePath, CFile::modeRead))
    {
        TRACE(_T("Cannot open file: %s\n"), strFilePath);
        return false;
    }

    CString strLine;
    int nLineIndex = 0;

    while (file.ReadString(strLine))
    {
        // Bỏ qua dòng header nếu có
        if (m_bHasHeader && nLineIndex == 0)
        {
            nLineIndex++;
            continue;
        }

        std::vector<CString> row = SplitLine(strLine);
        if (!row.empty())
        {
            outData.push_back(row);
        }
        nLineIndex++;
    }

    file.Close();
    return true;
}

bool CsvReader::ReadLines(const CString& strFilePath,
    std::function<bool(const std::vector<CString>&)> callback)
{
    CStdioFile file;
    if (!file.Open(strFilePath, CFile::modeRead))
    {
        TRACE(_T("Cannot open file: %s\n"), strFilePath);
        return false;
    }

    CString strLine;
    int nLineIndex = 0;

    while (file.ReadString(strLine))
    {
        if (m_bHasHeader && nLineIndex == 0)
        {
            nLineIndex++;
            continue;
        }

        std::vector<CString> row = SplitLine(strLine);
        if (!row.empty())
        {
            if (!callback(row))  // Nếu callback trả về false thì dừng
            {
                break;
            }
        }
        nLineIndex++;
    }

    file.Close();
    return true;
}

bool CsvReader::ReadHeader(const CString& strFilePath,
    std::vector<CString>& outHeader)
{
    outHeader.clear();

    CStdioFile file;
    if (!file.Open(strFilePath, CFile::modeRead))
    {
        TRACE(_T("Cannot open file: %s\n"), strFilePath);
        return false;
    }

    CString strFirstLine;
    if (file.ReadString(strFirstLine))
    {
        outHeader = SplitLine(strFirstLine);
    }

    file.Close();
    return !outHeader.empty();
}

bool CsvReader::ReadLine(const CString& strFilePath,
    int nLineIndex,
    std::vector<CString>& outRow)
{
    outRow.clear();

    CStdioFile file;
    if (!file.Open(strFilePath, CFile::modeRead))
    {
        TRACE(_T("Cannot open file: %s\n"), strFilePath);
        return false;
    }

    CString strLine;
    int nCurrentIndex = 0;
    int nTargetIndex = m_bHasHeader ? nLineIndex + 1 : nLineIndex;

    while (file.ReadString(strLine))
    {
        if (nCurrentIndex == nTargetIndex)
        {
            outRow = SplitLine(strLine);
            file.Close();
            return !outRow.empty();
        }
        nCurrentIndex++;
    }

    file.Close();
    return false;
}

std::vector<CString> CsvReader::SplitLine(const CString& strLine) const
{
    std::vector<CString> result;

    if (strLine.IsEmpty())
        return result;

    int nStart = 0;
    int nEnd = 0;

    while (nEnd != -1)
    {
        nEnd = strLine.Find(m_chDelimiter, nStart);

        CString strToken;
        if (nEnd == -1)
        {
            strToken = strLine.Mid(nStart);
        }
        else
        {
            strToken = strLine.Mid(nStart, nEnd - nStart);
        }

        // Xóa khoảng trắng đầu và cuối
        strToken.Trim();

        // Xóa dấu ngoặc kép nếu có
        if (strToken.GetLength() >= 2 &&
            strToken[0] == _T('"') &&
            strToken[strToken.GetLength() - 1] == _T('"'))
        {
            strToken = strToken.Mid(1, strToken.GetLength() - 2);
        }

        result.push_back(strToken);
        nStart = nEnd + 1;
    }

    return result;
}