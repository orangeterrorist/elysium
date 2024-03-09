//#include "il2cpp.hpp"
#include "main.hpp"
#include <Windows.h>
#include "pch.h"
#include <Minhook.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <random>
#include <vector>

#include "offsets.hpp"
#include "hooking.hpp"
//#include "Zydis.h"
#include "Globals.hpp"
#include "Discord/discord_rpc.h"

// SHUT THE FUCK UP YOU STUPID FUCKING PIECE OF SHIT IDE AND BUILD MY PROJECT YOU SUFFER FROM BRAINROT
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#pragma warning(disable:4244)
#pragma warning(disable:4267)

uintptr_t fixSup = GameAssembly + 0x122F470; // <FChk>b__1() { }
uintptr_t fixSpu = GameAssembly + 0x1226BE0;
uintptr_t IsOnCooldownAddress = GameAssembly + 0xE61CD0;
uintptr_t flyaddy = GameAssembly + 0x1322180;
uintptr_t testaddy = GameAssembly + 0x19072F0;

#if 0
bool CHook(uintptr_t hookAddress, void* hookFunction) {
	constexpr int hookLength = 14; // The length of the hook in bytes

	// Change memory protection
	DWORD oldProtection;
	if (!VirtualProtect((void*)hookAddress, hookLength, PAGE_EXECUTE_READWRITE, &oldProtection)) {
		std::cout << "[Debug] Failed to change memory protection" << std::endl;
		return false;
	}
	else {
		std::cout << "[Debug] Disabled memory protection." << std::endl;
	}

	// Save the original prologue
	uint8_t originalPrologue[hookLength];
	memcpy(originalPrologue, (void*)hookAddress, hookLength);

	// Insert JMP instruction to hook the function
	*(uint8_t*)(hookAddress) = 0x49;  // MOV R11, hookedIsOnCooldown
	*(uint8_t*)(hookAddress + 1) = 0xBB;
	*(uintptr_t*)(hookAddress + 2) = (uintptr_t)hookFunction;

	*(uint8_t*)(hookAddress + 10) = 0x41;  // JMP R11
	*(uint8_t*)(hookAddress + 11) = 0xFF;
	*(uint8_t*)(hookAddress + 12) = 0xE3;
	std::cout << "[Debug] Hooked: " << hookAddress << std::endl;

	// Restore memory protection
	DWORD temp;
	if (!VirtualProtect((void*)hookAddress, hookLength, oldProtection, &temp)) {
		std::cout << "[Debug] Failed to restore memory protection." << std::endl;
		return false;
	}
	else {
		std::cout << "[Debug] Memory protection Restored." << std::endl;
	}

	return true;
}
#endif



std::unordered_map<uintptr_t, uint8_t*> hookMap;
constexpr int hookLength = 14;


bool CHook(uintptr_t hookAddress, void* hookFunction) {
	// Check if hook already exists
	auto it = hookMap.find(hookAddress);
	if (it != hookMap.end()) {
		std::cout << "[Debug] Hook already exists at address: " << hookAddress << std::endl;
		return false;
	}

	// Change memory protection
	DWORD oldProtection;
	if (!VirtualProtect((void*)hookAddress, hookLength, PAGE_EXECUTE_READWRITE, &oldProtection)) {
		std::cout << "[Debug] Failed to change memory protection" << std::endl;
		return false;
	}
	else {
		std::cout << "[Debug] Disabled memory protection." << std::endl;
	}

	// Save the original prologue
	uint8_t* originalPrologue = new uint8_t[hookLength];
	memcpy(originalPrologue, (void*)hookAddress, hookLength);

	hookMap[hookAddress] = originalPrologue;

	// Insert JMP instruction to hook the function
	*(uint8_t*)(hookAddress) = 0x49;  // MOV R11, hookedIsOnCooldown
	*(uint8_t*)(hookAddress + 1) = 0xBB;
	*(uintptr_t*)(hookAddress + 2) = (uintptr_t)hookFunction;

	*(uint8_t*)(hookAddress + 10) = 0x41;  // JMP R11
	*(uint8_t*)(hookAddress + 11) = 0xFF;
	*(uint8_t*)(hookAddress + 12) = 0xE3;

	std::cout << "[Debug] Hooked: " << hookAddress << std::endl;

	// Restore memory protection
	DWORD temp;
	if (!VirtualProtect((void*)hookAddress, hookLength, oldProtection, &temp)) {
		std::cout << "[Debug] Failed to restore memory protection." << std::endl;
		return false;
	}
	else {
		std::cout << "[Debug] Memory protection Restored." << std::endl;
	}

	return true;
}

