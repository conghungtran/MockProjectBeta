#include "pch.h"
#include "Ticket.h"
#include <sstream>
#include <algorithm>

namespace PrinterHub {
    namespace Core {

        // ============================================
        // TICKET NOTE IMPLEMENTATION
        // ============================================

        TicketNote::TicketNote()
            : m_content("")
            , m_author("")
        {
            m_timestamp = COleDateTime::GetCurrentTime();
        }

        TicketNote::TicketNote(const std::string& content, const std::string& author)
            : m_content(content)
            , m_author(author)
        {
            m_timestamp = COleDateTime::GetCurrentTime();
        }

        TicketNote::~TicketNote()
        {
        }

        CString TicketNote::getFormattedTimestamp() const
        {
            return m_timestamp.Format(_T("%Y-%m-%d %H:%M:%S"));
        }

        std::string TicketNote::toString() const
        {
            std::stringstream ss;
            ss << "[" << CT2A(getFormattedTimestamp()) << "] "
                << m_author << ": " << m_content;
            return ss.str();
        }

        // ============================================
        // TICKET ATTACHMENT IMPLEMENTATION
        // ============================================

        TicketAttachment::TicketAttachment()
            : m_fileName("")
            , m_filePath("")
            , m_fileSize(0)
        {
            m_uploadTime = COleDateTime::GetCurrentTime();
        }

        TicketAttachment::TicketAttachment(const std::string& fileName, const std::string& filePath)
            : m_fileName(fileName)
            , m_filePath(filePath)
            , m_fileSize(0)
        {
            m_uploadTime = COleDateTime::GetCurrentTime();
        }

        TicketAttachment::~TicketAttachment()
        {
        }

        // ============================================
        // TICKET IMPLEMENTATION
        // ============================================

        Ticket::Ticket()
            : m_id(0)
            , m_priority(Priority::Medium)
            , m_status(TicketStatus::Open)
        {
            m_createdDate = COleDateTime::GetCurrentTime();
        }

        Ticket::Ticket(int id, const std::string& printerId, const std::string& title)
            : m_id(id)
            , m_printerId(printerId)
            , m_title(title)
            , m_priority(Priority::Medium)
            , m_status(TicketStatus::Open)
        {
            m_createdDate = COleDateTime::GetCurrentTime();
        }

        Ticket::~Ticket()
        {
        }

        Ticket::Ticket(const Ticket& other)
            : m_id(other.m_id)
            , m_printerId(other.m_printerId)
            , m_printerName(other.m_printerName)
            , m_title(other.m_title)
            , m_description(other.m_description)
            , m_priority(other.m_priority)
            , m_status(other.m_status)
            , m_reporter(other.m_reporter)
            , m_assignee(other.m_assignee)
            , m_solution(other.m_solution)
            , m_reopenReason(other.m_reopenReason)
            , m_createdDate(other.m_createdDate)
            , m_startTime(other.m_startTime)
            , m_closedDate(other.m_closedDate)
            , m_reopenDate(other.m_reopenDate)
            , m_notes(other.m_notes)
            , m_attachments(other.m_attachments)
        {
        }

        Ticket& Ticket::operator=(const Ticket& other)
        {
            if (this != &other) {
                m_id = other.m_id;
                m_printerId = other.m_printerId;
                m_printerName = other.m_printerName;
                m_title = other.m_title;
                m_description = other.m_description;
                m_priority = other.m_priority;
                m_status = other.m_status;
                m_reporter = other.m_reporter;
                m_assignee = other.m_assignee;
                m_solution = other.m_solution;
                m_reopenReason = other.m_reopenReason;
                m_createdDate = other.m_createdDate;
                m_startTime = other.m_startTime;
                m_closedDate = other.m_closedDate;
                m_reopenDate = other.m_reopenDate;
                m_notes = other.m_notes;
                m_attachments = other.m_attachments;
            }
            return *this;
        }

        int Ticket::getDurationMinutes() const
        {
            if (m_status == TicketStatus::Closed && m_closedDate.GetStatus() == COleDateTime::valid) {
                COleDateTimeSpan span = m_closedDate - m_createdDate;
                return (int)span.GetTotalMinutes();
            }
            return 0;
        }

        void Ticket::addNote(const TicketNote& note)
        {
            m_notes.push_back(note);
        }

        void Ticket::addAttachment(const TicketAttachment& attachment)
        {
            m_attachments.push_back(attachment);
        }

        void Ticket::clearNotes()
        {
            m_notes.clear();
        }

        void Ticket::clearAttachments()
        {
            m_attachments.clear();
        }

        // ========== ENUM CONVERSIONS ==========

        std::string Ticket::PriorityToString(Priority priority)
        {
            switch (priority) {
            case Priority::Critical: return "Critical";
            case Priority::High:     return "High";
            case Priority::Medium:   return "Medium";
            case Priority::Low:      return "Low";
            default:                 return "Medium";
            }
        }

        Priority Ticket::StringToPriority(const std::string& str)
        {
            if (str == "Critical") return Priority::Critical;
            if (str == "High")     return Priority::High;
            if (str == "Medium")   return Priority::Medium;
            if (str == "Low")      return Priority::Low;
            return Priority::Medium;
        }

