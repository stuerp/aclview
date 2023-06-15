
/** dllmain.cpp (2023.06.15) P. Stuer **/

#include "pch.h"

#include <CppCoreCheck/Warnings.h>

#pragma warning(disable: 4710 4711 5045 ALL_CPPCORECHECK_WARNINGS)

#include "Resources.h"
#include "Control.h"
#include "Console.h"

#include "ShowSecurityDescriptor.h"
#include "ShowSID.h"
#include "ShowACL.h"
#include "Support.h"

HMODULE _hDLL = 0;
HFONT _hFontGUI = 0;

Console _Console;
Control _Control;

char IniFileName[MAX_PATH] = "lsplugin.ini";

static BOOL OnProcessAttach();
static BOOL OnProcessDetach();
static void ProcessItem(const wstring & filePath);

/// <summary>
/// Entry Point
/// </summary>
BOOL APIENTRY DllMain(HANDLE, DWORD reason, LPVOID)
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            return OnProcessAttach();

        case DLL_PROCESS_DETACH:
            return OnProcessDetach();

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;
    }

    return TRUE;
}

/// <summary>
/// Called when a user opens lister with F3 or the Quick View Panel with Ctrl+Q, and when the definition string either doesn't exist, or its evaluation returns true.
/// </summary>
HWND __stdcall ListLoad(HWND hWnd, char * filePath, int showFlags)
{
    WCHAR FilePathW[MAX_PATH];

    ::MultiByteToWideChar(CP_ACP, 0, filePath, -1, FilePathW, _countof(FilePathW) - 1);

    return ListLoadW(hWnd, FilePathW, showFlags);
}

/// <summary>
/// Called when a user opens lister with F3 or the Quick View Panel with Ctrl+Q, and when the definition string either doesn't exist, or its evaluation returns true.
/// </summary>
HWND __stdcall ListLoadW(HWND hWnd, WCHAR * filePath, int showFlags)
{
    _Console.Create(hWnd, showFlags & lcp_wraptext, showFlags & lcp_darkmode);
    _Console.SetFont(_hFontGUI);

    ProcessItem(filePath);

    _Console.Show();
/*
    if (!_Control.Create(hWnd, TEXT("Learn to Program for Windows"), WS_CHILD))
        return 0;

    ::ShowWindow(_Control.Handle(), SW_SHOW);
*/
    return _Console.Handle();
}

/// <summary>
/// Called when a user switches to the next or previous file in lister with 'n' or 'p' keys, or goes to the next/previous file in the Quick View Panel, and when the definition string either doesn't exist, or its evaluation returns true.
/// </summary>
int __stdcall ListLoadNext(HWND hWnd, HWND ListWin, char * filePath, int showFlags)
{
    WCHAR FilePathW[MAX_PATH];

    ::MultiByteToWideChar(CP_ACP, 0, filePath, -1, FilePathW, _countof(FilePathW) - 1);

    return ListLoadNextW(hWnd, ListWin, FilePathW, showFlags);
}

/// <summary>
/// Called when a user switches to the next or previous file in lister with 'n' or 'p' keys, or goes to the next/previous file in the Quick View Panel, and when the definition string either doesn't exist, or its evaluation returns true.
/// </summary>
int __stdcall ListLoadNextW(HWND hParentWnd, HWND, WCHAR * filePath, int)
{
    ::SendMessageA(hParentWnd, WM_SETREDRAW, FALSE, 0);

    _Console.Hide();

    ProcessItem(filePath);

    _Console.Show();

    ::SendMessageA(hParentWnd, WM_SETREDRAW, TRUE, 0);

    _Console.Redraw();

    return LISTPLUGIN_OK;
}

/// <summary>
/// Called when a user closes lister, or loads a different file.
/// </summary>
void __stdcall ListCloseWindow(HWND hWnd)
{
    ::DestroyWindow(hWnd);
}

