project "RamenNetworking"
    kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
    
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")
    
	files
	{
		"src/**.h",
		"src/**.cpp",
	}
	includedirs
	{
		"src",
		"vendor/spdlog/include",
	}
	removefiles 
	{
		"src/Platform/**.h",
		"src/Platform/**.cpp"
	}
	
	filter "system:windows"
		systemversion "latest"

		defines
		{
			"RNET_PLATFORM_WINDOWS",
		}
		files
		{
			"src/Platform/Winsock/**.h",
			"src/Platform/Winsock/**.cpp"
		}

	filter "configurations:Debug"
		defines "RNET_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RNET_RELEASE"
		runtime "Release"
		optimize "on"