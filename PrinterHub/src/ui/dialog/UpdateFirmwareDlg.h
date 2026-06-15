#pragma once
#include "afxdialogex.h"


// UpdateFirmwareDlg dialog

class UpdateFirmwareDlg : public CDialogEx
{
	DECLARE_DYNAMIC(UpdateFirmwareDlg)

public:
	UpdateFirmwareDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~UpdateFirmwareDlg();
	BOOL OnInitDialog();

	void SetPrinterInfo(const CString& printerName, const CString& model, const CString& currentVersion);
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_UPDATE_FIRMWARE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Custom message for progress update
	afx_msg LRESULT OnProgressUpdate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDownloadComplete(WPARAM wParam, LPARAM lParam);


	DECLARE_MESSAGE_MAP()

public:
	CString cstr_fw_printer;
	CString cstr_model;
	CString cstr_current_version;
	CString cstr_choose_version;

	CString cstr_detail_version;
	CString cstr_detail_rl_date;
	CString cstr_detail_disk_space;
	CString m_strSelectedVersion;

public:
	CButton m_lbPrinter; // NO

	CStatic m_fw_printer;
	CStatic m_fw_model;
	CStatic m_fw_current_version;
	CComboBox m_combobox_version;
	CButton btn_dowload;
	CStatic detail_version;
	CStatic detail_rl_date;
	CStatic detail_disk_space;
	afx_msg void OnBnClickedButtonUpdatefwDownload();
	afx_msg void OnCbnSelchangeComboVersion();
	CProgressCtrl m_progressCtrl;

	// Thread
	CWinThread* m_pDownloadThread;
	bool m_bDownloading;

	// UI update methods
	void EnableControls(bool bEnable);
	void SetProgress(int nPercent, const CString& strStatus);
	void OnDownloadStarted();
	void OnDownloadFinished(bool bSuccess);
	void UpdateVersionDetail(const CString& version);

	CStatic m_staticProgressStatus;
	CButton m_btnOk;
};

// Custom messages
#define WM_PROGRESS_UPDATE  (WM_USER + 100)
#define WM_DOWNLOAD_COMPLETE (WM_USER + 101)