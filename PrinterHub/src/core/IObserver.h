// IObserver.h
#pragma once

namespace PrinterHub {
    namespace Core {
        enum class PrinterEvent {
            PrinterAdded,
            PrinterUpdated,
            PrinterDeleted,
            PrintersCleared,
            PrintersSorted
        };

        class IObserver {
        public:
            virtual ~IObserver() = default;
            virtual void OnPrinterChanged(PrinterEvent event, int index = -1) = 0;
        };

        class IObservable {
        public:
            virtual ~IObservable() = default;
            virtual void Attach(IObserver* observer) = 0;
            virtual void Detach(IObserver* observer) = 0;
            virtual void Notify(PrinterEvent event, int index = -1) = 0;
        };

        class MockObserver : public IObserver {
        public:
            struct Notification {
                PrinterEvent event;
                int index;
            };

            std::vector<Notification> notifications;
            int notifyCount = 0;

            void OnPrinterChanged(PrinterEvent event, int index) override {
                notifications.push_back({ event, index });
                notifyCount++;
            }

            void Reset() {
                notifications.clear();
                notifyCount = 0;
            }

            bool WasNotified(PrinterEvent event, int index = -1) const {
                for (const auto& n : notifications) {
                    if (n.event == event && (index == -1 || n.index == index)) {
                        return true;
                    }
                }
                return false;
            }
        };
    }
}