
// PrinterManager.cpp
#include "pch.h"
#include "PrinterManager.h"

using namespace PrinterHub::Core;


//void PrinterManager::SetUpFirmware() {
//    m_firmwares.push_back(Firmware("FW001", PrinterBrand::HP, "1.0.0", "2024-01-01", "100MB"));
//    m_firmwares.push_back(Firmware("FW002", PrinterBrand::HP, "1.0.0", "2024-01-01", "200MB"));
//    m_firmwares.push_back(Firmware("FW003", PrinterBrand::HP, "1.0.0", "2024-01-01", "300MB"));
//
//    m_firmwares.push_back(Firmware("FW001", PrinterBrand::CANON, "1.0.0", "2024-01-01", "100MB"));
//    m_firmwares.push_back(Firmware("FW002", PrinterBrand::CANON, "1.0.0", "2024-01-01", "200MB"));
//    m_firmwares.push_back(Firmware("FW003", PrinterBrand::CANON, "1.0.0", "2024-01-01", "300MB"));
//
//    m_firmwares.push_back(Firmware("FW001", PrinterBrand::EPSON, "1.0.0", "2024-01-01", "100MB"));
//    m_firmwares.push_back(Firmware("FW002", PrinterBrand::EPSON, "1.0.0", "2024-01-01", "200MB"));
//    m_firmwares.push_back(Firmware("FW003", PrinterBrand::EPSON, "1.0.0", "2024-01-01", "300MB"));
//}

void PrinterManager::ExecuteCommand(std::unique_ptr<ICommand> command)
{
    command->execute();
    m_undoStack.push(std::move(command));

    // Clear redo stack khi có command mới
    while (!m_redoStack.empty()) {
        m_redoStack.pop();
    }
}

void PrinterManager::executeAdd(const Printer& printer)
{
    // 1. Thêm vào bộ nhớ
    m_printers.push_back(printer);
    int newIndex = m_printers.size() - 1;

    // 2. Lưu xuống repository
    if (m_repository) {
        m_repository->Append(printer);
    }

    // 3. Thông báo
    Notify(PrinterEvent::PrinterAdded, newIndex);
}


void PrinterManager::executeUpdate(int index, const Printer& printer)
{
    if (index < 0 || index >= (int)m_printers.size()) return;

    m_printers[index] = printer;

    if (m_repository) {
        m_repository->Update(index, printer);
    }

    Notify(PrinterEvent::PrinterUpdated, index);
}

void PrinterManager::executeDelete(int index)
{
    if (index < 0 || index >= (int)m_printers.size()) return;

    m_printers.erase(m_printers.begin() + index);

    if (m_repository) {
        //m_repository->Delete(index);
		m_repository->Save(m_printers);  // Cập nhật lại toàn bộ sau khi xóa
    }

    Notify(PrinterEvent::PrinterDeleted, index);
}

void PrinterManager::executeUndoAdd(const Printer& printer)
{
    // Tìm index của printer cần undo
    int index = FindPrinterById(printer.getId());
    if (index != -1) {
        executeDelete(index);
    }
}

void PrinterManager::Undo()
{
    if (m_undoStack.empty()) return;

    auto command = std::move(m_undoStack.top());
    m_undoStack.pop();
    command->undo();
    m_redoStack.push(std::move(command));
}

void PrinterManager::Redo()
{
    if (m_redoStack.empty()) return;

    auto command = std::move(m_redoStack.top());
    m_redoStack.pop();
    command->execute();
    m_undoStack.push(std::move(command));
}

PrinterError PrinterManager::AddPrinter(const Printer& printer)
{
    // Validation
    if (printer.getId().empty()) {
        return PrinterError::EmptyId;
    }

    if (printer.getModel().empty()) {
        return PrinterError::EmptyModel;
    }

    if (FindPrinterById(printer.getId()) != -1) {
        return PrinterError::DuplicateId;
    }

    // Thêm vào bộ nhớ
    m_printers.push_back(printer);
    Notify(PrinterEvent::PrinterAdded, m_printers.size() - 1);

    // Lưu xuống repository
    if (m_repository) {
        if (!m_repository->Append(printer)) {
            // Rollback
            m_printers.pop_back();
            return PrinterError::RepositoryAppendFailed;
        }
    }
    else {
        return PrinterError::RepositoryNotSet;
    }

    return PrinterError::Success;
}


void PrinterManager::LoadPrinter(const Printer& printer)
{

    // Thêm vào bộ nhớ
    m_printers.push_back(printer);
    Notify(PrinterEvent::PrinterAdded, m_printers.size() - 1);

}


