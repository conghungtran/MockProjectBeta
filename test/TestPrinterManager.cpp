
#pragma once 
#include "pch.h"
#include "gtest/gtest.h"

#include "../PrinterHub/src/core/PrinterManager.h"
#include "../PrinterHub/src/core/Printer.h"

#include "../PrinterHub/src/core/repository/MockPrinterRepository.h"


using namespace PrinterHub::Core;


class PrinterManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockRepo = std::make_shared<MockPrinterRepository>();
        manager = new PrinterManager(mockRepo);
        observer = new MockObserver();
        manager->Attach(observer);
    }

    void TearDown() override {
        manager->Detach(observer);
        delete manager;
        delete observer;
    }

    Printer createValidPrinter(const std::string& id = "PRN001") {
        return Printer(id, "HP LaserJet", PrinterBrand::HP,
            PrinterStatus::ACTIVE, "2024-01-01", 24);
    }

    std::shared_ptr<MockPrinterRepository> mockRepo;
    PrinterManager* manager;
    MockObserver* observer;
};

// ========== AddPrinter Tests ==========
TEST_F(PrinterManagerTest, AddPrinter_ValidPrinter_ReturnsSuccess) {
    Printer printer = createValidPrinter();

    PrinterError error = manager->AddPrinter(printer);

    EXPECT_EQ(error, PrinterError::Success);
    EXPECT_EQ(manager->GetPrinterCount(), 1);
}
//
TEST_F(PrinterManagerTest, AddPrinter_EmptyId_ReturnsEmptyIdError) {
    Printer printer("", "HP LaserJet", PrinterBrand::HP,
        PrinterStatus::ACTIVE, "2024-01-01", 24);

    PrinterError error = manager->AddPrinter(printer);

    EXPECT_EQ(error, PrinterError::EmptyId);
    EXPECT_EQ(manager->GetPrinterCount(), 0);
}

TEST_F(PrinterManagerTest, AddPrinter_EmptyModel_ReturnsEmptyModelError) {
    Printer printer("PRN001", "", PrinterBrand::HP,
        PrinterStatus::ACTIVE, "2024-01-01", 24);

    PrinterError error = manager->AddPrinter(printer);

    EXPECT_EQ(error, PrinterError::EmptyModel);
    EXPECT_EQ(manager->GetPrinterCount(), 0);
}

TEST_F(PrinterManagerTest, AddPrinter_DuplicateId_ReturnsDuplicateError) {
    Printer printer1 = createValidPrinter();
    Printer printer2 = createValidPrinter("PRN001");

    manager->AddPrinter(printer1);
    PrinterError error = manager->AddPrinter(printer2);

    EXPECT_EQ(error, PrinterError::DuplicateId);
    EXPECT_EQ(manager->GetPrinterCount(), 1);
}

TEST_F(PrinterManagerTest, AddPrinter_NotifiesObservers) {
    Printer printer = createValidPrinter();

    manager->AddPrinter(printer);

    EXPECT_EQ(observer->notifyCount, 1);
    EXPECT_TRUE(observer->WasNotified(PrinterEvent::PrinterAdded));
}

TEST_F(PrinterManagerTest, AddPrinter_RollsBackWhenRepositoryFails) {
    mockRepo->m_appendShouldSucceed = false;
    Printer printer = createValidPrinter();

    PrinterError error = manager->AddPrinter(printer);

    EXPECT_EQ(error, PrinterError::RepositoryAppendFailed);
    EXPECT_EQ(manager->GetPrinterCount(), 0);
}

// ========== GetPrinter Tests ==========
TEST_F(PrinterManagerTest, GetPrinter_ValidIndex_ReturnsCorrectPrinter) {
    Printer printer = createValidPrinter();
    manager->AddPrinter(printer);

    const Printer& retrieved = manager->GetPrinter(0);

    EXPECT_EQ(retrieved.getId(), "PRN001");
}

TEST_F(PrinterManagerTest, GetPrinter_InvalidIndex_ThrowsException) {
    EXPECT_THROW(manager->GetPrinter(999), std::out_of_range);
}

// ========== GetPrinterById Tests ==========
TEST_F(PrinterManagerTest, GetPrinterById_ExistingId_ReturnsPrinter) {
    Printer printer = createValidPrinter();
    manager->AddPrinter(printer);

    Printer* found = manager->GetPrinterById("PRN001");

    EXPECT_NE(found, nullptr);
    EXPECT_EQ(found->getId(), "PRN001");
}

TEST_F(PrinterManagerTest, GetPrinterById_NonExistingId_ReturnsNullptr) {
    Printer* found = manager->GetPrinterById("NONEXISTENT");

    EXPECT_EQ(found, nullptr);
}

// ========== FindPrinterById Tests ==========
TEST_F(PrinterManagerTest, FindPrinterById_ExistingId_ReturnsIndex) {
    Printer printer1 = createValidPrinter("PRN001");
    Printer printer2 = createValidPrinter("PRN002");
    manager->AddPrinter(printer1);
    manager->AddPrinter(printer2);

    int index = manager->FindPrinterById("PRN002");

    EXPECT_EQ(index, 1);
}

TEST_F(PrinterManagerTest, FindPrinterById_NonExistingId_ReturnsMinusOne) {
    int index = manager->FindPrinterById("NONEXISTENT");

    EXPECT_EQ(index, -1);
}

// ========== UpdatePrinter Tests ==========
TEST_F(PrinterManagerTest, UpdatePrinter_ValidData_Success) {
    Printer printer = createValidPrinter();
    manager->AddPrinter(printer);

    Printer updated("PRN001", "Updated Model", PrinterBrand::HP,
        PrinterStatus::ACTIVE, "2024-01-01", 36);

    PrinterError error = manager->UpdatePrinter(0, updated);

    EXPECT_EQ(error, PrinterError::Success);
    EXPECT_EQ(manager->GetPrinter(0).getModel(), "Updated Model");
}

TEST_F(PrinterManagerTest, UpdatePrinter_InvalidIndex_ReturnsError) {
    Printer updated("PRN001", "Updated Model", PrinterBrand::HP,
        PrinterStatus::ACTIVE, "2024-01-01", 36);

    PrinterError error = manager->UpdatePrinter(999, updated);

    EXPECT_EQ(error, PrinterError::InvalidIndex);
}

// ========== DeletePrinter Tests ==========
TEST_F(PrinterManagerTest, DeletePrinter_ValidIndex_Success) {
    Printer printer = createValidPrinter();
    manager->AddPrinter(printer);

    PrinterError error = manager->DeletePrinter(0);

    EXPECT_EQ(error, PrinterError::Success);
    EXPECT_EQ(manager->GetPrinterCount(), 0);
}

TEST_F(PrinterManagerTest, DeletePrinter_InvalidIndex_ReturnsError) {
    PrinterError error = manager->DeletePrinter(999);

    EXPECT_EQ(error, PrinterError::InvalidIndex);
}

// ========== ClearAll Tests ==========
TEST_F(PrinterManagerTest, ClearAll_RemovesAllPrinters) {
    manager->AddPrinter(createValidPrinter("PRN001"));
    manager->AddPrinter(createValidPrinter("PRN002"));
    manager->AddPrinter(createValidPrinter("PRN003"));

    manager->ClearAll();

    EXPECT_EQ(manager->GetPrinterCount(), 0);
}