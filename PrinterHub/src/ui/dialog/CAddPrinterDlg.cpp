// CAddPrinterDlg.cpp : implementation file
//

#include "pch.h"
#include "../PrinterHub.h"
#include "afxdialogex.h"
#include "CAddPrinterDlg.h"
#include <iostream>
#include "../../core/EnumConverter.h"
#include "../../core/ConvertData.h"

// CAddPrinterDlg dialog

IMPLEMENT_DYNAMIC(CAddPrinterDlg, CDialogEx)

CAddPrinterDlg::CAddPrinterDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG_ADD_PRINTER, pParent),
    m_mode(ModeAdd),
	cstr_Id(_T("")),
	cstr_Model(_T("")),
	cstr_Brand(_T("")),
	cstr_Status(_T("")),
	int_WarrantyMonth(0)

{

  std::cout << "Created Dialog" << std::endl;
}

// MyDialog.cpp
BOOL CAddPrinterDlg::OnInitDialog()
{
    CDialog::OnInitDialog();



    UpdateData(FALSE);

    if (m_mode == ModeEdit)
    {
        GetDlgItem(IDC_EDIT_ADD_PRINTER_ID)->EnableWindow(FALSE);
        SetWindowText(_T("Edit Printer"));
        // Đổi text nút OK thành "Update"
        //GetDlgItem(IDOK)->SetWindowText(_T("&Update"));

        std::cout << "Edit Mode\n";
        std::cout << "Brand: " << CT2A(cstr_Brand) << "\n";
        std::cout << "Status: " << CT2A(cstr_Status) << "\n";
        // Load Brands
        m_cboBrand.AddString(_T("HP"));
        m_cboBrand.AddString(_T("CANON"));
        m_cboBrand.AddString(_T("EPSON"));
        m_cboBrand.AddString(_T("OTHER"));
        m_cboBrand.SetCurSel(0);
        // Load Brands
        m_cboStatus.AddString(_T("ACTIVE"));
        m_cboStatus.AddString(_T("RETIRED"));
        m_cboStatus.AddString(_T("INSERVICE"));
        m_cboStatus.SetCurSel(0);  // Set default

        if (m_cboBrand.SelectString(0, cstr_Brand) == CB_ERR)
        {
            m_cboBrand.SetCurSel(3);  // Chọn OTHER nếu không tìm thấy
        }

        if (m_cboStatus.SelectString(0, cstr_Status) == CB_ERR)
        {
            m_cboStatus.SetCurSel(0);  // Chọn ACTIVE nếu không tìm thấy
        }

        // Chuyển đổi CString → COleDateTime
        COleDateTime dt;
        int  nMonth, nDay, nYear;
        
        // Parse định dạng DD/MM/YYYY
        if (swscanf_s(cstr_PurchaseDate, _T("%d/%d/%d"), &nMonth, &nDay, &nYear) == 3)
        {
			std::cout << nMonth << "/" << nDay << "/" << nYear << "\n";
            // Tạo COleDateTime (thứ tự: Năm, Tháng, Ngày)
            dt.SetDate(nYear, nMonth, nDay);
			cstr_PurchaseDate = dt.Format(_T("%m/%d/%Y"));  // Chuyển về định dạng MM/DD/YYYY để hiển thị
            if (dt.GetStatus() == COleDateTime::valid)
            {
                // ✅ Set vào CDateTimeCtrl
                m_dtpPurchaseDate.SetTime(dt);
            }
        }

    }

	if (m_mode == ModeAdd)
	{
		SetWindowText(_T("Add Printer"));
		// Đổi text nút OK thành "Add and Continue"
		//GetDlgItem(IDOK)->SetWindowText(_T("&Add and Continue"));
         // Load Brands
        m_cboBrand.AddString(_T("HP"));
        m_cboBrand.AddString(_T("CANON"));
        m_cboBrand.AddString(_T("EPSON"));
        m_cboBrand.AddString(_T("OTHER"));
        m_cboBrand.SetCurSel(0);

        // Load Brands
        m_cboStatus.AddString(_T("ACTIVE"));
        m_cboStatus.AddString(_T("RETIRED"));
        m_cboStatus.AddString(_T("INSERVICE"));
        m_cboStatus.SetCurSel(0);
	}



    m_btnCustom.SetWindowText(_T("OK"));
    m_btnCustom.SetBackgroundColor(RGB(0, 100, 200));
    m_btnCustom.SetHoverColor(RGB(0, 80, 170));
    m_btnCustom.SetPressedColor(RGB(0, 60, 140));
    m_btnCustom.SetTextColor(RGB(255, 255, 255));
    m_btnCustom.SetBorderColor(RGB(0, 50, 120));
    m_btnCustom.SetCornerRadius(8);
    m_btnCustom.SetButtonFont(18, _T("Segoe UI"));


    m_editModel.SetPlaceholder(_T("Enter Model"));
    m_editModel.SetBorderColor(RGB(0, 120, 215));


    m_editId.SetPlaceholder(_T("Enter ID"));
    m_editId.SetBorderColor(RGB(0, 120, 215));


    return TRUE;
}

