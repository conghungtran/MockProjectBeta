// Ticket.h
#pragma once
#include <string>
#include <vector>
#include <afx.h>
#include <vector>
#include <ATLComTime.h>

namespace PrinterHub {
    namespace Core {

        // ========== ENUMS ==========

        // Độ ưu tiên của ticket
        enum class Priority {
            Critical,   // Nguy cấp - ảnh hưởng nhiều máy
            High,       // Cao - ảnh hưởng sản xuất
            Medium,     // Trung bình - có thể chờ
            Low         // Thấp - không ảnh hưởng nhiều
        };

        // Trạng thái của ticket
        enum class TicketStatus {
            Open,           // Mới tạo, chưa xử lý
            InProgress,     // Đang xử lý
            Closed,         // Đã đóng
            Reopen,         // Mở lại sau khi đóng
            Cancelled       // Đã hủy
        };

        // ========== TICKET NOTE CLASS ==========

        class TicketNote {
        public:
            TicketNote();
            TicketNote(const std::string& content, const std::string& author);
            ~TicketNote();

            // Getters
            std::string getContent() const { return m_content; }
            std::string getAuthor() const { return m_author; }
            COleDateTime getTimestamp() const { return m_timestamp; }

            // Setters
            void setContent(const std::string& content) { m_content = content; }
            void setAuthor(const std::string& author) { m_author = author; }
            void setTimestamp(COleDateTime timestamp) { m_timestamp = timestamp; }

            // Helper
            CString getFormattedTimestamp() const;
            std::string toString() const;

        private:
            std::string m_content;      // Nội dung ghi chú
            std::string m_author;       // Người ghi chú
            COleDateTime m_timestamp;   // Thời gian ghi chú
        };

        // ========== TICKET ATTACHMENT CLASS ==========

        class TicketAttachment {
        public:
            TicketAttachment();
            TicketAttachment(const std::string& fileName, const std::string& filePath);
            ~TicketAttachment();

            // Getters
            std::string getFileName() const { return m_fileName; }
            std::string getFilePath() const { return m_filePath; }
            int getFileSize() const { return m_fileSize; }
            COleDateTime getUploadTime() const { return m_uploadTime; }

            // Setters
            void setFileName(const std::string& name) { m_fileName = name; }
            void setFilePath(const std::string& path) { m_filePath = path; }
            void setFileSize(int size) { m_fileSize = size; }

        private:
            std::string m_fileName;     // Tên file
            std::string m_filePath;     // Đường dẫn file
            int m_fileSize;             // Kích thước (bytes)
            COleDateTime m_uploadTime;  // Thời gian upload
        };

        // ========== TICKET CLASS ==========

        class Ticket {
        public:
            // ========== Constructors ==========
            Ticket();
            Ticket(int id, const std::string& printerId, const std::string& title);
            ~Ticket();

            // ========== Copy & Assignment ==========
            Ticket(const Ticket& other);
            Ticket& operator=(const Ticket& other);

            // ========== Getters ==========
            int getId() const { return m_id; }
            std::string getPrinterId() const { return m_printerId; }
            std::string getPrinterName() const { return m_printerName; }
            std::string getTitle() const { return m_title; }
            std::string getDescription() const { return m_description; }
            Priority getPriority() const { return m_priority; }
            TicketStatus getStatus() const { return m_status; }
            std::string getReporter() const { return m_reporter; }
            std::string getAssignee() const { return m_assignee; }
            std::string getSolution() const { return m_solution; }
            std::string getReopenReason() const { return m_reopenReason; }
            COleDateTime getCreatedDate() const { return m_createdDate; }
            COleDateTime getStartTime() const { return m_startTime; }
            COleDateTime getClosedDate() const { return m_closedDate; }
            COleDateTime getReopenDate() const { return m_reopenDate; }
            int getDurationMinutes() const;

            // Notes & Attachments
            std::vector<TicketNote> getNotes() const { return m_notes; }
            std::vector<TicketAttachment> getAttachments() const { return m_attachments; }

            // ========== Setters ==========
            void setId(int id) { m_id = id; }
            void setPrinterId(const std::string& printerId) { m_printerId = printerId; }
            void setPrinterName(const std::string& printerName) { m_printerName = printerName; }
            void setTitle(const std::string& title) { m_title = title; }
            void setDescription(const std::string& description) { m_description = description; }
            void setPriority(Priority priority) { m_priority = priority; }
            void setStatus(TicketStatus status) { m_status = status; }
            void setReporter(const std::string& reporter) { m_reporter = reporter; }
            void setAssignee(const std::string& assignee) { m_assignee = assignee; }
            void setSolution(const std::string& solution) { m_solution = solution; }
            void setReopenReason(const std::string& reason) { m_reopenReason = reason; }
            void setCreatedDate(COleDateTime date) { m_createdDate = date; }
            void setStartTime(COleDateTime time) { m_startTime = time; }
            void setClosedDate(COleDateTime date) { m_closedDate = date; }
            void setReopenDate(COleDateTime date) { m_reopenDate = date; }

