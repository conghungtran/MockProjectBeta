// UpdateFirmwareDlg.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "UpdateFirmwareDlg.h"
#include "ui/resource.h"
#include <iostream>
#include "core/ConvertData.h"


// UpdateFirmwareDlg dialog

IMPLEMENT_DYNAMIC(UpdateFirmwareDlg, CDialogEx)

UpdateFirmwareDlg::UpdateFirmwareDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_UPDATE_FIRMWARE, pParent)
{
	std::cout << "\nCreated UpdateFirmwareDlg\n";
}

BOOL UpdateFirmwareDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// m_btnOk.EnableWindow(false);
	//m_fw_printer.SetWindowText(cstr_fw_printer);      // Lấy từ biến member
	std::cout << "On init dialog\n:" << ConvertData::CStringToString(cstr_model) << "\n";
	m_fw_model.SetWindowText(cstr_model);              // Lấy từ biến member
	m_fw_current_version.SetWindowText(cstr_current_version); // Lấy từ biến member

	m_combobox_version.AddString(_T("v1.1"));
	m_combobox_version.AddString(_T("v1.2"));
	m_combobox_version.AddString(_T("v1.3"));
	m_combobox_version.AddString(_T("v1.4"));

	m_combobox_version.SetCurSel(0);

	// Khởi tạo Progress Bar
	m_progressCtrl.SetRange(0, 100);
	m_progressCtrl.SetPos(0);
	m_progressCtrl.ShowWindow(SW_HIDE);
	m_staticProgressStatus.ShowWindow(SW_HIDE);

	return TRUE;
}

UpdateFirmwareDlg::~UpdateFirmwareDlg()
{
}

void UpdateFirmwareDlg::SetPrinterInfo(
	const CString& printerName,
	const CString& model,
	const CString& currentVersion) 
{
	cstr_fw_printer = printerName;
	cstr_model = model;
	cstr_current_version = currentVersion;

	std::cout << "SetPrinterInfo called:" << std::endl;
	std::cout << "  printerName: " << CT2A(cstr_fw_printer) << std::endl;
	std::cout << "  model: " << CT2A(cstr_model) << std::endl;
	std::cout << "  currentVersion: " << CT2A(cstr_current_version) << std::endl;

	//if (GetSafeHwnd() != NULL) {
	//	m_fw_printer.SetWindowText(printerName);
	//	m_fw_model.SetWindowText(model);
	//	m_fw_current_version.SetWindowText(currentVersion);
	//}
}

void UpdateFirmwareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_FW_printer, m_fw_printer);
	DDX_Control(pDX, IDC_STATIC_FW_model, m_fw_model);
	DDX_Control(pDX, IDC_STATIC_FW_currentVersion, m_fw_current_version);
	DDX_Control(pDX, IDC_COMBO2, m_combobox_version);
	DDX_Control(pDX, IDC_BUTTON_UPDATEFW_DOWNLOAD, btn_dowload);
	DDX_Control(pDX, IDC_STATIC_FW_VERSION_DETAIL, detail_version);
	DDX_Control(pDX, IDC_STATIC_RL_DATE, detail_rl_date);
	DDX_Control(pDX, IDC_STATIC_DISK_SPACE, detail_disk_space);

	//// Automatically map
	//DDX_Text(pDX, IDC_STATIC_FW_printer, cstr_fw_printer);
	//DDX_Text(pDX, IDC_STATIC_FW_model, cstr_model);
	//DDX_Text(pDX, IDC_STATIC_FW_currentVersion, cstr_current_version);
	//DDX_Text(pDX, IDC_COMBO2, cstr_choose_version);
	//DDX_Text(pDX, IDC_STATIC_FW_VERSION_DETAIL, cstr_detail_version);
	//DDX_Text(pDX, IDC_STATIC_RL_DATE, cstr_detail_rl_date);
	//DDX_Text(pDX, IDC_STATIC_DISK_SPACE, cstr_detail_disk_space);
	DDX_Control(pDX, IDC_PROGRESS1, m_progressCtrl);
	DDX_Control(pDX, IDC_STATIC_DOWNLOADING, m_staticProgressStatus);
	DDX_Control(pDX, IDOK, m_btnOk);
}

