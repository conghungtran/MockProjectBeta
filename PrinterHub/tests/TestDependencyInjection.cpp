// tests/TestDependencyInjection.cpp
#include <gtest/gtest.h>
#include "../mocks/MockPrinterRepository.h"
#include "../mocks/MockObserver.h"
#include "../../PrinterHubCore/core/PrinterManager.h"
#include "../../PrinterHubCore/core/PrinterError.h"

using namespace PrinterHub::Core;

class DependencyInjectionTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create mock repository
        m_mockRepo = std::make_shared<MockPrinterRepository>();
        m_mockRepo->AddTestData();
        
        // Create PrinterManager with injected mock
        m_manager = std::make_unique<PrinterManager>(m_mockRepo);
        
        // Add observer
        m_observer = std::make_unique<MockObserver>();
        m_manager->Attach(m_observer.get());
    }
    
    void TearDown() override {
        m_manager->Detach(m_observer.get());
    }
    
    std::shared_ptr<MockPrinterRepository> m_mockRepo;
    std::unique_ptr<PrinterManager> m_manager;
    std::unique_ptr<MockObserver> m_observer;
};

// ========== Test 1: Verify repository is injected correctly ==========
TEST_F(DependencyInjectionTest, RepositoryIsInjectedCorrectly) {
    // Verify that manager has the repository
    auto repo = m_manager->GetRepository();
    EXPECT_NE(repo, nullptr);
    EXPECT_EQ(repo.get(), m_mockRepo.get());
}

// ========== Test 2: AddPrinter calls repository Append ==========
TEST_F(DependencyInjectionTest, AddPrinter_CallsRepositoryAppend) {
    Printer printer("PRN003", "Canon PIXMA", 
                    PrinterBrand::CANON, PrinterStatus::ACTIVE, 
                    "2024-03-01", 36);
    
    PrinterError error = m_manager->AddPrinter(printer);
    
    EXPECT_EQ(error, PrinterError::Success);
    EXPECT_EQ(m_mockRepo->m_appendCallCount, 1);
    EXPECT_EQ(m_mockRepo->m_lastAppendedPrinter.getId(), "PRN003");
}

// ========== Test 3: AddPrinter notifies observers ==========
TEST_F(DependencyInjectionTest, AddPrinter_NotifiesObservers) {
    Printer printer("PRN003", "Canon PIXMA", 
                    PrinterBrand::CANON, PrinterStatus::ACTIVE, 
                    "2024-03-01", 36);
    
    m_manager->AddPrinter(printer);
    
    EXPECT_EQ(m_observer->m_notifyCount, 1);
    EXPECT_TRUE(m_observer->WasNotified(PrinterEvent::PrinterAdded));
}

// ========== Test 4: AddPrinter rolls back when repository fails ==========
TEST_F(DependencyInjectionTest, AddPrinter_RollsBackWhenAppendFails) {
    m_mockRepo->m_appendShouldSucceed = false;
    
    int initialCount = m_manager->GetPrinterCount();
    Printer printer("PRN003", "Canon PIXMA", 
                    PrinterBrand::CANON, PrinterStatus::ACTIVE, 
                    "2024-03-01", 36);
    
    PrinterError error = m_manager->AddPrinter(printer);
    
    EXPECT_EQ(error, PrinterError::RepositoryAppendFailed);
    EXPECT_EQ(m_manager->GetPrinterCount(), initialCount);  // No change
    EXPECT_EQ(m_mockRepo->m_appendCallCount, 1);
}

// ========== Test 5: UpdatePrinter calls repository Update ==========
TEST_F(DependencyInjectionTest, UpdatePrinter_CallsRepositoryUpdate) {
    // First add a printer
    Printer printer("PRN003", "Canon PIXMA", 
                    PrinterBrand::CANON, PrinterStatus::ACTIVE, 
                    "2024-03-01", 36);
    m_manager->AddPrinter(printer);
    
    // Update it
    Printer updatedPrinter("PRN003", "Canon PIXMA Pro", 
                           PrinterBrand::CANON, PrinterStatus::ACTIVE, 
                           "2024-03-01", 48);
    
    int index = m_manager->GetPrinterCount() - 1;
    PrinterError error = m_manager->UpdatePrinter(index, updatedPrinter);
    
    EXPECT_EQ(error, PrinterError::Success);
    EXPECT_EQ(m_mockRepo->m_updateCallCount, 1);
    EXPECT_EQ(m_mockRepo->m_lastUpdatedIndex, index);
}

