// Crash fixes
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

u32 RETURN_CTaskComplexCarSlowBeDraggedOut__PrepareVehicleForPedExit = 0x6485B2;
u32 RETURN_CTaskComplexCarSlowBeDraggedOut__PrepareVehicleForPedExit_Invalid = 0x6485E1;
void OnMY_CTaskComplexCarSlowBeDraggedOut__PrepareVehicleForPedExit()
{
    //SAO_LOGWARNING("CTaskComplexCarSlowBeDraggedOut race condition");
}

void __declspec(naked) Hook_CTaskComplexCarSlowBeDraggedOut__PrepareVehicleForPedExit()
{
    __asm
    {
        test eax, eax;
        jz InvalidVehicle;

        mov ecx, [eax + 460h];
        jmp RETURN_CTaskComplexCarSlowBeDraggedOut__PrepareVehicleForPedExit;

    InvalidVehicle:
        pushad;
        call OnMY_CTaskComplexCarSlowBeDraggedOut__PrepareVehicleForPedExit;
        popad;
        jmp RETURN_CTaskComplexCarSlowBeDraggedOut__PrepareVehicleForPedExit_Invalid;
    }
}

static Util::HookFunction hookFunction([]()
{
    // Crash fix for creating more than 8 Players
    // Well, it isn't because the game actually has a limit of 8 players.
    // It's because on CPlayerPed contstruction, it actually creates a CPedGroup for that player
    // And CPedGroups limit is 8. (There's an array with size 8 - CPedGroups g_pPedGroups[8])
    // Skip the code which creates ped group, so we can create more than 8 players with ease
    MakeNOP(0x60D64D);
    MemPatch<u8>(0x60D64E, 0xE9);

    // Disable call to FxSystem_c::GetCompositeMatrix in CAEFireAudioEntity::UpdateParameters 
    // Which was causing a crash. The crash happens if you create 40 or 
    // so vehicles that catch fire (upside down) then delete them, repeating a few times.
    MakeNOP(0x4DCF87, 6);

    // Fixed a crash (race condition triggered when jacking a vehicle)
    MakeNOP(0x6485AC, 6);
    MakeJMP(0x6485AC, Hook_CTaskComplexCarSlowBeDraggedOut__PrepareVehicleForPedExit);

    // Mirror crash fixed
    MemPatch<u8>(0x7271CB + 0, 0x85); // test eax, eax
    MemPatch<u8>(0x7271CB + 1, 0xC0);
    MemPatch<u8>(0x7271CB + 2, 0x74); // je 0x727203
    MemPatch<u8>(0x7271CB + 3, 0x34);
    MemPatch<u8>(0x7271CB + 4, 0x83); // add esp, 04
    MemPatch<u8>(0x7271CB + 5, 0xC4);
    MemPatch<u8>(0x7271CB + 6, 0x04);
    MemPatch<u8>(0x7271CB + 7, 0xC6); // mov byte ptr [00C7C728],01
});