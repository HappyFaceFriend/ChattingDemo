project "ChattingDemo-Core"
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
	}

	filter "configurations:Debug"
		defines "CHAT_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CHAT_RELEASE"
		runtime "Release"
		optimize "on"