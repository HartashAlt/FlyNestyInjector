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

#pragma comment(lib, "ws2_32.lib")


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

std::vector<char> compress_jest(const std::string& bytecode, size_t& byte_size) {
	const auto data_size = bytecode.size();
	const auto max_size = ZSTD_compressBound(data_size);
	auto buffer = std::vector<char>(max_size + 8);

	// "RSB1" prefix and data size
	buffer[0] = 'R'; buffer[1] = 'S'; buffer[2] = 'B'; buffer[3] = '1';
	std::memcpy(&buffer[4], &data_size, sizeof(data_size));

	const auto compressed_size = ZSTD_compress(&buffer[8], max_size, bytecode.data(), data_size, ZSTD_maxCLevel());
	if (ZSTD_isError(compressed_size)) {
		throw std::runtime_error("Failed to compress the bytecode.");
	}

	const auto size = compressed_size + 8;
	const auto key = XXH32(buffer.data(), size, 42u);
	const auto bytes = reinterpret_cast<const uint8_t*>(&key);

	// XOR encryption loop
	for (auto i = 0u; i < size; ++i) {
		buffer[i] ^= bytes[i % 4] + i * 41u;
	}

	byte_size = size;
	buffer.resize(size);  // Shrink buffer to actual size

	return buffer;
}

class bytecode_encoder_t : public Luau::BytecodeEncoder {
	inline void encode(uint32_t* data, size_t count) override {
		for (size_t i = 0; i < count;) {
			auto& opcode = *reinterpret_cast<uint8_t*>(data + i);
			i += Luau::getOpLength(LuauOpcode(opcode));
			opcode *= 227;
		}
	}
};

Luau::CompileOptions compile_options;

std::string compile(const std::string& source) {
	if (compile_options.debugLevel != 2 || compile_options.optimizationLevel != 2) {
		compile_options.debugLevel = 2;
		compile_options.optimizationLevel = 2;
	}

	static bytecode_encoder_t encoder;

	// Compilation with larger data handling
	std::string bytecode = Luau::compile(source, {}, {}, &encoder);
	return bytecode;
}

HWND gHwnd = NULL;
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	DWORD windowProcessId;
	GetWindowThreadProcessId(hwnd, &windowProcessId);
	if (windowProcessId == (DWORD) lParam) {
		gHwnd = hwnd;
		return FALSE;
	}
	return TRUE;
}

HWND getWindowHandleFromProcessId(DWORD processId) {
	EnumWindows(EnumWindowsProc, processId);
	return gHwnd;
}

std::string message;

std::string GetInitLuaScript() {
	HRSRC resourceHandle = FindResourceW(NULL, MAKEINTRESOURCEW(101), RT_RCDATA);
	if (resourceHandle == NULL) return "";

	HGLOBAL loadedResource = LoadResource(NULL, resourceHandle);
	if (loadedResource == NULL) return "";

	DWORD size = SizeofResource(NULL, resourceHandle);
	void* data = LockResource(loadedResource);

	return std::string(static_cast<char*>(data), size);
}

extern "C" __declspec(dllexport) boolean inject() {
	std::string luaContent = GetInitLuaScript();
	DWORD PID = getPIDbyName(L"RobloxPlayerBeta.exe");

	std::cout << luaContent << std::endl;

	if (PID == 0) {
		return false;
	}

	memory->attach(PID);
	auto window = getWindowHandleFromProcessId(PID);

	auto renderView = logs::get_render_view();
	std::chex(renderView);

	auto dmptr = memory->read<uintptr_t>(renderView + 0x118);
	std::chex(dmptr);

	auto dmaddy = memory->read<uintptr_t>(dmptr + 0x1a8);
	std::chex(dmaddy);

	storage::datamodel = static_cast<rbx::instance_t>(dmaddy);

	std::chex(storage::datamodel.address);
	std::cout << storage::datamodel.getname() << std::endl;
	std::cout << storage::datamodel.find_first_child("Workspace").getname() << std::endl;

	rbx::instance_t corePackages = storage::datamodel.find_first_child("CorePackages");
	rbx::instance_t CoreGui = storage::datamodel.find_first_child("CoreGui");
	rbx::instance_t RobloxGui = CoreGui.find_first_child("RobloxGui");
	rbx::instance_t Modules = RobloxGui.find_first_child("Modules");

	rbx::instance_t corepkgs = storage::datamodel.find_first_child("CorePackages");
	std::cout << corepkgs.getname() << std::endl;

	rbx::instance_t ws = corepkgs.find_first_child("Workspace");
	std::cout << ws.getname() << std::endl;

	rbx::instance_t pkgs = ws.find_first_child("Packages");
	std::cout << pkgs.getname() << std::endl;

	rbx::instance_t _ws = pkgs.find_first_child("_Workspace");
	std::cout << _ws.getname() << std::endl;

	rbx::instance_t smsp = _ws.find_first_child("SMSProtocol");
	std::cout << smsp.getname() << std::endl;

	rbx::instance_t dev = smsp.find_first_child("Dev");
	std::cout << dev.getname() << std::endl;

	rbx::instance_t jestglobals = dev.find_first_child("JestGlobals");
	std::cout << jestglobals.getname() << std::endl;
	std::cout << std::hex << jestglobals.address << std::endl;

	rbx::instance_t player_list = Modules.find_first_child("PlayerList");
	rbx::instance_t player_list_manager = player_list.find_first_child("PlayerListManager");

	rbx::instance_t Common = Modules.find_first_child("Common");
	rbx::instance_t Url = Common.find_first_child("Url");

	jestglobals.modulebypassi();

	size_t old_bytecode_size;
	std::vector<char> old_bytecode;

	jestglobals.getBytecode(old_bytecode, old_bytecode_size);

	memory->write<uintptr_t>(player_list_manager.address + 0x8, jestglobals.address);

	size_t target_bytecode_size;
	auto raper = compress_jest(compile(luaContent), target_bytecode_size);

	jestglobals.setBytecode(raper, target_bytecode_size);

	// jestglobals.wait_for_child("Initialized", 5);

	HWND foregroundWindow = GetForegroundWindow();
	SendMessage(window, WM_CLOSE, NULL, NULL);

	jestglobals.setBytecode(old_bytecode, old_bytecode_size);
	memory->write<uintptr_t>(player_list_manager.address + 0x8, player_list_manager.address);

	storage::jestglobals = jestglobals;

	std::cout << "Initialized" << std::endl;

	return true;
}

extern "C" __declspec(dllexport) boolean executeLuaCode(const char* luaContent) {
	std::string luaScript(luaContent);

	auto jestglobals = storage::jestglobals;
	auto holder = jestglobals.find_first_child("Holder").objectValue();
	auto exec = jestglobals.find_first_child("Exec");

	holder.modulebypassi();

	size_t insert_size;
	auto bytes = compress_jest(compile(std::string("return function(...)\n" + luaScript + "\nend")), insert_size);
	holder.setBytecode(bytes, insert_size);
	exec.setBoolValue(true);

	return true;
}