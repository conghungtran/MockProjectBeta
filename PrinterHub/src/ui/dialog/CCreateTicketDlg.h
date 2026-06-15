#pragma once
#include "afxdialogex.h"


// CCreateTicketDlg dialog

class CCreateTicketDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCreateTicketDlg)

public:
	CCreateTicketDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CCreateTicketDlg();
	BOOL OnInitDialog();

	void SetTicketInfo(const CString& printerName, const CString& model, const CString& fwVersion);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TICKETS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStatic ticket_printerName;
	CStatic ticketModel;
	CStatic ticketVersion;
	CEdit ticketTitle;
	CEdit ticketDetail;
	CComboBox ticketPriority;

	CString cstr_ticket_printerName;
	CString cstr_ticketModel;
	CString cstr_ticketVersion;
	CString cstr_ticketTitle;
	CString cstr_ticketDetail;
	CString cstr_ticketPriority;

};
