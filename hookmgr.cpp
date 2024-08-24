#include "plugin.h"
#include <string>
#include <sstream>
#include <fstream>

#include <intrin.h>
#pragma intrinsic(_ReturnAddress)

bool HookMgr::isSteam = false;
bool HookMgr::isGOG = false;

std::ofstream out;
std::ofstream outSpawn;

void Log(const char* str, ...)
{
	char buffer[0xFFA];
	va_list arg;
	va_start(arg, str);
	vsnprintf(buffer, sizeof(buffer), str, arg);
	va_end(arg);

	static bool once = false;

	if (!once)
	{
		AllocConsole();
		FILE* fp;
		freopen_s(&fp, "CONOUT$", "w", stdout);
		//out.open("C://mm_log.txt");
		once = true;
	}

	/*if (out.is_open())
	{
		out << buffer << std::endl;
	}*/

	std::cout << buffer << std::endl;
}

void HookMgr::Initialize() {
	MH_Initialize();

	if (*(uint64_t*)(0x14192A150) == 0x1c3c00001bf0) // RUNTIME_FUNCTION rva ?Win32CRTStartupHook@@YAHXZ rva algn_140001C3C
		isGOG = true;
	else
		isSteam = true;

	if (isSteam) {

		static void*(*GetStartupInfoW_orig)(LPSTARTUPINFOW, uintptr_t) = nullptr;
		Install(GetStartupInfoW, [](LPSTARTUPINFOW info, uintptr_t valid) {
				auto addr = _ReturnAddress();
				static bool once = false;
				if (!once && ((uintptr_t)addr == 0x14303978D || valid == 0x14303978D)) { // _tmainCRTStartup
					PluginHooks();
					once = true;
					valid = 0x14303978D;
				}
				GetStartupInfoW_orig(info, valid); // call other hooks on this address
				// the "valid" argument passes to the following plugins that this hook call worked in the right place (_tmainCRTStartup) , 
				// since _ReturnAddress will have a different value if the hook is placed on another hook of another plugin
				
			}, 
			GetStartupInfoW_orig);
	}
	else {
		PluginHooks();
	}
}