project "Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"
    systemversion "latest"

    targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs {
        "src",
        "../vendor/raylib/include",
        "../vendor/imgui",
        "../Core/src",
        "../vendor/glm"
    }

    libdirs {
        "../vendor/raylib/lib"
    }

    links {
        "Core",
        "raylibdll",
        "ImGui"
    }

    postbuildcommands { ("{COPY} ..\\vendor\\raylib\\lib\\raylib.dll ..\\bin\\" .. outputdir .. "\\Editor") }    

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
