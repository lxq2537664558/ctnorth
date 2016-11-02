﻿// Client Launcher entry point
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  AliAssassiN <ailsynthax@gmail.com>
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE.md or copy at
// https://opensource.org/licenses/MIT)

#include "Precompiled.hpp"
#include <cassert>
#include <Registry.hpp>

#define TEST

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR strCmdLine, int nCmdShow)
{
#ifdef TEST
    using namespace Util;
    if (!WriteRegStr(HKLM, "SOFTWARE\\SAO Test", "hey", "meow"))
        MessageBox(0, 0, 0, 0);

    MessageBoxW(0, WString(ReadRegStr(HKLM, "SOFTWARE\\SAO Test", "hey")).CString(), 0, 0);
    return 1;
#endif

    // Check Windows version
    if (!IsWindows7SP1OrGreater())
    {
        // ERR_WINDOWS_NOT_SUPPORTED
        MessageBoxA(NULL, "You need Windows 7 SP1 or greater to play San Andreas Online.", NULL, MB_ICONSTOP);
        return 1;
    }

    // Check for safe mode
    if (GetSystemMetrics(SM_CLEANBOOT) != 0)
    {
        // ERR_CLEANBOOT
        MessageBoxA(NULL, "You cannot play San Andreas Online in Safe Mode.", NULL, MB_ICONSTOP);
        return 1;
    }

    // Launch GTA:SA
    STARTUPINFO si = { 0 };
    PROCESS_INFORMATION pi = { 0 };
    si.cb = sizeof(si);

    if (!CreateProcessW(L"D:\\SA\\gta_sa_compact.exe", L"", nullptr, nullptr, FALSE, CREATE_SUSPENDED, nullptr, L"D:\\SA\\", &si, &pi))
    {
        MessageBoxA(nullptr, "Failed to start San Andreas. Cannot launch SA:Online.", nullptr, MB_ICONSTOP);
        return 1;
    }

#ifdef SAO_DEBUG
    const wchar_t* pszLibToInject = L"Core_d.dll";
#else
    const wchar_t* pszLibToInject = L"Core.dll";
#endif

    SIZE_T ulWrittenBytes;
    void* pAllocedMem = nullptr;
    HANDLE hExternThread = nullptr;

    // Allocate memory in the remote process
    pAllocedMem = VirtualAllocEx(pi.hProcess, NULL, wcslen(pszLibToInject) * 2 + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    if(!(pAllocedMem))
        return 1;

    // Write the DLL path to the remote process memory
    if(!WriteProcessMemory(pi.hProcess, pAllocedMem, (void*)pszLibToInject, wcslen(pszLibToInject) * 2 + 1, &ulWrittenBytes))
        return 1;

    if(wcslen(pszLibToInject) * 2 + 1 != ulWrittenBytes)
        return 1;

    // Create a remote thread at LoadLibraryW and pass the dll path to it
    hExternThread = CreateRemoteThread(pi.hProcess, NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(LoadLibraryW), pAllocedMem, 0, NULL);

    if(!hExternThread)
        return 1;

    // Set the thread's priority so we can be sure the DLL injects
    SetThreadPriority(hExternThread, THREAD_PRIORITY_HIGHEST);

    // Wait for the thread to do its job
    WaitForSingleObject(hExternThread, 3000);

    // Finally, close the thread's handle
    CloseHandle(hExternThread);

    // Free the memory we allocated so we don't cause a memory leak
    VirtualFreeEx(pi.hProcess, pAllocedMem, wcslen(pszLibToInject) * 2 + 1, MEM_RELEASE);

    // And resume the game's thread now
    ResumeThread(pi.hThread);

    // Success
    return 0;
}
