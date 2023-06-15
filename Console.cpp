
/** Console.cpp (2023.06.15) P. Stuer **/

#include "pch.h"

#include <CppCoreCheck/Warnings.h>

#pragma warning(disable: 4710 4711 ALL_CPPCORECHECK_WARNINGS)

#include "Console.h"

#include <Uxtheme.h>
#include <dwmapi.h>

#pragma comment(lib, "uxtheme.lib")
#pragma comment(lib, "dwmapi.lib")

/// <summary>
/// Creates the console.
/// </summary>
void Console::Create(HWND hWnd, bool wordwrap, bool darkMode) noexcept
{
    RECT cr;

    ::GetClientRect(hWnd, &cr);

    const DWORD Style = WS_CHILD | (wordwrap ? (DWORD) 0 : WS_HSCROLL) | WS_VSCROLL | ES_MULTILINE | ES_READONLY | ES_NOHIDESEL;

    _hRichEdit = ::CreateWindowExW(0, MSFTEDIT_CLASS, L"", Style, cr.left, cr.top, cr.right - cr.left, cr.bottom - cr.top, hWnd, NULL, ::GetModuleHandleW(NULL), NULL);

    if (_hRichEdit == 0)
        _hRichEdit = ::CreateWindowExW(0, L"RichEdit20A", L"", Style, cr.left, cr.top, cr.right - cr.left, cr.bottom - cr.top, hWnd, NULL, ::GetModuleHandleW(NULL), NULL);

    if (_hRichEdit == 0)
        _hRichEdit = ::CreateWindowExW(0, L"RichEdit", L"", Style, cr.left, cr.top, cr.right - cr.left, cr.bottom - cr.top, hWnd, NULL, ::GetModuleHandleW(NULL), NULL);

    if (_hRichEdit == 0)
        return;

    ::SendMessageW(_hRichEdit, EM_SETMARGINS, EC_LEFTMARGIN, 8);
    ::SendMessageW(_hRichEdit, EM_SETEVENTMASK, 0, ENM_UPDATE); // ENM_SCROLL doesn't work for thumb movements.

    SetBackgroundColor(darkMode ? RGB(31, 31, 31) : ::GetSysColor(COLOR_3DFACE));

    ::PostMessageW(hWnd, WM_COMMAND, MAKEWPARAM(lcp_ansi, itm_fontstyle), (LPARAM) _hRichEdit);
}

/// <summary>
/// Writes the specfied formated text to the console.
/// </summary>
void Console::Write(LPCWSTR format, ...) const noexcept
{
    va_list va;
    WCHAR Text[1024];

    va_start(va, format);
    ::_vsnwprintf(Text, _countof(Text) - 1, format, va);
    va_end(va);

    ::SendMessageW(_hRichEdit, EM_REPLACESEL, FALSE, (LPARAM) Text);
}

/// <summary>
/// Set the background color.
/// </summary>
void Console::SetBackgroundColor(DWORD color) const noexcept
{
    ::SendMessageW(_hRichEdit, EM_SETBKGNDCOLOR, 0, (LPARAM) color);
}

/// <summary>
/// Set the default font.
/// </summary>
void Console::SetFont(HFONT hFont) const noexcept
{
    ::SendMessageW(_hRichEdit, WM_SETFONT, (WPARAM) hFont, FALSE);
}

/// <summary>
/// Set the text color.
/// </summary>
void Console::SetTextForeColor(COLORREF color) const noexcept
{
    CHARFORMAT cf = { sizeof(cf) };

    cf.dwMask      = CFM_COLOR;
    cf.crTextColor = color;
    cf.dwEffects   = 0;

    ::SendMessageW(_hRichEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);
}

/// <summary>
/// Set the text color.
/// </summary>
void Console::SetTextBackColor(COLORREF color) const noexcept
{
    CHARFORMAT cf = { sizeof(cf) };

    cf.dwMask      = CFM_BACKCOLOR;
    cf.crTextColor = color;
    cf.dwEffects   = 0;

    ::SendMessageW(_hRichEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);
}

/// <summary>
/// Set the text color to default (GetSysColor(COLOR_WINDOWTEXT)).
/// </summary>
void Console::ResetTextColor() const noexcept
{
    CHARFORMAT cf = { sizeof(cf) };

    cf.dwMask    = CFM_COLOR;
    cf.dwEffects = CFE_AUTOCOLOR;

    ::SendMessageW(_hRichEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);
}

/// <summary>
/// Set the character style to bold.
/// </summary>
void Console::SetBold(bool enabled) const noexcept
{
    CHARFORMAT cf = { sizeof(cf) };

    cf.dwMask    = CFM_BOLD;
    cf.dwEffects = (DWORD)(enabled ? CFE_BOLD : 0);

    ::SendMessageW(_hRichEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);
}

/// <summary>
/// Set the character style to italic.
/// </summary>
void Console::SetItalic(bool enabled) const noexcept
{
    CHARFORMAT cf = { sizeof(cf) };

    cf.dwMask    = CFM_ITALIC;
    cf.dwEffects = (DWORD)(enabled ? CFE_ITALIC : 0);

    ::SendMessageW(_hRichEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);
}

/// <summary>
/// Enables or disables bullet list mode.
/// </summary>
void Console::SetBullet(bool enabled) const noexcept
{
    PARAFORMAT pf = { sizeof(pf) };

    pf.dwMask     = PFM_NUMBERING;
    pf.wNumbering = (DWORD)(enabled ? PFN_BULLET : 0);

    ::SendMessageW(_hRichEdit, EM_SETPARAFORMAT, 0, (LPARAM) &pf);
}

/// <summary>
/// Increases the paragraph indent.
/// </summary>
void Console::IncreaseIndent() const noexcept
{
    PARAFORMAT pf = { sizeof(pf) };

    pf.dwMask        = PFM_OFFSETINDENT;
    pf.dxStartIndent = InchToTwips(.25f);

    ::SendMessageW(_hRichEdit, EM_SETPARAFORMAT, 0, (LPARAM) &pf);
}

/// <summary>
/// Decreases the paragraph indent.
/// </summary>
void Console::DecreaseIndent() const noexcept
{
    PARAFORMAT pf = { sizeof(pf) };

    pf.dwMask        = PFM_OFFSETINDENT;
    pf.dxStartIndent = InchToTwips(-.25f);

    ::SendMessageW(_hRichEdit, EM_SETPARAFORMAT, 0, (LPARAM) &pf);
}
