#pragma once

#include <afx.h>  // For CObject
#include <iostream>
#include <string>
#include <vector>

#include "PrinterConfig.h"

namespace PrinterHub {
    namespace Core {
        class Printer : public CObject {
            DECLARE_DYNAMIC(Printer)  // Thêm macro này

        private:
            std::string ID;
            std::string model;
            PrinterBrand brand;
            PrinterStatus status;
            std::string purchaseDate;
            int warrantyMonth;

        public:
            std::vector<std::string> m_idFirmware;  // ID của printer
        public:
            // Constructor
            Printer(std::string ID = ""
                , std::string model = ""
                , PrinterBrand brand = PrinterBrand::OTHER
                , PrinterStatus status = PrinterStatus::ACTIVE
                , std::string purchaseDate = ""
                , int warrantyMonth = 12
            );

            // Destructor
            ~Printer();

            // Copy constructor & assignment
            Printer(const Printer& other);
            Printer& operator=(const Printer& other);
            // Move operator
            Printer& operator=(Printer&& other) noexcept
            {
                if (this != &other)
                {
                    // Move tất cả dữ liệu từ other sang this
                    ID = std::move(other.ID);
                    model = std::move(other.model);
                    brand = other.brand;
                    status = other.status;
                    purchaseDate = std::move(other.purchaseDate);
                    warrantyMonth = other.warrantyMonth;

                    // Reset other về trạng thái mặc định
                    other.ID.clear();
                    other.model.clear();
                    other.brand = PrinterBrand::OTHER;
                    other.status = PrinterStatus::ACTIVE;
                    other.purchaseDate.clear();
                    other.warrantyMonth = 0;
                }
                return *this;
            }


            // Operators
            bool operator==(const Printer& other) const;
            bool operator>(const Printer& other) const;
            friend std::ostream& operator<<(std::ostream& os, const Printer& p);

            // Getters
            std::string getId() const { return ID; }
            std::string getModel() const { return model; }
            PrinterBrand getBrand() const { return brand; }
            PrinterStatus getStatus() const { return status; }
            std::string getPurchaseDate() const { return purchaseDate; }
            int getWarrantyMonth() const { return warrantyMonth; }  // Sửa typo: getWarantyMonth → getWarrantyMonth


            // ✅ Setters (thêm mới)
            void setId(const std::string& id) { ID = id; }
            void setModel(const std::string& mdl) { model = mdl; }
            void setBrand(PrinterBrand b) { brand = b; }
            void setStatus(PrinterStatus s) { status = s; }
            void setPurchaseDate(const std::string& date) { purchaseDate = date; }
            void setWarrantyMonth(int months) { warrantyMonth = months; }

            // Hoặc gộp lại cho tiện
            void setAll(const std::string& mdl,
                PrinterBrand b, PrinterStatus s,
                const std::string& date, int months);
        };
    }
}