#include <iostream>
#include <Windows.h>
#include "utils.h"
#include "vars.h"

using namespace std;

struct game_addresses
{
	uintptr_t gem_address;
	uintptr_t is_alive;
};

DWORD getGameProcess(DWORD process) {
	if (process != NULL)
		return NULL;

	else {
		process = utils::get_process_by_name(L"dd.exe");
		
		if (process == NULL)
			process = utils::get_process_by_name(L"devildaggers.exe");
	}

	return process;
}

void readGameOffsets(HANDLE proc, game_addresses &addresses, uintptr_t &base_address) {
	addresses.gem_address = base_address + 0x24B4A0;
	addresses.is_alive = base_address + 0x250DC0;

	addresses.gem_address = utils::read_memory_offsets(proc, addresses.gem_address, 0x0ull, 0x354ull);
	addresses.is_alive = utils::read_memory_offsets(proc, addresses.is_alive, 0x354ull);
}

void show_debug(bool &show_debug, game_addresses& addresses, uintptr_t& base_address) {
	show_debug = !show_debug;
	if (show_debug) {
		cout << "PID: " << dec << procId << endl;
		cout << "BASE: " << hex << base_address << endl;
		cout << "GEM: " << hex << addresses.gem_address << endl;
		cout << "IS_ALIVE: " << hex << addresses.is_alive;
	}
	else
		system("cls");
}

int main() {

	game_addresses addresses{};

	procId = getGameProcess(procId);

	HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);

	if (proc == NULL) {
		cout << "Null PID.";
		Sleep(3000);
		exit(-1);
	}
		
	else {
		uintptr_t base_address = utils::get_module_base_address(procId, L"dd.exe");
		readGameOffsets(proc, addresses, base_address);

		SetConsoleTitleA("fuck the devil");
		while (true) {
			if (GetAsyncKeyState(VK_F9)) {
				show_debug(debug, addresses, base_address);
			}

			if (GetAsyncKeyState(VK_F1)) {
				gem_value = 0;
				WriteProcessMemory(proc, (LPVOID)addresses.gem_address, &gem_value, sizeof(gem_value), 0);
			}

			if (GetAsyncKeyState(VK_F2)) {
				gem_value = 20;
				WriteProcessMemory(proc, (LPVOID)addresses.gem_address, &gem_value, sizeof(gem_value), 0);
			}

			if (GetAsyncKeyState(VK_F3)) {
				gem_value = 70;
				WriteProcessMemory(proc, (LPVOID)addresses.gem_address, &gem_value, sizeof(gem_value), 0);
			}

			if (GetAsyncKeyState(VK_F4)) {
				gem_value = 101;
				WriteProcessMemory(proc, (LPVOID)addresses.gem_address, &gem_value, sizeof(gem_value), 0);
			}

			if (GetAsyncKeyState(VK_F5)) {
				alive = !alive;
				WriteProcessMemory(proc, (LPVOID)addresses.is_alive, &alive, sizeof(alive), 0);
			}

			if (GetAsyncKeyState(VK_SHIFT)) {
				if (GetAsyncKeyState(0x5A)) {
					base_address = utils::get_module_base_address(procId, L"devildaggers.exe");
					readGameOffsets(proc, addresses, base_address);

					if (debug) {
						system("cls"); debug = false;
						show_debug(debug, addresses, base_address);
					}
				}
			}

			Sleep(150);
			}
		}

	return 0;
}