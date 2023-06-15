
/** Console.h (2023.06.15) P. Stuer **/

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

    void Clear() const noexcept { ::SetWindowTextW(_hRichEdit, L""); }

    void Write(LPCWSTR format, ...) const noexcept;

    HWND Handle() const noexcept { return _hRichEdit; }

    void SetBackgroundColor(DWORD color) const noexcept;

    void SetTextForeColor(COLORREF color) const noexcept;
    void SetTextBackColor(COLORREF color) const noexcept;
    void ResetTextColor() const noexcept;

    void SetFont(HFONT hFont) const noexcept;
    void SetBold(bool enabled) const noexcept;
    void SetItalic(bool enabled) const noexcept;
    void SetBullet(bool enabled) const noexcept;

    void IncreaseIndent() const noexcept;
    void DecreaseIndent() const noexcept;

    void ScrollToTop() const noexcept { ::SendMessageW(_hRichEdit, WM_VSCROLL, SB_TOP, 0L); }

private:
    // 20 twips / printer's point; 72 printer's points / inch; 1440 twips / inch.
    LONG InchToTwips(float inch) const { return (LONG)(1440.f * inch); }

private:
    HWND _hRichEdit;
};
