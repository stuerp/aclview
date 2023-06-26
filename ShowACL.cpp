
/** ShowACL.cpp (2023.06.16) P. Stuer **/

#include "pch.h"

#include <CppCoreCheck/Warnings.h>

#pragma warning(disable: 4710 4711 4820 ALL_CPPCORECHECK_WARNINGS)

#include "Console.h"
#include "Support.h"

#include "ShowACL.h"
#include "ShowSID.h"

extern Console _Console;

static HRESULT ShowExplicitAccess(const EXPLICIT_ACCESS_W * ea, ObjectType objectType);
static HRESULT ShowTrustee(const TRUSTEE_W * Trustee);

///<summary>
/// Shows the information in an ACL.
///</summary>
HRESULT ShowACL(const PACL acl, ObjectType objectType)
{
    HRESULT hResult = S_OK;

    EXPLICIT_ACCESS_W * Entries = nullptr;
    ULONG EntryCount = 0;

    DWORD ErrorCode = ::GetExplicitEntriesFromAclW(acl, &EntryCount, &Entries);

    if (ErrorCode == ERROR_SUCCESS)
    {
        if (EntryCount > 0)
        {
            for (ULONG i = 0; i < EntryCount; i++)
            {
                _Console.Write(L"Entry %d\n", i + 1);
                _Console.IncreaseIndent();

                ShowExplicitAccess(&Entries[i], objectType);

                _Console.DecreaseIndent();
            }
        }
        else
            _Console.Write(L"ACL contains no entries.\n");

        LocalFree(Entries);
    }
    else
        hResult = HRESULT_FROM_WIN32(ShowError(L"GetExplicitEntriesFromAcl() failed."));

    return hResult;
}