#if 0
bool CHook(uintptr_t hookAddress, void* hookFunction) {
	constexpr size_t hookLength = 14;  // Length of the JMP instruction sequence

	// Check if hook already exists
	auto it = hookMap.find(hookAddress);
	if (it != hookMap.end()) {
		std::cout << "[Debug] Hook already exists at address: " << hookAddress << std::endl;
		return false;
	}

	// Change memory protection
	DWORD oldProtection;
	if (!VirtualProtect((void*)hookAddress, hookLength, PAGE_EXECUTE_READWRITE, &oldProtection)) {
		std::cout << "[Debug] Failed to change memory protection" << std::endl;
		return false;
	}
	else {
		std::cout << "[Debug] Disabled memory protection." << std::endl;
	}

	// Save the original prologue
	uint8_t* originalPrologue = new uint8_t[hookLength];
	memcpy(originalPrologue, (void*)hookAddress, hookLength);

	hookMap[hookAddress] = originalPrologue;

	// Insert JMP instruction to hook the function
	// Allocate a buffer for the trampoline
	uint8_t* trampoline = (uint8_t*)VirtualAlloc(NULL, hookLength + 14, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	// Copy the original prologue to the trampoline
	memcpy(trampoline, originalPrologue, hookLength);

	// Add a jump from the trampoline back to the original function (after the overwritten part)
	trampoline[hookLength] = 0x49;  // MOV R11, hookAddress+hookLength
	trampoline[hookLength + 1] = 0xBB;
	*(uintptr_t*)(trampoline + hookLength + 2) = hookAddress + hookLength;
	trampoline[hookLength + 10] = 0x41;  // JMP R11
	trampoline[hookLength + 11] = 0xFF;
	trampoline[hookLength + 12] = 0xE3;

	// Now overwrite the original function's prologue with a jump to our hook
	*(uint8_t*)(hookAddress) = 0x49;  // MOV R11, hookFunction
	*(uint8_t*)(hookAddress + 1) = 0xBB;
	*(uintptr_t*)(hookAddress + 2) = (uintptr_t)hookFunction;
	*(uint8_t*)(hookAddress + 10) = 0x41;  // JMP R11
	*(uint8_t*)(hookAddress + 11) = 0xFF;
	*(uint8_t*)(hookAddress + 12) = 0xE3;

	std::cout << "[Debug] Hooked: " << hookAddress << std::endl;

	// Restore memory protection
	DWORD temp;
	if (!VirtualProtect((void*)hookAddress, hookLength, oldProtection, &temp)) {
		std::cout << "[Debug] Failed to restore memory protection." << std::endl;
		return false;
	}
	else {
		std::cout << "[Debug] Memory protection Restored." << std::endl;
	}

	return true;
}
#endif


bool CUnHook(uintptr_t hookAddress) {
	// Lookup the original prologue in the map
	auto it = hookMap.find(hookAddress);
	if (it == hookMap.end()) {
		std::cout << "[Debug] Failed to find hook in map." << std::endl;
		return false;
	}

	uint8_t* originalPrologue = it->second;

	// Change memory protection
	DWORD oldProtection;
	if (!VirtualProtect((void*)hookAddress, hookLength, PAGE_EXECUTE_READWRITE, &oldProtection)) {
		std::cout << "[Debug] Failed to change memory protection" << std::endl;
		return false;
	}
	else {
		std::cout << "[Debug] Disabled memory protection." << std::endl;
	}

	// Copy back the original prologue
	memcpy((void*)hookAddress, originalPrologue, hookLength);
	std::cout << "[Debug] Unhooked: " << hookAddress << std::endl;

	// Restore memory protection
	DWORD temp;
	if (!VirtualProtect((void*)hookAddress, hookLength, oldProtection, &temp)) {
		std::cout << "[Debug] Failed to restore memory protection." << std::endl;
		return false;
	}
	else {
		std::cout << "[Debug] Memory protection Restored." << std::endl;
	}

	delete[] originalPrologue;
	hookMap.erase(hookAddress);

	return true;
}















uintptr_t(__fastcall* original_f_chk)(uintptr_t _this);
uintptr_t f_chk(uintptr_t _this)
{
	std::cout << "[FUCK] before:" << _this << std::endl;
	*(uintptr_t*)(_this + 40) = 0;
	std::cout << "[FUCK] after:" << _this << std::endl;
	return original_f_chk(_this);
}

float __fastcall fixFuck(uintptr_t thisptr) {
	*(uintptr_t*)(thisptr + 40) = 0;
	return (thisptr);
}

float __fastcall sizeThingy(void* thisptr) {
	return 2.0;
}

void __fastcall uNop(void* thisptr) {
	return;
}

bool __fastcall uuTrue(DWORD*, DWORD*) {
	std::cout << "called uufly" << std::endl;
	return true;
}

void(__fastcall* nNop_o)(DWORD*);
void __stdcall nNop_hook(DWORD* uh) {
	nNop_o(uh);  // Call the trampoline
	// Modify the behavior of the original function, if needed
	// No need for a return statement in a void function
}

bool(__fastcall* nTrue_o)(DWORD*, DWORD*);
bool __stdcall nTrue_hook(DWORD* uh, DWORD* uhh) {
	std::cout << "[Debug] trampoline called" << std::endl;
	bool originalResult1 = nTrue_o(uh, uhh);
	std::cout << "[Debug] after trampoline called" << std::endl;
	return false;
}

bool(__fastcall* ftrue_o)(DWORD*, DWORD*);
bool __stdcall ftrue_hook(DWORD* uh, DWORD* uhh) {
	std::cout << "called fly" << std::endl;
	return true;
}



bool(__fastcall* nFalse_o)(DWORD*);
bool __stdcall nFalse_hook(DWORD* uh) {
	bool originalResult = nFalse_o(uh);  // Call the trampoline
	return false;  // Modify the behavior of the original function
}
/*
bool(__fastcall* ufalse_o)(DWORD*);
bool __fastcall uFalse(void* thisptr) {
	return false;
}
*/

extern "C" {
	bool(__fastcall* ktrue_o)(DWORD*); // __fastcall specifies the calling convention
	void ktrue_hook(); // Hook function implemented in assembly
}


void UpdatePresence();

void preAuth()
{
	MessageBox(NULL, "injected lol111", "YURRRRRRRR", MB_OK);

	while (true) {

	}


	MH_Initialize();

	MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + 0x15AA540), &unop_hook, (LPVOID*)&unop_o);
	MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + 0x15AA540));

	//MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + 0x1893560), &unop_hook, (LPVOID*)&unop_o);
	MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + 0x1893560), &f_chk, (LPVOID*)&original_f_chk);
	MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + 0x1893560));


	//MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + 0x1598A90), &unop_hook, (LPVOID*)&unop_o);
	MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + 0x1598A90), &f_chk, (LPVOID*)&original_f_chk);
	MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + 0x1598A90));

	MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + 0x187C710), &unop_hook, (LPVOID*)&unop_o);
	MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + 0x187C710));

	MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + 0x187C780), &unop_hook, (LPVOID*)&unop_o);
	MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + 0x187C780));


	MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + 0x1226BE0), &unop_hook, (LPVOID*)&unop_o);
	MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + 0x1226BE0));

	MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + 0x121FC40), &unop_hook, (LPVOID*)&unop_o);
	MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + 0x121FC40));

	MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + 0x1886B00), &unop_hook, (LPVOID*)&unop_o);
	MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + 0x1886B00));


	MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + 0x188E3F0), &utrue_hook, (LPVOID*)&utrue_o);
	MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + 0x188E3F0));

	//MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + 0x1226BE0), &f_chk, (LPVOID*)&original_f_chk);
	//MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + 0x1226BE0));

	//MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + 0x2678520), &ftrue_hook, (LPVOID*)&ftrue_o);
	//MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + 0x2678520));

	//CHook(fixSup, fixFuck); // fixSpu
	//MessageBox(NULL, "preauth after", "preauth done", MB_OK);
}

