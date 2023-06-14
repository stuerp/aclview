
/** BaseWindow.h (2023.06.14) P. Stuer **/

#pragma once

template <class DERIVED_TYPE>
class BaseWindow
{
public:
    BaseWindow() noexcept : _hWnd(0)
    {
    }

    virtual ~BaseWindow() noexcept
    {
    }

    BOOL Create(HWND hWndParent, PCTSTR windowName, DWORD style, DWORD styleEx = 0, int x = 0, int y = 0, int w = 640, int h = 480, HMENU hMenu = 0) noexcept
    {
        WNDCLASS wc = { 0 };

        wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
        wc.hInstance = ::GetModuleHandle(NULL);
        wc.lpszClassName = ClassName();

        ::RegisterClass(&wc);

        _hWnd = ::CreateWindowEx(styleEx, ClassName(), windowName, style, x, y, w, h, hWndParent, hMenu, ::GetModuleHandle(NULL), this);

        return (_hWnd ? TRUE : FALSE);
    }

    HWND Handle() const noexcept
    {
        return _hWnd;
    }

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
    {
        DERIVED_TYPE * This = nullptr;

        if (msg == WM_NCCREATE)
        {
            CREATESTRUCT * cs = (CREATESTRUCT *) lParam;

            This = (DERIVED_TYPE *) cs->lpCreateParams;

            ::SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR) This);

            This->_hWnd = hwnd;
        }
        else
        {
            This = (DERIVED_TYPE *) ::GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }

        if (This)
        {
            return This->HandleMessage(msg, wParam, lParam);
        }
        else
        {
            return ::DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }

protected:
    virtual PCTSTR ClassName() const noexcept = 0;
    virtual LRESULT HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) = 0;

    HWND _hWnd;
};
