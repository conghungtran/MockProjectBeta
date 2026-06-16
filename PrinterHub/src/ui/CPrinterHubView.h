
// CPrinterHubView.h : interface of the CPrinterHubView class
//

#pragma once

class CPrinterHubView : public CFormView
{
protected: // create from serialization only
	CPrinterHubView() noexcept;
	DECLARE_DYNCREATE(CPrinterHubView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_PRINTERHUB_FORM };
#endif

// Attributes
public:
	CPrinterHubDoc* GetDocument() const;
	
// Operations
public:
	void CPrinterHubView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	void CPrinterHubView::OnDraw(CDC* pDC);

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	
	void UpdatePrinterInList(int nIndex, const PrinterHub::Core::Printer& printer);
	void LoadFile();
	void CPrinterHubView::RefreshFirmwareQueue();

	void AddFirmwareToList();
	//void UpdateFirmwareInList(int nIndex, const PrinterHub::Core::Firmware& firmware);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
// Implementation
public:
	virtual ~CPrinterHubView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listFirmWare;
	CListCtrl m_listTicket;
	CListCtrl m_listPrinters;



	void DeletePrinterFromList(int nIndex);

	void RefreshPrintersList();
	void RefreshTicketsList();
	//void UpdateUndoRedoButtons();

	//void UpdateStatusBar();

	void InitializeListControl();
	void AddTicketToList(
		const CString& id,
		const CString& title,
		const CString& printer,
		const CString& priority,
		const CString& status);

	void AddPrinterToList(

		const CString& strId,
		const CString& strModel,
		const CString& strBrand,
		const CString& strStatus,
		const CString& strPurchaseDate,
		const int& intWarrantyMonth);

	afx_msg; void OnBnClickedButton8();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnLvnItemchangedList3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButtonPrinterEditPrinter();
	afx_msg void OnBnClickedButtonPrinterDeletePrinter();
	afx_msg void OnBnClickedButtonPrinterUndo();
	afx_msg void OnBnClickedButtonPrinterRedo();
	afx_msg void OnBnClickedButtonPrinterUpdateFirmware();


	// Timer

	// Timer handler
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	// Start/Stop timer
	void StartTimer();
	void StopTimer();

	UINT_PTR m_nTimerID;
	bool m_bTimerRunning;
	int m_nCounter = 0;
	int randomAZ(int n, int m);

	afx_msg void OnBnClickedButtonFirmwareProcessQueue();
	CStatic m_static_time;
	afx_msg void OnBnClickedButtonTicketNewTicket();
	afx_msg void OnBnClickedButtonTicketCloseTicket();
	afx_msg void OnBnClickedButtonTicketRemove();
	afx_msg void OnBnClickedButtonSortId();
	CComboBox m_combo_sort;
	afx_msg void OnCbnSelchangeComboCoboboxSort();
};

#ifndef _DEBUG  // debug version in CPrinterHubView.cpp
inline CPrinterHubDoc* CPrinterHubView::GetDocument() const
   { return reinterpret_cast<CPrinterHubDoc*>(m_pDocument); }
#endif

