#pragma once
#include "ISortStrategy.h"
#include <algorithm>

namespace PrinterHub {
    namespace Core {

        class SortByModel : public ISortStrategy {
        public:
            void sort(std::vector<Printer>& printers) override {
                std::sort(printers.begin(), printers.end(),
                    [](const Printer& a, const Printer& b) {
                        return a.getModel() < b.getModel();
                    });
            }

            CString getName() const override {
                return _T("Sort by Model");
            }
        };

    }
}