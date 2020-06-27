workspace "molecool" -- workspace/solution name
    architecture "x86_64"

    configurations
    {
        "Debug",
        "Release"
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    -- e.g. \Debug-windows-x86_64

    filter "system:windows"
        mklRootDir = "C:/Program Files (x86)/IntelSWTools/compilers_and_libraries_2020.1.216/windows/"
        mklLibDir = mklRootDir .. "mkl/lib/intel64_win/"
        boostDir = "C:/Boost/boost_1_73_0"


project "molecool"
    location "molecool"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    systemversion "latest"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("build/" .. outputdir .. "/%{prj.name}")

    files -- files to include in project
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs -- compiler include directories
    {
        "%{prj.name}/vendor/spdlog/include",
        boostDir,
        mklRootDir .. "mkl/include"
    }

    libdirs
    {
        mklLibDir
    }

    links 
    {
        mklLibDir .. "mkl_intel_ilp64.lib",     -- interface
        mklLibDir .. "mkl_intel_thread.lib",    -- threading
        mklLibDir .. "mkl_core.lib",            -- core
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

        links 
        {
            mklRootDir .. "compiler/lib/intel64_win/libiomp5md.lib"     -- openmp
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

    files -- files to include in project
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs -- compiler include directories
    {
        "molecool/vendor/spdlog/include",
        "molecool/src",
        boostDir,
        mklRootDir .. "mkl/include"
    }

    libdirs
    {
        mklLibDir
    }

    links 
    {
        "molecool",
        mklLibDir .. "mkl_intel_ilp64.lib",     -- interface
        mklLibDir .. "mkl_intel_thread.lib",    -- threading
        mklLibDir .. "mkl_core.lib",            -- core
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

        links 
        {
            mklRootDir .. "compiler/lib/intel64_win/libiomp5md.lib"     -- openmp
        }

    filter "configurations:Debug"
        defines "_DEBUG"
        symbols "on"

    filter "configurations:Release"
        defines "NDEBUG"
        optimize "on"

