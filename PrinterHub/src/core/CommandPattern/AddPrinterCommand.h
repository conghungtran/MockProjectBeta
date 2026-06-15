// AddPrinterCommand.h
#pragma once
#include "ICommand.h"
#include "../Printer.h"
#include "../PrinterManager.h"

namespace PrinterHub {
    namespace Core {
        class AddPrinterCommand : public ICommand {
        private:
            PrinterManager* m_manager;
            Printer m_printer;
            int m_addedIndex;

        public:
            AddPrinterCommand(PrinterManager* manager, const Printer& printer)
                : m_manager(manager)
                , m_printer(printer)
                , m_addedIndex(-1)
            {
            }

            void execute() override {
                m_addedIndex = m_manager->GetPrinterCount();
                m_manager->AddPrinter(m_printer);
            }

            void undo() override {
                if (m_addedIndex != -1) {
                    m_manager->DeletePrinter(m_addedIndex);
                }
            }

            CommandType getType() const override { return CommandType::Add; }
            std::string getDescription() const override {
                return "Add printer: " + m_printer.getId();
            }
        };
    }
}