#pragma once

#include "minhook/include/MinHook.h"

// MadMax.exe 1.0.3.0
#define STEAM_BASE_OFFS (0x0) // 0x1447400 ???
#define STEAM_BASE (0x140000000)
#define STEAM_ADDR_TO_BASE(addr) ((uintptr_t)(addr) - STEAM_BASE_OFFS)
#define ADDRESS(gog, steam) (HookMgr::isSteam ? (STEAM_BASE_OFFS + steam) : (gog))

#define CMETHOD(gog, steam, type, name, ...) type name { return HookMgr::Call<type>(ADDRESS(gog, steam), this, __VA_ARGS__); }
#define SMETHOD(gog, steam, type, name, ...) type name { return HookMgr::Call<type>(ADDRESS(gog, steam), __VA_ARGS__); }
#define CMETHODV(gog, steam, type, name) type name { return HookMgr::Call<type>(ADDRESS(gog, steam), this); }
#define SMETHODV(gog, steam, type, name) type name { return HookMgr::Call<type>(ADDRESS(gog, steam)); }
class HookMgr
{
public:
	
	static bool isSteam;
	static bool isGOG;

	static void Initialize();

	template<typename R, typename...Args>
	static R Call(uintptr_t addr, Args... args)
	{
		return ((R(*)(Args...))(addr))(args...);
	}

	template<typename A, typename T, typename TO>
	static void Install(A addr, T func, TO& orig)
	{
		MH_CreateHook((LPVOID)addr, (LPVOID)func, (LPVOID*)&orig);
		MH_EnableHook((LPVOID)addr);
	}

	template<typename T>
	static void Install(uintptr_t addr, T func)
	{
		MH_CreateHook((LPVOID)addr, (LPVOID)func, (LPVOID*)nullptr);
		MH_EnableHook((LPVOID)addr);
	}

	template<typename T, typename TO>
	static void SetAbsFunc(uintptr_t addr, T func, TO& orig) {
		DWORD d;
		VirtualProtect((LPVOID)addr, 8, PAGE_EXECUTE_READWRITE, &d);
		orig = *(TO*)(addr);
		*(T*)(addr) = func;
		VirtualProtect((LPVOID)addr, 8, d, &d);
	}

	template<typename T>
	static void SetAbsFunc(uintptr_t addr, T func) {
		DWORD d;
		VirtualProtect((LPVOID)addr, 8, PAGE_EXECUTE_READWRITE, &d);
		*(T*)(addr) = func;
		VirtualProtect((LPVOID)addr, 8, d, &d);
	}

	template<typename T>
	static void InstallJMP(uintptr_t addr, T func) {
		DWORD d;
		VirtualProtect((LPVOID)addr, 8, PAGE_EXECUTE_READWRITE, &d);

	}

	template<typename T, typename D>
	static void Patch(T addr, D data, size_t size)
	{
		DWORD d;
		VirtualProtect((LPVOID)addr, size, PAGE_EXECUTE_READWRITE, &d);
		memcpy((void*)addr, (const void*)data, size);
		VirtualProtect((LPVOID)addr, size, d, &d);
	}

	template<typename T>
	static void UnProtect(T addr, size_t size)
	{
		DWORD d;
		VirtualProtect((LPVOID)addr, size, PAGE_EXECUTE_READWRITE, &d);
	}

	template<typename T>
	static void MakeNOP(T addr, size_t size)
	{
		uint8_t d = 0x90;
		for (int i = 0; i < size; i++)
			Patch(((uintptr_t)addr) + i, &d, 1);
	}

	static void MakeRET(uintptr_t addr) {
		Patch(addr, "\xC3", 1);
	}

	static void* RETNULLPTR()
	{
		return nullptr;
	}

	static int RET1()
	{
		return 1;
	}

	static int RET2()
	{
		return 2;
	}

	static int RET0()
	{
		return 0;
	}

	static bool RETTRUE()
	{
		return true;
	}

	static bool RETFALSE()
	{
		return false;
	}

	static void EMPTY()
	{
		return;
	}

private:

};

void Log(const char* str, ...);