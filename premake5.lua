-- include paths and linking depend on the environment variables MKLROOT and BOOSTROOT

workspace "molecool" -- workspace/solution name
    architecture "x86_64"
    startproject "sandbox"

    configurations
    {
        "Debug",
        "Release"
    }

    flags 
    {
        "MultiProcessorCompile"
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}" -- e.g. \Debug-windows-x86_64

    -- MKL library paths etc.
    mklRootDir = os.getenv("MKLROOT")   -- e.g. "C:/Program Files (x86)/IntelSWTools/compilers_and_libraries_2020.1.216/windows/"
    mklArch = "intel64"
    filter "system:windows"
        mklArch = mklArch .. "_win"
    filter {}   -- deactivate filter
    mklLibDir = mklRootDir .. "mkl/lib/" .. mklArch .. "/"
    mklOmpDir = mklRootDir .. "compiler/lib/" .. mklArch .. "/"
    mklIncDir = mklRootDir .. "mkl/include"

    -- Boost library paths etc.
    boostDir = os.getenv("BOOSTROOT")   -- e.g. "C:/Boost/boost_1_73_0/"
    boostLibDir = boostDir .. "stage/lib/"  


------------------------------------------------------------------
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

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        boostDir,
        mklIncDir,
        "%{solutionDir}/vendor/lua"
    }

    libdirs {
        mklLibDir,
        mklOmpDir,
        boostLibDir
    }

    links {
        "mkl_intel_ilp64.lib",     -- interface
        "mkl_intel_thread.lib",    -- threading
        "mkl_core.lib",            -- core
        "libiomp5md.lib"           -- openmp
    }

    filter "system:windows"
        
        defines {
            MC_PLATFORM_WINDOWS
        }

        buildoptions {
            "/openmp",
            "/DMKL_ILP64"
        }

    filter "system:macosx"

        defines {
            MC_PLATFORM_MACOSX
        }

        buildoptions {
            "-qopenmp",
            "-DMKL_ILP64"
        }

    filter "system:linux"

        defines {
            MC_PLATFORM_LINUX
        }

        buildoptions {
            "-qopenmp",
            "-DMKL_ILP64"
        }

    filter "configurations:Debug"
        symbols "On"
        runtime "Debug"
        
        defines {
            "MC_DEBUG"
        }

    filter "configurations:Release"
        optimize "Speed"
        runtime "Release"
        
        defines {
            "MC_RELEASE"
        }


------------------------------------------------------------------
project "sandbox"
    location "sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    systemversion "latest"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("build/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "molecool/src",
        "molecool/vendor/spdlog/include",
        boostDir,
        mklIncDir,
        "%{solutionDir}/vendor/lua"
    }

    libdirs {
        mklLibDir,
        mklOmpDir,
        boostLibDir
    }

    links {
        "molecool",
        "mkl_intel_ilp64.lib",    
        "mkl_intel_thread.lib",    
        "mkl_core.lib",   
        "libiomp5md.lib"
    }

    filter "system:windows"

        defines {
            MC_PLATFORM_WINDOWS
        }

        buildoptions {
            "/openmp",
            "/DMKL_ILP64"
        }

    filter "system:macosx"

        defines {
            MC_PLATFORM_MACOSX
        }

        buildoptions {
            "-qopenmp",
            "-DMKL_ILP64"
        }

    filter "system:linux"

        defines {
            MC_PLATFORM_LINUX
        }

        buildoptions {
            "-qopenmp",
            "-DMKL_ILP64"
        }

    filter "configurations:Debug"
        symbols "On"
        runtime "Debug"
        
        defines {
            "_DEBUG",
            "MC_DEBUG"
        }

    filter "configurations:Release"
        optimize "Speed"
        runtime "Release"
        
        defines {
            "NDEBUG",
            "MC_RELEASE"
        }

