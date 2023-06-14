
/** Control.cpp (2023.06.14) P. Stuer **/

#include "pch.h"

#include <CppCoreCheck/Warnings.h>

#pragma warning(disable: 4710 ALL_CPPCORECHECK_WARNINGS)

#include "Control.h"

LRESULT Control::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;

            HDC hDC = ::BeginPaint(_hWnd, &ps);

            ::FillRect(hDC, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW + 1));

            ::EndPaint(_hWnd, &ps);

            return 0;
        }

        default:
            return ::DefWindowProc(_hWnd, uMsg, wParam, lParam);
    }
}
