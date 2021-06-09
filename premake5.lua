workspace "SimCombat"
	architecture "x64"
	startproject "Game"
	
	configurations
	{
		"Debug-DX",
		"Release-DX",
		"Debug-GL",
		"Release-GL",
	}

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

project "Engine"
	location "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "Engine/src/pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/src",
		"Common/spdlog/include",
		"Common/glfw/include",
		"Common/glew/include",
	}

	libdirs
	{
		"Common/glfw/lib",
		"Common/glew/lib",
	}

	links
	{
		"d3d11.lib",
		"opengl32.lib",
		"glfw3.lib",
		"glew32s.lib",
		"dxgi.lib",
		"ws2_32.lib"
	}

	filter "system:windows"
		staticruntime "on"
		systemversion "latest"

		defines
		{
			"_LIB",
			"GLEW_STATIC",
		}

	filter "configurations:Debug-DX"
		defines 
		{
			"_DEBUG",
			"_DX"
		}
		symbols "on"
		buildoptions "/MDd"

	filter "configurations:Release-DX"
		defines 
		{
			"_DX"
		}
		optimize "on"
		buildoptions "/MD"

	filter "configurations:Debug-GL"
		defines 
		{
			"_DEBUG",
			"_GL"
		}
		symbols "on"
		buildoptions "/MDd"

	filter "configurations:Release-GL"
		defines 
		{
			"_GL"
		}
		optimize "on"
		buildoptions "/MD"

project "Game"
	location "Game"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Engine/src",
		"Common/spdlog/include",
		"Common/glfw/include",
		"Common/glew/include",
	}

	links
	{
		"Engine"
	}

	filter "system:windows"		
		systemversion "latest"

		defines
		{
			
		}

	filter "configurations:Debug-DX"
		defines 
		{
			"_DEBUG",
			"_DX"
		}
		symbols "on"
		buildoptions "/MDd"

	filter "configurations:Release-DX"
		defines 
		{
			"_DX"
		}
		optimize "on"
		buildoptions "/MD"

	filter "configurations:Debug-GL"
		defines 
		{
			"_DEBUG",
			"_GL"
		}
		symbols "on"
		buildoptions "/MDd"

	filter "configurations:Release-GL"
		defines 
		{
			"_GL"
		}
		optimize "on"
		buildoptions "/MD"