void init()
{
	DiscordEventHandlers Handle;
	memset(&Handle, 0, sizeof(Handle));
	Discord_Initialize("1079687619773988865", &Handle, 1, NULL);
	std::thread(UpdatePresence).detach();

	//CHook((GameAssembly + 0x1322180), &uuTrue);

	//MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + 0x2678520));

	//CHook(IsOnCooldownAddress, &uNop);
	//CHook((GameAssembly + 0xE5E3F0), &uNop);
	//CHook((GameAssembly + 0xE5E4B0), &uNop);

	//CHook((GameAssembly + 0xE611B0), &uNop);

	//CHook((GameAssembly + 0x438BC40), &uNop);

	//CHook((GameAssembly + 0x1322220), &uNop);

	//CHook((GameAssembly + 0x1322220), &uNop);


	//CHook((GameAssembly + 0x1322180), &uuTrue);

	//MH_Initialize();
	//MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + fixSup), &f_chk, (LPVOID*)&original_f_chk);
	//MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + fixSup));

	//MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + 0x1322180), &utrue_hook, (LPVOID*)&utrue_o);
	//MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + 0x1322180));

	//CHook(flyaddy, &uuTrue);
	//CHook(get_DesiredAvatarSkeletonTrackingSpaceScale, &sizeThingy);
	//CHook(testaddy, &uFalse);

	//CHook((GameAssembly + TryApplicationQuit), &uNop);
	//CHook((GameAssembly + TryApplicationQuit1), &uNop);
	//CHook((GameAssembly + FatalApplicationQuit), &uNop);
	//CHook(flyaddy, &hook32);
	//UnHook(flyaddy);
}