// Thread function
UINT DownloadFirmwareThread(LPVOID pParam)
{
	UpdateFirmwareDlg* pDlg = (UpdateFirmwareDlg*)pParam;

	// Giả lập download trong 5 giây
	for (int i = 0; i <= 100; i += 5)
	{
		// Kiểm tra nếu dialog bị hủy
		if (!pDlg->GetSafeHwnd()) {
			return 1;
		}

		// Gửi message cập nhật progress về UI thread
		::PostMessage(pDlg->GetSafeHwnd(), WM_PROGRESS_UPDATE, i, 0);

		// Sleep 250ms * 20 lần = 5 giây
		Sleep(250);
	}

	// Gửi message hoàn thành
	::PostMessage(pDlg->GetSafeHwnd(), WM_DOWNLOAD_COMPLETE, 1, 0);

	return 0;
}

BEGIN_MESSAGE_MAP(UpdateFirmwareDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_UPDATEFW_DOWNLOAD, &UpdateFirmwareDlg::OnBnClickedButtonUpdatefwDownload)
	ON_CBN_SELCHANGE(IDC_COMBO_LIST_FW, &UpdateFirmwareDlg::OnCbnSelchangeComboVersion)
	ON_MESSAGE(WM_PROGRESS_UPDATE, &UpdateFirmwareDlg::OnProgressUpdate)
	ON_MESSAGE(WM_DOWNLOAD_COMPLETE, &UpdateFirmwareDlg::OnDownloadComplete)
END_MESSAGE_MAP()


// UpdateFirmwareDlg message handlers

void UpdateFirmwareDlg::OnBnClickedButtonUpdatefwDownload()
{
	if (m_bDownloading) {
		AfxMessageBox(_T("Đang tải firmware, vui lòng chờ..."));
		return;
	}

	// Lấy version được chọn
	int nSel = m_combobox_version.GetCurSel();
	if (nSel != CB_ERR) {
		m_combobox_version.GetLBText(nSel, m_strSelectedVersion);
	}

	// Disable các control trong khi download
	EnableControls(false);

	// Hiển thị progress bar
	m_progressCtrl.SetPos(0);
	m_progressCtrl.ShowWindow(SW_SHOW);
	m_staticProgressStatus.ShowWindow(SW_SHOW);
	m_staticProgressStatus.SetWindowText(_T("Đang kết nối..."));

	m_bDownloading = true;

	// Tạo thread download
	m_pDownloadThread = AfxBeginThread(DownloadFirmwareThread, this);
}
void UpdateFirmwareDlg::EnableControls(bool bEnable)
{
	m_combobox_version.EnableWindow(bEnable);
	btn_dowload.EnableWindow(bEnable);
	m_btnOk.EnableWindow(bEnable);
}

void UpdateFirmwareDlg::SetProgress(int nPercent, const CString& strStatus)
{
	m_progressCtrl.SetPos(nPercent);
	m_staticProgressStatus.SetWindowText(strStatus);
}

void UpdateFirmwareDlg::OnDownloadStarted()
{
	// Có thể thêm animation hoặc log ở đây
}

void UpdateFirmwareDlg::OnDownloadFinished(bool bSuccess)
{
	m_bDownloading = false;

	if (bSuccess) {
		// Ẩn progress bar
		m_progressCtrl.ShowWindow(SW_HIDE);
		m_staticProgressStatus.ShowWindow(SW_HIDE);

		// Hiển thị thông báo thành công
		CString msg;
		msg.Format(_T("✅ Download firmware %s sucessful!\nAn update can be performed."),
			m_strSelectedVersion);
		AfxMessageBox(msg, MB_ICONINFORMATION);

		// Enable các control
		EnableControls(true);

		// Cập nhật thông tin version detail (nếu có)
		 UpdateVersionDetail(m_strSelectedVersion);

	}
	else {
		m_progressCtrl.ShowWindow(SW_HIDE);
		m_staticProgressStatus.ShowWindow(SW_HIDE);
		AfxMessageBox(_T("❌ Tải firmware thất bại!\nVui lòng kiểm tra kết nối mạng."),
			MB_ICONERROR);
		EnableControls(true);
	}
}

