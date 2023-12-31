project "Simulation"
    kind "StaticLib"
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
        "../vendor/glm",
        "../vendor/rlImGui",
        "../vendor/entt",
        "../vendor/yml-cpp/include"
    }

    libdirs {
        "../vendor/raylib/lib"
    }

    links {
        "raylibdll",
        "ImGui",
        "YAML-CPP"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
