workspace "TBA"
    configurations { 
        "Debug", 
        "Release" 
    }

    startproject "Editor"

    architecture "x64"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Core"
include "Editor"