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
#include <iostream>
#include "datamodel.hpp"

using namespace std;

namespace logs {
	static wstring get_appdata_path()
	{
		wchar_t path[MAX_PATH];
		if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path))) {
			return path;
		}
		return L"";
	}

	vector<filesystem::path> get_roblox_logs()
	{
		vector<filesystem::path> roblox_logs;
		wstring roblox_logs_path = get_appdata_path() + L"\\Roblox\\logs";

		wcout << roblox_logs_path << endl;

		try {
			for (const auto& filename : filesystem::directory_iterator(roblox_logs_path)) {
				if (filename.is_regular_file() && filename.path().extension() == L".log" && filename.path().filename().wstring().find(L"Player") != wstring::npos)
					roblox_logs.push_back(filename.path());
			}
		}
		catch (const filesystem::filesystem_error& error) {
			cout << "Something went wrong (filesystem)" << endl;
		}

		return roblox_logs;
	}

	filesystem::path get_latest_log()
	{
		auto logs = get_roblox_logs();
		
		if (logs.empty()) {
			cout << "Roblox Logs not Found" << endl;
		}

		sort(logs.begin(), logs.end(), [](const auto& a, const auto& b) {
			return filesystem::last_write_time(a) > filesystem::last_write_time(b);
			});

		return logs.front();
	}

	uint64_t get_render_view()
	{
		auto latest_log = get_latest_log();
		ifstream opened_log(latest_log);
		
		if (!opened_log.is_open()) {
			throw runtime_error("Unable to open Roblox Log file.");
		}

		string log_line;
		while (getline(opened_log, log_line)) {
			size_t start_pos = log_line.find("initialize view(");
			if (start_pos != string::npos) {
				start_pos += string("initialize view(").length();
				size_t end_pos = log_line.find(')', start_pos);
				if (end_pos != string::npos) {
					string view_hex = log_line.substr(start_pos, end_pos - start_pos);
					uint64_t render_view = strtoull(view_hex.c_str(), nullptr, 16);
					return render_view;
				}
			}
		}

		throw runtime_error("Unable to find render view in log file.");
	}
	
	uint64_t get_game_id()
	{
		auto latest_log = get_latest_log();
		ifstream opened_log(latest_log);

		if (!opened_log.is_open()) {
			throw runtime_error("Unable to open Roblox Log file.");
		}

		string log_line;
		string game_id_str;
		while(getline(opened_log, log_line)) {
			size_t start_pos = log_line.find("[FLog::Output] ! Joining game");
			if (start_pos != string::npos) {
				start_pos = log_line.find("place", start_pos);
				if (start_pos != string::npos) {
					start_pos += string("place ").length();
					size_t end_pos = log_line.find(' ', start_pos);
					if (end_pos != string::npos) {
						game_id_str = log_line.substr(start_pos, end_pos - start_pos);
					}
				}
			}
		}

		if (!game_id_str.empty()) {
			uint64_t game_id = strtoull(game_id_str.c_str(), nullptr, 10);
			return game_id;
		}
	}
}