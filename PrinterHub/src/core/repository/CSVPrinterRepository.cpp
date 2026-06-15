#include "pch.h"
#include "CSVPrinterRepository.h"
#include "../EnumConverter.h"
#include <io.h>
#include <fstream>

using namespace PrinterHub::Core;

CSVPrinterRepository::CSVPrinterRepository(const CString& filePath)
    : m_filePath(filePath)
    , m_isAvailable(false)
{
    m_isAvailable = EnsureDirectoryExists();
}

bool CSVPrinterRepository::EnsureDirectoryExists()
{
    // Lấy đường dẫn thư mục (không bao gồm tên file)
    CString directory = m_filePath;
    int pos = directory.ReverseFind('\\');
    if (pos != -1) {
        directory = directory.Left(pos);

        // Tạo thư mục nếu chưa tồn tại
        if (GetFileAttributes(directory) == INVALID_FILE_ATTRIBUTES) {
            return CreateDirectory(directory, NULL) != 0;
        }
    }
    return true;
}

bool CSVPrinterRepository::FileExists() const
{
    return GetFileAttributes(m_filePath) != INVALID_FILE_ATTRIBUTES;
}

bool CSVPrinterRepository::Load(std::vector<Printer>& printers)
{
	std::cout << " 111 CSVPrinterRepository::Load called with file path: " << CT2A(m_filePath) << std::endl;
    printers.clear();

    if (!FileExists()) {
		std::cout << "CSV file does not exist: " << CT2A(m_filePath) << std::endl;
        return true; // File chưa tồn tại không phải lỗi
    }

    CStdioFile file;
    if (!file.Open(m_filePath, CFile::modeRead)) {
		std::cout << "Failed to open CSV file: " << CT2A(m_filePath) << std::endl;
        return false;
    }

    CString line;
    bool isFirstLine = true;
	std::cout << "Start reading CSV file...\n";
    while (file.ReadString(line)) {
        std::cout << "Readline \n";
        // Bỏ qua header
        if (isFirstLine) {
            isFirstLine = false;
            continue;
        }

        if (!line.IsEmpty()) {
            Printer printer = ParseCSVToPrinter(line);
            printers.push_back(printer);
        }
    }

    file.Close();
    TRACE(_T("Loaded %zu printers from CSV\n"), printers.size());
    return true;
}

bool CSVPrinterRepository::Save(const std::vector<Printer>& printers)
{
    CStdioFile file;
    if (!file.Open(m_filePath, CFile::modeCreate | CFile::modeWrite)) {
        TRACE(_T("Cannot create CSV file: %s\n"), m_filePath);
        return false;
    }

    // Write header
    file.WriteString(_T("ID,Model,Brand,Status,PurchaseDate,WarrantyMonth\n"));

    // Write data
    for (const auto& printer : printers) {
        CString line = FormatPrinterToCSV(printer);
        file.WriteString(line + _T("\n"));
    }

    file.Close();
    TRACE(_T("Saved %zu printers to CSV\n"), printers.size());
    return true;
}

bool CSVPrinterRepository::Append(const Printer& printer)
{
    bool fileExists = FileExists();

    CStdioFile file;
    UINT openFlags = CFile::modeWrite | CFile::modeCreate;
    if (fileExists) {
        openFlags |= CFile::modeNoTruncate;  // Append mode
    }

    if (!file.Open(m_filePath, openFlags)) {
        return false;
    }

    // Nếu file chưa tồn tại, ghi header
    if (!fileExists) {
        file.WriteString(_T("ID,Model,Brand,Status,PurchaseDate,WarrantyMonth\n"));
    }
    else {
        file.SeekToEnd();  // Đến cuối file
    }

    CString line = FormatPrinterToCSV(printer);
    file.WriteString(line + _T("\n"));
    file.Close();

    return true;
}

bool CSVPrinterRepository::Update(int index, const Printer& printer)
{
    // Đọc toàn bộ file
    std::vector<Printer> printers;
    if (!Load(printers)) return false;

    // Cập nhật
    if (index < 0 || index >= (int)printers.size()) return false;
    printers[index] = printer;

    // Ghi lại toàn bộ
    return Save(printers);
}

bool CSVPrinterRepository::Delete(int index)
{
    std::vector<Printer> printers;
    if (!Load(printers)) return false;

	std::cout << "11111111" << printers[index].getId() << " - " << printers[index].getModel() << std::endl;
    if (index < 0 || index >= (int)printers.size()) return false;
    printers.erase(printers.begin() + index);
	std::cout << "Deleted printer at index " << index << std::endl;
    for (auto o : printers) {
		std::cout << o.getId() << " - " << o.getModel() << std::endl;
    }
    return Save(printers);
}

CString CSVPrinterRepository::FormatPrinterToCSV(const Printer& printer) const
{
    CString line;
    line.Format(_T("%s,%s,%s,%s,%s,%d"),
        CString(printer.getId().c_str()),
        CString(printer.getModel().c_str()),
        EnumConverter::FromPrinterBrand(printer.getBrand()),
        EnumConverter::FromPrinterStatus(printer.getStatus()),
        CString(printer.getPurchaseDate().c_str()),
        printer.getWarrantyMonth()
    );
    return line;
}

Printer CSVPrinterRepository::ParseCSVToPrinter(const CString& line) const
{
	std::cout << "Parsing line: " << CT2A(line) << std::endl;
    int pos = 0;
    CString token;

    token = line.Tokenize(_T(","), pos);
    std::string id = CT2A(token);

    token = line.Tokenize(_T(","), pos);
    std::string model = CT2A(token);

    token = line.Tokenize(_T(","), pos);
    PrinterBrand brand = EnumConverter::ToPrinterBrand(token);

    token = line.Tokenize(_T(","), pos);
    PrinterStatus status = EnumConverter::ToPrinterStatus(token);

    token = line.Tokenize(_T(","), pos);
    std::string purchaseDate = CT2A(token);

    token = line.Tokenize(_T(","), pos);
    int warrantyMonth = _ttoi(token);

    return Printer(id, model, brand, status, purchaseDate, warrantyMonth);
}

std::string CSVPrinterRepository::GetStoragePath() const
{
    return CT2A(m_filePath);
}

bool CSVPrinterRepository::IsAvailable() const
{
    return m_isAvailable;
}