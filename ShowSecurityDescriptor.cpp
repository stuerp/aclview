
/** ShowSecurityDescriptor.cpp (2023.06.14) P. Stuer **/

#include "pch.h"

#include <CppCoreCheck/Warnings.h>

#pragma warning(disable: 4710 4820 ALL_CPPCORECHECK_WARNINGS)

#include "Console.h"
#include "Support.h"

extern Console _Console;

///<summary>
/// Shows the specified security descriptor.
///</summary>
void ShowSecurityDescriptor(PSECURITY_DESCRIPTOR sd)
{
    _Console.SetBullet(true);

    _Console.Write(L"Security Descriptor, %s, %d bytes\n", IsValidSecurityDescriptor(sd) ? L"Valid" : L"Invalid", GetSecurityDescriptorLength(sd));

    SECURITY_DESCRIPTOR_CONTROL sdc;
    DWORD Revision;

    if (::GetSecurityDescriptorControl(sd, &sdc, &Revision))
    {
        _Console.IncreaseIndent();

        if (sdc & SE_DACL_AUTO_INHERITED) _Console.Write(L"Contains DACL that is set up to support automatic propagation of inheritable ACEs to existing child objects.\n");
        if (sdc & SE_DACL_DEFAULTED)      _Console.Write(L"Has a default DACL.\n");
        if (sdc & SE_DACL_PRESENT)        _Console.Write(L"Has a DACL.\n");
        if (sdc & SE_DACL_PROTECTED)      _Console.Write(L"Has a DACL that is protected from being modified by inheritable ACEs.\n");

        if (sdc & SE_GROUP_DEFAULTED)     _Console.Write(L"Has a default group.\n");
        if (sdc & SE_OWNER_DEFAULTED)     _Console.Write(L"Has a default owner.\n");

        if (sdc & SE_SACL_AUTO_INHERITED) _Console.Write(L"Has a SACL that is set up to support automatic propagation of inheritable ACEs to existing child objects\n");
        if (sdc & SE_SACL_DEFAULTED)      _Console.Write(L"Has a default SACL.\n");
        if (sdc & SE_SACL_PRESENT)        _Console.Write(L"Has a SACL.\n");
        if (sdc & SE_SACL_PROTECTED)      _Console.Write(L"Has a SACL that is protected from being modified by inheritable ACEs.\n");

        if (sdc & SE_SELF_RELATIVE)       _Console.Write(L"Is in self-relative format.\n");

        if (sdc & SE_RM_CONTROL_VALID)    _Console.Write(L"Has valid resource manager control.\n");

        _Console.DecreaseIndent();
    }
    else
        ShowError(L"GetSecurityDescriptorControl() failed.");

    _Console.SetBullet(false);
}