void CAddPrinterDlg::SetEditData(
                    const CString& CstrId
                   ,const CString& CstrModel
                   ,const CString& CstrBrand
                   ,const CString& CstrStatus
                   ,const CString& CstrPurchaseDate
                   ,const int intWarrantyMonth)
{

    std::cout << ConvertData::CStringToString(CstrId) << "\n";
    cstr_Id = CstrId;
	cstr_Model = CstrModel;
	cstr_Brand = CstrBrand;
    cstr_Status = CstrStatus;
	cstr_PurchaseDate = CstrPurchaseDate;
    int_WarrantyMonth = intWarrantyMonth;
	
}

CAddPrinterDlg::~CAddPrinterDlg()
{
}

void CAddPrinterDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_ADD_PRINTER_ID, m_editId);

    DDX_Control(pDX, IDC_EDIT_ADD_PRINTER_MODEL, m_editModel);
    DDX_Control(pDX, IDC_EDIT_ADD_PRINTER_BRAND, m_cboBrand);
    DDX_Control(pDX, IDC_EDIT_ADD_PRINTER_DATE, m_dtpPurchaseDate);
    DDX_Control(pDX, IDC_EDIT_ADD_PRINTER_WARRANTY, m_editWarrantyMonth);
    DDX_Control(pDX, IDC_EDIT_ADD_PRINTER_STATUS, m_cboStatus);

    DDX_Control(pDX, IDC_BUTTON_ADD_CONTINUE, m_btnCustom);

    // Automatically map
    DDX_Text(pDX,IDC_EDIT_ADD_PRINTER_ID, cstr_Id);
    DDX_Text(pDX, IDC_EDIT_ADD_PRINTER_MODEL, cstr_Model);
    DDX_Text(pDX, IDC_EDIT_ADD_PRINTER_BRAND, cstr_Brand);
    DDX_Text(pDX, IDC_EDIT_ADD_PRINTER_STATUS, cstr_Status);
    DDX_Text(pDX, IDC_EDIT_ADD_PRINTER_DATE, cstr_PurchaseDate);
    DDX_Text(pDX, IDC_EDIT_ADD_PRINTER_WARRANTY, int_WarrantyMonth);

}


BEGIN_MESSAGE_MAP(CAddPrinterDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_ADD_CONTINUE, &CAddPrinterDlg::OnBnClickedButtonAddContinue)
    ON_EN_CHANGE(IDC_EDIT_ADD_PRINTER_ID, &CAddPrinterDlg::OnEnChangeEditAddPrinterId)
END_MESSAGE_MAP()


// CAddPrinterDlg message handlers

void CAddPrinterDlg::OnBnClickedButtonAddContinue()
{
    UpdateData(TRUE);

    //if (cstr_Id.IsEmpty())
    //{
    //    AfxMessageBox(_T("ID cannot be empty"));
    //    return;
    //}

    //if (cstr_Model.IsEmpty())
    //{
    //    AfxMessageBox(_T("Model cannot be empty"));
    //    return;
    //}

    //if (cstr_Brand.IsEmpty())
    //{
    //    AfxMessageBox(_T("Brand cannot be empty"));
    //    return;
    //}

    //if (cstr_PurchaseDate.IsEmpty())
    //{
    //    AfxMessageBox(_T("Purchase cannot be empty"));
    //    return;
    //}

    //if (int_WarrantyMonth == 0)
    //{
    //    AfxMessageBox(_T("Warranty cannot be empty"));
    //    return;
    //}

    std::cout << "CAdPrinterDlg Close\n";
    CDialogEx::OnOK();
}

void CAddPrinterDlg::GetPrinter(PrinterHub::Core::Printer& printer) const
{
    using namespace PrinterHub::Core;

    // Chuyển đổi dữ liệu
    std::string str_id = ConvertData::CStringToString(cstr_Id);
    std::string str_model = ConvertData::CStringToString(cstr_Model);
    std::string str_purchaseDate = ConvertData::CStringToString(cstr_PurchaseDate);

    PrinterStatus status = EnumConverter::ToPrinterStatus(cstr_Status);
    PrinterBrand brand = EnumConverter::ToPrinterBrand(cstr_Brand);

    printer = Printer(str_id, str_model, brand, status, str_purchaseDate, int_WarrantyMonth);

}


void CAddPrinterDlg::OnEnChangeEditAddPrinterId()
{
   
}
