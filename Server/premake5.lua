project "DedicatedServer"
    language "C++"
    kind "ConsoleApp"
    targetdir(buildpath("Server"))
    
    vpaths { 
        ["Headers/*"] = { "**.h", "**.hpp" },
        ["Sources/*"] = "**.cpp",
        ["Resources/*"] = {"*.rc", "res/sao_icon.ico"},
        ["*"] = "premake5.lua"
    }

    files {
        "premake5.lua",
        "**.hpp",
        "**.cpp"
    }
    
    filter "system:windows"
        targetname "SAOServer-Win32"
        files {
            "**.rc",
            "res/sao_icon.ico"
        }

    filter {"system:windows", "platforms:x64"}
        targetname "SAOServer-Win64"

    filter "system:linux"
        links { "dl" }

    filter {"system:linux", "platforms:x86"}
        targetname "sao-server"

    filter {"system:linux", "platforms:x64"}
        targetname "sao-server64"
    
    