void OnDraw();
void InitImGui()
{
	ImGui::CreateContext();

	auto io = ImGui::GetIO();
	auto& style = ImGui::GetStyle();

	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;

	style.WindowMinSize = ImVec2(0, 0);
	style.WindowTitleAlign = ImVec2(0.5, 0.5);
	style.FrameBorderSize = 2;
	style.ChildBorderSize = 0;
	style.WindowBorderSize = 2;
	style.WindowRounding = 10;
	style.FrameRounding = 35;
	style.ChildRounding = 35;
	style.Colors[ImGuiCol_TitleBg] = ImColor(70, 70, 70);
	style.Colors[ImGuiCol_TitleBgActive] = ImColor(70, 70, 70);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(70, 70, 70);
	style.Colors[ImGuiCol_WindowBg] = ImColor(25, 25, 25, 240);
	style.Colors[ImGuiCol_CheckMark] = ImColor(70, 70, 70);
	style.Colors[ImGuiCol_Border] = ImColor(70, 70, 70);
	style.Colors[ImGuiCol_Button] = ImColor(32, 32, 32);
	style.Colors[ImGuiCol_ButtonActive] = ImColor(42, 42, 42);
	style.Colors[ImGuiCol_ButtonHovered] = ImColor(42, 42, 42);
	style.Colors[ImGuiCol_ChildBg] = ImColor(45, 45, 45);
	style.Colors[ImGuiCol_FrameBg] = ImColor(32, 32, 32);
	style.Colors[ImGuiCol_FrameBgActive] = ImColor(42, 42, 42);
	style.Colors[ImGuiCol_FrameBgHovered] = ImColor(42, 42, 42);
	style.Colors[ImGuiCol_SliderGrab] = ImColor(255, 255, 255);
	style.Colors[ImGuiCol_SliderGrabActive] = ImColor(255, 255, 255);

	// Get the home directory
	const char* homeDir = getenv("USERPROFILE");

	// Construct the path to the downloads directory
	std::string pathStr = std::string(homeDir) + "\\Documents\\SF-Pro-Display-Regular.otf";
	const char* path = pathStr.c_str();

	// Attempt to load the font
	ImFont* font = io.Fonts->AddFontFromFileTTF(path, 16.0f);

	// Check if the font was loaded successfully
	if (font == nullptr) {
		std::cout << "[Elysium] Failed to load font to: " << path << std::endl;
	}
	else {
		std::cout << "[Elysium] Font loaded successfully." << std::endl;
	}

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
	std::cin;
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (uMsg == WM_KEYUP && wParam == VK_HOME)
		Globals::Open ^= 1;

	if (Globals::Open)
	{
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		return true;
	}

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	static bool Init = false;
	if (!Init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			std::cin;
			InitImGui();
			Init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	ImGui::GetIO().MouseDrawCursor = Globals::Open;
	OnDraw();
	ImGui::Render();
	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

//bool bounds_ui1{ false };
//bool rapidfire_ui1{ false };

bool bounds_ui1{ false };
bool ammo_ui1{ false };
bool rapidfire_ui1{ false };
bool ai_ui1{ false };
bool hijack_ui1{ false };
bool respawn_ui1{ false };
bool infhealth_ui1{ false };
bool instakill_ui1{ false };
bool cct_ui1{ false };
bool teleporttoggle_ui1{ false };
bool movement_ui1{ false };
bool grab_ui1{ false };
bool trialitems_ui1{ false };
bool scalespoof_ui1{ false };
bool antichatcooldown_ui1{ false };
bool makerpen_ui1{ false };
bool fly_ui1{ false };
bool flyd_ui1{ false };
bool hashchecks_ui1{ false };
bool antikick_ui1{ false };
bool inkspoof_ui1{ false };
bool developer_ui1{ false };
bool snowballtoggle_ui1{ false };
bool foodsploittoggle_ui1{ false };
bool scalegui1{ false };
bool coach_ui1{ false };
bool doors_ui1{ false };

void OnDraw()
{
	ImGui::SetWindowSize(ImVec2(700, 300));

	static auto Checkbox = [&](const char* label, bool* value)
	{
		ImGui::PushID(label);
		ImGui::Checkbox(label, value);
		ImGui::PopID();
		ImGui::Spacing();
	};

	ImGui::End();

	if (Globals::Open) {
	    std::cout << "test";
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(200, 200));
		ImGui::Begin("", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration);
		ImGui::Text("Elysium BETA 0.7.3");
		ImGui::Text("by neptune#1995");

		// width height
		ImGui::End();
		ImGui::SetNextWindowSize(ImVec2(260, 280));
		ImGui::Begin("Combat", 0, ImGuiWindowFlags_NoResize);
		Checkbox(_("No Fire Rate"), &Globals::Combat::rapidfire_ui);
		Checkbox(_("Infinite Ammo"), &Globals::Combat::ammo_ui);
		Checkbox(_("AI Insta-Kill & No Damage"), &Globals::Combat::ai_ui);
		Checkbox(_("Hijack"), &Globals::Combat::hijack_ui);
		Checkbox(_("In Bounds"), &Globals::Combat::bounds_ui);
		Checkbox(_("Respawn"), &Globals::Combat::respawn_ui);
		Checkbox(_("Infinite Health (requires photon master)"), &Globals::Combat::infhealth_ui);
		Checkbox(_("Insta Kill (requires photon master)"), &Globals::Combat::instakill_ui);

		ImGui::SetNextWindowSize(ImVec2(220, 455));
		ImGui::Begin("Player", 0, ImGuiWindowFlags_NoResize);
		Checkbox(_("Player Scale"), &Globals::Player::scalegui);
		ImGui::InputFloat("float", &Globals::Player::PlayerSize, 1.0, 100.0);

		Checkbox(_("Fly [Dynamic]"), &Globals::Player::flyd_ui);
		Checkbox(_("Fly [Static]"), &Globals::Player::fly_ui);
		//Checkbox(_("Trail Items"), &Globals::Player::trialitems_ui);
		Checkbox(_("Clothing Customizer Tool"), &Globals::Player::cct_ui);
		Checkbox(_("Movement Mode Bypass"), &Globals::Player::movement_ui);
		//Slider(_("Player Scale"), &Globals::Player::bodysizegui, 0, 1000);
		Checkbox(_("Teleport Range"), &Globals::Player::teleporttoggle_ui);
		//Slider(_("Teleport Range"), &Globals::Player::teleport_ui, 0, 99999999);
		Checkbox(_("No Chat Cooldown"), &Globals::Player::antichatcooldown_ui);
		Checkbox(_("Grab & Unfreeze"), &Globals::Player::grab_ui);
		Checkbox(_("Unlock Doors"), &Globals::Player::doors_ui);
		Checkbox(_("Maker Pen"), &Globals::Player::makerpen_ui);

		//Checkbox(_("Client Side Broacasting"), &Globals::Player::broadcast_ui);
		//Checkbox(_("Client Side Event Perms"), &Globals::Player::eventperms_ui);
		Checkbox(_("No Self Scale Limit"), &Globals::Player::scalespoof_ui);
		Checkbox(_("Client Side Coach"), &Globals::Player::coach_ui);

		ImGui::SetNextWindowSize(ImVec2(160, 155));
		ImGui::Begin("Misc", 0, ImGuiWindowFlags_NoResize);
		Checkbox(_("Developer Mode"), &Globals::Misc::developer_ui);
		Checkbox(_("Anti-Kick"), &Globals::Misc::antikick_ui);
		Checkbox(_("Ink Spoof"), &Globals::Misc::inkspoof_ui);
		//Checkbox(_("Anti-Cheat Disabler"), &Globals::Misc::hashchecks_ui);

		ImGui::SetNextWindowSize(ImVec2(160, 200));
		ImGui::Begin("Beta", 0, ImGuiWindowFlags_NoResize);
		Checkbox(_("Snowball Exploit"), &Globals::Exploit::snowballtoggle_ui);
		ImGui::InputInt("Int ", &Globals::Exploit::snowballcount, 1, 100);

		Checkbox(_("Foodsploit"), &Globals::Exploit::foodsploittoggle_ui);
		ImGui::InputInt("Int", &Globals::Exploit::foodsploitcount, 1, 100);

		if (ImGui::Button(_("Unload")))
			Globals::IsClosing = true;
			FreeConsole();
			DLL_PROCESS_DETACH;

		//if (ImGui::Button(_("Hook")))

		//if (ImGui::Button(_("UnHook")))
			//CUnHook(flyaddy);

		ImGui::End();


		if (Globals::Combat::bounds_ui)
		{
			if (bounds_ui1 != true)
			{
				//if (!CHook(flyaddy, &hook32))
				//{
				//	MessageBox(NULL, "failed Hooked", "Message", MB_OK);
				//}
				//else {
				//	MessageBox(NULL, "Hooked", "Message", MB_OK);
				//}
				MessageBox(NULL, "bounds", "bounds1", MB_OK);
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_LocalPlayerInBounds));
				bounds_ui1 = true;
			}
		}
		else
		{
			if (bounds_ui1 != false)
			{
				//UnHook(flyaddy);
				//MessageBox(NULL, "UnHooked", "Message", MB_OK);
				MessageBox(NULL, "boundsff", "boundsff1", MB_OK);
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_LocalPlayerInBounds));
				bounds_ui1 = false;
			}
		}

		if (Globals::Combat::ammo_ui)
		{
			if (ammo_ui1 != true)
			{
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_MagazineAmmunition));
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_HasEnoughMagazineAmmunition));
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_TotalAmmunition));
				ammo_ui1 = true;
			}
		}
		else
		{
			if (ammo_ui1 != false)
			{
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_MagazineAmmunition));
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_HasEnoughMagazineAmmunition));
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_TotalAmmunition));
				ammo_ui1 = false;
			}
		}

		if (Globals::Combat::rapidfire_ui)
		{
			if (rapidfire_ui1 != true)
			{
				MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + RangedWeapon_get_IsOnCooldown));
				MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + SandboxGunHandle_get_IsOnCooldown));
				rapidfire_ui1 = true;
			}
		}
		else
		{
			if (rapidfire_ui1 != false)
			{
				MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + RangedWeapon_get_IsOnCooldown));
				MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + SandboxGunHandle_get_IsOnCooldown));
				rapidfire_ui1 = false;
			}
		}

		if (Globals::Combat::ai_ui)
		{
			if (ai_ui1 != true)
			{
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + Weapon_GetEnemyDamage));
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + WeaponMelee_GetEnemyDamageMelee));
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + GetPlayerAttackDamage));
				ai_ui1 = true;
			}
		}
		else
		{
			if (ai_ui1 != false)
			{
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + Weapon_GetEnemyDamage));
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + WeaponMelee_GetEnemyDamageMelee));
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + GetPlayerAttackDamage));
				ai_ui1 = false;
			}
		}

		if (Globals::Combat::hijack_ui)
		{
			if (hijack_ui1 != true)
			{
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanAnyPlayerSit));
				hijack_ui1 = true;
			}
		}
		else
		{
			if (hijack_ui1 != false)
			{
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanAnyPlayerSit));
				hijack_ui1 = false;
			}
		}

		if (Globals::Combat::respawn_ui)
		{
			if (respawn_ui1 != true)
			{
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + Player_get_RespawnDuration));
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + GameCombatManager_get_RespawnDuration));
				respawn_ui1 = true;
			}
		}
		else
		{
			if (respawn_ui1 != false)
			{
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + Player_get_RespawnDuration));
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + GameCombatManager_get_RespawnDuration));
				respawn_ui1 = false;
			}
		}

		if (Globals::Combat::infhealth_ui)
		{
			if (infhealth_ui1 != true)
			{
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + PlayerIsInvincible));
				infhealth_ui1 = true;
			}
		}
		else
		{
			if (infhealth_ui1 != false)
			{
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + PlayerIsInvincible));
				infhealth_ui1 = false;
			}
		}

		if (Globals::Combat::instakill_ui)
		{
			if (instakill_ui1 != true)
			{
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + EnemyDamageSettings));
				instakill_ui1 = true;
			}
		}
		else
		{
			if (instakill_ui1 != false)
			{
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + EnemyDamageSettings));
				instakill_ui1 = false;
			}
		}

		if (Globals::Player::cct_ui)
		{
			if (cct_ui1 != true)
			{
				MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CCT_get_LocalPlayerCanUse));
				cct_ui1 = true;
			}
		}
		else
		{
			if (cct_ui1 != false)
			{
				MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CCT_get_LocalPlayerCanUse));
				cct_ui1 = false;
			}
		}

		if (Globals::Player::teleporttoggle_ui)
		{
			//teleport = Globals::Player::teleport_ui;
			if (teleporttoggle_ui1 != true)
			{
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_MaxTeleportHorizontalDistance));
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_MaxTeleportDropDistance));
				teleporttoggle_ui1 = true;
			}
		}
		else
		{
			if (teleporttoggle_ui1 != false)
			{
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_MaxTeleportHorizontalDistance));
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_MaxTeleportDropDistance));
				teleporttoggle_ui1 = false;
			}
		}

		if (Globals::Player::movement_ui)
		{
			if (movement_ui1 != true)
			{
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanChangeMovementMode));
				movement_ui1 = true;
			}
		}
		else
		{
			if (movement_ui1 != false)
			{
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanChangeMovementMode));
				movement_ui1 = false;
			}
		}

		if (Globals::Player::grab_ui)
		{
			if (grab_ui1 != true)
			{
				MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + DoesLocalPlayerOwnKey1));
				MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + DoesLocalPlayerOwnKey2));
				//MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + DoesLocalPlayerOwnKey3));
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + CreationObject_get_IsGrabbable));
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + OMCreationObject_get_IsGrabbable));
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + Polaroid_get_IsGrabbable));
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + Chip_get_IsFrozen));
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + CreationObject_get_IsFrozen));
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + Tool_get_IsFrozen));
				grab_ui1 = true;
			}
		}
		else
		{
			if (grab_ui1 != false)
			{
				MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + DoesLocalPlayerOwnKey1));
				MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + DoesLocalPlayerOwnKey2));
				//MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + DoesLocalPlayerOwnKey3));
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + CreationObject_get_IsGrabbable));
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + OMCreationObject_get_IsGrabbable));
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + Polaroid_get_IsGrabbable));
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + Chip_get_IsFrozen));
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + CreationObject_get_IsFrozen));
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + Tool_get_IsFrozen));
				grab_ui1 = false;
			}
		}

		if (Globals::Player::trialitems_ui)
		{
			if (trialitems_ui1 != true)
			{
				//MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + LocalRemoveTrialItems));
				trialitems_ui1 = true;
			}
		}
		else
		{
			if (trialitems_ui1 != false)
			{
				//MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + LocalRemoveTrialItems));
				trialitems_ui1 = false;
			}
		}

		if (Globals::Player::scalespoof_ui)
		{
			if (scalespoof_ui1 != true)
			{
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_SelfScaleMaxSize));
				scalespoof_ui1 = true;
			}
		}
		else
		{
			if (scalespoof_ui1 != false)
			{
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_SelfScaleMaxSize));
				scalespoof_ui1 = false;
			}
		}

		if (Globals::Player::antichatcooldown_ui)
		{
			if (antichatcooldown_ui1 != true)
			{
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_IsSendChatOnCooldown));
				antichatcooldown_ui1 = true;
			}
		}
		else
		{
			if (antichatcooldown_ui1 != false)
			{
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_IsSendChatOnCooldown));
				antichatcooldown_ui1 = false;
			}
		}

		if (Globals::Player::makerpen_ui)
		{
			if (makerpen_ui1 != true)
			{
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + CanPlayerCreateWithMakerPen));
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanUseMakerPen));
				MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_WasMakerPenEverOut));
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + LogWasMakerPenEverOut));
				//MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + CanUseMakerPenInternal1));
				//MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + CanUsemakerPenInternal2));
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + CreatorRoleCanUseMakerPen));
				makerpen_ui1 = true;
			}
		}
		else
		{
			if (makerpen_ui1 != false)
			{
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + CanPlayerCreateWithMakerPen));
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanUseMakerPen));
				MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_WasMakerPenEverOut));
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + LogWasMakerPenEverOut));
				//MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + CanUseMakerPenInternal1));
				//MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + CanUsemakerPenInternal2));
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + CreatorRoleCanUseMakerPen));
				makerpen_ui1 = false;
			}
		}

		if (Globals::Player::fly_ui)
		{
			if (fly_ui1 != true)
			{
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + AddFlyEnabled));
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + RemoveFlyEnabled));
				fly_ui1 = true;
			}
		}
		else
		{
			if (fly_ui1 != false)
			{
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + AddFlyEnabled));
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + RemoveFlyEnabled));
				fly_ui1 = false;
			}
		}

		if (Globals::Player::flyd_ui)
		{
			if (flyd_ui1 != true)
			{
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_IsFlyingEnabled));
				flyd_ui1 = true;
			}
		}
		else
		{
			if (flyd_ui1 != false)
			{
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_IsFlyingEnabled));
				flyd_ui1 = false;
			}
		}

		if (Globals::Player::coach_ui)
		{
			if (coach_ui1 != true)
			{
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_PlayerId));
				coach_ui1 = true;
			}
		}
		else
		{
			if (coach_ui1 != false)
			{
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_PlayerId));
				coach_ui1 = false;
			}
		}

		if (Globals::Player::doors_ui)
		{
			if (doors_ui1 != true)
			{
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + QuestRoomExitDoor_get_IsLocked));
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + ArenaDoor_get_IsLocked));
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + RoomDoorV2_get_IsLocked));
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + RoomDoor_get_IsLocked));
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + InteractablePortal_get_IsLocked));
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + LockableDoor_get_IsLocked));
				doors_ui1 = true;
			}
		}
		else
		{
			if (doors_ui1 != false)
			{
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + QuestRoomExitDoor_get_IsLocked));
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + ArenaDoor_get_IsLocked));
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + RoomDoorV2_get_IsLocked));
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + RoomDoor_get_IsLocked));
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + InteractablePortal_get_IsLocked));
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + LockableDoor_get_IsLocked));
				doors_ui1 = false;
			}
		}

		if (Globals::Misc::hashchecks_ui)
		{
			if (hashchecks_ui1 != true)
			{
				//MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + CheckHashesInBackground1));
				//MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + CheckHashesInBackground2));
				//MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + CheckHashesInBackground3));
				//MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + CheckHashesInBackground4));
				hashchecks_ui1 = true;
			}
		}
		else
		{
			if (hashchecks_ui1 != false)
			{
				//MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + CheckHashesInBackground1));
				//MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + CheckHashesInBackground2));
				//MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + CheckHashesInBackground3));
				//MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + CheckHashesInBackground4));
				hashchecks_ui1 = false;
			}
		}

		if (Globals::Misc::antikick_ui)
		{
			if (antikick_ui1 != true)
			{
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + LogoutToBootScene));
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + LogoutToBootSceneAsync));
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + GoToDorm));
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + FatalApplicationQuit));
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + TryApplicationQuit));
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + TryApplicationQuit1));
				//MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + RunJoinDormRoom1));
				//MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + RunJoinDormRoom2));
				//MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + RunJoinDormRoom3));
				antikick_ui1 = true;
			}
		}
		else
		{
			if (antikick_ui1 != false)
			{
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + LogoutToBootScene));
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + LogoutToBootSceneAsync));
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + GoToDorm));
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + FatalApplicationQuit));
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + TryApplicationQuit));
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + TryApplicationQuit1));
				//MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + RunJoinDormRoom1));
				//MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + RunJoinDormRoom2));
				//MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + RunJoinDormRoom3));
				antikick_ui1 = false;
			}
		}

		if (Globals::Misc::inkspoof_ui)
		{
			if (inkspoof_ui1 != true)
			{
				MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanAffordResourceCost));
				MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanAffordTool1));
				MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanAffordTool2));
				MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_IsOverInkLimit));
				inkspoof_ui1 = true;
			}
		}
		else
		{
			if (inkspoof_ui1 != false)
			{
				MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanAffordResourceCost));
				MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanAffordTool1));
				MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanAffordTool2));
				MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_IsOverInkLimit));
				inkspoof_ui1 = false;
			}
		}

		if (Globals::Misc::developer_ui)
		{
			if (developer_ui1 != true)
			{
				MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_LocalAccountIsModerator));
				MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_LocalAccountIsDeveloper));
				MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_IsDeveloper));
				developer_ui1 = true;
			}
		}
		else
		{
			if (developer_ui1 != false)
			{
				MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_LocalAccountIsModerator));
				MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_LocalAccountIsDeveloper));
				MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_IsDeveloper));
				developer_ui1 = false;
			}
		}

		if (Globals::Exploit::snowballtoggle_ui)
		{
			if (snowballtoggle_ui1 != true)
			{
				MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + RangedWeapon_Fire));
				snowballtoggle_ui1 = true;
			}
		}
		else
		{
			if (snowballtoggle_ui1 != false)
			{
				MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + RangedWeapon_Fire));
				snowballtoggle_ui1 = false;
			}
		}

		if (Globals::Exploit::foodsploittoggle_ui)
		{
			if (foodsploittoggle_ui1 != true)
			{
				MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + SpawnConsumableForLocalPlayer));
				foodsploittoggle_ui1 = true;
			}
		}
		else
		{
			if (foodsploittoggle_ui1 != false)
			{
				MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + SpawnConsumableForLocalPlayer));
				foodsploittoggle_ui1 = false;
			}
		}

		if (Globals::Player::scalegui)
		{
			if (scalegui1 != true)
			{
				MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_DesiredAvatarSkeletonTrackingSpaceScale));
				scalegui1 = true;
			}
		}
		else
		{
			if (scalegui1 != false)
			{
				MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_DesiredAvatarSkeletonTrackingSpaceScale));
				scalegui1 = false;
			}
		}