            // Notes & Attachments
            void addNote(const TicketNote& note);
            void addAttachment(const TicketAttachment& attachment);
            void clearNotes();
            void clearAttachments();

            // ========== Helper Methods ==========

            // Chuyển đổi enum ↔ String
            static std::string PriorityToString(Priority priority);
            static Priority StringToPriority(const std::string& str);
            static std::string StatusToString(TicketStatus status);
            static TicketStatus StringToStatus(const std::string& str);

            // Kiểm tra trạng thái
            bool isOpen() const { return m_status == TicketStatus::Open; }
            bool isInProgress() const { return m_status == TicketStatus::InProgress; }
            bool isClosed() const { return m_status == TicketStatus::Closed; }
            bool isReopen() const { return m_status == TicketStatus::Reopen; }
            bool isCancelled() const { return m_status == TicketStatus::Cancelled; }

            // Lấy màu sắc cho UI
            COLORREF getStatusColor() const;
            COLORREF getPriorityColor() const;

            // Hiển thị
            CString getPriorityCString() const;
            CString getStatusCString() const;
            std::string toString() const;

            // Time formatting
            CString getFormattedCreatedDate() const;
            CString getFormattedClosedDate() const;

        private:
            // ========== Basic Information ==========
            int m_id;                       // Mã ticket (tự tăng)
            std::string m_printerId;        // ID máy in bị lỗi
            std::string m_printerName;      // Tên máy in
            std::string m_title;            // Tiêu đề ngắn gọn
            std::string m_description;      // Mô tả chi tiết
            Priority m_priority;            // Độ ưu tiên
            TicketStatus m_status;          // Trạng thái

            // ========== Personnel ==========
            std::string m_reporter;         // Người báo cáo
            std::string m_assignee;         // Người xử lý

            // ========== Resolution ==========
            std::string m_solution;         // Giải pháp
            std::string m_reopenReason;     // Lý do mở lại

            // ========== Timestamps ==========
            COleDateTime m_createdDate;     // Ngày tạo
            COleDateTime m_startTime;       // Thời gian bắt đầu xử lý
            COleDateTime m_closedDate;      // Ngày đóng
            COleDateTime m_reopenDate;      // Ngày mở lại

            // ========== Notes & Attachments ==========
            std::vector<TicketNote> m_notes;            // Các ghi chú
            std::vector<TicketAttachment> m_attachments; // File đính kèm
        };

        // ========== TICKET MANAGER CLASS ==========

        class TicketManager {
        public:
            TicketManager();
            ~TicketManager();

            // CRUD Operations
            void addTicket(const Ticket& ticket);
            void updateTicket(const Ticket& ticket);
            void deleteTicket(int id);
            Ticket* getTicket(int id);
            std::vector<Ticket>& getAllTickets() { return m_tickets; }  // Non-const
            const std::vector<Ticket>& getAllTickets() const { return m_tickets; }  // Const

            // Query by filters
            std::vector<Ticket> getTicketsByPrinter(const std::string& printerId) const;
            std::vector<Ticket> getTicketsByStatus(TicketStatus status) const;
            std::vector<Ticket> getTicketsByPriority(Priority priority) const;
            std::vector<Ticket> getTicketsByReporter(const std::string& reporter) const;
            std::vector<Ticket> getOpenTickets() const;
            std::vector<Ticket> getTicketsInDateRange(COleDateTime from, COleDateTime to) const;

            // Statistics
            int getOpenTicketCount() const;
            int getTotalTicketCount() const;
            int getAverageResolutionTime() const;

            // Serialization
            bool saveToFile(const CString& filePath);
            bool loadFromFile(const CString& filePath);

        private:
            std::vector<Ticket> m_tickets;
            int m_nextId;

            int generateId();
        };

        // ========== HELPER FUNCTIONS ==========

        // Chuyển đổi Priority sang CString (cho MFC UI)
        inline CString PriorityToCString(Priority priority) {
            return CString(Ticket::PriorityToString(priority).c_str());
        }

        inline CString TicketStatusToCString(TicketStatus status) {
            return CString(Ticket::StatusToString(status).c_str());
        }
    }
}