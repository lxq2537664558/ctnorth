// Game EXE loader
// Author(s):       iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 The San Andreas Online Open Source Project
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

#pragma once

class GTASALauncher
{
public:
    static void Launch(const char* gamePath);

private:
    static void InvokeEntryPoint(void(*)());
};