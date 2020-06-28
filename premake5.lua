workspace "molecool" -- workspace/solution name
    architecture "x86_64"

    configurations
    {
        "Debug",
        "Release"
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    -- e.g. \Debug-windows-x86_64

    -- include paths and linking depend on the following environment variables
    mklRootDir = os.getenv("MKLROOT")   -- e.g. "C:/Program Files (x86)/IntelSWTools/compilers_and_libraries_2020.1.216/windows/"
    boostDir = os.getenv("BOOSTROOT")   -- e.g. "C:/Boost/boost_1_73_0/"

    -- setup the mkl architecture for path construction
    mklArch = "intel64"
    filter "system:windows"
        mklArch = mklArch .. "_win"
    filter {}   -- deactivate filter

    mklLibDir = mklRootDir .. "mkl/lib/" .. mklArch .. "/"
    mklOmpDir = mklRootDir .. "compiler/lib/" .. mklArch .. "/"
    mklIncDir = mklRootDir .. "mkl/include"


project "molecool"
    location "molecool"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    systemversion "latest"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("build/" .. outputdir .. "/%{prj.name}")

    pchheader "mcpch.h"
    pchsource "molecool/src/mcpch.cpp" -- VS only, ignored on other compilers

    files -- files to include in project
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs -- compiler include directories
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        boostDir,
        mklIncDir
    }

    libdirs -- library search paths for the linker
    {
        mklLibDir,
        mklOmpDir
    }

    links -- libraries/projects to link against, like VS's "references" 
    {
        "mkl_intel_ilp64.lib",     -- interface
        "mkl_intel_thread.lib",    -- threading
        "mkl_core.lib",            -- core
        "libiomp5md.lib"           -- openmp
    }

    filter "system:windows"
        
        defines 
        {
            
        }

        buildoptions 
        {
            "/openmp",
            "/DMKL_ILP64"
        }

    filter "configurations:Debug"
        defines "_DEBUG"
        symbols "on"

    filter "configurations:Release"
        defines "NDEBUG"
        optimize "on"

 
project "sandbox"
    location "sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    systemversion "latest"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("build/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "molecool/src",
        "molecool/vendor/spdlog/include",
        boostDir,
        mklIncDir
    }

    libdirs
    {
        mklLibDir,
        mklOmpDir
    }

    links 
    {
        "molecool",
        "mkl_intel_ilp64.lib",    
        "mkl_intel_thread.lib",    
        "mkl_core.lib",   
        "libiomp5md.lib"
    }

    filter "system:windows"

        defines 
        {
         
        }

        buildoptions 
        {
            "/openmp",
            "/DMKL_ILP64"
        }

    filter "configurations:Debug"
        defines "_DEBUG"
        symbols "on"

    filter "configurations:Release"
        defines "NDEBUG"
        optimize "on"

