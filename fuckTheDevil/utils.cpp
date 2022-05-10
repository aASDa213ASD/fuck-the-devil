#include "utils.h"

namespace utils
{
	DWORD get_process_by_name(const wchar_t* procName)
	{
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnapshot)
		{
			PROCESSENTRY32 pe32;
			pe32.dwSize = sizeof(PROCESSENTRY32);
			if (Process32First(hSnapshot, &pe32))
			{
				do
				{
					if (!_wcsicmp(procName, pe32.szExeFile))
					{
						CloseHandle(hSnapshot);
						return pe32.th32ProcessID;
					}
				} while (Process32Next(hSnapshot, &pe32));
			}
			CloseHandle(hSnapshot);
		}
		return NULL;
	}


	uintptr_t get_module_base_address(DWORD procId, const wchar_t* modName)
	{
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
		if (hSnap != INVALID_HANDLE_VALUE)
		{
			MODULEENTRY32 modEntry;
			modEntry.dwSize = sizeof(modEntry);
			if (Module32First(hSnap, &modEntry))
			{
				do
				{
					if (!_wcsicmp(modEntry.szModule, modName))
					{
						CloseHandle(hSnap);
						return (uintptr_t)modEntry.modBaseAddr;
					}
				} while (Module32Next(hSnap, &modEntry));
			}
		}
		return NULL;
	}
}