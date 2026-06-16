
#pragma once
#include "ISortStrategy.h"
#include <algorithm>

namespace PrinterHub {
    namespace Core {

        class SortById : public ISortStrategy {
        public:
            void sort(std::vector<Printer>& printers) override {
                std::sort(printers.begin(), printers.end(),
                    [](const Printer& a, const Printer& b) {
                        std::cout << "Sort \n";
                        return a.getId() < b.getId();
                    });
            }

            CString getName() const override {
                return _T("Sort by ID");
            }
        };

    }
}