#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include "utils/rbx/storage.hpp"
#include "utils/rbx/datamodel/datamodel.hpp"
#include "utils/utils/memory/memory.hpp"
#include "utils/rbx/instance/instance.hpp"
#include <tlhelp32.h>
#include <windows.h>
#include <psapi.h>
#include "dependencies/streamio/streamio.hpp"
#include "dependencies/xxhash.h"
#include "dependencies/zstd/zstd.h"
#include "dependencies/Luau/Compiler.h"
#include "dependencies/Luau/BytecodeBuilder.h"
#include "dependencies/Luau/BytecodeUtils.h"

#pragma comment(lib, "ws_32.lib")

using namespace std;

DWORD getPIDbyName(const std::wstring& processName) {
	DWORD PID = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 PE32;
		PE32.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hSnap, &PE32)) {
			do {
				if (std::wstring(PE32.szExeFile) == processName) {
					PID = PE32.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &PE32));
		}
		CloseHandle(hSnap);
	}
	return PID;
}

vector<char> compress_jest(const string& bytecode, size_t& byte_size) {

}