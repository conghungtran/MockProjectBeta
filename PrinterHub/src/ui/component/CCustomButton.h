#pragma once

class CCustomButton : public CButton
{
    DECLARE_DYNAMIC(CCustomButton)

public:
    CCustomButton();
    virtual ~CCustomButton();

    // Màu sắc
    void SetBackgroundColor(COLORREF color) { m_clrBackground = color; }
    void SetTextColor(COLORREF color) { m_clrText = color; }
    void SetHoverColor(COLORREF color) { m_clrHover = color; }
    void SetPressedColor(COLORREF color) { m_clrPressed = color; }
    void SetBorderColor(COLORREF color) { m_clrBorder = color; }

    // Font chữ
    void SetButtonFont(int nHeight, const CString& strFaceName = _T("Segoe UI"));

    // Bo góc
    void SetCornerRadius(int nRadius) { m_nCornerRadius = nRadius; }

    // Set text
    void SetButtonText(const CString& strText) { SetWindowText(strText); }

protected:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) override;
    virtual void PreSubclassWindow() override;

    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseLeave();

    DECLARE_MESSAGE_MAP()

private:
    void DrawRoundedRect(CDC& dc, CRect rect, COLORREF color, int radius);

    COLORREF m_clrBackground;   // Màu nền
    COLORREF m_clrText;         // Màu chữ
    COLORREF m_clrHover;        // Màu khi hover
    COLORREF m_clrPressed;      // Màu khi nhấn
    COLORREF m_clrBorder;       // Màu viền
    bool m_bHover;              // Đang hover?
    bool m_bPressed;            // Đang nhấn?
    int m_nCornerRadius;        // Độ bo góc (pixels)
    CFont m_font;               // Font chữ
};