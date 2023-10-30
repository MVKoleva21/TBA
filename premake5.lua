workspace "TBA"
    configurations { 
        "Debug", 
        "Release" 
    }

    startproject "Editor"

    architecture "x64"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "ImGui"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"
    systemversion "latest"
    location "vendor/imgui"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "vendor/imgui/imconfig.h",
        "vendor/imgui/imgui.cpp",
        "vendor/imgui/imgui_demo.cpp",
        "vendor/imgui/imgui_draw.cpp",
        "vendor/imgui/imgui.h",
        "vendor/imgui/imgui_internal.h",
        "vendor/imgui/imgui_tables.cpp",
        "vendor/imgui/imgui_widgets.cpp",
        "vendor/imgui/imstb_rectpack.h",
        "vendor/imgui/imstb_textedit.h",
        "vendor/imgui/imstb_truetype.h",

        "vendor/rlImGui/rlImGui.cpp",
        "vendor/rlImGui/rlImGui.h",
        "vendor/rlImGui/imgui_impl_raylib.h",
    }

    includedirs {
        "vendor/imgui",
        "vendor/rlImGui",
        "vendor/raylib/include"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"

project "YAML-CPP"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"
    systemversion "latest"
    location "vendor/yml-cpp"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "vendor/yml-cpp/src/**.h",
        "vendor/yml-cpp/src/**.cpp"
    }

    includedirs {
        "vendor/yml-cpp/include",
        "vendor/yml-cpp/src"
    }

    defines {
        "YAML_CPP_STATIC_DEFINE"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"

include "Core"
include "Editor"