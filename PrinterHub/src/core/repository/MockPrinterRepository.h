#pragma once
#include "IPrinterRepository.h"

#include <vector>

namespace PrinterHub {
    namespace Core {

        class MockPrinterRepository : public IPrinterRepository {
        public:
            // Mock data
            std::vector<Printer> m_printers;

            // Call counters
            int m_loadCallCount = 0;
            int m_saveCallCount = 0;
            int m_appendCallCount = 0;
            int m_updateCallCount = 0;
            int m_deleteCallCount = 0;

            // Last called parameters
            Printer m_lastAppendedPrinter;
            int m_lastUpdatedIndex = -1;
            Printer m_lastUpdatedPrinter;
            int m_lastDeletedIndex = -1;

            // Return value control
            bool m_loadShouldSucceed = true;
            bool m_saveShouldSucceed = true;
            bool m_appendShouldSucceed = true;
            bool m_updateShouldSucceed = true;
            bool m_deleteShouldSucceed = true;

            // IPrinterRepository implementation
            bool Load(std::vector<Printer>& printers) override {
                m_loadCallCount++;
                if (m_loadShouldSucceed) {
                    printers = m_printers;
                    return true;
                }
                return false;
            }

            bool Save(const std::vector<Printer>& printers) override {
                m_saveCallCount++;
                if (m_saveShouldSucceed) {
                    m_printers = printers;
                    return true;
                }
                return false;
            }

            bool Append(const Printer& printer) override {
                m_appendCallCount++;
                m_lastAppendedPrinter = printer;
                if (m_appendShouldSucceed) {
                    m_printers.push_back(printer);
                    return true;
                }
                return false;
            }

            bool Update(int index, const Printer& printer) override {
                m_updateCallCount++;
                m_lastUpdatedIndex = index;
                m_lastUpdatedPrinter = printer;
                if (m_updateShouldSucceed && index >= 0 && index < (int)m_printers.size()) {
                    m_printers[index] = printer;
                    return true;
                }
                return false;
            }

            bool Delete(int index) override {
                m_deleteCallCount++;
                m_lastDeletedIndex = index;
                if (m_deleteShouldSucceed && index >= 0 && index < (int)m_printers.size()) {
                    m_printers.erase(m_printers.begin() + index);
                    return true;
                }
                return false;
            }

            // Helper methods
            void Reset() {
                m_loadCallCount = 0;
                m_saveCallCount = 0;
                m_appendCallCount = 0;
                m_updateCallCount = 0;
                m_deleteCallCount = 0;
                m_lastUpdatedIndex = -1;
                m_lastDeletedIndex = -1;
                m_loadShouldSucceed = true;
                m_saveShouldSucceed = true;
                m_appendShouldSucceed = true;
                m_updateShouldSucceed = true;
                m_deleteShouldSucceed = true;
            }

            void AddTestData() {
                m_printers.push_back(Printer("PRN001", "HP LaserJet",
                    PrinterBrand::HP, PrinterStatus::ACTIVE, "2024-01-01", 24));
                m_printers.push_back(Printer("PRN002", "Epson EcoTank",
                    PrinterBrand::EPSON, PrinterStatus::ACTIVE, "2024-02-01", 12));
            }
        };

    }
}