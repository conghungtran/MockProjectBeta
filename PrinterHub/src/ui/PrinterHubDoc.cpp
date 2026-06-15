
// PrinterHubDoc.cpp : implementation of the CPrinterHubDoc class
//

#include "pch.h"
#include "framework.h"
#include "../core/ControllerStatus.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "PrinterHub.h"
#endif

#include "PrinterHubDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "io/CsvReader.h"
#include "io/CsvWriter.h"
#include "core/ConvertData.h"
#include "core/Enumconverter.h"
#include "../core/CommandPattern/AddPrinterCommand.h"
#include "../core/CommandPattern/EditPrinterCommand.h"
#include "../core/CommandPattern/DeletePrinterCommand.h"


using namespace PrinterHub::Core;

// CPrinterHubDoc

IMPLEMENT_DYNCREATE(CPrinterHubDoc, CDocument)

BEGIN_MESSAGE_MAP(CPrinterHubDoc, CDocument)
END_MESSAGE_MAP()

// CPrinterHubDoc construction/destruction

CPrinterHubDoc::CPrinterHubDoc() noexcept : m_manager(std::make_shared<PrinterManager>())  // ✅ Khởi tạo ngay
{
	// Document đăng ký làm observer của PrinterManager
	m_manager.get()->Attach(this);

}

CPrinterHubDoc::~CPrinterHubDoc()
{
	m_manager.get()->Detach(this);
}

void CPrinterHubDoc::SetPrinterManager(std::shared_ptr<PrinterManager> manager)
{
	m_manager = manager;
	if (m_manager) {
		m_manager->Attach(this);
	}
}

void CPrinterHubDoc::SetRepository(std::shared_ptr<IPrinterRepository> repository)
{
	if (m_manager) {
		m_manager->SetRepository(repository);
	}
}

bool CPrinterHubDoc::LoadFromStorage()
{
	// ✅ Kiểm tra m_manager có null không
	if (!m_manager) {
		std::cout << "CPrinterHubDoc::LoadFromStorage: m_manager is null\n";
		return false;
	}

	if (!m_manager->GetRepository()) {
		std::cout << "CPrinterHubDoc::LoadFromStorage: No repository set\n";
		return false;
	}

	std::vector<Printer> printers;
	if (m_manager->GetRepository()->Load(printers)) {
		// Clear and reload

		std::cout << "CPrinterHubDoc::LoadFromStorage: Loaded printers: " << printers.size() << "\n";
		m_manager->ClearAll();
		for (const auto& printer : printers) {
			std::cout << printer.getModel() << std::endl;
			m_manager->LoadPrinter(printer);
		}
		return true;
	}
	return false;
}

bool CPrinterHubDoc::SaveToStorage()
{
	if (!m_manager.get()->GetRepository()) return false;
	return m_manager.get()->GetRepository()->Save(m_manager.get()->GetAllPrinters());
}



void CPrinterHubDoc::AddPrinter(const Printer& printer)
{
	if (!m_manager) return;

	auto command = std::make_unique<AddPrinterCommand>(m_manager.get(), printer);
	m_manager->ExecuteCommand(std::move(command));
}

void CPrinterHubDoc::UpdatePrinter(int index, const Printer& newPrinter)
{
	if (!m_manager) return;

	const Printer& oldPrinter = m_manager->GetPrinter(index);
	auto command = std::make_unique<EditPrinterCommand>(
		m_manager.get(), index, oldPrinter, newPrinter);
	m_manager->ExecuteCommand(std::move(command));
}

void CPrinterHubDoc::DeletePrinter(int index)
{
	if (!m_manager) return;

	const Printer& printer = m_manager->GetPrinter(index);
	auto command = std::make_unique<DeletePrinterCommand>(
		m_manager.get(), index, printer);
	m_manager->ExecuteCommand(std::move(command));
}

void CPrinterHubDoc::ClearAllPrinters()
{
	m_manager.get()->ClearAll();
}

const Printer& CPrinterHubDoc::GetPrinter(int index) const
{
	return m_manager.get()->GetPrinter(index);
}

int CPrinterHubDoc::GetPrinterCount() const
{
	return m_manager.get()->GetPrinterCount();
}

void CPrinterHubDoc::Undo()
{
	if (m_manager) m_manager->Undo();
}

void CPrinterHubDoc::Redo()
{
	if (m_manager) m_manager->Redo();
}

bool CPrinterHubDoc::CanUndo() const
{
	return m_manager ? m_manager->CanUndo() : false;
}

bool CPrinterHubDoc::CanRedo() const
{
	return m_manager ? m_manager->CanRedo() : false;
}

void CPrinterHubDoc::OnPrinterChanged(PrinterEvent event, int index)
{
	// Chuyển đổi event thành hint cho View
	LPARAM lHint = 0;
	switch (event)
	{
	case PrinterEvent::PrinterAdded:
		lHint = DocumentStatus::PRINTER_ADDED;
		break;
	case PrinterEvent::PrinterUpdated:
		lHint = DocumentStatus::PRINTER_UPDATED;
		break;
	case PrinterEvent::PrinterDeleted:
		lHint = DocumentStatus::PRINTER_DELETED;
		break;
	case PrinterEvent::PrintersCleared:
		lHint = DocumentStatus::PRINTERS_CLEARED;
		break;
	}

	// Thông báo cho View
	UpdateAllViews(nullptr, lHint, (CObject*)(INT_PTR)index);
}

BOOL CPrinterHubDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

void CPrinterHubDoc::MinusTimer() {
	for (auto o : m_firmwareTable) {
		o.setProgress(o.getProgress() - 1);
		std::cout << " progress: " << o.getProgress() << std::endl;
	}
}




// CPrinterHubDoc serialization

void CPrinterHubDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CPrinterHubDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CPrinterHubDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CPrinterHubDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CPrinterHubDoc diagnostics

#ifdef _DEBUG
void CPrinterHubDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPrinterHubDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CPrinterHubDoc commands
