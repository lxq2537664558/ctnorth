// Game hooks
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include <Hooking/HookingUtils.hpp>
#include <Hooking/HookFunction.hpp>

using namespace Util;

static bool menuFirstProcessed = false;
void ProcessFrontEndMenu()
{
    if (!menuFirstProcessed)
    {
        // Start the game now - unpause the timers
        MemPatch<u8>(0xB7CB49, 0);
        MemPatch<u8>(0xBA67A4, 0);
        MemPatch<u32>(0xC8D4C0, 8); // gGameState
        menuFirstProcessed = true;
    }

    ThisCall<void>(0x573A60); // Call original - Sets Render States
}

static Util::HookFunction hookFunction([]()
{
    // Hook menu process
    MakeCALL(0x57C2BC, ProcessFrontEndMenu);

    // Better than SetWindowText
    MemPatch(0x619608, "San Andreas Online");

    // Unlocked widescreen resolutions
    MemPatch<u32>(0x745B81, 0x9090587D);
    MemPatch<u32>(0x74596C, 0x9090127D);
    MakeNOP(0x745970, 2);
    MakeNOP(0x745B85, 2);
    MakeNOP(0x7459E1, 2);
});
