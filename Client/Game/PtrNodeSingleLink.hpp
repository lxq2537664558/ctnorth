// PtrNodeSingleLink.hpp
// Author(s):       iFarbod <>
//
// Copyright (c) 2015-2017 CTNorth Team
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include "Common.hpp"

namespace ctn::SA
{

class CPtrNodeSingleLink
{
public:
    // Memory allocation in pool.
    void* operator new(u32 size);
    // Memory deallocation in pool.
    void operator delete(void* ptr);

    void* m_pointer;            // +0x0
    CPtrNodeSingleLink* m_next; // +0x4
};

VALIDATE_SIZE(CPtrNodeSingleLink, 8);

}