#if 0
		if (Globals::Combat::bounds_ui)
		{
			if (bounds_ui1 != true)
			{
				if (!CHook(flyaddy, &nTrue_hook))
				{
					MessageBox(NULL, "failed Hooked", "Message", MB_OK);
				}
				else {
					//MessageBox(NULL, "Hooked", "Message", MB_OK);
				}
				bounds_ui1 = true;
			}
		}
		else
		{
			if (bounds_ui1 != false)
			{
				if (!CUnHook(flyaddy))
				{
					MessageBox(NULL, "failed UnHooked", "Message", MB_OK);
				}
				MessageBox(NULL, "UnHooked", "Message", MB_OK);
				bounds_ui1 = false;
			}
		}
		if (Globals::Combat::rapidfire_ui)
		{
			if (rapidfire_ui1 != true)
			{
				if (!CHook(nofire, &uufalse))
				{
					MessageBox(NULL, "failed Hooked", "Message", MB_OK);
				}
				rapidfire_ui1 = true;
			}
		}
		else
		{
			if (rapidfire_ui1 != false)
			{
				if (!UnHook(nofire))
				{
					MessageBox(NULL, "failed UnHooked", "Message", MB_OK);
				}
				rapidfire_ui1 = false;
			}
		}
#endif
			
	}

