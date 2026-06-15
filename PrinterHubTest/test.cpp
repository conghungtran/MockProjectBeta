#include <gtest/gtest.h>
#include "../PrinterHub/src/core/Printer.h"
#include "../PrinterHub/src/core/PrinterManager.h"

using namespace PrinterHub::Core;

class PrinterManagerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        manager = new PrinterManager();
    }

    void TearDown() override
    {
        delete manager;
    }

    PrinterManager* manager;
};

// Test 1: Thêm printer hợp lệ
TEST_F(PrinterManagerTest, AddValidPrinter_Success)
{
    Printer printer("PRN001", "HP LaserJet Pro",
        PrinterBrand::HP, PrinterStatus::ACTIVE,
        "2024-01-15", 24);

    PrinterError error = manager->AddPrinter(printer);

    EXPECT_EQ(error, PrinterError::Success);
    EXPECT_EQ(manager->GetPrinterCount(), 1);
}

// Test 2: Thêm printer với ID rỗng
TEST_F(PrinterManagerTest, AddPrinter_EmptyId_ReturnsError)
{
    Printer printer("", "HP LaserJet Pro",
        PrinterBrand::HP, PrinterStatus::ACTIVE,
        "2024-01-15", 24);

    PrinterError error = manager->AddPrinter(printer);

    EXPECT_EQ(error, PrinterError::EmptyId);
    EXPECT_EQ(manager->GetPrinterCount(), 0);
}

// Test 3: Thêm printer bị trùng ID
TEST_F(PrinterManagerTest, AddPrinter_DuplicateId_ReturnsError)
{
    Printer printer1("PRN001", "HP LaserJet Pro",
        PrinterBrand::HP, PrinterStatus::ACTIVE,
        "2024-01-15", 24);

    Printer printer2("PRN001", "Epson EcoTank",
        PrinterBrand::EPSON, PrinterStatus::ACTIVE,
        "2024-02-20", 12);

    manager->AddPrinter(printer1);
    PrinterError error = manager->AddPrinter(printer2);

    EXPECT_EQ(error, PrinterError::DuplicateId);
    EXPECT_EQ(manager->GetPrinterCount(), 1);
}

// Test 4: Thêm printer với model rỗng
TEST_F(PrinterManagerTest, AddPrinter_EmptyModel_ReturnsError)
{
    Printer printer("PRN001", "",
        PrinterBrand::HP, PrinterStatus::ACTIVE,
        "2024-01-15", 24);

    PrinterError error = manager->AddPrinter(printer);

    EXPECT_EQ(error, PrinterError::EmptyModel);
    EXPECT_EQ(manager->GetPrinterCount(), 0);
}

// Test 5: Thêm nhiều printer
TEST_F(PrinterManagerTest, AddMultiplePrinters_Success)
{
    Printer printer1("PRN001", "HP LaserJet Pro",
        PrinterBrand::HP, PrinterStatus::ACTIVE,
        "2024-01-15", 24);

    Printer printer2("PRN002", "Epson EcoTank",
        PrinterBrand::EPSON, PrinterStatus::ACTIVE,
        "2024-02-20", 12);

    Printer printer3("PRN003", "Canon PIXMA",
        PrinterBrand::CANON, PrinterStatus::ACTIVE,
        "2024-03-10", 36);

    manager->AddPrinter(printer1);
    manager->AddPrinter(printer2);
    manager->AddPrinter(printer3);

    EXPECT_EQ(manager->GetPrinterCount(), 3);
}