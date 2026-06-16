#pragma once
#include <vector>
#include "../Printer.h"

namespace PrinterHub {
    namespace Core {

        class ISortStrategy {
        public:
            virtual ~ISortStrategy() = default;
            virtual void sort(std::vector<Printer>& printers) = 0;
            virtual CString getName() const = 0;
        };

    }
}