/// <summary>
/// Called when the user changes some options in Lister's menu.
/// </summary>
int __stdcall ListSendCommand(HWND hWnd, int command, int parameter)
{
    switch (command)
    {
        // Copy current selection to the clipboard
        case lc_copy:
        {
            ::SendMessageW(hWnd, WM_COPY, 0, 0);

            return LISTPLUGIN_OK;
        }

        // New parameters passed to plugin
        case lc_newparams:
        {
            ::PostMessageW(::GetParent(hWnd), WM_COMMAND, MAKEWPARAM(0, itm_next), (LPARAM) hWnd);

            return LISTPLUGIN_OK;
        }

        // Select the whole contents.
        case lc_selectall:
        {
            ::SendMessageW(hWnd, EM_SETSEL, 0, -1);

            return LISTPLUGIN_OK;
        }

        case lc_setpercent:
        {
            int LineCount = (int) ::SendMessageW(hWnd, EM_GETLINECOUNT, 0, 0);

            if (LineCount <= 0)
                return LISTPLUGIN_ERROR;

            int Line = ::MulDiv(parameter, LineCount, 100);

            int FirstVisibleLine = (int) ::SendMessageW(hWnd, EM_GETFIRSTVISIBLELINE, 0, 0);

            ::SendMessageW(hWnd, EM_LINESCROLL, 0, (LPARAM) Line - FirstVisibleLine);

            FirstVisibleLine = (int) ::SendMessageW(hWnd, EM_GETFIRSTVISIBLELINE, 0, 0);

            // Place caret on first visible line!
            int FirstChar = (int) ::SendMessageW(hWnd, EM_LINEINDEX, (WPARAM) FirstVisibleLine, 0);

            ::SendMessageW(hWnd, EM_SETSEL, (WPARAM) FirstChar, FirstChar);

            int Percentage = ::MulDiv(FirstVisibleLine, 100, LineCount);

            // Update percentage display
            ::PostMessage(::GetParent(hWnd), WM_COMMAND, MAKEWPARAM(Percentage, itm_percent), (LPARAM) hWnd);

            return LISTPLUGIN_OK;
        }
    }

    return LISTPLUGIN_ERROR;
}

/// <summary>
/// Called when the user chooses the print function.
/// </summary>
int __stdcall ListPrint(HWND hWnd, char * filePath, char * printerName, int printerFlags, RECT * margins)
{
    return LISTPLUGIN_ERROR;
}

/// <summary>
/// Called when the user tries to find text in the plugin.
/// </summary>
int _stdcall ListSearchText(HWND hWnd, char * searchString, int searchParameter)
{
    if (searchString == nullptr)
        return LISTPLUGIN_ERROR;

    if (searchString[0] == '\0')
        return LISTPLUGIN_ERROR;

    int FirstVisibleLine = (int) ::SendMessageW(hWnd, EM_GETFIRSTVISIBLELINE, 0, 0);

    int StartPos;

    if (searchParameter & lcs_findfirst)
    {
        //Find first: Start at top visible line
        StartPos = (int) ::SendMessageW(hWnd, EM_LINEINDEX, (WPARAM) FirstVisibleLine, 0);

        ::SendMessageW(hWnd, EM_SETSEL, (WPARAM) StartPos, StartPos);
    }
    else
    {
        //Find next: Start at current selection + 1
        ::SendMessageW(hWnd, EM_GETSEL, (WPARAM) &StartPos, 0);

        StartPos += 1;
    }

    FINDTEXTW ft = { { 0 }, 0 };

    ft.chrg.cpMin = StartPos;
    ft.chrg.cpMax = (LONG) ::SendMessageW(hWnd, WM_GETTEXTLENGTH, 0, 0);

    int Flags = 0;

    if (searchParameter & lcs_wholewords)
        Flags |= FR_WHOLEWORD;

    if (searchParameter & lcs_matchcase)
        Flags |= FR_MATCHCASE;

    if (!(searchParameter & lcs_backwards))
        Flags |= FR_DOWN;

    WCHAR SearchStringW[1024];

    ::MultiByteToWideChar(CP_ACP, 0, searchString, -1, SearchStringW, _countof(SearchStringW) - 1);

    ft.lpstrText = SearchStringW;

    int TextHead = (int) ::SendMessageW(hWnd, EM_FINDTEXT, (WPARAM) Flags, (LPARAM) &ft);

    if (TextHead != -1)
    {
        int TextTail = TextHead + (int) ::strlen(searchString);

        ::SendMessageW(hWnd, EM_SETSEL, (WPARAM) TextHead, TextTail);

        int LineIndex = (int) ::SendMessageW(hWnd, EM_LINEFROMCHAR, (WPARAM) TextHead, 0) - 3;

        if (LineIndex < 0)
            LineIndex = 0;

        LineIndex -= FirstVisibleLine;

        ::SendMessageW(hWnd, EM_LINESCROLL, 0, LineIndex);

        return LISTPLUGIN_OK;
    }
    else
        ::SendMessageW(hWnd, EM_SETSEL, (WPARAM) -1, -1);  // Restart search at the beginning

    return LISTPLUGIN_ERROR;
}

