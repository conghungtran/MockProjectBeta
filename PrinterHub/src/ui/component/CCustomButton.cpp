#include "pch.h"
#include "CCustomButton.h"

IMPLEMENT_DYNAMIC(CCustomButton, CButton)

CCustomButton::CCustomButton()
    : m_clrBackground(RGB(0, 80, 200))      // Xanh dương đậm
    , m_clrText(RGB(255, 255, 255))         // Trắng
    , m_clrHover(RGB(0, 60, 170))           // Xanh đậm hơn khi hover
    , m_clrPressed(RGB(0, 40, 140))         // Xanh rất đậm khi nhấn
    , m_clrBorder(RGB(0, 50, 120))          // Viền xanh đậm
    , m_bHover(false)
    , m_bPressed(false)
    , m_nCornerRadius(200)                   // Bo góc 10 pixels
{
}

CCustomButton::~CCustomButton()
{
}

BEGIN_MESSAGE_MAP(CCustomButton, CButton)
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()

void CCustomButton::PreSubclassWindow()
{
    ModifyStyle(0, BS_OWNERDRAW);
    CButton::PreSubclassWindow();
}

void CCustomButton::DrawRoundedRect(CDC& dc, CRect rect, COLORREF color, int radius)
{
    CBrush brush(color);
    CPen pen(PS_SOLID, 1, color);

    CBrush* pOldBrush = dc.SelectObject(&brush);
    CPen* pOldPen = dc.SelectObject(&pen);

    dc.RoundRect(rect, CPoint(radius, radius));

    dc.SelectObject(pOldBrush);
    dc.SelectObject(pOldPen);
}

void CCustomButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC dc;
    dc.Attach(lpDrawItemStruct->hDC);

    CRect rect = lpDrawItemStruct->rcItem;

    // Chọn màu dựa vào trạng thái
    COLORREF clrBg = m_clrBackground;

    if (lpDrawItemStruct->itemState & ODS_SELECTED || m_bPressed)
    {
        clrBg = m_clrPressed;  // Màu khi đang nhấn
    }
    else if (m_bHover)
    {
        clrBg = m_clrHover;    // Màu khi hover
    }

    // Vẽ nền có bo góc
    DrawRoundedRect(dc, rect, clrBg, m_nCornerRadius);

    // Vẽ viền - CÁCH 1: Dùng FrameRect (Đơn giản hơn, không lỗi)
    CPen pen(PS_SOLID, 1, m_clrBorder);
    CPen* pOldPen = dc.SelectObject(&pen);
    CBrush* pOldBrush = (CBrush*)dc.SelectStockObject(NULL_BRUSH);
    dc.RoundRect(rect, CPoint(m_nCornerRadius, m_nCornerRadius));
    dc.SelectObject(pOldPen);
    dc.SelectObject(pOldBrush);

    // Lấy text
    CString strText;
    GetWindowText(strText);

    // Thiết lập font
    CFont* pOldFont = dc.SelectObject(&m_font);

    // Vẽ text với hiệu ứng shadow (bóng đổ)
    dc.SetBkMode(TRANSPARENT);

    // Shadow (bóng đổ) - tạo hiệu ứng nổi
    if (!(lpDrawItemStruct->itemState & ODS_SELECTED) && !m_bPressed)
    {
        dc.SetTextColor(RGB(0, 30, 80));
        CRect shadowRect = rect;
        shadowRect.OffsetRect(1, 1);
        dc.DrawText(strText, shadowRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    // Text chính
    dc.SetTextColor(m_clrText);
    dc.DrawText(strText, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // Khôi phục
    dc.SelectObject(pOldFont);
    dc.Detach();
}

void CCustomButton::OnMouseMove(UINT nFlags, CPoint point)
{
    if (!m_bHover)
    {
        m_bHover = true;
        Invalidate();  // Vẽ lại button

        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(tme);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = m_hWnd;
        _TrackMouseEvent(&tme);
    }

    CButton::OnMouseMove(nFlags, point);
}

void CCustomButton::OnMouseLeave()
{
    m_bHover = false;
    m_bPressed = false;
    Invalidate();  // Vẽ lại button
    CButton::OnMouseLeave();
}

void CCustomButton::SetButtonFont(int nHeight, const CString& strFaceName)
{
    m_font.DeleteObject();
    m_font.CreateFont(nHeight, 0, 0, 0, FW_BOLD, FALSE, FALSE,
        0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, strFaceName);
    SetFont(&m_font);
}