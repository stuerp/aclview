#pragma once

#include <string>

using namespace std;

HRESULT GUIDToText(GUID * guid, LPWSTR data, int size);

wstring GetLastErrorMessage(DWORD messageId);
wstring GetLastErrorMessage(HMODULE hModule, DWORD messageId);

DWORD ShowError(const wstring & message);
DWORD ShowError(const wstring & message, DWORD errorCode);