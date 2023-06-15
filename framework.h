
/** $VER: framework.h (2023.06.15) P. Stuer **/

#pragma once

#define STRICT
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <windowsx.h>
#include <strsafe.h>

#define SECURITY_WIN32

#include <security.h>
#pragma comment(lib, "secur32.lib")

// For Authorization functions, https://msdn.microsoft.com/en-us/library/windows/desktop/aa375742(v=vs.85).aspx
#include <sddl.h>

// For GetExplicitEntriesFromAcl
#include <aclapi.h>

// For Security Management functions, https://msdn.microsoft.com/en-us/library/windows/desktop/ms721849(v=vs.85).aspx
#include <ntsecapi.h>

#pragma comment(lib,"comctl32.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <commctrl.h>
#include <shellapi.h>
#include <richedit.h>
#include <commdlg.h>

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>

#include <string>
#include <stdexcept>

#include "listplug.h"