//#if 0

	{
		const auto draw = ImGui::GetBackgroundDrawList();
		static const auto size = ImGui::GetIO().DisplaySize;
		static const auto center = ImVec2(size.x / 2, size.y / 2);

	}
//#endif
}

void consoleThread() {
	std::cout << XorStr("Console Thread Started") << std::endl;
	
	int input = 1;
	std::cin >> input;

	while (true)
	{
		std::string cmd;
		std::cout << XorStr(" > ");
		std::getline(std::cin, cmd);
		std::transform(cmd.begin(), cmd.end(), cmd.begin(), [](unsigned char c) { return std::tolower(c); });
		if (cmd == "alloff")
		{
			MH_DisableHook(MH_ALL_HOOKS);
			std::cout << XorStr("[Elysium] Toggled Everything Off.") << std::endl;
			const char* e_AllOff = XorStr("All Modules Toggled Off.");
		}
		else if (cmd == "quit")
		{
			MH_DisableHook(MH_ALL_HOOKS);
			MH_Uninitialize();
			Discord_Shutdown();
			Globals::IsClosing = false;
			Globals::Open = false;

			kiero::unbind(8);
			kiero::shutdown();

			FreeLibrary((HMODULE)Globals::DllInstance);
			std::cout << XorStr("[Elysium] Unloading DLL, you can now close the console.") << std::endl;
			const char* elysium_unload = XorStr("Elysium Client has Unloaded.");
			break;
		}
		else if (cmd == "test")
		{
			std::cout << XorStr("[Elysium] Testing . . .") << endl;
			uintptr_t function_address = GameAssembly + 0x13A3540;
			int(*Add)(int x, void* method_info) = (int(*)(int, void*))function_address;
			Add(1, NULL);
		}
		else if (cmd == "exit")
		{
			MH_DisableHook(MH_ALL_HOOKS);
			MH_Uninitialize();
			//Discord_Shutdown();
			std::cout << XorStr("[Elysium] Exiting.") << std::endl;
			const char* elysium_exited = XorStr("Elysium Client Exited.");
			break;
		}
		else if (cmd == "help")
		{
			std::cout << XorStr("Available commands:") << std::endl;
			std::cout << XorStr("- test: Crashes your game (test module, does not work)") << std::endl;
			std::cout << XorStr("- neptune: turns your screen into a cool colour") << std::endl;
			std::cout << XorStr("- alloff: Toggles all modules off") << std::endl;
			std::cout << XorStr("- quit: Unloads Elysium's console") << std::endl;
			std::cout << XorStr("- exit: Unloads Elysium & quits Rec Room") << std::endl;
			std::cout << XorStr("- help: Shows this text") << std::endl;
		}
		else
		{
		}
		std::cin.clear();
	}
	FreeConsole();
	DLL_PROCESS_DETACH;
}