        std::string Ticket::StatusToString(TicketStatus status)
        {
            switch (status) {
            case TicketStatus::Open:        return "Open";
            case TicketStatus::InProgress:  return "In Progress";
            case TicketStatus::Closed:      return "Closed";
            case TicketStatus::Reopen:      return "Reopen";
            case TicketStatus::Cancelled:   return "Cancelled";
            default:                        return "Open";
            }
        }

        TicketStatus Ticket::StringToStatus(const std::string& str)
        {
            if (str == "Open")         return TicketStatus::Open;
            if (str == "In Progress")  return TicketStatus::InProgress;
            if (str == "Closed")       return TicketStatus::Closed;
            if (str == "Reopen")       return TicketStatus::Reopen;
            if (str == "Cancelled")    return TicketStatus::Cancelled;
            return TicketStatus::Open;
        }

        // ========== UI HELPER METHODS ==========

        COLORREF Ticket::getStatusColor() const
        {
            switch (m_status) {
            case TicketStatus::Open:        return RGB(255, 193, 7);    // Vàng
            case TicketStatus::InProgress:  return RGB(0, 120, 215);    // Xanh dương
            case TicketStatus::Closed:      return RGB(40, 167, 69);    // Xanh lá
            case TicketStatus::Reopen:      return RGB(255, 120, 0);    // Cam
            case TicketStatus::Cancelled:   return RGB(108, 117, 125);  // Xám
            default:                        return RGB(0, 0, 0);
            }
        }

        COLORREF Ticket::getPriorityColor() const
        {
            switch (m_priority) {
            case Priority::Critical: return RGB(220, 53, 69);   // Đỏ
            case Priority::High:     return RGB(255, 120, 0);   // Cam
            case Priority::Medium:   return RGB(255, 193, 7);   // Vàng
            case Priority::Low:      return RGB(40, 167, 69);   // Xanh lá
            default:                 return RGB(0, 0, 0);
            }
        }

        CString Ticket::getPriorityCString() const
        {
            return CString(PriorityToString(m_priority).c_str());
        }

        CString Ticket::getStatusCString() const
        {
            return CString(StatusToString(m_status).c_str());
        }

        CString Ticket::getFormattedCreatedDate() const
        {
            if (m_createdDate.GetStatus() == COleDateTime::valid) {
                return m_createdDate.Format(_T("%d/%m/%Y %H:%M"));
            }
            return _T("");
        }

        CString Ticket::getFormattedClosedDate() const
        {
            if (m_closedDate.GetStatus() == COleDateTime::valid) {
                return m_closedDate.Format(_T("%d/%m/%Y %H:%M"));
            }
            return _T("");
        }

        std::string Ticket::toString() const
        {
            std::stringstream ss;
            ss << "Ticket{id=" << m_id
                << ", printer=" << m_printerId
                << ", title=" << m_title
                << ", priority=" << PriorityToString(m_priority)
                << ", status=" << StatusToString(m_status)
                << ", created=" << CT2A(getFormattedCreatedDate())
                << "}";
            return ss.str();
        }

        // ============================================
        // TICKET MANAGER IMPLEMENTATION
        // ============================================

        TicketManager::TicketManager()
            : m_nextId(1)
        {
        }

        TicketManager::~TicketManager()
        {
        }

        int TicketManager::generateId()
        {
            return m_nextId++;
        }

        void TicketManager::addTicket(const Ticket& ticket)
        {
            Ticket newTicket = ticket;
            if (newTicket.getId() == 0) {
                newTicket.setId(generateId());
            }
            m_tickets.push_back(newTicket);
        }

        void TicketManager::updateTicket(const Ticket& ticket)
        {
            for (auto& t : m_tickets) {
                if (t.getId() == ticket.getId()) {
                    t = ticket;
                    break;
                }
            }
        }

        void TicketManager::deleteTicket(int id)
        {
            auto it = std::remove_if(m_tickets.begin(), m_tickets.end(),
                [id](const Ticket& t) { return t.getId() == id; });
            m_tickets.erase(it, m_tickets.end());
        }

        Ticket* TicketManager::getTicket(int id)
        {
            for (auto& t : m_tickets) {
                if (t.getId() == id) {
                    return &t;
                }
            }
            return nullptr;
        }

        //std::vector<Ticket> TicketManager::getAllTickets() const
        //{
        //    return m_tickets;
        //}

        std::vector<Ticket> TicketManager::getTicketsByPrinter(const std::string& printerId) const
        {
            std::vector<Ticket> result;
            for (const auto& t : m_tickets) {
                if (t.getPrinterId() == printerId) {
                    result.push_back(t);
                }
            }
            return result;
        }

        std::vector<Ticket> TicketManager::getTicketsByStatus(TicketStatus status) const
        {
            std::vector<Ticket> result;
            for (const auto& t : m_tickets) {
                if (t.getStatus() == status) {
                    result.push_back(t);
                }
            }
            return result;
        }

        std::vector<Ticket> TicketManager::getOpenTickets() const
        {
            std::vector<Ticket> result;
            for (const auto& t : m_tickets) {
                if (t.isOpen() || t.isInProgress() || t.isReopen()) {
                    result.push_back(t);
                }
            }
            return result;
        }

        int TicketManager::getOpenTicketCount() const
        {
            return (int)getOpenTickets().size();
        }

        int TicketManager::getTotalTicketCount() const
        {
            return (int)m_tickets.size();
        }
    }
}