// CCreateTicketDlg.cpp : implementation file
//

#include "pch.h"
#include "afxdialogex.h"
#include "CCreateTicketDlg.h"
#include "ui/resource.h"


// CCreateTicketDlg dialog

IMPLEMENT_DYNAMIC(CCreateTicketDlg, CDialogEx)

CCreateTicketDlg::CCreateTicketDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TICKETS, pParent)
{

}

BOOL CCreateTicketDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    UpdateData(FALSE);
    ticketPriority.AddString(_T("Critical"));
    ticketPriority.AddString(_T("High"));
    ticketPriority.AddString(_T("Medium"));
    ticketPriority.AddString(_T("Low"));
    return TRUE;
}

CCreateTicketDlg::~CCreateTicketDlg()
{
}


void CCreateTicketDlg::SetTicketInfo(
	const CString& printerName
	, const CString& model
	, const CString& fwVersion) 
{
	cstr_ticket_printerName = printerName;
	cstr_ticketModel = model;
	cstr_ticketVersion = fwVersion;
}


void CCreateTicketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TICKET_NAME, ticket_printerName);
	DDX_Control(pDX, IDC_STATIC_TICKET_MODEL, ticketModel);
	DDX_Control(pDX, IDC_STATIC_TICKET_VERSIOn, ticketVersion);
	DDX_Control(pDX, IDC_EDIT_TICKET_TITLE, ticketTitle);
	DDX_Control(pDX, IDD_DIALOG_TICKET_DETAIL2, ticketDetail);
	DDX_Control(pDX, IDC_COMBO_TICKET_PRIORITY, ticketPriority);


	DDX_Text(pDX, IDC_STATIC_TICKET_NAME, cstr_ticket_printerName);
	DDX_Text(pDX, IDC_STATIC_TICKET_MODEL, cstr_ticketModel);
	DDX_Text(pDX, IDC_STATIC_TICKET_VERSIOn, cstr_ticketVersion);
	DDX_Text(pDX, IDC_EDIT_TICKET_TITLE, cstr_ticketTitle);
	DDX_Text(pDX, IDD_DIALOG_TICKET_DETAIL2, cstr_ticketDetail);
	DDX_Text(pDX, IDC_COMBO_TICKET_PRIORITY, cstr_ticketPriority);
}


BEGIN_MESSAGE_MAP(CCreateTicketDlg, CDialogEx)
END_MESSAGE_MAP()


// CCreateTicketDlg message handlers