int handler()
{
	static bool Done = false;

	while (!Done)
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)&oPresent, hkPresent);
			Done = true;
		}
	}
	while (true)
	{
		if ((GetAsyncKeyState(VK_END) & 1) || Globals::IsClosing)
			break;

		Sleep(1000);
	}

	Globals::IsClosing = false;
	Globals::Open = false;

	kiero::unbind(8);
	kiero::shutdown();

	FreeLibrary((HMODULE)Globals::DllInstance);
	return 0;
}

void main() {
	init();
	std::cout << XorStr("[Elysium] Initialized.") << std::endl;
	//Sleep(1500);
	hook();
	std::thread(&consoleThread).detach();
	handler();
}

void UpdatePresence()
{
	time_t start_time = time(nullptr);
	time_t elapsed_time = time(nullptr) - start_time;

	int hours = elapsed_time / 3600;
	int minutes = (elapsed_time % 3600) / 60;
	int seconds = elapsed_time % 60;

	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));

	discordPresence.state = "Buy now at Discord.gg/ApeShop";
	discordPresence.details = "Elysium BETA b0.7.3";
	discordPresence.startTimestamp = start_time * 1000;
	discordPresence.largeImageKey = "nword";
	discordPresence.largeImageText = "Made by neptune#1995 & VT#1065";
	Discord_UpdatePresence(&discordPresence);
}
