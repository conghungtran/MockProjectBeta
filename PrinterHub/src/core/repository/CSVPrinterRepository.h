// CSVPrinterRepository.h
#pragma once
#include "IPrinterRepository.h"
#include <afx.h>

namespace PrinterHub {
    namespace Core {
        class CSVPrinterRepository : public IPrinterRepository {
        public:
            CSVPrinterRepository(const CString& filePath);
            virtual ~CSVPrinterRepository() = default;

            // IPrinterRepository implementation
            bool Load(std::vector<Printer>& printers) override;
            bool Save(const std::vector<Printer>& printers) override;
            bool Append(const Printer& printer) override;
            bool Update(int index, const Printer& printer) override;
            bool Delete(int index) override;

            std::string GetStoragePath() const override;
            bool IsAvailable() const override;

        private:
            CString m_filePath;
            bool m_isAvailable;

            // Helper methods
            bool EnsureDirectoryExists();
            bool FileExists() const;
            CString FormatPrinterToCSV(const Printer& printer) const;
            Printer ParseCSVToPrinter(const CString& line) const;
        };
    }
}