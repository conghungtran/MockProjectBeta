#pragma once
#include <afx.h>
#include <vector>
#include <string>
#include <functional>

namespace PrinterHub {
    namespace Core {
        class CsvReader {
        public:
            CsvReader();
            virtual ~CsvReader();

            // Đọc toàn bộ file
            bool ReadAll(const CString& strFilePath,
                std::vector<std::vector<CString>>& outData);

            // Đọc từng dòng (callback)
            bool ReadLines(const CString& strFilePath,
                std::function<bool(const std::vector<CString>&)> callback);

            // Đọc header (dòng đầu tiên)
            bool ReadHeader(const CString& strFilePath,
                std::vector<CString>& outHeader);

            // Đọc dòng cụ thể
            bool ReadLine(const CString& strFilePath,
                int nLineIndex,
                std::vector<CString>& outRow);

            // Getter/Setter
            void SetDelimiter(TCHAR chDelimiter) { m_chDelimiter = chDelimiter; }
            void SetHasHeader(bool bHasHeader) { m_bHasHeader = bHasHeader; }

        private:
            TCHAR m_chDelimiter;      // Dấu phân cách, mặc định là ','
            bool m_bHasHeader;        // Có header không?

            // Helper: Tách một dòng thành các cột
            std::vector<CString> SplitLine(const CString& strLine) const;
        };
    }
}