/// <summary>
/// Called when the user tries to find text in the plugin.
/// </summary>
int _stdcall ListSearchDialog(HWND, int)
{
    return LISTPLUGIN_ERROR; // Total Commander should show its own text search dialog.
}

/// <summary>
/// Called when the parent window receives a notification message from the child window.
/// </summary>
int __stdcall ListNotificationReceived(HWND hWnd, int msg, WPARAM wParam, LPARAM)
{
    if (msg != WM_COMMAND)
        return 0;

    if (HIWORD(wParam) != EN_UPDATE)
        return 0;

    int LineCount = (int) ::SendMessageW(hWnd, EM_GETLINECOUNT, 0, 0);

    if (LineCount > 0)
    {
        int FirstVisibleLine = (int) ::SendMessageW(hWnd, EM_GETFIRSTVISIBLELINE, 0, 0);

        int Percentage = ::MulDiv(FirstVisibleLine, 100, LineCount);

        // Set the percentage in the Lister title bar.
        ::PostMessageW(::GetParent(hWnd), WM_COMMAND, MAKEWPARAM(Percentage, itm_percent), (LPARAM) hWnd);
    }

    return 0;
}

/// <summary>
/// Called immediately after loading the DLL, before ListLoad.
/// </summary>
void __stdcall ListSetDefaultParams(ListDefaultParamStruct * dps)
{
    if (dps)
        ::StringCchCopy(IniFileName, _countof(IniFileName) - 1, dps->DefaultIniName);
}

/// <summary>
/// Called when the plugin gets loaded.
/// </summary>
static BOOL OnProcessAttach()
{
    {
        INITCOMMONCONTROLSEX icc = { sizeof(icc) };

        icc.dwICC = ICC_STANDARD_CLASSES | ICC_TAB_CLASSES | ICC_UPDOWN_CLASS | ICC_USEREX_CLASSES;

        ::InitCommonControlsEx(&icc);
    }

    // Load the RichEdit library.
    {
        if (_hDLL == 0)
        {
            UINT OldErrorMode = ::SetErrorMode(SEM_NOOPENFILEERRORBOX);

            _hDLL = ::LoadLibrary("msftedit.dll"); // 4.1

            if (_hDLL < (HMODULE) HINSTANCE_ERROR)
                _hDLL = ::LoadLibrary("riched20.dll"); // 3.0

            if (_hDLL < (HMODULE) HINSTANCE_ERROR)
                _hDLL = ::LoadLibrary("riched32.dll"); // 1.0

            if (_hDLL < (HMODULE) HINSTANCE_ERROR)
                _hDLL = 0;

            ::SetErrorMode(OldErrorMode);
        }
    }

    // Create the GUI font (based on the menu font).
    {
        NONCLIENTMETRICS nm = { sizeof(nm) };

        ::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, nm.cbSize, &nm, 0);

        _hFontGUI = ::CreateFontIndirect(&nm.lfMenuFont);
    }

    return TRUE;
}

/// <summary>
/// Called when the plugin gets unloaded.
/// </summary>
static BOOL OnProcessDetach()
{
    if (_hFontGUI)
    {
        ::DeleteObject(_hFontGUI);
        _hFontGUI = 0;
    }

    if (_hDLL)
    {
        ::FreeLibrary(_hDLL);
        _hDLL = 0;
    }

    return TRUE;
}

