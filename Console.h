
/** Console.h (2023.06.16) P. Stuer **/

#pragma once

class Console
{
public:
    void Create(HWND hWnd, bool wordrap, bool darkMode) noexcept;

    void Delete() noexcept
    {
        if (_hRichEdit)
        {
            ::DestroyWindow(_hRichEdit);
            _hRichEdit = 0;
        }
    }

    void Show() const noexcept { ::ShowWindow(_hRichEdit, SW_SHOW); }
    void Hide() const noexcept { ::ShowWindow(_hRichEdit, SW_HIDE); }
    void Redraw() const noexcept { ::RedrawWindow(_hRichEdit, nullptr, 0, RDW_INVALIDATE | RDW_NOERASE); }

    void Clear() const noexcept { ::SetWindowTextW(_hRichEdit, L""); }

    void Write(LPCWSTR format, ...) const noexcept;

    HWND Handle() const noexcept { return _hRichEdit; }
    bool DarkMode() const noexcept { return _DarkMode; }
    
    void SetDefaultTextColor(COLORREF color) noexcept;
    void SetBold(bool enabled) noexcept;
    void SetItalic(bool enabled) noexcept;

    void SetFont(HFONT hFont) const noexcept;

    void SetBullet(bool enabled) const noexcept;

    void IncreaseIndent() const noexcept;
    void DecreaseIndent() const noexcept;

    void SetBackgroundColor(DWORD color) const noexcept { ::SendMessageW(_hRichEdit, EM_SETBKGNDCOLOR, 0, (LPARAM) color); }
    void SetFormat() const noexcept { ::SendMessageW(_hRichEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &_Format); }
    void Select(int from, int to) const noexcept { ::SendMessageW(_hRichEdit, EM_SETSEL, (WPARAM) from, to); }
    void ScrollToTop() const noexcept { ::SendMessageW(_hRichEdit, WM_VSCROLL, SB_TOP, 0L); }

private:
    // 20 twips / printer's point; 72 printer's points / inch; 1440 twips / inch.
    LONG InchToTwips(float inch) const { return (LONG)(1440.f * inch); }

private:
    HWND _hRichEdit;
    CHARFORMAT _Format = { sizeof(_Format) };
    bool _DarkMode;
};