PrinterError PrinterManager::DeletePrinter(int index)
{
    // 1. Kiểm tra index hợp lệ
    if (index < 0 || index >= (int)m_printers.size()) {
        return PrinterError::InvalidIndex;
    }

    // 2. Lưu lại printer để rollback nếu cần
    Printer deletedPrinter = m_printers[index];



    std::cout << "Requested delete at index: " << index << std::endl;
    std::string deletedId = m_printers[index].getId();
    std::string deletedModel = m_printers[index].getModel();
    std::cout << "Will delete: [" << index << "] "
        << deletedId << " - " << deletedModel << std::endl;

    // In ra tất cả index hiện tại
    std::cout << "\nCurrent list (before delete):" << std::endl;
    for (int i = 0; i < (int)m_printers.size(); i++) {
        std::cout << "  Index " << i << ": "
            << m_printers[i].getId() << " - "
            << m_printers[i].getModel() << std::endl;
    }


	std::cout << "-------------------\n";

    // 3. Xóa khỏi bộ nhớ
    m_printers.erase(m_printers.begin() + index);

    // In ra kết quả
    std::cout << "\nList after delete:" << std::endl;
    for (int i = 0; i < (int)m_printers.size(); i++) {
        std::cout << "  Index " << i << ": "
            << m_printers[i].getId() << " - "
            << m_printers[i].getModel() << std::endl;
    }

    // 4. Lưu xuống repository
    if (m_repository) {
        if (!m_repository->Delete(index)) {
            // Rollback: thêm lại printer vào vị trí cũ
            m_printers.insert(m_printers.begin() + index, deletedPrinter);
            return PrinterError::RepositoryDeleteFailed;
        }
    }
    else {
        // Rollback: thêm lại printer
        m_printers.insert(m_printers.begin() + index, deletedPrinter);
        return PrinterError::RepositoryNotSet;
    }

    // 5. Thông báo cho observers
    Notify(PrinterEvent::PrinterDeleted, index);

    return PrinterError::Success;
}


PrinterError PrinterManager::DeletePrinterById(const std::string& id)
{
    int index = FindPrinterById(id);
    if (index == -1) {
        return PrinterError::PrinterNotFound;
    }

    return DeletePrinter(index);
}

PrinterError PrinterManager::UpdatePrinter(int index, const Printer& printer)
{
    // 1. Kiểm tra index hợp lệ
    if (index < 0 || index >= (int)m_printers.size()) {
        return PrinterError::InvalidIndex;
    }

    // 2. Lưu lại printer cũ để rollback
    Printer oldPrinter = m_printers[index];

    // 3. Kiểm tra ID mới có bị trùng không (nếu ID thay đổi)
    if (printer.getId() != oldPrinter.getId()) {
        if (FindPrinterById(printer.getId()) != -1) {
            return PrinterError::DuplicateId;
        }
    }

    // 4. Validate dữ liệu
    if (printer.getId().empty()) {
        return PrinterError::EmptyId;
    }

    if (printer.getModel().empty()) {
        return PrinterError::EmptyModel;
    }

    // 5. Cập nhật trong bộ nhớ
    m_printers[index] = printer;

    // 6. Lưu xuống repository
    if (m_repository) {
        if (!m_repository->Update(index, printer)) {
            // Rollback: khôi phục printer cũ
            m_printers[index] = oldPrinter;
            return PrinterError::RepositoryUpdateFailed;
        }
    }
    else {
        // Rollback: khôi phục printer cũ
        m_printers[index] = oldPrinter;
        return PrinterError::RepositoryNotSet;
    }

    // 7. Thông báo cho observers
    Notify(PrinterEvent::PrinterUpdated, index);

    return PrinterError::Success;
}


PrinterError PrinterManager::AddPrinterAt(int index, const Printer& printer)
{
    // 1. Kiểm tra index hợp lệ (0 đến size)
    if (index < 0 || index >(int)m_printers.size()) {
        return PrinterError::InvalidIndex;
    }

    // 2. Validate dữ liệu
    if (printer.getId().empty()) {
        return PrinterError::EmptyId;
    }

    if (printer.getModel().empty()) {
        return PrinterError::EmptyModel;
    }

    // 3. Kiểm tra trùng ID
    if (FindPrinterById(printer.getId()) != -1) {
        return PrinterError::DuplicateId;
    }

    // 4. Thêm vào bộ nhớ tại vị trí chỉ định
    m_printers.insert(m_printers.begin() + index, printer);

    // 5. Lưu xuống repository
    if (m_repository) {
        if (!m_repository->Save(m_printers)) {
            // Rollback: xóa vừa thêm
            m_printers.erase(m_printers.begin() + index);
            return PrinterError::RepositoryAppendFailed;
        }
    }
    else {
        // Rollback
        m_printers.erase(m_printers.begin() + index);
        return PrinterError::RepositoryNotSet;
    }

    // 6. Thông báo cho observers
    Notify(PrinterEvent::PrinterAdded, index);

    return PrinterError::Success;
}



void PrinterManager::ClearAll()
{
    m_printers.clear();
    Notify(PrinterEvent::PrintersCleared, -1);
}

const Printer& PrinterManager::GetPrinter(int index) const
{
    static Printer emptyPrinter;
    if (index >= 0 && index < (int)m_printers.size())
    {
        return m_printers[index];
    }
    return emptyPrinter;
}

Printer& PrinterManager::GetPrinter(int index)
{
    static Printer emptyPrinter;
    if (index >= 0 && index < (int)m_printers.size())
    {
        return m_printers[index];
    }
    return emptyPrinter;
}

int PrinterManager::FindPrinterById(const std::string& id) const
{
    for (size_t i = 0; i < m_printers.size(); i++)
    {
        if (m_printers[i].getId() == id)
        {
            return (int)i;
        }
    }
    return -1;
}

void PrinterManager::Attach(IObserver* observer)
{
    if (observer && std::find(m_observers.begin(), m_observers.end(), observer) == m_observers.end())
    {
        m_observers.push_back(observer);
    }
}

void PrinterManager::Detach(IObserver* observer)
{
    auto it = std::find(m_observers.begin(), m_observers.end(), observer);
    if (it != m_observers.end())
    {
        m_observers.erase(it);
    }
}

void PrinterManager::Notify(PrinterEvent event, int index)
{
    for (auto observer : m_observers)
    {
        observer->OnPrinterChanged(event, index);
    }
}