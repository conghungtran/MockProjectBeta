// AppConfig.h
#pragma once
#include <afx.h>

namespace PrinterHub {
    namespace Core {

        class AppConfig {
        private:
            AppConfig();  // Singleton
            ~AppConfig();

        public:
            // Singleton instance
            static AppConfig& GetInstance();

            // Load/Save config
            bool Load();
            bool Save();

            // Getter/Setter cho data path
            CString GetDataPath() const { return m_dataPath; }
            void SetDataPath(const CString& path);

            // Các đường dẫn được xây dựng từ dataPath
            CString GetCsvFilePath() const;
            CString GetLogFilePath() const;
            CString GetFirmwareDirPath() const;
            CString GetBackupDirPath() const;

            // Đường dẫn mặc định
            static CString GetDefaultDataPath();
            static CString GetConfigFilePath();

            // Tạo thư mục nếu chưa có
            bool EnsureDirectoriesExist();

        private:
            CString m_dataPath;      // Chỉ 1 biến duy nhất!
            CString m_configFilePath;
            bool m_isLoaded;
        };

#define APP_CONFIG PrinterHub::Core::AppConfig::GetInstance()

    }
}