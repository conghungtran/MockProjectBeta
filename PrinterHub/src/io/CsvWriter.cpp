#include "pch.h"
#include "CsvWriter.h"
#include "CsvReader.h"

using namespace PrinterHub::Core;

CsvWriter::CsvWriter()
    : m_chDelimiter(_T(','))
    , m_bQuoteStrings(false)
{
}

CsvWriter::~CsvWriter()
{
}

bool CsvWriter::WriteAll(const CString& strFilePath,
    const std::vector<std::vector<CString>>& data,
    bool bOverwrite)
{
    UINT nOpenFlags = CFile::modeWrite | CFile::modeCreate;
    if (!bOverwrite)
    {
        nOpenFlags |= CFile::modeNoTruncate;
    }

    CStdioFile file;
    if (!file.Open(strFilePath, nOpenFlags))
    {
        TRACE(_T("Cannot open file: %s\n"), strFilePath);
        return false;
    }

    // Nếu không ghi đè, seek đến cuối file
    if (!bOverwrite)
    {
        file.SeekToEnd();
    }

    for (const auto& row : data)
    {
        CString strLine = FormatLine(row);
        file.WriteString(strLine + _T("\n"));
    }

    file.Close();
    return true;
}

bool CsvWriter::AppendRow(const CString& strFilePath,
    const std::vector<CString>& row)
{
    CStdioFile file;
    if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate))
    {
        TRACE(_T("Cannot open file: %s\n"), strFilePath);
        return false;
    }

    file.SeekToEnd();  // Chuyển đến cuối file

    CString strLine = FormatLine(row);
    file.WriteString(strLine + _T("\n"));

    file.Close();
    return true;
}

bool CsvWriter::AppendRows(const CString& strFilePath,
    const std::vector<std::vector<CString>>& rows)
{
    CStdioFile file;
    if (!file.Open(strFilePath, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate))
    {
        TRACE(_T("Cannot open file: %s\n"), strFilePath);
        return false;
    }

    file.SeekToEnd();

    for (const auto& row : rows)
    {
        CString strLine = FormatLine(row);
        file.WriteString(strLine + _T("\n"));
    }

    file.Close();
    return true;
}

bool CsvWriter::UpdateRow(const CString& strFilePath,
    int nRowIndex,
    const std::vector<CString>& newRow,
    bool bHasHeader)
{
    // Đọc toàn bộ file
    CsvReader reader;
    reader.SetDelimiter(m_chDelimiter);
    reader.SetHasHeader(bHasHeader);

    std::vector<std::vector<CString>> allData;
    if (!reader.ReadAll(strFilePath, allData))
    {
        return false;
    }

    // Kiểm tra index
    if (nRowIndex < 0 || nRowIndex >= (int)allData.size())
    {
        TRACE(_T("Invalid row index: %d\n"), nRowIndex);
        return false;
    }

    // Cập nhật dòng
    allData[nRowIndex] = newRow;

    // Ghi lại toàn bộ
    return WriteAll(strFilePath, allData, true);
}

bool CsvWriter::WriteHeader(const CString& strFilePath,
    const std::vector<CString>& header,
    bool bOverwrite)
{
    std::vector<std::vector<CString>> data;
    data.push_back(header);

    return WriteAll(strFilePath, data, bOverwrite);
}

CString CsvWriter::FormatLine(const std::vector<CString>& row) const
{
    CString strLine;

    for (size_t i = 0; i < row.size(); i++)
    {
        CString strField = row[i];

        if (m_bQuoteStrings)
        {
            strField = QuoteString(strField);
        }

        strLine += strField;

        if (i < row.size() - 1)
        {
            strLine += m_chDelimiter;
        }
    }

    return strLine;
}

CString CsvWriter::QuoteString(const CString& str) const
{
    // Nếu string có chứa dấu phẩy hoặc dấu ngoặc kép, cần thêm quotes
    bool bNeedQuote = (str.Find(m_chDelimiter) != -1) ||
        (str.Find(_T('"')) != -1) ||
        (str.Find(_T('\n')) != -1);

    if (!bNeedQuote && !m_bQuoteStrings)
    {
        return str;
    }

    // Thay thế " bằng ""
    CString strQuoted = str;
    strQuoted.Replace(_T("\""), _T("\"\""));

    return _T("\"") + strQuoted + _T("\"");
}