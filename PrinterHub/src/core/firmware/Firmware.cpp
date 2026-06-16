// Firmware.cpp
#include "pch.h"
#include "Firmware.h"
#include <sstream>
#include <map>

namespace PrinterHub {
    namespace Core {

        // ========== Constructors ==========
        Firmware::Firmware()
            : m_id("")
            , m_printerId("")
            , m_brand(PrinterBrand::OTHER)
            , m_version("")
            , m_releaseDate("")
            , m_diskSpace("")
            , m_updateStatus(FirmwareUpdateStatus::Pending)
            , m_progress(0)
            , m_errorMessage("")
        {
        }

        Firmware::Firmware(
            const std::string& id,
            const std::string& printerId,
            PrinterBrand brand,
            const std::string& version,
            const std::string& releaseDate,
            const std::string& diskSpace)
            : m_id(id)
            , m_printerId(printerId)
            , m_brand(brand)
            , m_version(version)
            , m_releaseDate(releaseDate)
            , m_diskSpace(diskSpace)
            , m_updateStatus(FirmwareUpdateStatus::Pending)
            , m_progress(0)
            , m_errorMessage("")
        {
        }

        Firmware::~Firmware()
        {
        }

        // ========== Copy & Assignment ==========
        Firmware::Firmware(const Firmware& other)
            : m_id(other.m_id)
            , m_printerId(other.m_printerId)
            , m_brand(other.m_brand)
            , m_version(other.m_version)
            , m_releaseDate(other.m_releaseDate)
            , m_diskSpace(other.m_diskSpace)
            , m_updateStatus(other.m_updateStatus)
            , m_progress(other.m_progress)
            , m_errorMessage(other.m_errorMessage)
        {
        }

        Firmware& Firmware::operator=(const Firmware& other)
        {
            if (this != &other) {
                m_id = other.m_id;
                m_printerId = other.m_printerId;
                m_brand = other.m_brand;
                m_version = other.m_version;
                m_releaseDate = other.m_releaseDate;
                m_diskSpace = other.m_diskSpace;
                m_updateStatus = other.m_updateStatus;
                m_progress = other.m_progress;
                m_errorMessage = other.m_errorMessage;
            }
            return *this;
        }

        // ========== Chuyển đổi FirmwareUpdateStatus ↔ String ==========
        std::string Firmware::UpdateStatusToString(FirmwareUpdateStatus status)
        {
            static const std::map<FirmwareUpdateStatus, std::string> statusMap = {
                {FirmwareUpdateStatus::Pending,     "Pending"},
                {FirmwareUpdateStatus::Downloading, "Downloading"},
                {FirmwareUpdateStatus::Verifying,   "Verifying"},
                {FirmwareUpdateStatus::Updating,    "Updating"},
                {FirmwareUpdateStatus::Completed,   "Completed"},
                {FirmwareUpdateStatus::Failed,      "Failed"},
                {FirmwareUpdateStatus::Cancelled,   "Cancelled"}
            };

            auto it = statusMap.find(status);
            return (it != statusMap.end()) ? it->second : "Unknown";
        }

        FirmwareUpdateStatus Firmware::StringToUpdateStatus(const std::string& str)
        {
            static const std::map<std::string, FirmwareUpdateStatus> stringMap = {
                {"Pending",     FirmwareUpdateStatus::Pending},
                {"Downloading", FirmwareUpdateStatus::Downloading},
                {"Verifying",   FirmwareUpdateStatus::Verifying},
                {"Updating",    FirmwareUpdateStatus::Updating},
                {"Completed",   FirmwareUpdateStatus::Completed},
                {"Failed",      FirmwareUpdateStatus::Failed},
                {"Cancelled",   FirmwareUpdateStatus::Cancelled}
            };

            auto it = stringMap.find(str);
            return (it != stringMap.end()) ? it->second : FirmwareUpdateStatus::Pending;
        }

        // ========== Chuyển đổi PrinterBrand ↔ String ==========
        std::string Firmware::BrandToString(PrinterBrand brand)
        {
            switch (brand) {
            case PrinterBrand::HP:      return "HP";
            case PrinterBrand::CANON:   return "CANON";
            case PrinterBrand::EPSON:   return "EPSON";
            case PrinterBrand::OTHER:   return "OTHER";
            default:                    return "UNKNOWN";
            }
        }

        PrinterBrand Firmware::StringToBrand(const std::string& str)
        {
            if (str == "HP")        return PrinterBrand::HP;
            if (str == "CANON")     return PrinterBrand::CANON;
            if (str == "EPSON")     return PrinterBrand::EPSON;
            if (str == "OTHER")     return PrinterBrand::OTHER;
            return PrinterBrand::OTHER;
        }

        // ========== Lấy màu sắc cho UI ==========
        COLORREF Firmware::getStatusColor() const
        {
            switch (m_updateStatus) {
            case FirmwareUpdateStatus::Pending:     return RGB(255, 193, 7);    // Vàng
            case FirmwareUpdateStatus::Downloading: return RGB(0, 120, 215);    // Xanh dương
            case FirmwareUpdateStatus::Verifying:   return RGB(0, 120, 215);    // Xanh dương
            case FirmwareUpdateStatus::Updating:    return RGB(0, 120, 215);    // Xanh dương
            case FirmwareUpdateStatus::Completed:   return RGB(40, 167, 69);    // Xanh lá
            case FirmwareUpdateStatus::Failed:      return RGB(220, 53, 69);    // Đỏ
            case FirmwareUpdateStatus::Cancelled:   return RGB(108, 117, 125);  // Xám
            default:                                return RGB(0, 0, 0);
            }
        }

        // ========== Lấy tên brand dạng CString (cho MFC UI) ==========
        CString Firmware::getBrandCString() const
        {
            return CString(BrandToString(m_brand).c_str());
        }

        // ========== Hiển thị thông tin ==========
        std::string Firmware::toString() const
        {
            std::stringstream ss;
            ss << "Firmware{"
                << "id=" << m_id
                << ", printerId=" << m_printerId
                << ", brand=" << BrandToString(m_brand)
                << ", version=" << m_version
                << ", releaseDate=" << m_releaseDate
                << ", diskSpace=" << m_diskSpace
                << ", status=" << UpdateStatusToString(m_updateStatus)
                << ", progress=" << m_progress << "%"
                << "}";
            return ss.str();
        }

    }
}