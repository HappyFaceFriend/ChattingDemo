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

	externalproject "assimp"
		location "/vendor/Ramensoup/Ramensoup/vendor/assimp"
   		kind "StaticLib"
	externalproject "GLFW"
		location"/vendor/Ramensoup/Ramensoup/vendor/GLFW"
		kind "StaticLib"
	externalproject "Glad"
		location "/vendor/Ramensoup/Ramensoup/vendor/Glad"
		kind "StaticLib"
	externalproject "ImGui"
		location "/vendor/Ramensoup/Ramensoup/vendor/ImGui"
		kind "StaticLib"
	externalproject "yaml-cll"
		location "/vendor/Ramensoup/Ramensoup/vendor/yaml-cpp"
		kind "StaticLib"

	externalproject "Ramensoup"
		location "vendor/Ramensoup/Ramensoup"
		kind "StaticLib"
		language "C++"
		staticruntime "on"

    include "RamenNetworking"
    include "ChattingDemo-Server"
    include "ChattingDemo-Client"