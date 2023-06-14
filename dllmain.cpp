
/** dllmain.cpp (2023.06.14) P. Stuer **/

#include "pch.h"

#include <CppCoreCheck/Warnings.h>

#pragma warning(disable: 4710 4711 ALL_CPPCORECHECK_WARNINGS)

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

static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/// <summary>
/// Entry Point
/// </summary>
BOOL APIENTRY DllMain(HANDLE hModule, DWORD reason, LPVOID)
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
/// 
/// </summary>
HWND __stdcall ListLoad(HWND hWnd, char * filePath, int showFlags)
{
    WCHAR FilePathW[wdirtypemax];

    return ListLoadW(hWnd, awfilenamecopy(FilePathW, filePath), showFlags);
}

/// <summary>
/// 
/// </summary>
HWND __stdcall ListLoadW(HWND hWnd, WCHAR * filePath, int)
{
    _Console.Create(hWnd);
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
    WCHAR FilePathW[wdirtypemax];

    return ListLoadNextW(hWnd, ListWin, awfilenamecopy(FilePathW, filePath), showFlags);
}

/// <summary>
/// 
/// </summary>
int __stdcall ListLoadNextW(HWND ParentWin, HWND ListWin, WCHAR * filePath, int)
{
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
/// 
/// </summary>
int __stdcall ListSendCommand(HWND hWnd, int command, int parameter)
{
    return LISTPLUGIN_ERROR;
}

/// <summary>
/// 
/// </summary>
int __stdcall ListPrint(HWND hWnd, char * filePath, char * printerName, int printerFlags, RECT * margins)
{
    return 0;
}

/// <summary>
/// 
/// </summary>
int _stdcall ListSearchText(HWND hWnd, char * searchString, int searchParameter)
{
    return LISTPLUGIN_ERROR;
}

/// <summary>
/// 
/// </summary>
int _stdcall ListSearchDialog(HWND hWnd, int FindNext)
{
    return LISTPLUGIN_ERROR;
}

/// <summary>
/// 
/// </summary>
int __stdcall ListNotificationReceived(HWND hWnd, int msg, WPARAM wParam, LPARAM lParam)
{
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
/// 
/// </summary>
static BOOL OnProcessAttach()
{
    {
        INITCOMMONCONTROLSEX icc;

        icc.dwSize = sizeof(icc);
        icc.dwICC  = ICC_STANDARD_CLASSES | ICC_TAB_CLASSES | ICC_UPDOWN_CLASS | ICC_USEREX_CLASSES;

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
        NONCLIENTMETRICS nm;

        nm.cbSize = sizeof(nm);

        ::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, nm.cbSize, &nm, 0);

        _hFontGUI = ::CreateFontIndirect(&nm.lfMenuFont);
    }

    return TRUE;
}

/// <summary>
/// 
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
/// 
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
    _Console.Write(L"Item: \"");
    _Console.SetTextForeColor(GetSysColor(COLOR_HIGHLIGHT));
    _Console.Write(pathName.c_str());
    _Console.Write(L"\"\n", pathName.c_str());

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

    _Console.ScrollToTop();
}
