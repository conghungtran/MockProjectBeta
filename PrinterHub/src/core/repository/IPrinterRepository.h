// IPrinterRepository.h
#pragma once
#include <vector>
#include <string>
#include "../Printer.h"

namespace PrinterHub {
    namespace Core {
        class IPrinterRepository {
        public:
            virtual ~IPrinterRepository() = default;

            // CRUD operations
            virtual bool Load(std::vector<Printer>& printers) = 0;
            virtual bool Save(const std::vector<Printer>& printers) = 0;
            virtual bool Append(const Printer& printer) = 0;
            virtual bool Update(int index, const Printer& printer) = 0;
            virtual bool Delete(int index) = 0;

            // Metadata
            virtual std::string GetStoragePath() const = 0;
            virtual bool IsAvailable() const = 0;
        };
    }
}