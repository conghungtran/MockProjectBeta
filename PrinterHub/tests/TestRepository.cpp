// tests/TestRepository.cpp
#include <gtest/gtest.h>
#include "../mocks/MockPrinterRepository.h"

using namespace PrinterHub::Core;

class RepositoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        m_repo = std::make_shared<MockPrinterRepository>();
    }
    
    std::shared_ptr<MockPrinterRepository> m_repo;
};

TEST_F(RepositoryTest, Append_AddsPrinterToRepository) {
    Printer printer("PRN001", "HP LaserJet", 
                    PrinterBrand::HP, PrinterStatus::ACTIVE, 
                    "2024-01-01", 24);
    
    bool result = m_repo->Append(printer);
    
    EXPECT_TRUE(result);
    EXPECT_EQ(m_repo->m_appendCallCount, 1);
    EXPECT_EQ(m_repo->m_printers.size(), 1);
}

TEST_F(RepositoryTest, Load_ReturnsPrintersFromRepository) {
    m_repo->AddTestData();
    
    std::vector<Printer> printers;
    bool result = m_repo->Load(printers);
    
    EXPECT_TRUE(result);
    EXPECT_EQ(printers.size(), 2);
    EXPECT_EQ(printers[0].getId(), "PRN001");
}

TEST_F(RepositoryTest, Update_ModifiesPrinterAtIndex) {
    m_repo->AddTestData();
    
    Printer updatedPrinter("PRN001", "HP LaserJet Pro", 
                           PrinterBrand::HP, PrinterStatus::ACTIVE, 
                           "2024-01-01", 36);
    
    bool result = m_repo->Update(0, updatedPrinter);
    
    EXPECT_TRUE(result);
    EXPECT_EQ(m_repo->m_updateCallCount, 1);
    EXPECT_EQ(m_repo->m_printers[0].getModel(), "HP LaserJet Pro");
}

TEST_F(RepositoryTest, Delete_RemovesPrinterAtIndex) {
    m_repo->AddTestData();
    
    bool result = m_repo->Delete(0);
    
    EXPECT_TRUE(result);
    EXPECT_EQ(m_repo->m_deleteCallCount, 1);
    EXPECT_EQ(m_repo->m_printers.size(), 1);
    EXPECT_EQ(m_repo->m_printers[0].getId(), "PRN002");
}