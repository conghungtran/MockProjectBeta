// SortByBrand.h
#pragma once
#include "ISortStrategy.h"
#include <algorithm>

namespace PrinterHub {
    namespace Core {

        class SortByBrand : public ISortStrategy {
        public:
            void sort(std::vector<Printer>& printers) override {
                std::sort(printers.begin(), printers.end(),
                    [](const Printer& a, const Printer& b) {
                        return static_cast<int>(a.getBrand()) <
                            static_cast<int>(b.getBrand());
                    });
            }

            CString getName() const override {
                return _T("Sort by Brand");
            }
        };

    }
}