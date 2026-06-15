// IObserver.h
#pragma once

namespace PrinterHub {
    namespace Core {
        enum class PrinterEvent {
            PrinterAdded,
            PrinterUpdated,
            PrinterDeleted,
            PrintersCleared
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
    }
}