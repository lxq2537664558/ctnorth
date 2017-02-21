// WeaponModelInfo.hpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"
#include "ClumpModelInfo.hpp"

namespace ctn::SA
{

class CWeaponModelInfo : public CClumpModelInfo
{
public:
    u32 weaponInfo_; // eWeaponType
};

VALIDATE_SIZE(CWeaponModelInfo, 0x28);

}