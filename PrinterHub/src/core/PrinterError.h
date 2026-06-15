// PrinterError.h
#pragma once
#include <string>
#include <afx.h>

namespace PrinterHub {
    namespace Core {

        enum class PrinterError {
            // Success
            Success = 0,

            // Validation errors (1-99)
            EmptyId = 1,
            EmptyModel,
            InvalidId,
            InvalidModel,
            InvalidWarrantyMonth,
            InvalidPurchaseDate,
            InvalidCharacter,
            IdTooLong,
            ModelTooLong,

            // Business logic errors (100-199)
            DuplicateId = 100,
            PrinterNotFound,
            InvalidIndex,
            ManagerNotInitialized,

            // Storage errors (200-299)
            FileNotFound = 200,
            FileReadError,
            FileWriteError,
            ParseError,
            DatabaseError,
            StorageNotAvailable,

            // Repository errors (300-399)
            RepositoryNotSet = 300,
            RepositoryLoadFailed,
            RepositorySaveFailed,
            RepositoryAppendFailed,
            RepositoryUpdateFailed,
            RepositoryDeleteFailed,

            // Unknown error
            UnknownError = 999
        };

        // Chuyển đổi enum → string (cho console, log)
        inline std::string PrinterErrorToString(PrinterError error) {
            switch (error) {
                // Success
            case PrinterError::Success: return "Success";

                // Validation errors
            case PrinterError::EmptyId: return "ID cannot be empty";
            case PrinterError::EmptyModel: return "Model cannot be empty";
            case PrinterError::InvalidId: return "ID is invalid";
            case PrinterError::InvalidModel: return "Model is invalid";
            case PrinterError::InvalidWarrantyMonth: return "Warranty month must be between 0 and 120";
            case PrinterError::InvalidPurchaseDate: return "Purchase date is invalid (expected DD/MM/YYYY)";
            case PrinterError::InvalidCharacter: return "ID contains invalid characters (allowed: A-Z, a-z, 0-9, -, _, .)";
            case PrinterError::IdTooLong: return "ID is too long (max 50 characters)";
            case PrinterError::ModelTooLong: return "Model name is too long (max 100 characters)";

                // Business logic errors
            case PrinterError::DuplicateId: return "ID already exists";
            case PrinterError::PrinterNotFound: return "Printer not found";
            case PrinterError::InvalidIndex: return "Invalid printer index";
            case PrinterError::ManagerNotInitialized: return "PrinterManager is not initialized";

                // Storage errors
            case PrinterError::FileNotFound: return "CSV file not found";
            case PrinterError::FileReadError: return "Cannot read CSV file";
            case PrinterError::FileWriteError: return "Cannot write to CSV file";
            case PrinterError::ParseError: return "Cannot parse CSV data";
            case PrinterError::DatabaseError: return "Database operation failed";
            case PrinterError::StorageNotAvailable: return "Storage is not available";

                // Repository errors
            case PrinterError::RepositoryNotSet: return "Repository is not set";
            case PrinterError::RepositoryLoadFailed: return "Failed to load from repository";
            case PrinterError::RepositorySaveFailed: return "Failed to save to repository";
            case PrinterError::RepositoryAppendFailed: return "Failed to append to repository";
            case PrinterError::RepositoryUpdateFailed: return "Failed to update repository";
            case PrinterError::RepositoryDeleteFailed: return "Failed to delete from repository";

                // Unknown
            case PrinterError::UnknownError: return "Unknown error";

            default: return "Undefined error";
            }
        }

        // Chuyển đổi enum → CString (cho MFC UI)
        inline CString PrinterErrorToCString(PrinterError error) {
            return CString(PrinterErrorToString(error).c_str());
        }

        // Kiểm tra có thành công không
        inline bool IsSuccess(PrinterError error) {
            return error == PrinterError::Success;
        }

        // Kiểm tra có phải lỗi validation không
        inline bool IsValidationError(PrinterError error) {
            int code = static_cast<int>(error);
            return code >= 1 && code <= 99;
        }

        // Kiểm tra có phải lỗi storage không
        inline bool IsStorageError(PrinterError error) {
            int code = static_cast<int>(error);
            return code >= 200 && code <= 299;
        }

        // Kiểm tra có phải lỗi repository không
        inline bool IsRepositoryError(PrinterError error) {
            int code = static_cast<int>(error);
            return code >= 300 && code <= 399;
        }
    }
}