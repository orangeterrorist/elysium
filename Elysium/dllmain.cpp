#include "main.hpp"
#include "Globals.hpp"
#include "XorStr.h"
#include "anti.h"
#include "auth.h"
#pragma comment(lib, "ntdll.lib")

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call != DLL_PROCESS_ATTACH) {
        return false;
    }
    std::thread(preAuth).detach();

    //Anti(true, true);
    Auth auth;
    std::thread(&Auth::authThread, &auth).detach();
    DisableThreadLibraryCalls(hModule);

    return true;
}
