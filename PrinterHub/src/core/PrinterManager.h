// PrinterManager.h
#pragma once
#include <vector>
#include <stack>
#include <memory>
#include "Printer.h"
#include "IObserver.h"
#include "./CommandPattern/ICommand.h"
#include "repository/IPrinterRepository.h"
#include "PrinterError.h"
//#include "firmware/Firmware.h"

namespace PrinterHub {
    namespace Core {
        class PrinterManager : public IObservable {
        public:

            explicit PrinterManager(std::shared_ptr<IPrinterRepository> repository = nullptr);

            // Command execution (tự động lưu vào undo stack)
            void ExecuteCommand(std::unique_ptr<ICommand> command);

            // Undo/Redo
            void Undo();
            void Redo();
            bool CanUndo() const { return !m_undoStack.empty(); }
            bool CanRedo() const { return !m_redoStack.empty(); }

            // CRUD Operations (tự động lưu xuống repository)
            PrinterError AddPrinter(const Printer& printer);
            PrinterError UpdatePrinter(int index, const Printer& printer);
            PrinterError DeletePrinter(int index);
            PrinterError DeletePrinterById(const std::string& id);

            PrinterError AddPrinterAt(int index, const Printer& printer);
            //PrinterError AddPrinterAt(int index, Printer&& printer);  // Move version

            void LoadPrinter(const Printer& printer);
            void PrinterManager::SetUpFirmware();

            void ClearAll();

            // Internal execution methods (called by commands)
            void executeAdd(const Printer& printer);
            //void executeAddAt(int index, const Printer& printer);
            void executeUpdate(int index, const Printer& printer);
            void executeDelete(int index);
            void executeUndoAdd(const Printer& printer);

            // Queries
            const Printer& GetPrinter(int index) const;
            Printer& GetPrinter(int index);
            int GetPrinterCount() const { return m_printers.size(); }
            int FindPrinterById(const std::string& id) const;
            const std::vector<Printer>& GetAllPrinters() const { return m_printers; }

            // Observer pattern
            void Attach(IObserver* observer) override;
            void Detach(IObserver* observer) override;
            void Notify(PrinterEvent event, int index = -1) override;
            
            // Getter: Trả về hằng tham chiếu để tối ưu hiệu năng (không tăng ref count)
            const std::shared_ptr<IPrinterRepository>& GetRepository() const
            {
                return m_repository;
            }

            // Setter: Truyền tham trị kết hợp std::move để tối ưu việc chuyển quyền sở hữu
            void SetRepository(std::shared_ptr<IPrinterRepository> repository)
            {
                m_repository = std::move(repository);
            }

        
          
        private:
            std::vector<Printer> m_printers;

            std::vector<IObserver*> m_observers;
            std::shared_ptr<IPrinterRepository> m_repository;

            // Command stacks
            std::stack<std::unique_ptr<ICommand>> m_undoStack;
            std::stack<std::unique_ptr<ICommand>> m_redoStack;

        public:
            //std::vector<Firmware> m_firmwares;
        };
    }
}