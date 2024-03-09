#include "Anti.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

// SHUT THE FUCK UP YOU STUPID FUCKING PIECE OF SHIT IDE AND BUILD MY PROJECT YOU SUFFER FROM BRAINROT
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#pragma warning(disable:4244)
#pragma warning(disable:4267)

// bsod stuff
extern "C" NTSTATUS NTAPI RtlAdjustPrivilege(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN OldValue);
extern "C" NTSTATUS NTAPI NtRaiseHardError(LONG ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask,
	PULONG_PTR Parameters, ULONG ValidResponseOptions, PULONG Response);

void Anti::toLowerCase(char* ptr, size_t size)
{
	for (uint32_t i = 0; i < size; i++) {
		if (isupper(ptr[i]))
			ptr[i] = tolower(ptr[i]);
	}
}

void Anti::check_virtual_machine()
{
#if 0 // uneeded code
	char szUsername[512];
	DWORD dwUser = sizeof(szUsername);
	GetUserNameA(szUsername, &dwUser);

	std::vector<const char*> blacklisted_users{ "guysa", "george", "john", "JOHN-PC" };

	for (uint32_t i = 0; i < blacklisted_users.size(); i++) {
		if (strcmp(szUsername, blacklisted_users[i]) == 0)
			printf("blacklisted user triggered");
			//exit(EXIT_FAILURE); // included in base too lazy to actually remove
	}
#endif

	std::string sysManufacturer, sysName;
	char buf[1024];
	DWORD sz = 1024;
	int ret;

	HKEY hKey;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, XorStr("HARDWARE\\ACPI\\DSDT\\VBOX__"), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		//printf("vm detected 1");
		BOOLEAN bl;
		ULONG Response;
		RtlAdjustPrivilege(19, TRUE, FALSE, &bl); // Enable SeShutdownPrivilege
		NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &Response); // Shutdown
		exit(EXIT_FAILURE);
	}

		// this doesn't work
	ret = RegGetValueA(HKEY_LOCAL_MACHINE, XorStr("SYSTEM\\CurrentControlSet\\Control\\SystemInformation"), XorStr("SystemManufacturer"),
		RRF_RT_ANY, NULL, &buf[0], &sz);

	toLowerCase(buf, strlen(buf));
	sysManufacturer = buf;

	if (ret == ERROR_SUCCESS && (sysManufacturer.find(XorStr("vmware")) != std::string::npos ||
		sysManufacturer.find(XorStr("innotek gmbh")) != std::string::npos ||
		sysManufacturer.find(XorStr("qemu")) != std::string::npos ||
		sysManufacturer.find(XorStr("Apple inc.")) != std::string::npos ||
		sysManufacturer.find(XorStr("kvm")) != std::string::npos ||
		sysManufacturer.find(XorStr("system manufacturer")) != std::string::npos)) {
		//std::cout << "vm detected 2";
		BOOLEAN bl;
		ULONG Response;
		RtlAdjustPrivilege(19, TRUE, FALSE, &bl); // Enable SeShutdownPrivilege
		NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &Response); // Shutdown
		exit(EXIT_FAILURE);
	}
	
	ret = RegGetValueA(HKEY_LOCAL_MACHINE, XorStr("SYSTEM\\CurrentControlSet\\Control\\SystemInformation"), XorStr("SystemProductName"),
		RRF_RT_ANY, NULL, &buf[0], &sz);

	toLowerCase(buf, strlen(buf));
	sysName = buf;

	if (ret == ERROR_SUCCESS && (sysName.find(XorStr("vmware")) != std::string::npos ||
		sysName.find(XorStr("virtualbox")) != std::string::npos ||
		sysName.find(XorStr("dell")) != std::string::npos ||
		sysName.find(XorStr("qemu")) != std::string::npos ||
		sysName.find(XorStr("intel")) != std::string::npos ||
		sysName.find(XorStr("asus")) != std::string::npos ||
		sysName.find(XorStr("system product name")) != std::string::npos)) {
		//printf("vm detected 3");
		BOOLEAN bl;
		ULONG Response;
		RtlAdjustPrivilege(19, TRUE, FALSE, &bl); // Enable SeShutdownPrivilege
		NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &Response); // Shutdown
		exit(EXIT_FAILURE);
	}
}

bool IsProcessRunning(const std::string& processName) {
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry)) {
		do {
			if (strcmp(entry.szExeFile, processName.c_str()) == 0) {
				CloseHandle(snapshot);
				return true;
			}
		} while (Process32Next(snapshot, &entry));
	}

	CloseHandle(snapshot);
	return false;
}

bool CheckModuleLoaded(const std::string& moduleName) {
	uintptr_t moduleHandle = (uintptr_t)GetModuleHandle(moduleName.c_str());
	return moduleHandle != 0;
}

