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
    kind "SharedLib"
    language "C++"

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
        mklLibDir .. "mkl_intel_ilp64_dll.lib",                     -- interface
        mklLibDir .. "mkl_intel_thread_dll.lib",                    -- threading
        mklLibDir .. "mkl_core_dll.lib",                            -- core
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "Off"
        systemversion "latest"

        defines 
        {
            "MC_BUILD_DLL"
        }

        buildoptions 
        {
            "/openmp",
            "/DMKL_ILP64"
        }

        links 
        {
            mklRootDir .. "compiler/lib/intel64_win/libiomp5md.dll"     -- openmp
        }

    filter "configurations:Debug"
        defines "_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "NDEBUG"
        optimize "On"

    filter {"system:windows", "configurations:Debug"}
        buildoptions {"/MDd"}

    filter {"system:windows", "configurations:Release"}
        buildoptions {"/MD"}

 
project "sandbox"
    location "sandbox"
    kind "ConsoleApp"
    language "C++"

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
        mklLibDir .. "mkl_intel_ilp64_dll.lib",                     -- interface
        mklLibDir .. "mkl_intel_thread_dll.lib",                    -- threading
        mklLibDir .. "mkl_core_dll.lib",                            -- core
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "Off"
        systemversion "latest"

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
            mklRootDir .. "compiler/lib/intel64_win/libiomp5md.dll"     -- openmp
        }

        postbuildmessage "copying dll..."
        postbuildcommands 
        {
            -- still in project directory so need to use ../ to backup one level and access bin/
            ("{COPY} ../bin/" .. outputdir.. "/molecool/molecool.dll ../bin/" ..  outputdir .. "/%{prj.name}")
        }

    filter "configurations:Debug"
        defines "_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "NDEBUG"
        optimize "On"

    filter {"system:windows", "configurations:Debug"}
        buildoptions {"/MTd"}

    filter {"system:windows", "configurations:Release"}
        buildoptions {"/MT"}
