project "ChattingDemo-Server"
    kind "ConsoleApp"
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
		"../RamenNetworking/src",
		"src",

		"../vendor/Ramensoup/Ramensoup/src",
		"%{IncludeDirs.ImGui}",
		"%{IncludeDirs.glm}",
		"%{IncludeDirs.assimp}",
		"%{IncludeDirs.entt}",
		"%{IncludeDirs.yaml}",
		"%{IncludeDirs.spdlog}",
	}

	links
	{
		"RamenNetworking",
		"Ramensoup"
	}

	filter "system:windows"
	systemversion "latest"

	defines
	{
		"RNET_PLATFORM_WINDOWS",
	}

	filter "configurations:Debug"
		defines "CHAT_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CHAT_RELEASE"
		runtime "Release"
		optimize "on"