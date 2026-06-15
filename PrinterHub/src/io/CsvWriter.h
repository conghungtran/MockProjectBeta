#pragma once
#include <afx.h>
#include <vector>
#include <string>

namespace PrinterHub {
    namespace Core {
        class CsvWriter {
        public:
            CsvWriter();
            virtual ~CsvWriter();

            // Ghi toàn bộ dữ liệu
            bool WriteAll(const CString& strFilePath,
                const std::vector<std::vector<CString>>& data,
                bool bOverwrite = true);

            // Ghi thêm vào cuối file (append)
            bool AppendRow(const CString& strFilePath,
                const std::vector<CString>& row);

            // Ghi nhiều dòng
            bool AppendRows(const CString& strFilePath,
                const std::vector<std::vector<CString>>& rows);

            // Cập nhật một dòng cụ thể (đọc -> sửa -> ghi lại)
            bool UpdateRow(const CString& strFilePath,
                int nRowIndex,
                const std::vector<CString>& newRow,
                bool bHasHeader = false);

            // Ghi header
            bool WriteHeader(const CString& strFilePath,
                const std::vector<CString>& header,
                bool bOverwrite = false);

            // Getter/Setter
            void SetDelimiter(TCHAR chDelimiter) { m_chDelimiter = chDelimiter; }
            void SetQuoteStrings(bool bQuote) { m_bQuoteStrings = bQuote; }

        private:
            TCHAR m_chDelimiter;      // Dấu phân cách
            bool m_bQuoteStrings;      // Có thêm dấu ngoặc kép cho string không?

            // Helper: Format một dòng từ các cột
            CString FormatLine(const std::vector<CString>& row) const;

            // Helper: Thêm dấu ngoặc kép nếu cần
            CString QuoteString(const CString& str) const;
        };
    }
}