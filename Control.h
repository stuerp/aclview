
/** Control.h (2023.06.14) P. Stuer **/

#pragma once

#include "Resources.h"
#include "BaseWindow.h"

class Control : public BaseWindow<Control>
{
public:
    PCTSTR ClassName() const noexcept
    {
        return STR_INTERNAL_NAME TEXT(".Control");
    }

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};