// ========== Test 6: UpdatePrinter rolls back when repository fails ==========
TEST_F(DependencyInjectionTest, UpdatePrinter_RollsBackWhenUpdateFails) {
    // Add a printer
    Printer printer("PRN003", "Canon PIXMA", 
                    PrinterBrand::CANON, PrinterStatus::ACTIVE, 
                    "2024-03-01", 36);
    m_manager->AddPrinter(printer);
    
    // Make repository fail
    m_mockRepo->m_updateShouldSucceed = false;
    
    Printer updatedPrinter("PRN003", "Canon PIXMA Pro", 
                           PrinterBrand::CANON, PrinterStatus::ACTIVE, 
                           "2024-03-01", 48);
    
    int index = m_manager->GetPrinterCount() - 1;
    std::string oldModel = m_manager->GetPrinter(index).getModel();
    
    PrinterError error = m_manager->UpdatePrinter(index, updatedPrinter);
    
    EXPECT_EQ(error, PrinterError::RepositoryUpdateFailed);
    EXPECT_EQ(m_manager->GetPrinter(index).getModel(), oldModel);  // Rolled back
}

// ========== Test 7: DeletePrinter calls repository Delete ==========
TEST_F(DependencyInjectionTest, DeletePrinter_CallsRepositoryDelete) {
    // Add a printer
    Printer printer("PRN003", "Canon PIXMA", 
                    PrinterBrand::CANON, PrinterStatus::ACTIVE, 
                    "2024-03-01", 36);
    m_manager->AddPrinter(printer);
    
    int count = m_manager->GetPrinterCount();
    int index = count - 1;
    
    PrinterError error = m_manager->DeletePrinter(index);
    
    EXPECT_EQ(error, PrinterError::Success);
    EXPECT_EQ(m_mockRepo->m_deleteCallCount, 1);
    EXPECT_EQ(m_mockRepo->m_lastDeletedIndex, index);
    EXPECT_EQ(m_manager->GetPrinterCount(), count - 1);
}

// ========== Test 8: DeletePrinter rolls back when repository fails ==========
TEST_F(DependencyInjectionTest, DeletePrinter_RollsBackWhenDeleteFails) {
    // Add a printer
    Printer printer("PRN003", "Canon PIXMA", 
                    PrinterBrand::CANON, PrinterStatus::ACTIVE, 
                    "2024-03-01", 36);
    m_manager->AddPrinter(printer);
    
    // Make repository fail
    m_mockRepo->m_deleteShouldSucceed = false;
    
    int count = m_manager->GetPrinterCount();
    int index = count - 1;
    
    PrinterError error = m_manager->DeletePrinter(index);
    
    EXPECT_EQ(error, PrinterError::RepositoryDeleteFailed);
    EXPECT_EQ(m_manager->GetPrinterCount(), count);  // No change
}

// ========== Test 9: LoadFromStorage calls repository Load ==========
TEST_F(DependencyInjectionTest, LoadFromStorage_CallsRepositoryLoad) {
    m_manager->LoadFromStorage();
    
    EXPECT_GE(m_mockRepo->m_loadCallCount, 1);
}

// ========== Test 10: Multiple observers all receive notifications ==========
TEST_F(DependencyInjectionTest, MultipleObservers_AllReceiveNotifications) {
    MockObserver observer2;
    m_manager->Attach(&observer2);
    
    Printer printer("PRN003", "Canon PIXMA", 
                    PrinterBrand::CANON, PrinterStatus::ACTIVE, 
                    "2024-03-01", 36);
    
    m_manager->AddPrinter(printer);
    
    EXPECT_EQ(m_observer->m_notifyCount, 1);
    EXPECT_EQ(observer2.m_notifyCount, 1);
    
    m_manager->Detach(&observer2);
}

// ========== Test 11: Detached observer no longer receives notifications ==========
TEST_F(DependencyInjectionTest, DetachedObserver_NoLongerReceivesNotifications) {
    MockObserver observer2;
    m_manager->Attach(&observer2);
    m_manager->Detach(&observer2);
    
    Printer printer("PRN003", "Canon PIXMA", 
                    PrinterBrand::CANON, PrinterStatus::ACTIVE, 
                    "2024-03-01", 36);
    
    m_manager->AddPrinter(printer);
    
    EXPECT_EQ(m_observer->m_notifyCount, 1);
    EXPECT_EQ(observer2.m_notifyCount, 0);
}