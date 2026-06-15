// mocks/MockObserver.h
#pragma once
#include "../../PrinterHubCore/core/IObserver.h"
#include <vector>

namespace PrinterHub {
    namespace Core {
        
        class MockObserver : public IObserver {
        public:
            struct Notification {
                PrinterEvent event;
                int index;
            };
            
            std::vector<Notification> m_notifications;
            int m_notifyCount = 0;
            
            void OnPrinterChanged(PrinterEvent event, int index) override {
                m_notifications.push_back({event, index});
                m_notifyCount++;
            }
            
            void Reset() {
                m_notifications.clear();
                m_notifyCount = 0;
            }
            
            bool WasNotified(PrinterEvent event, int index = -1) const {
                for (const auto& n : m_notifications) {
                    if (n.event == event && (index == -1 || n.index == index)) {
                        return true;
                    }
                }
                return false;
            }
        };
        
    }
}