static HRESULT ShowExplicitAccess(const EXPLICIT_ACCESS_W * ea, ObjectType objectType)
{
    {
        _Console.SetItalic(true);
        _Console.Write(L"Trustee\n");
        _Console.SetItalic(false);
        _Console.IncreaseIndent();

        ShowTrustee(&ea->Trustee);

        _Console.DecreaseIndent();
    }

    {
        _Console.SetItalic(true);
        _Console.Write(L"Access Rights\n");
        _Console.SetItalic(false);
        _Console.IncreaseIndent();

            LPCWSTR AccessMode = NULL;

            switch (ea->grfAccessMode)
            {
                // DACL
                case GRANT_ACCESS:      AccessMode = L"Grant"; break;
                case DENY_ACCESS:       AccessMode = L"Deny"; break;

                // SACL
                case SET_AUDIT_SUCCESS: AccessMode = L"Enable audit success of"; break;
                case SET_AUDIT_FAILURE: AccessMode = L"Enable audit failure of"; break;

                case SET_ACCESS:        AccessMode = L"Set"; break;
                case REVOKE_ACCESS:     AccessMode = L"Revoke"; break;

                case NOT_USED_ACCESS:   AccessMode = L"Unused"; break;

                default:                AccessMode = L"<Unknown access mode>";
            }

            _Console.Write(L"Specific Access Rights\n");
            _Console.IncreaseIndent();

                _Console.SetBullet(true);

                // Access Rights for Access-Token Objects, https://msdn.microsoft.com/en-us/library/aa374905(v=vs.85).aspx
                if (objectType == AccessTokenObject)
                {
                }
                else

                // Process Security and Access Rights, https://msdn.microsoft.com/en-us/library/ms684880(v=vs.85).aspx
                if (objectType == ProcessObject)
                {
                    if (ea->grfAccessPermissions & PROCESS_TERMINATE)
                        _Console.Write(L"%s terminate\n", AccessMode);

                    if (ea->grfAccessPermissions & PROCESS_CREATE_THREAD)
                        _Console.Write(L"%s create thread\n", AccessMode);

                    if (ea->grfAccessPermissions & PROCESS_SET_SESSIONID)
                        _Console.Write(L"%s read data\n", AccessMode);

                    if (ea->grfAccessPermissions & PROCESS_VM_OPERATION)
                        _Console.Write(L"%s address space operations\n", AccessMode);

                    if (ea->grfAccessPermissions & PROCESS_VM_READ)
                        _Console.Write(L"%s read process memory\n", AccessMode);

                    if (ea->grfAccessPermissions & PROCESS_VM_WRITE)
                        _Console.Write(L"%s write process memory\n", AccessMode);

                    if (ea->grfAccessPermissions & PROCESS_DUP_HANDLE)
                        _Console.Write(L"%s duplicate handles\n", AccessMode);

                    if (ea->grfAccessPermissions & PROCESS_CREATE_PROCESS)
                        _Console.Write(L"%s create process\n", AccessMode);

                    if (ea->grfAccessPermissions & PROCESS_SET_QUOTA)
                        _Console.Write(L"%s set quota\n", AccessMode);

                    if (ea->grfAccessPermissions & PROCESS_SET_INFORMATION)
                        _Console.Write(L"%s read data\n", AccessMode);

                    if (ea->grfAccessPermissions & PROCESS_QUERY_INFORMATION)
                        _Console.Write(L"%s query information\n", AccessMode);

                    if (ea->grfAccessPermissions & PROCESS_SUSPEND_RESUME)
                        _Console.Write(L"%s suspend/resume\n", AccessMode);

                    if (ea->grfAccessPermissions & PROCESS_QUERY_LIMITED_INFORMATION)
                        _Console.Write(L"%s query limited information\n", AccessMode);

                    if (ea->grfAccessPermissions & PROCESS_SET_LIMITED_INFORMATION)
                        _Console.Write(L"%s set limited information\n", AccessMode);
                }
                else

                // File Security and Access Rights, https://msdn.microsoft.com/en-us/library/aa364399(v=vs.85).aspx
                if ((objectType == FileObject) || (objectType == NamedPipeObject))
                {
                    if (ea->grfAccessPermissions & FILE_READ_DATA)
                        _Console.Write(L"%s read data\n", AccessMode);
                    else
                    if (ea->grfAccessPermissions & FILE_LIST_DIRECTORY)
                        _Console.Write(L"%s list\n", AccessMode);

                    if (ea->grfAccessPermissions & FILE_WRITE_DATA)
                        _Console.Write(L"%s write data\n", AccessMode);
                    else
                    if (ea->grfAccessPermissions & FILE_ADD_FILE)
                        _Console.Write(L"%s add file\n", AccessMode);

                    if ((objectType == FileObject) && ea->grfAccessPermissions & FILE_APPEND_DATA)
                        _Console.Write(L"%s append data\n", AccessMode);

                    if ((objectType == FileObject) && ea->grfAccessPermissions & FILE_EXECUTE)
                        _Console.Write(L"%s execute\n", AccessMode);

                    if ((objectType == NamedPipeObject) && ea->grfAccessPermissions & FILE_CREATE_PIPE_INSTANCE)
                        _Console.Write(L"%s create instance\n", AccessMode);
                }
                else
                if (objectType == DirectoryObject)
                {
                    if (ea->grfAccessPermissions & FILE_ADD_SUBDIRECTORY)
                        _Console.Write(L"%s add subdirectory\n", AccessMode);

                    if (ea->grfAccessPermissions & FILE_TRAVERSE)
                        _Console.Write(L"%s traverse\n", AccessMode);

                    if (ea->grfAccessPermissions & FILE_DELETE_CHILD)
                        _Console.Write(L"%s delete child\n", AccessMode);
                }

                if ((objectType == FileObject) || (objectType == DirectoryObject))
                {
                    if (ea->grfAccessPermissions & FILE_READ_EA)
                        _Console.Write(L"%s read extended attributes\n", AccessMode);

                    if (ea->grfAccessPermissions & FILE_WRITE_EA)
                        _Console.Write(L"%s write extended attributes\n", AccessMode);

                    if (ea->grfAccessPermissions & FILE_READ_ATTRIBUTES)
                        _Console.Write(L"%s read attributes\n", AccessMode);

                    if (ea->grfAccessPermissions & FILE_WRITE_ATTRIBUTES)
                        _Console.Write(L"%s write attributes\n", AccessMode);
                }

                _Console.SetBullet(false);

            _Console.DecreaseIndent();

            _Console.Write(L"Standard Access Rights\n");
            _Console.IncreaseIndent();

                _Console.SetBullet(true);

                if (ea->grfAccessPermissions & DELETE)                  _Console.Write(L"%s delete access\n", AccessMode);
                if (ea->grfAccessPermissions & READ_CONTROL)            _Console.Write(L"%s read access to the owner, group, and DACL\n", AccessMode);
                if (ea->grfAccessPermissions & WRITE_DAC)               _Console.Write(L"%s write access to the DACL\n", AccessMode);
                if (ea->grfAccessPermissions & WRITE_OWNER)             _Console.Write(L"%s write access to the owner\n", AccessMode);
                if (ea->grfAccessPermissions & SYNCHRONIZE)             _Console.Write(L"%s synchronize access\n", AccessMode);

                _Console.SetBullet(false);

            _Console.DecreaseIndent();

            _Console.Write(L"Generic Access Rights\n");
            _Console.IncreaseIndent();

                _Console.SetBullet(true);

                if (ea->grfAccessPermissions & ACCESS_SYSTEM_SECURITY)  _Console.Write(L"%s access to system access control list (SACL)\n", AccessMode);
                if (ea->grfAccessPermissions & MAXIMUM_ALLOWED)         _Console.Write(L"%s maximum allowed access\n", AccessMode);

                if (ea->grfAccessPermissions & GENERIC_ALL)             _Console.Write(L"%s generic all\n", AccessMode);
                if (ea->grfAccessPermissions & GENERIC_EXECUTE)         _Console.Write(L"%s generic execute\n", AccessMode);
                if (ea->grfAccessPermissions & GENERIC_WRITE)           _Console.Write(L"%s generic write\n", AccessMode);
                if (ea->grfAccessPermissions & GENERIC_READ)            _Console.Write(L"%s generic read\n", AccessMode);

                _Console.SetBullet(false);

            _Console.DecreaseIndent();
        _Console.DecreaseIndent();
    }

    {
        _Console.SetItalic(true);
        _Console.Write(L"Inheritance\n");
        _Console.SetItalic(false);
        _Console.IncreaseIndent();

            _Console.SetBullet(true);

            if (ea->grfInheritance & INHERITED_ACE) _Console.Write(L"Inherited from parent\n");

            if (ea->grfInheritance & CONTAINER_INHERIT_ACE) _Console.Write(L"Applies to child containers\n");
            if (ea->grfInheritance & OBJECT_INHERIT_ACE) _Console.Write(L"Applies to child objects\n");

            if (ea->grfInheritance & INHERIT_NO_PROPAGATE) _Console.Write(L"Inherited but not propagated\n");
            if (ea->grfInheritance & INHERIT_ONLY) _Console.Write(L"Inherited\n");
            if (ea->grfInheritance & INHERIT_ONLY_ACE) _Console.Write(L"Does not apply to this object. Will only be inherited by child objects and child containers\n"); else _Console.Write(L"Applies to this object only.\n");

            if (ea->grfInheritance & NO_INHERITANCE) _Console.Write(L"Will not be inherited by child objects or child containers\n");
            if (ea->grfInheritance & NO_PROPAGATE_INHERIT_ACE) _Console.Write(L"Does not propagate to child objects or child containers\n");

            _Console.SetBullet(false);

        _Console.DecreaseIndent();
    }

    return S_OK;
}

