// AddPrinterCommand.h
#pragma once
#include "ICommand.h"
#include "../Printer.h"
#include "../PrinterManager.h"

namespace PrinterHub {
    namespace Core {
        // EditPrinterCommand.h
        class EditPrinterCommand : public ICommand {
        private:
            PrinterManager* m_manager;
            int m_index;
            Printer m_oldPrinter;
            Printer m_newPrinter;

        public:
            EditPrinterCommand(PrinterManager* manager, int index,
                const Printer& oldPrinter, const Printer& newPrinter)
                : m_manager(manager)
                , m_index(index)
                , m_oldPrinter(oldPrinter)
                , m_newPrinter(newPrinter)
            {
            }

            void execute() override {
                m_manager->UpdatePrinter(m_index, m_newPrinter);
            }

            void undo() override {
                m_manager->UpdatePrinter(m_index, m_oldPrinter);
            }

            CommandType getType() const override { return CommandType::Edit; }
            std::string getDescription() const override {
                return "Edit printer at index " + std::to_string(m_index);
            }
        };

    }
}