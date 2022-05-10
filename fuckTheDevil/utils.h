#include <vector>
#include <windows.h>
#include <TlHelp32.h>

namespace utils
{
	DWORD get_process_by_name(const wchar_t* procName);

	uintptr_t get_module_base_address(DWORD procId, const wchar_t* modName);

	template< class...Args>
	uintptr_t read_memory_offsets(HANDLE process, uintptr_t base_address, const Args&...offsets)
	{
		uintptr_t new_address = base_address;
		std::vector<uintptr_t> offset_vec{ {offsets... } };
		for (const auto& offset : offset_vec)
		{
			uintptr_t address = -1;
			if (ReadProcessMemory(process, (void*)new_address, &address, sizeof(address), NULL))
			{
				new_address = address + offset;
			}
		}
		return new_address;
	}
}
