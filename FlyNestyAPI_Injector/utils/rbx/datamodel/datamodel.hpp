#pragma once
#include <fstream> // Opening and closing files
#include <thread> // Simple threading
#include <cstdlib> // For adding (idk)
#include <Windows.h> // WinAPI xd
#include <ShlObj.h> // for manage windows shell
#include <vector> // Dinamic arrays
#include <filesystem> // manage fs
#include <string>
#include <algorithm> // Simple algorithms
#include <cstdint> // uint данные

namespace logs {
	static std::wstring get_appdata_path();
	std::vector<std::filesystem::path> get_roblox_logs();
	std::filesystem::path get_latest_log();
	std::uint64_t get_render_view();
	std::uint64_t get_game_id();
}