void UpdateFirmwareDlg::OnCbnSelchangeComboVersion()
{
	int nSel = m_combobox_version.GetCurSel();
	if (nSel != CB_ERR)
	{
		CString strSelectedVersion;
		m_combobox_version.GetLBText(nSel, strSelectedVersion);

		// Cập nhật thông tin chi tiết
		UpdateVersionDetail(strSelectedVersion);
	}
}

LRESULT UpdateFirmwareDlg::OnProgressUpdate(WPARAM wParam, LPARAM lParam)
{
	int nPercent = (int)wParam;

	// Cập nhật text trạng thái theo tiến độ
	CString strStatus;
	if (nPercent < 10) {
		strStatus = _T("Connecting to server...");
	}
	else if (nPercent < 30) {
		strStatus = _T("Downloading firmware...");
	}
	else if (nPercent < 60) {
		strStatus = _T("Verifying data...");
	}
	else if (nPercent < 90) {
		strStatus = _T("Almost done...");
	}
	else {
		strStatus = _T("Complete! Processing...");
	}

	m_progressCtrl.SetPos(nPercent);
	m_staticProgressStatus.SetWindowText(strStatus);

	// Cập nhật percentage text (nếu có)
	CString strPercent;
	strPercent.Format(_T("%d%%"), nPercent);
	// m_staticPercent.SetWindowText(strPercent);

	return 0;
}

LRESULT UpdateFirmwareDlg::OnDownloadComplete(WPARAM wParam, LPARAM lParam)
{
	bool bSuccess = (wParam == 1);
	OnDownloadFinished(bSuccess);
	return 0;
}

void UpdateFirmwareDlg::UpdateVersionDetail(const CString& version)
{
	// Ví dụ cập nhật chi tiết dựa trên version
	if (version == _T("v1.1")) {
		detail_version.SetWindowText(_T("Version: v1.1"));
		detail_rl_date.SetWindowText(_T("Release Date: 2024-01-15"));
		detail_disk_space.SetWindowText(_T("Disk Space Required: 50MB"));
	}
	else if (version == _T("v1.2")) {
		detail_version.SetWindowText(_T("Version: v1.2"));
		detail_rl_date.SetWindowText(_T("Release Date: 2024-03-10"));
		detail_disk_space.SetWindowText(_T("Disk Space Required: 75MB"));
	}
	else if (version == _T("v1.3")) {
		detail_version.SetWindowText(_T("Version: v1.3"));
		detail_rl_date.SetWindowText(_T("Release Date: 2024-05-05"));
		detail_disk_space.SetWindowText(_T("Disk Space Required: 100MB"));
	}
	else if (version == _T("v1.4")) {
		detail_version.SetWindowText(_T("Version: v1.4"));
		detail_rl_date.SetWindowText(_T("Release Date: 2024-06-20"));
		detail_disk_space.SetWindowText(_T("Disk Space Required: 150MB"));
	}
	else {
		detail_version.SetWindowText(_T(""));
		detail_rl_date.SetWindowText(_T(""));
		detail_disk_space.SetWindowText(_T(""));
	}
}

//void UpdateFirmwareDlg::OnBnClickedOk()
//{
//	if (m_bDownloading) {
//		if (AfxMessageBox(_T("Đang tải firmware. Hủy bỏ và thoát?"),
//			MB_YESNO | MB_ICONQUESTION) == IDYES) {
//			// Có thể hủy thread ở đây
//			CDialogEx::OnOK();
//		}
//		return;
//	}
//
//	CDialogEx::OnOK();
//}
//
//void UpdateFirmwareDlg::OnBnClickedCancel()
//{
//	if (m_bDownloading) {
//		if (AfxMessageBox(_T("Đang tải firmware. Hủy bỏ và thoát?"),
//			MB_YESNO | MB_ICONQUESTION) != IDYES) {
//			return;
//		}
//	}
//	CDialogEx::OnCancel();
//}
