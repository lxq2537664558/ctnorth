// PE32 Loader
// Author(s):       iFarbod <ifarbod@outlook.com>
//                  NTAuthority
//
// Copyright (c) 2015-2016 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

#include <winnt.h>

class ExecutableLoader
{
public:
    ExecutableLoader(const uint8_t* origBinary);

    void SetLoadLimit(uintptr_t loadLimit) { loadLimit_ = loadLimit; }

    void* GetEntryPoint() { return entryPoint_; }

    void LoadIntoModule(HMODULE module);

    void SetLibraryLoader(HMODULE(*loader)(const char*)) { libraryLoader_ = loader; }

    void SetFunctionResolver(LPVOID (*functionResolver)(HMODULE, const char*)) { functionResolver_ = functionResolver; }

private:
    const uint8_t* origBinary_;
    HMODULE executableHandle_;
    uintptr_t loadLimit_;
    void* entryPoint_;
    HMODULE(*libraryLoader_)(const char*);
    LPVOID(*functionResolver_)(HMODULE, const char*);

    void LoadSection(IMAGE_SECTION_HEADER* section);
    void LoadSections(IMAGE_NT_HEADERS* ntHeader);

    void LoadImports(IMAGE_NT_HEADERS* ntHeader);

    HMODULE ResolveLibrary(const char* name);

    LPVOID ResolveLibraryFunction(HMODULE module, const char* name);

    template <class T> inline const T* GetRVA(uint32_t rva) { return (T*)(origBinary_ + rva); }

    template <class T> inline T* GetTargetRVA(uint32_t rva) { return (T*)((uint8_t*)executableHandle_ + rva); }
};
