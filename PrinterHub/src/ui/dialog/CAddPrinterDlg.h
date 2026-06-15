#pragma once
#include "afxdialogex.h"
#include "../../core/Printer.h"
#include "../component/CCustomButton.h"
#include "../component/CCustomEdit.h"


// CAddPrinterDlg dialog

class CAddPrinterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddPrinterDlg)

public:
	CAddPrinterDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CAddPrinterDlg();
	BOOL OnInitDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ADD_PRINTER };
#endif

public:
	CString cstr_Id;
	CString cstr_Model;
	CString cstr_Brand;
	CString cstr_Status;
	CString cstr_PurchaseDate;
	int		int_WarrantyMonth = 0;
	void GetPrinter(PrinterHub::Core::Printer& printer) const;


	// Enum để biết là Add hay Edit
	enum DialogMode { ModeAdd, ModeEdit };
	void SetMode(DialogMode mode) { m_mode = mode; }
	void SetEditData(
                   const CString& CstrId
                   ,const CString& CstrModel
                   ,const CString& CstrBrand
                   ,const CString& CstrStatus
                   ,const CString& CstrPurchaseDate
                   ,const int intWarrantyMonth
                   );



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DialogMode m_mode;
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cboBrand;
	//CEdit m_editModel;
	CDateTimeCtrl m_dtpPurchaseDate;
	CEdit m_editWarrantyMonth;
	CComboBox m_cboStatus;
	afx_msg void OnBnClickedButtonAddContinue();
	afx_msg void OnEnChangeEditAddPrinterId();

public:
	CCustomButton m_btnCustom;
	CCustomEdit m_editId;

	CCustomEdit m_editModel;
};
