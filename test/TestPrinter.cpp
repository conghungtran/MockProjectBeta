#include "pch.h"
#include "gtest/gtest.h"

#include "../PrinterHub/src/core/PrinterManager.h"
#include "../PrinterHub/src/core/Printer.h"

#include "../PrinterHub/src/core/repository/MockPrinterRepository.h"


using namespace PrinterHub::Core;

class PrinterTest : public ::testing::Test {
protected:
    void SetUp() override {
        printer = new Printer("PRN001", "HP LaserJet Pro",
            PrinterBrand::HP, PrinterStatus::ACTIVE,
            "2024-01-15", 24);
    }

    void TearDown() override {
        delete printer;
    }

    Printer* printer;
};

// ========== Constructor Tests ==========
TEST_F(PrinterTest, Constructor_ValidData_InitializesCorrectly) {
    EXPECT_EQ(printer->getId(), "PRN001");
    EXPECT_EQ(printer->getModel(), "HP LaserJet Pro");
    EXPECT_EQ(printer->getBrand(), PrinterBrand::HP);
    EXPECT_EQ(printer->getStatus(), PrinterStatus::ACTIVE);
    EXPECT_EQ(printer->getPurchaseDate(), "2024-01-15");
    EXPECT_EQ(printer->getWarrantyMonth(), 24);
}

TEST_F(PrinterTest, Constructor_EmptyId_InitializesEmpty) {
    Printer emptyPrinter("", "Model", PrinterBrand::OTHER,
        PrinterStatus::ACTIVE, "2024-01-01", 12);
    EXPECT_TRUE(emptyPrinter.getId().empty());
}

// ========== Setter Tests ==========
TEST_F(PrinterTest, SetModel_ValidModel_UpdatesModel) {
    printer->setModel("HP LaserJet Pro M404dn");
    EXPECT_EQ(printer->getModel(), "HP LaserJet Pro M404dn");
}

TEST_F(PrinterTest, SetBrand_UpdatesBrand) {
    printer->setBrand(PrinterBrand::CANON);
    EXPECT_EQ(printer->getBrand(), PrinterBrand::CANON);
}

TEST_F(PrinterTest, SetStatus_UpdatesStatus) {
    printer->setStatus(PrinterStatus::INSERVICE);
    EXPECT_EQ(printer->getStatus(), PrinterStatus::INSERVICE);
}

TEST_F(PrinterTest, SetWarrantyMonth_ValidValue_UpdatesWarranty) {
    printer->setWarrantyMonth(36);
    EXPECT_EQ(printer->getWarrantyMonth(), 36);
}

// ========== Copy Constructor Tests ==========
TEST_F(PrinterTest, CopyConstructor_CreatesExactCopy) {
    Printer copy(*printer);

    EXPECT_EQ(copy.getId(), printer->getId());
    EXPECT_EQ(copy.getModel(), printer->getModel());
    EXPECT_EQ(copy.getBrand(), printer->getBrand());
    EXPECT_EQ(copy.getStatus(), printer->getStatus());
    EXPECT_EQ(copy.getWarrantyMonth(), printer->getWarrantyMonth());
}

// ========== Assignment Operator Tests ==========
TEST_F(PrinterTest, AssignmentOperator_CopiesCorrectly) {
    Printer other("OTHER", "Other Model", PrinterBrand::EPSON,
        PrinterStatus::RETIRED, "2023-01-01", 0);

    other = *printer;

    EXPECT_EQ(other.getId(), printer->getId());
    EXPECT_EQ(other.getModel(), printer->getModel());
}

//// ========== Equality Operator Tests ==========
//TEST_F(PrinterTest, EqualityOperator_SameId_ReturnsTrue) {
//    Printer sameId("PRN001", "Different Model", PrinterBrand::CANON,
//        PrinterStatus::INSERVICE, "2024-02-01", 12);
//
//    EXPECT_TRUE(*printer == sameId);
//}
//
//TEST_F(PrinterTest, EqualityOperator_DifferentId_ReturnsFalse) {
//    Printer differentId("PRN002", "HP LaserJet Pro", PrinterBrand::HP,
//        PrinterStatus::ACTIVE, "2024-01-15", 24);
//
//    EXPECT_FALSE(*printer == differentId);
//}