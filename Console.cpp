
/** Console.cpp (2023.06.26) P. Stuer **/

#include "pch.h"

#include <CppCoreCheck/Warnings.h>

#pragma warning(disable: 4710 4711 4820 ALL_CPPCORECHECK_WARNINGS)

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

    const HMODULE hInstance = ::GetModuleHandleW(NULL);

    _hRichEdit = ::CreateWindowExW(0, MSFTEDIT_CLASS, L"", Style, cr.left, cr.top, cr.right - cr.left, cr.bottom - cr.top, hWnd, NULL, hInstance, NULL);

    if (_hRichEdit == 0)
        _hRichEdit = ::CreateWindowExW(0, L"RichEdit20A", L"", Style, cr.left, cr.top, cr.right - cr.left, cr.bottom - cr.top, hWnd, NULL, hInstance, NULL);

    if (_hRichEdit == 0)
        _hRichEdit = ::CreateWindowExW(0, L"RichEdit", L"", Style, cr.left, cr.top, cr.right - cr.left, cr.bottom - cr.top, hWnd, NULL, hInstance, NULL);

    if (_hRichEdit == 0)
        return;

    ::SendMessageW(_hRichEdit, EM_SETMARGINS, EC_LEFTMARGIN, 8);
    ::SendMessageW(_hRichEdit, EM_SETEVENTMASK, 0, ENM_UPDATE); // ENM_SCROLL doesn't work for thumb movements.

    SetBackgroundColor(darkMode ? RGB(32, 32, 32) : ::GetSysColor(COLOR_WINDOW));

    ::PostMessageW(hWnd, WM_COMMAND, MAKEWPARAM(lcp_ansi, itm_fontstyle), (LPARAM) _hRichEdit);

    (void) ::SetWindowTheme(_hRichEdit, L"DarkMode_Explorer", nullptr);

    _DarkMode = darkMode;
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

    SetFormat();
    ::SendMessageW(_hRichEdit, EM_REPLACESEL, FALSE, (LPARAM) Text);
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
void Console::SetDefaultTextColor(COLORREF color) noexcept
{
    _Format.dwMask |= CFM_COLOR;
    _Format.crTextColor = color;
}

/// <summary>
/// Set the character style to bold.
/// </summary>
void Console::SetBold(bool enabled) noexcept
{
    _Format.dwMask |= CFM_BOLD;
    _Format.dwEffects = (DWORD)(enabled ? CFE_BOLD : 0);
}

/// <summary>
/// Set the character style to italic.
/// </summary>
void Console::SetItalic(bool enabled) noexcept
{
    _Format.dwMask |= CFM_ITALIC;
    _Format.dwEffects = (DWORD)(enabled ? CFE_ITALIC : 0);
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
