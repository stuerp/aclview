
/** ShowSID.cpp (2023.06.14) P. Stuer **/

#include "pch.h"

#include <CppCoreCheck/Warnings.h>

#pragma warning(disable: 4710 ALL_CPPCORECHECK_WARNINGS)

#include "Console.h"
#include "Support.h"

extern Console _Console;

static LPCWSTR GetSidNameUseDescription(SID_NAME_USE sidNameUse);

///<summary>
/// Shows the specified SID.
///</summary>
HRESULT ShowSID(const PSID sid)
{
    WCHAR AccountName[256] = { 0 };
    DWORD AccountNameSize = ARRAYSIZE(AccountName);
    WCHAR DomainName[256] = { 0 };
    DWORD DomainNameSize = ARRAYSIZE(DomainName);
    SID_NAME_USE SidNameUse;

    if (!::LookupAccountSidW(NULL, sid, AccountName, &AccountNameSize, DomainName, &DomainNameSize, &SidNameUse))
    {
        if (::GetLastError() == ERROR_NONE_MAPPED)
            ::wcscpy_s(AccountName, AccountNameSize, L"<Not Mapped>");
        else
            return HRESULT_FROM_WIN32(ShowError(L"LookupAccountSid() failed."));
    }

    LPWSTR AccountSidString = nullptr;

    if (::ConvertSidToStringSidW(sid, &AccountSidString))
    {
        _Console.SetBullet(true);

        _Console.Write(L"SID: \"");
        _Console.SetTextForeColor(GetSysColor(COLOR_HIGHLIGHT));

        if (DomainName[0])
            _Console.Write(L"%s\\%s", DomainName, AccountName);
        else
            _Console.Write(L"%s", AccountName);

        _Console.ResetTextColor();
        _Console.Write(L"\", %s, %s\n", AccountSidString, GetSidNameUseDescription(SidNameUse));

        _Console.SetBullet(false);

        ::LocalFree((HLOCAL)AccountSidString);
    }
    else
        return HRESULT_FROM_WIN32(ShowError(L"ConvertSidToStringSid() failed."));

    return S_OK;
}

///<summary>
/// Gets the description of the specified SID name use.
///</summary>
static LPCWSTR GetSidNameUseDescription(SID_NAME_USE sidNameUse)
{
    switch (sidNameUse)
    {
        case SidTypeUser: return L"User SID";
        case SidTypeGroup: return L"Group SID";
        case SidTypeDomain: return L"Domain SID";
        case SidTypeAlias: return L"Alias SID";
        case SidTypeWellKnownGroup: return L"Well-known Group SID";
        case SidTypeDeletedAccount: return L"Deleted Account SID";
        case SidTypeInvalid: return L"Invalid SID";
        case SidTypeUnknown: return L"Unknown SID Type";
        case SidTypeComputer: return L"Computer SID";
        case SidTypeLabel: return L"Mandatory Integrity Label SID";
        case SidTypeLogonSession: return L"Logon Session SID";

        default: return L"Unknown SID Name Use";
    }
}
