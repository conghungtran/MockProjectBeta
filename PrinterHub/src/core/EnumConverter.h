// EnumConverter.h
#pragma once
#include <afx.h>
#include <map>
#include "PrinterConfig.h"

namespace PrinterHub {
    namespace Core {
        class EnumConverter {
        public:
            // Convert String → PrinterStatus
            static PrinterStatus ToPrinterStatus(const CString& strStatus)
            {
                static const std::map<CString, PrinterStatus> map = {
                    {_T("ACTIVE"), PrinterStatus::ACTIVE},
                    {_T("INSERVICE"), PrinterStatus::INSERVICE},
                    {_T("RETIRED"), PrinterStatus::RETIRED},
                };

                auto it = map.find(strStatus);
                return (it != map.end()) ? it->second : PrinterStatus::ACTIVE;
            }

            static CString FromPrinterStatus(PrinterStatus status)
            {
                static const std::map<PrinterStatus, CString> map = {
                    {PrinterStatus::ACTIVE, _T("ACTIVE")},
                    {PrinterStatus::INSERVICE, _T("INSERVICE")},
                    {PrinterStatus::RETIRED, _T("RETIRED")},
                };

                auto it = map.find(status);
                return (it != map.end()) ? it->second : _T("ACTIVE");
            }

            // Convert String → PrinterBrand
            static PrinterBrand ToPrinterBrand(const CString& strPrinterBrand)
            {
                static const std::map<CString, PrinterBrand> map = {
                    {_T("HP"), PrinterBrand::HP},
                    {_T("CANON"), PrinterBrand::CANON},
                    {_T("EPSON"), PrinterBrand::EPSON},
                    {_T("OTHER"), PrinterBrand::OTHER},
                };

                auto it = map.find(strPrinterBrand);
                return (it != map.end()) ? it->second : PrinterBrand::OTHER;
            }

            static CString FromPrinterBrand(PrinterBrand printerBrand)
            {
                static const std::map<PrinterBrand, CString> map = {
                    {PrinterBrand::HP, _T("HP")},
                    {PrinterBrand::CANON, _T("CANON")},
                    {PrinterBrand::EPSON, _T("EPSON")},
                    {PrinterBrand::OTHER, _T("OTHER")},
                };

                auto it = map.find(printerBrand);
                return (it != map.end()) ? it->second : _T("OTHER");
            }

            // Convert String → Priority
            static PrinterPriority ToPriority(const CString& strPriority)
            {
                static const std::map<CString, PrinterPriority> map = {
                    {_T("LOW"), PrinterPriority::LOW},
                    {_T("MEDIUM"), PrinterPriority::MEDIUM},
                    {_T("HIGH"), PrinterPriority::HIGH},
                    {_T("CRITICAL"), PrinterPriority::CRITICAL},
                };

                auto it = map.find(strPriority);
                return (it != map.end()) ? it->second : PrinterPriority::MEDIUM;
            }

            static CString FromPriority(PrinterPriority priority)
            {
                static const std::map<PrinterPriority, CString> map = {
                    {PrinterPriority::LOW, _T("LOW")},
                    {PrinterPriority::MEDIUM, _T("MEDIUM")},
                    {PrinterPriority::HIGH, _T("HIGH")},
                    {PrinterPriority::CRITICAL, _T("CRITICAL")},
                };

                auto it = map.find(priority);
                return (it != map.end()) ? it->second : _T("MEDIUM");
            }
        };
    }
}