///<summary>
/// Shows the trustee defined in an ACE.
///</summary>
static HRESULT ShowTrustee(const TRUSTEE_W * Trustee)
{
    switch (Trustee->TrusteeForm)
    {
        case TRUSTEE_IS_SID:
            ShowSID((PSID)(Trustee->ptstrName));
            break;

        case TRUSTEE_IS_NAME:
            _Console.Write(L"%s\n", Trustee->ptstrName);
            break;

        case TRUSTEE_IS_OBJECTS_AND_SID:
        {
            POBJECTS_AND_SID oas = (POBJECTS_AND_SID)Trustee->ptstrName;

            ShowSID(oas->pSid);

            WCHAR GuidString[256] = { '\0' };

            if ((oas->ObjectsPresent & ACE_OBJECT_TYPE_PRESENT) && SUCCEEDED(GUIDToText(&oas->ObjectTypeGuid, GuidString, ARRAYSIZE(GuidString))))
            {
                _Console.Write(L"Object Type: %s\n", GuidString);
            }

            if ((oas->ObjectsPresent & ACE_INHERITED_OBJECT_TYPE_PRESENT) && SUCCEEDED(GUIDToText(&oas->InheritedObjectTypeGuid, GuidString, ARRAYSIZE(GuidString))))
            {
                _Console.Write(L"Inherited Object Type: %s\n", GuidString);
            }
            break;
        }

        case TRUSTEE_IS_OBJECTS_AND_NAME:
        {
            POBJECTS_AND_NAME_W oan = (POBJECTS_AND_NAME_W)Trustee->ptstrName;

            _Console.Write(L"%s\n", Trustee->ptstrName);

            if (oan->ObjectsPresent & ACE_OBJECT_TYPE_PRESENT)
            {
                _Console.Write(L"Object Type: %s\n", oan->ObjectTypeName);
            }

            if (oan->ObjectsPresent & ACE_INHERITED_OBJECT_TYPE_PRESENT)
            {
                _Console.Write(L"Inherited Object Type: %s\n", oan->InheritedObjectTypeName);
            }
            break;
        }

        case TRUSTEE_BAD_FORM:
        {
            _Console.Write(L"<Invalid>\n");
            break;
        }

        default:
            _Console.Write(L"Unknow trustee form %08X\n", Trustee->TrusteeForm);
            break;
    }

    return S_OK;
}

