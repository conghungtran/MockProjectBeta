#include "pch.h"
#include "gtest/gtest.h"

#include "../PrinterHub/src/core/PrinterManager.h"


using namespace PrinterHub::Core;

TEST(PrinterManagerTest, AddPrinter_ValidPrinter_ReturnsSuccess)
{
    PrinterManager manager;

    Printer printer(
        "P001",
        "HP LaserJet",
        PrinterBrand::HP,
        PrinterStatus::ACTIVE,
        "01/01/2026",
        12
    );

    PrinterError result = manager.AddPrinter(printer);

    EXPECT_EQ(result, PrinterError::Success);
    EXPECT_EQ(manager.GetPrinterCount(), 1);
}

TEST(PrinterManagerTest, AddPrinter_EmptyId_ReturnsEmptyId)
{
    PrinterManager manager;

    Printer printer(
        "",
        "HP LaserJet",
        PrinterBrand::HP,
        PrinterStatus::ACTIVE,
        "01/01/2026",
        12
    );

    PrinterError result = manager.AddPrinter(printer);

    EXPECT_EQ(result, PrinterError::EmptyId);
    EXPECT_EQ(manager.GetPrinterCount(), 0);
}

TEST(PrinterManagerTest, AddPrinter_EmptyModel_ReturnsEmptyModel)
{
    PrinterManager manager;

    Printer printer(
        "P001",
        "",
        PrinterBrand::HP,
        PrinterStatus::ACTIVE,
        "01/01/2026",
        12
    );

    PrinterError result = manager.AddPrinter(printer);

    EXPECT_EQ(result, PrinterError::EmptyModel);
    EXPECT_EQ(manager.GetPrinterCount(), 0);
}

TEST(PrinterManagerTest, AddPrinter_DuplicateId_ReturnsDuplicateId)
{
    PrinterManager manager;

    Printer printer1(
        "P001",
        "HP LaserJet",
        PrinterBrand::HP,
        PrinterStatus::ACTIVE,
        "01/01/2026",
        12
    );

    Printer printer2(
        "P001",
        "Canon LBP",
        PrinterBrand::CANON,
        PrinterStatus::ACTIVE,
        "02/01/2026",
        24
    );

    manager.AddPrinter(printer1);

    PrinterError result = manager.AddPrinter(printer2);

    EXPECT_EQ(result, PrinterError::DuplicateId);
    EXPECT_EQ(manager.GetPrinterCount(), 1);
}