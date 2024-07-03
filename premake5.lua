workspace "ChattingDemo"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

    include "RamenNetworking"
    include "ChattingDemo-Server"
    include "ChattingDemo-Client"