///<summary>
/// Gets the description of the ACE type.
///</summary>
LPCWSTR GetACETypeDescription(BYTE aceType)
{
    switch (aceType)
    {
        case ACCESS_ALLOWED_ACE_TYPE: return L"Access Allowed";
        case ACCESS_DENIED_ACE_TYPE: return L"Access Denied";
        case SYSTEM_AUDIT_ACE_TYPE: return L"System Audit";
        case SYSTEM_ALARM_ACE_TYPE: return L"System Alarm";

        case ACCESS_ALLOWED_COMPOUND_ACE_TYPE: return L"Access Allowed Compound";

        case ACCESS_ALLOWED_OBJECT_ACE_TYPE: return L"Object-specific Access Allowed";
        case ACCESS_DENIED_OBJECT_ACE_TYPE: return L"Object-specific Access Allowed";
        case SYSTEM_AUDIT_OBJECT_ACE_TYPE: return L"Object-specific System Audit";
        case SYSTEM_ALARM_OBJECT_ACE_TYPE: return L"Object-specific System Alarm";

        case ACCESS_ALLOWED_CALLBACK_ACE_TYPE: return L"Access Allowed Callback";
        case ACCESS_DENIED_CALLBACK_ACE_TYPE: return L"Access Allowed Callback";
        case ACCESS_ALLOWED_CALLBACK_OBJECT_ACE_TYPE: return L"Object-specific Access Allowed Callback";
        case ACCESS_DENIED_CALLBACK_OBJECT_ACE_TYPE: return L"Object-specific Access Denied Callback";
        case SYSTEM_AUDIT_CALLBACK_ACE_TYPE: return L"System Audit Callback";
        case SYSTEM_ALARM_CALLBACK_ACE_TYPE: return L"System Alarm Callback";
        case SYSTEM_AUDIT_CALLBACK_OBJECT_ACE_TYPE: return L"Object-specific System Audit Callback";
        case SYSTEM_ALARM_CALLBACK_OBJECT_ACE_TYPE: return L"Object-specific System Alarm Callback";

        case SYSTEM_MANDATORY_LABEL_ACE_TYPE: return L"System Mandatory Label";
        case SYSTEM_RESOURCE_ATTRIBUTE_ACE_TYPE: return L"System Resource Attribute";
        case SYSTEM_SCOPED_POLICY_ID_ACE_TYPE: return L"System Scoped Policy";
        case SYSTEM_PROCESS_TRUST_LABEL_ACE_TYPE: return L"System Process Trust Label";
        case SYSTEM_ACCESS_FILTER_ACE_TYPE: return L"System Access Filter";

        default: return L"Unknown ACE Type";
    }
}
