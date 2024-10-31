workspace "ChattingDemo"
	architecture "x64"
	platforms {"Win64"}
	configurations {"Debug", "Release"}


	-- includes from Ramensoup, used in server,client projects
	IncludeDirs = {}
	RamensoupPath = "../vendor/Ramensoup/Ramensoup/" 
	IncludeDirs["GLFW"] = RamensoupPath .. "vendor/GLFW/include"
	IncludeDirs["Glad"] = RamensoupPath .. "vendor/Glad/include"
	IncludeDirs["ImGui"] = RamensoupPath .. "vendor/ImGui"
	IncludeDirs["glm"] = RamensoupPath .. "vendor/glm"
	IncludeDirs["stb_image"] = RamensoupPath .. "vendor/stb_image"
	IncludeDirs["assimp"] = RamensoupPath .. "vendor/assimp/include"
	IncludeDirs["entt"] = RamensoupPath .. "vendor/entt/include"
	IncludeDirs["yaml_cpp"] = RamensoupPath .. "vendor/yaml-cpp/include"
	IncludeDirs["spdlog"] = RamensoupPath .. "vendor/spdlog/include"
	
	
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	
	include "/vendor/Ramensoup/Ramensoup/vendor/assimp"
	include "/vendor/Ramensoup/Ramensoup/vendor/GLFW"
	include "/vendor/Ramensoup/Ramensoup/vendor/Glad"
	include "/vendor/Ramensoup/Ramensoup/vendor/ImGui"
	include "/vendor/Ramensoup/Ramensoup/vendor/yaml-cpp"

	externalproject "Ramensoup"
		location "vendor/Ramensoup/Ramensoup"
		kind "StaticLib"
		language "C++"
		staticruntime "on"

    include "RamenNetworking"
    include "ChattingDemo-Server"
    include "ChattingDemo-Client"