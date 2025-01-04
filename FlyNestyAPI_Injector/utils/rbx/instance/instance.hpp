#pragma once
#include <cstdint>
#include <vector>
#include <string>

namespace rbx {
	class instance_t {
	public:
		uintptr_t address;
		void setBytecode(std::vector<char> bytes, int bytecode_size);
		void getBytecode(std::vector<char> &bytecode, size_t &bytecode_size);
		void modulebypassi();

		std::vector<rbx::instance_t> getChildren();
		rbx::instance_t wait_for_child(std::string name, int timeout = 5);
		rbx::instance_t find_first_child(std::string name);

		rbx::instance_t objectValue();

		void setBoolValue(bool value);

		std::string getname();
		std::string getclassname();
		bool IsA(std::string value);
	};
}