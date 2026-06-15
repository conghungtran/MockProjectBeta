
// PrinterHubDoc.h : interface of the CPrinterHubDoc class
//


#pragma once
#include "../core/Printer.h"
#include "../core/PrinterManager.h"
#include "../core/repository/IPrinterRepository.h"
#include "../core/firmware/Firmware.h"
#include "../core/Ticket.h"

using namespace PrinterHub::Core;


class CPrinterHubDoc : public CDocument, public PrinterHub::Core::IObserver
{
protected: // create from serialization only
	CPrinterHubDoc() noexcept;
	DECLARE_DYNCREATE(CPrinterHubDoc)

// Attributes
public:
	//CArray<PrinterHub::Core::Printer, PrinterHub::Core::Printer> m_arrPrinters;
// Operations
public:

	// Dependency Injection - Set repository
	//void SetRepository(std::shared_ptr<IPrinterRepository> repository);

	 // Delegate CRUD operations to PrinterManager
	void AddPrinter(const PrinterHub::Core::Printer& printer);
	void UpdatePrinter(int index, const PrinterHub::Core::Printer& printer);
	void DeletePrinter(int index);
	void ClearAllPrinters();

	// Load/Save
	bool LoadFromStorage();
	bool SaveToStorage();

	const PrinterHub::Core::Printer& GetPrinter(int index) const;
	int GetPrinterCount() const;

	// IObserver implementation
	void OnPrinterChanged(PrinterHub::Core::PrinterEvent event, int index) override;

	// For testing - get reference to manager
	//PrinterManager& GetPrinterManager() { return m_manager; }
	
	// Undo/Redo support
	void Undo();
	void Redo();
	bool CanUndo() const;
	bool CanRedo() const;
	// Setter

	void SetPrinterManager(std::shared_ptr<PrinterManager> manager);
	void SetRepository(std::shared_ptr<IPrinterRepository> repository);
	void MinusTimer();
public:
	std::shared_ptr<PrinterManager> m_manager;
	std::vector<Firmware> m_firmwareTable;
	TicketManager m_ticketManager;
	//bool m_isDataModified;

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);


#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CPrinterHubDoc();

	//const CArray<PrinterInfo, PrinterInfo&>& GetPrinters() const { return m_arrPrinters; }

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
