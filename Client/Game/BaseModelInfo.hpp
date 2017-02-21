// BaseModelInfo.hpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"

namespace ctn::SA
{

class CBaseModelInfo
{
public:
    virtual ~CBaseModelInfo() = 0;
    virtual CBaseModelInfo* AsAtomicModelInfoPtr();
    virtual CBaseModelInfo* AsDamageAtomicModelInfoPtr();
    virtual CBaseModelInfo* AsLodAtomicModelInfoPtr();
    virtual void GetModelType() = 0;
    virtual class CTimeInfo* GetTimeInfo();
    virtual void Init();
    virtual void Shutdown();
    virtual void DeleteRwObject() = 0;
    virtual u32 GetRwModelType() = 0;
    virtual struct RwObject* CreateInstance(class RwMatrix* matrix) = 0;
    virtual struct RwObject* CreateInstance() = 0;
    virtual void SetAnimFile(char* filename);
    virtual void ConvertAnimFileIndex();
    virtual s32 GetAnimFileIndex();

public:
    u32 hashKey_;
    u16 refCount_;
    s16 txdSlot_;
    u8 alpha_;
    u8 num2dfx_;
    s16 m_2dfxIndex_;
    s16 objectIndex_;
    u16 flags_;
    class CColModel* colModel_;
    f32 drawDistance_;

    union
    {
        struct RwObject* rwObject_;
        struct RpClump* rpClump_;
        struct RpAtomic* rpAtomic_;
    };
};

VALIDATE_SIZE(CBaseModelInfo, 0x20);

}