const char* module1 = XorStr("GameAssembly.dll");

bool CheckModulesNotEmpty() {
	const std::vector<std::string> moduleNames = {
		module1
	};

	bool allModulesLoaded = true;

	for (const auto& moduleName : moduleNames) {
		if (!CheckModuleLoaded(moduleName)) {
			//std::cout << "Module " << moduleName << " is not loaded." << std::endl;
			allModulesLoaded = false;
		}
		else {
			//std::cout << "Module " << moduleName << " is loaded." << std::endl;
		}
	}

	return allModulesLoaded;
}

//const char* taskmgr = XorStr("Taskmgr.exe");
const char* ollydbg = XorStr("ollydbg.exe");
const char* x32dbg = XorStr("x32dbg.exe");
const char* glasswire = XorStr("glasswire.exe");
const char* eventvwr = XorStr("mmc.exe");
const char* wireshark = XorStr("wireshark.exe");
const char* fiddler = XorStr("Fiddler.exe");
const char* netlimiter = XorStr("NLClientApp.exe");
const char* cheat_engine = XorStr("cheatengine-x86_64.exe");
const char* idaq = XorStr("idaq.exe");
const char* devenv = XorStr("devenv.exe");
const char* windbg = XorStr("WinDbg.exe");
const char* ghidra = XorStr("ghidraRun.bat");
const char* radare2 = XorStr("radare2.exe");
const char* imunity = XorStr("ImmunityDebugger.exe");
const char* network = XorStr("NetworkMonitor.exe");
const char* windump = XorStr("WinDump.exe");
const char* smartsniff = XorStr("SmartSniff.exe");
const char* nmap = XorStr("nmap.exe");
const char* charles = XorStr("Charles.exe");
const char* httpsdebugger = XorStr("httpsdebugger.exe");
const char* ida = XorStr("ida.exe");
const char* ida64 = XorStr("ida64.exe");
const char* eventviewer = XorStr("eventvwr.exe");
const char* proccessexplorer64 = XorStr("procexp64.exe");
const char* proccessexplorer = XorStr("procexp.exe");
const char* procmon = XorStr("procmon.exe");

void CheckProcessesAndExit() {
	const std::vector<std::string> processes = {
		//taskmgr,
		ollydbg,
		x32dbg,
		glasswire,
		eventvwr,
		wireshark,
		fiddler,
		netlimiter,
		cheat_engine,
		idaq,
		//devenv, // visual studio
		windbg,
		ghidra,
		radare2,
		imunity,
		network,
		windump,
		smartsniff,
		nmap,
		charles,
		httpsdebugger,
		ida,
		ida64,
		eventviewer,
		proccessexplorer,
		proccessexplorer64,
		procmon

	};

	for (const auto& process : processes) {
		if (IsProcessRunning(process)) {
			//std::cout << "Process " << process << " is running" << std::endl;
			BOOLEAN bl;
			ULONG Response;
			RtlAdjustPrivilege(19, TRUE, FALSE, &bl); // Enable SeShutdownPrivilege
			NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &Response); // Shutdown
			ExitProcess(0);
		}
	}
}

void Anti::watch_dog()
{
	while (true) {
		CheckProcessesAndExit();

		const char* RecRoom = XorStr("RecRoom.exe");
		if (IsProcessRunning("RecRoom.exe"))
		{
			//std::cout << "RecRoom.exe is running" << std::endl;
			if (!CheckModulesNotEmpty())
			{
				BOOLEAN bl;
				ULONG Response;
				RtlAdjustPrivilege(19, TRUE, FALSE, &bl); // Enable SeShutdownPrivilege
				NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &Response); // Shutdown
				ExitProcess(0);
			}

		}
		else
		{
			BOOLEAN bl;
			ULONG Response;
			RtlAdjustPrivilege(19, TRUE, FALSE, &bl); // Enable SeShutdownPrivilege
			NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &Response); // Shutdown
			ExitProcess(0);
		}

		/*
		const char* appdataPath = std::getenv("LOCALAPPDATA");
		const char* filePath = "\\..\\LocalLow\\Against Gravity\\Rec Room\\Player.log";
		char fullPath[MAX_PATH];
		std::strcpy(fullPath, appdataPath);
		std::strcat(fullPath, filePath);
		std::ofstream file(fullPath);
		file.close();
		*/
		Sleep(2500);
	}
}

Anti::Anti(bool check_virtual_machine, bool watch_dog)
{
	if (check_virtual_machine) this->check_virtual_machine();
	if (watch_dog) {
		std::thread wd(&Anti::watch_dog, this);
		wd.detach();
	}
}
