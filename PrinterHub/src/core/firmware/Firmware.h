// Firmware.h
#pragma once
#include <string>
#include <afx.h>
#include "core/PrinterConfig.h"

namespace PrinterHub {
    namespace Core {

        // Enum cho Firmware update status
        enum class FirmwareUpdateStatus {
            Pending,        // Đang chờ
            Downloading,    // Đang tải
            Verifying,      // Đang xác thực
            Updating,       // Đang cập nhật
            Completed,      // Hoàn thành
            Failed,         // Thất bại
            Cancelled       // Đã hủy
        };

        class Firmware
        {
        public:
            // ========== Constructors & Destructor ==========
            Firmware();
            Firmware(
                const std::string& id,
                PrinterBrand brand,
                const std::string& version,
                const std::string& releaseDate,
                const std::string& diskSpace);
            ~Firmware();

            // ========== Copy & Assignment ==========
            Firmware(const Firmware& other);
            Firmware& operator=(const Firmware& other);

            // ========== Getters ==========
            std::string getId() const { return m_id; }
            PrinterBrand getBrand() const { return m_brand; }
            std::string getVersion() const { return m_version; }
            std::string getReleaseDate() const { return m_releaseDate; }
            std::string getDiskSpace() const { return m_diskSpace; }
            FirmwareUpdateStatus getUpdateStatus() const { return m_updateStatus; }
            int getProgress() const { return m_progress; }
            std::string getErrorMessage() const { return m_errorMessage; }

            // ========== Setters ==========
            void setId(const std::string& id) { m_id = id; }
            void setBrand(PrinterBrand brand) { m_brand = brand; }
            void setVersion(const std::string& version) { m_version = version; }
            void setReleaseDate(const std::string& date) { m_releaseDate = date; }
            void setDiskSpace(const std::string& space) { m_diskSpace = space; }
            void setUpdateStatus(FirmwareUpdateStatus status) { m_updateStatus = status; }
            void setProgress(int progress) { m_progress = progress; }
            void setErrorMessage(const std::string& error) { m_errorMessage = error; }

            // ========== Helper Methods ==========

            // Chuyển đổi FirmwareUpdateStatus ↔ String
            static std::string UpdateStatusToString(FirmwareUpdateStatus status);
            static FirmwareUpdateStatus StringToUpdateStatus(const std::string& str);

            // Chuyển đổi PrinterBrand ↔ String
            static std::string BrandToString(PrinterBrand brand);
            static PrinterBrand StringToBrand(const std::string& str);

            // Kiểm tra trạng thái
            bool isPending() const { return m_updateStatus == FirmwareUpdateStatus::Pending; }
            bool isProcessing() const {
                return m_updateStatus == FirmwareUpdateStatus::Downloading ||
                    m_updateStatus == FirmwareUpdateStatus::Verifying ||
                    m_updateStatus == FirmwareUpdateStatus::Updating;
            }
            bool isCompleted() const { return m_updateStatus == FirmwareUpdateStatus::Completed; }
            bool isFailed() const { return m_updateStatus == FirmwareUpdateStatus::Failed; }

            // Lấy màu sắc cho UI
            COLORREF getStatusColor() const;

            // Lấy tên brand dạng CString (cho MFC)
            CString getBrandCString() const;

            // Hiển thị thông tin
            std::string toString() const;

        public:
            std::string m_id;                   // Mã firmware
            PrinterBrand m_brand;               // Hãng máy in (HP, Canon, Epson, Other)
            std::string m_version;              // Phiên bản (vd: v1.3.0)
            std::string m_releaseDate;          // Ngày phát hành (vd: 2024-06-01)
            std::string m_diskSpace;            // Dung lượng (vd: 4.2 MB)
            FirmwareUpdateStatus m_updateStatus; // Trạng thái cập nhật
            int m_progress;                     // Tiến độ (0-100)
            std::string m_errorMessage;         // Thông báo lỗi (nếu có)
        };

    }
}