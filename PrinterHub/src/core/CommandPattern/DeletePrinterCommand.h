// AddPrinterCommand.h
#pragma once
#include "ICommand.h"
#include "../Printer.h"
#include "../PrinterManager.h"

namespace PrinterHub {
    namespace Core {
        // DeletePrinterCommand.h
        class DeletePrinterCommand : public ICommand {
        private:
            PrinterManager* m_manager;
            int m_index;
            Printer m_deletedPrinter;

        public:
            DeletePrinterCommand(PrinterManager* manager, int index,
                const Printer& deletedPrinter)
                : m_manager(manager)
                , m_index(index)
                , m_deletedPrinter(deletedPrinter)
            {
            }

            void execute() override {
                m_manager->DeletePrinter(m_index);
            }

            void undo() override {
                m_manager->AddPrinterAt(m_index, m_deletedPrinter);
               
            }

            CommandType getType() const override { return CommandType::Delete; }
            std::string getDescription() const override {
                return "Delete printer: " + m_deletedPrinter.getId();
            }
        };
    }
}
