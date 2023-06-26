
/** Support.cpp (2023.06.16) P. Stuer **/

#include "pch.h"

#include <CppCoreCheck/Warnings.h>

#pragma warning(disable: 4710 4711 4820 ALL_CPPCORECHECK_WARNINGS)

#include "Console.h"
#include "Support.h"

extern Console _Console;

///<summary>
/// Converts a GUID to text.
///</summary>
HRESULT GUIDToText(GUID * guid, LPWSTR data, int size)
{
    return ::StringFromGUID2(*guid, (LPOLESTR) data, size) ? S_OK : E_FAIL;
}

///<summary>
/// Gets the message describing the specified id.
///</summary>
wstring GetLastErrorMessage(DWORD messageId)
{
    return GetLastErrorMessage(NULL, messageId);
}

///<summary>
/// Gets the message describing the specified id.
///</summary>
wstring GetLastErrorMessage(HMODULE hModule, DWORD messageId)
{
    const DWORD LanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);

    DWORD Flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK;

    if (hModule != 0)
        Flags |= FORMAT_MESSAGE_FROM_HMODULE;

    LPWSTR Message = nullptr;

    if (::FormatMessageW(Flags, hModule, messageId, LanguageId, (LPWSTR)&Message, 0, nullptr) == 0)
    {
        char ExceptionMessage[256];

        ::_snprintf(ExceptionMessage, _countof(ExceptionMessage), "FormatMessageW() failed (0x%08X).", (uint32_t)::GetLastError());

        throw runtime_error(ExceptionMessage);
    }

    return Message;
}

///<summary>
/// Shows an error message.
///</summary>
DWORD ShowError(const wstring & message)
{
    return ShowError(message, ::GetLastError());
}

///<summary>
/// Shows an error message.
///</summary>
DWORD ShowError(const wstring & message, DWORD errorCode)
{
    wstring ErrorMessage = ::GetLastErrorMessage(errorCode);

    _Console.Write(L"%s %s(0x%08X)\n", message.c_str(), ErrorMessage.c_str(), errorCode);

    return errorCode;
}