/// <summary>
/// Processes the specified item.
/// </summary>
static void ProcessItem(const wstring & pathName)
{
    _Console.Clear();

    DWORD FileAttributes = ::GetFileAttributesW(pathName.c_str());

    if (FileAttributes == INVALID_FILE_ATTRIBUTES)
    {
        ShowError(L"Unable to get file attributes. GetFileAttributesW() failed.");
        return;
    }

    ObjectType ObjectType = ((FileAttributes & FILE_ATTRIBUTE_DIRECTORY) && !(FileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)) ? DirectoryObject : FileObject;

    _Console.SetBold(true);
    _Console.Write(L"Item: \"%s\"\n", pathName.c_str());

    // Low-level Security Descriptor Functions, https://msdn.microsoft.com/en-us/library/aa379204(v=vs.85).aspx
    const SECURITY_INFORMATION RequestedInformation = OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION;
    DWORD sdSize = 0;

    if (::GetFileSecurityW(pathName.c_str(), RequestedInformation, NULL, 0, &sdSize) && (::GetLastError() != ERROR_INSUFFICIENT_BUFFER))
    {
        ShowError(L"Unable to get security information. GetFileSecurityW() failed.");
        return;
    }

    PSECURITY_DESCRIPTOR sd = (PSECURITY_DESCRIPTOR) ::LocalAlloc(LMEM_FIXED, sdSize);

    if (sd == nullptr)
    {
        ShowError(L"Unable to allocated memory for security descriptor. LocalAlloc() failed.", ::GetLastError());
        return;
    }

    if (::GetFileSecurityW(pathName.c_str(), RequestedInformation, sd, sdSize, &sdSize)) // Requires READ_CONTROL right.
    {
        {
            _Console.SetBold(true);
            _Console.Write(L"\nSecurity Descriptor:\n");

            ShowSecurityDescriptor(sd);
        }

        {
            _Console.SetBold(true);
            _Console.Write(L"\nOwner:\n");

            PSID OwnerSID;
            BOOL IsOwnerDefaulted;

            if (::GetSecurityDescriptorOwner(sd, &OwnerSID, &IsOwnerDefaulted))
                ShowSID(OwnerSID);
            else
                ShowError(L"Unable to determine owner. GetSecurityDescriptorOwner() failed.");
        }

        {
            _Console.SetBold(true);
            _Console.Write(L"\nPrimary Group:\n");

            PSID GroupSID;
            BOOL IsGroupDefaulted;

            if (::GetSecurityDescriptorGroup(sd, &GroupSID, &IsGroupDefaulted))
                ShowSID(GroupSID);
            else
                ShowError(L"Unable to determine primary group. GetSecurityDescriptorGroup() failed.");
        }

        {
            _Console.SetBold(true);
            _Console.Write(L"\nDACL:\n");

            PACL DACL;
            BOOL IsDACLPresent;
            BOOL IsDACLDefaulted;

            if (::GetSecurityDescriptorDacl(sd, &IsDACLPresent, &DACL, &IsDACLDefaulted))
            {
                if (IsDACLPresent && ::IsValidAcl(DACL))
                    ShowACL(DACL, ObjectType);
            }
            else
                ShowError(L"Unable to get DACL. GetSecurityDescriptorDacl() failed.");
        }
    }
    else
        ShowError(L"Unable to get security information. GetFileSecurityW() failed.");

    if (sd)
        ::LocalFree(sd);

    // Highlight any quoted text.
    {
        FINDTEXTW ft = { { 0, (LONG) ::SendMessageW(_Console.Handle(), WM_GETTEXTLENGTH, 0, 0) }, L"\"" };

        int OpeningQuote = (int) ::SendMessageW(_Console.Handle(), EM_FINDTEXT, (WPARAM) FR_DOWN, (LPARAM) &ft);

        while (OpeningQuote != -1)
        {
            ft.chrg.cpMin = ++OpeningQuote;

            int ClosingQuote = (int) ::SendMessageW(_Console.Handle(), EM_FINDTEXT, (WPARAM) FR_DOWN, (LPARAM) &ft);

            if (ClosingQuote == -1)
                break;

            _Console.Select(OpeningQuote, ClosingQuote);
            _Console.SetTextForeColor(::GetSysColor(COLOR_HIGHLIGHT));

            ft.chrg.cpMin = ClosingQuote + 1;

            OpeningQuote = (int) ::SendMessageW(_Console.Handle(), EM_FINDTEXT, (WPARAM) FR_DOWN, (LPARAM) &ft);
        }
    }

    _Console.ScrollToTop();
    _Console.Select(0, 0);
}
