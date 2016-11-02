// Client Core module entry point
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  AliAssassiN <ailsynthax@gmail.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE.md or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include "Core.hpp"

Core * g_pCore;

BOOL WINAPI DllMain(HINSTANCE hDllInstance, DWORD dwReason, LPVOID lpvReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hDllInstance);

        //SetCurrentDirectory(gtaDir)
        //SetDllDirectory(SAOPath("SAO"));
        g_pCore = new Core();
        //SetCurrentDirectory(gtaDir)
    }

    else if (dwReason == DLL_PROCESS_DETACH)
    {
        TerminateProcess(GetCurrentProcess(), 0);

        delete g_pCore;
        g_pCore = nullptr;
    }
    return TRUE;
}
