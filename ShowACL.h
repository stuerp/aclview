
/** ShowACL.h (2023.06.14) P. Stuer **/

#pragma once

enum ObjectType
{
    UnknownObjectType,
    DirectoryObject,
    FileObject,
    NamedPipeObject,
    ProcessObject,
    AccessTokenObject,
};

HRESULT ShowACL(const PACL acl, ObjectType objectType);
