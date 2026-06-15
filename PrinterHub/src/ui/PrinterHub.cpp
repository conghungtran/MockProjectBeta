
// PrinterHub.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "PrinterHub.h"
#include "MainFrm.h"

#include "PrinterHubDoc.h"
#include "CPrinterHubView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <iostream>
#include "../core/repository/CSVPrinterRepository.h"


// PrinterHubApp

BEGIN_MESSAGE_MAP(PrinterHubApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &PrinterHubApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// PrinterHubApp construction

PrinterHubApp::PrinterHubApp() noexcept
{
	m_bHiColorIcons = TRUE;


	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("PrinterHub.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only PrinterHubApp object

PrinterHubApp theApp;


// PrinterHubApp initialization

BOOL PrinterHubApp::InitInstance()
{



	// Tạo console window
	if (!AllocConsole()) {
		// Xử lý lỗi nếu cần
	}

	// Chuyển hướng stdout/stderr đến console
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONOUT$", "w", stderr);
	freopen_s(&fp, "CONIN$", "r", stdin);

	// Bây giờ có thể dùng cout, printf, v.v.
	std::cout << "Welcome to MFC: \n" << std::endl;


	
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CPrinterHubDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CPrinterHubView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 4. Inject repository into document (Dependency Injection!)
	CFrameWnd* pMainFrame = (CFrameWnd*)m_pMainWnd;

	CPrinterHubDoc* pDoc = (CPrinterHubDoc*)pMainFrame->GetActiveDocument();
	if (pDoc) {
		//pDoc->m_manager->SetUpFirmware();
		//auto manager = std::make_shared<PrinterManager>();
		CString strFilePath = GetCSVFilePath(); // Lấy đường dẫn
		auto repository = std::make_shared<CSVPrinterRepository>(strFilePath);

		// 2. Inject repository vào manager
		pDoc->m_manager->SetRepository(repository);

		// 3. Inject manager vào document
		//pDoc->SetPrinterManager(manager);

		if (pDoc->LoadFromStorage()) {
			std::cout << "LoadStorage Sucessful \n";
		}
		else {
			std::cout << "Error LoadStorage Sucessful \n";
		}
		std::cout << "Load Repo Sucessful \n";
	}
	else {
		std::cout << "Error for loading repo \n";
	}

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

CString PrinterHubApp::GetCSVFilePath()
{
	TCHAR szCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szCurrentDir);

	CString strFilePath;
	strFilePath.Format(_T("%s\\data\\printers.csv"), szCurrentDir);

	// Nếu không tìm thấy, thử lên 1 cấp
	if (GetFileAttributes(strFilePath) == INVALID_FILE_ATTRIBUTES) {
		strFilePath.Format(_T("%s\\..\\data\\printers.csv"), szCurrentDir);
	}

	return strFilePath;
}

int PrinterHubApp::ExitInstance()
{
	//TODO: handle additional resources you may have added

	FreeConsole();
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// PrinterHubApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void PrinterHubApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// PrinterHubApp customization load/save methods

void PrinterHubApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void PrinterHubApp::LoadCustomState()
{
}

void PrinterHubApp::SaveCustomState()
{
}

// PrinterHubApp message handlers



