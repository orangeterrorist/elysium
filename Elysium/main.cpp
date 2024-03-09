#include "il2cpp.hpp"
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
#include "together.hpp"
#include "offsets.hpp"
#include "hooking.hpp"
#include "Globals.hpp"
#include <random>





using namespace std;

void init()
{
	MH_Initialize();
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

	style.WindowMinSize = ImVec2(256, 300);
	style.WindowTitleAlign = ImVec2(0.5, 0.5);
	style.FrameBorderSize = 1;
	style.ChildBorderSize = 1;
	style.WindowBorderSize = 1;
	style.WindowRounding = 0;
	style.FrameRounding = 0;
	style.ChildRounding = 0;
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

	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 14.0f);

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
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

void OnDraw()
{
	static auto Slider = [&](const char* label, float* value, float min, float max, float width = ImGui::GetContentRegionAvail().x)
	{
		ImGui::PushID(label);
		ImGui::PushItemWidth(width);
		ImGui::SliderFloat(_("##CustomSliderF_"), value, min, max);
		ImGui::PopItemWidth();
		ImGui::PopID();
		ImGui::Spacing();
	};

	static auto IntSlider = [&](const char* label, int* value, int min, int max, float width = ImGui::GetContentRegionAvail().x)
	{
		ImGui::PushID(label);
		ImGui::PushItemWidth(width);
		ImGui::SliderInt(_("##CustomSliderF_"), value, min, max);
		ImGui::PopItemWidth();
		ImGui::PopID();
		ImGui::Spacing();
	};

	static auto Checkbox = [&](const char* label, bool* value)
	{
		ImGui::PushID(label);
		ImGui::Checkbox(label, value);
		ImGui::PopID();
		ImGui::Spacing();
	};

	if (Globals::Open) {
		ImGui::Begin(_("Elysium v0.7"), 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		{
			if (ImGui::Button(_("Combat"), ImVec2(ImGui::GetContentRegionAvail().x / 4, 29)))
				Globals::Tab = 0;

			ImGui::SameLine();

			if (ImGui::Button(_("Player"), ImVec2(ImGui::GetContentRegionAvail().x / 3, 29)))
				Globals::Tab = 1;

			ImGui::SameLine();

			if (ImGui::Button(_("Misc"), ImVec2(ImGui::GetContentRegionAvail().x / 2, 29)))
				Globals::Tab = 2;

			ImGui::SameLine();

			if (ImGui::Button(_("Beta"), ImVec2(ImGui::GetContentRegionAvail().x / 1, 29)))
				Globals::Tab = 3;

			ImGui::Spacing();
			ImGui::Separator();

			switch (Globals::Tab)
			{
			case 0: // Combat
				Checkbox(_("In Bounds"), &Globals::Combat::bounds_ui);
				Checkbox(_("Infinite Ammo"), &Globals::Combat::ammo_ui);
				Checkbox(_("No Fire Rate"), &Globals::Combat::rapidfire_ui);
				Checkbox(_("AI Insta-Kill & No Damage"), &Globals::Combat::ai_ui);
				Checkbox(_("Hijack"), &Globals::Combat::hijack_ui);
				Checkbox(_("Respawn"), &Globals::Combat::respawn_ui);
				Checkbox(_("Infinite Health"), &Globals::Combat::infhealth_ui);
				Checkbox(_("Infinite Health"), &Globals::Combat::instakill_ui);
				break;

			case 1: // Player
				Checkbox(_("Player Scale"), &Globals::Player::scalegui);
				Slider(_("Player Scale"), &Globals::Player::bodysizegui, 0, 1000);
				Checkbox(_("Teleport Range"), &Globals::Player::teleporttoggle_ui);
				Slider(_("Teleport Range"), &Globals::Player::teleport_ui, 0, 99999999);


				Checkbox(_("Broacasting"), &Globals::Player::broadcast_ui);
				Checkbox(_("Event Perms"), &Globals::Player::eventperms_ui);
				Checkbox(_("Clothing Customizer Tool"), &Globals::Player::cct_ui);
				Checkbox(_("Fly"), &Globals::Player::fly_ui);
				Checkbox(_("Movement Mode Bypass"), &Globals::Player::movement_ui);
				Checkbox(_("Grab"), &Globals::Player::grab_ui);
				Checkbox(_("Trail Items"), &Globals::Player::trialitems_ui);
				Checkbox(_("No Self Scale Limit"), &Globals::Player::scalespoof_ui);
				Checkbox(_("No Chat Cooldown"), &Globals::Player::antichatcooldown_ui);
				Checkbox(_("Maker Pen"), &Globals::Player::makerpen_ui);
				break;

			case 2: // Misc
				Checkbox(_("Disable Anti Cheat Checks"), &Globals::Misc::hashchecks_ui);
				Checkbox(_("Anti-Kick"), &Globals::Misc::antikick_ui);
				Checkbox(_("Ink Spoof"), &Globals::Misc::inkspoof_ui);
				Checkbox(_("Developer Mode"), &Globals::Misc::developer_ui);
				break;

			case 3: // Settings
				Checkbox(_("Snowball Exploit"), &Globals::Exploit::snowballtoggle_ui);
				IntSlider(_("Snowball Exploit"), &Globals::Exploit::snowball_ui, 0, 1000);
				Checkbox(_("Foodsploit"), &Globals::Exploit::foodsploittoggle_ui);
				IntSlider(_("Foodsploit"), &Globals::Exploit::foodsploit_ui, 0, 2000);
				if (ImGui::Button(_("Unload")))
					Globals::IsClosing = true;
				break;
			}
		}
		ImGui::End();
	}
	// Cheat loop here lol

	static bool Once = true;

	if (Once) {

		// Do your initialize stuff here..
		Once = false;
	}

	{
		const auto draw = ImGui::GetBackgroundDrawList();
		static const auto size = ImGui::GetIO().DisplaySize;
		static const auto center = ImVec2(size.x / 2, size.y / 2);

		if (Globals::Combat::bounds_ui)
		{
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + InBounds));
		}
		else
		{
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + InBounds));
		}

		if (Globals::Combat::ammo_ui)
		{
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_MagazineAmmunition));
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_HasEnoughMagAmmo));
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_TotalAmmunition));
		}
		else
		{
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_MagazineAmmunition));
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_HasEnoughMagAmmo));
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_TotalAmmunition));
		}

		if (Globals::Combat::rapidfire_ui)
		{
			MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + RapidFire));
			MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + RapidFire2));
		}
		else
		{
			MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + RapidFire));
			MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + RapidFire2));
		}

		if (Globals::Combat::ai_ui)
		{
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + GetEnemyDamage));
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + GetPlayerAttackDamage));
		}
		else
		{
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + GetEnemyDamage));
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + GetPlayerAttackDamage));
		}

		if (Globals::Combat::hijack_ui)
		{
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanAnyPlayerSit));
		}
		else
		{
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanAnyPlayerSit));
		}

		if (Globals::Combat::respawn_ui)
		{
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_RespawnDuration));
		}
		else
		{
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_RespawnDuration));
		}

		if (Globals::Combat::infhealth_ui)
		{
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + playerinvincible));
		}
		else
		{
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + playerinvincible));
		}

		if (Globals::Combat::instakill_ui)
		{
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + instakill));
		}
		else
		{
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + instakill));
		}



		if (Globals::Player::broadcast_ui)
		{
			MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_CanRequestBroadcastingAuthorization));
			MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_CanViewBroadcasters));
			MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_CanManageBroadcasters));
		}

		else
		{
			MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_CanRequestBroadcastingAuthorization));
			MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_CanViewBroadcasters));
			MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_CanManageBroadcasters));
		}

		if (Globals::Player::eventperms_ui)
		{
			MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_CanRSVP));
			MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_CanJoinEvent));
			MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_CanCreatePrivateEventInstance));
			MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_CanInviteFriends));
			MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_EventOrganizerIsLocalPlayer));
		}
		else
		{
			MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_CanRSVP));
			MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_CanJoinEvent));
			MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_CanCreatePrivateEventInstance));
			MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_CanInviteFriends));
			MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_EventOrganizerIsLocalPlayer));
		}

		if (Globals::Player::cct_ui)
		{
			MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanUseCCT));
		}
		else
		{
			MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanUseCCT));
		}

		if (Globals::Player::teleporttoggle_ui)
		{
			teleport = Globals::Player::teleport_ui;
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + MaxTeleportHorizontalDistance));
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + MaxTeleportDropDistance));
		}
		else
		{
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + MaxTeleportHorizontalDistance));
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + MaxTeleportDropDistance));
		}

		if (Globals::Player::movement_ui)
		{
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanChangeMovementMode));
		}
		else
		{
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanChangeMovementMode));
		}

		if (Globals::Player::grab_ui)
		{
			MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + DLPOKG));
			MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + DLPOKL));
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + creationobjectisgrabbable));
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + creationobjectisfrozen));
		}
		else
		{
			MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + DLPOKG));
			MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + DLPOKL));
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + creationobjectisgrabbable));
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + creationobjectisfrozen));
		}

		if (Globals::Player::trialitems_ui)
		{
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + RemoveTrialItem));
		}
		else
		{
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + RemoveTrialItem));
		}

		if (Globals::Player::scalespoof_ui)
		{
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_SelfScaleMaxSize));
		}
		else
		{
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_SelfScaleMaxSize));
		}

		if (Globals::Player::antichatcooldown_ui)
		{
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + IsSendChatOnCooldown));
		}
		else
		{
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + IsSendChatOnCooldown));
		}

		if (Globals::Player::makerpen_ui)
		{
			//MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + PlayerCanUseMakerPenAccordingToRoles));
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + CanPlayerCreateWithMakerPen));
			//MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + CanPlayerCreate));
			//MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_LocalPlayerCanUseCreationMode));
			//MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanUseMakerPenGR));
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanUseMakerPenRR));
			//MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_LocalPlayerCanUseMakerPen));
			MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + WasMakerPenEverOut));
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + LogWasMakerPenEverOut));
			//MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + DeleteAll));
		}
		else
		{
			//MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + PlayerCanUseMakerPenAccordingToRoles));
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + CanPlayerCreateWithMakerPen));
			//MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + CanPlayerCreate));
			//MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_LocalPlayerCanUseCreationMode));
			//MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanUseMakerPenGR));
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanUseMakerPenRR));
			//MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_LocalPlayerCanUseMakerPen));
			MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + WasMakerPenEverOut));
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + LogWasMakerPenEverOut));
			//MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + DeleteAll));
		}

		if (Globals::Player::fly_ui)
		{
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_IsFlyingEnabled));
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + AddFly));
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + RemoveFly));
		}
		else
		{
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_IsFlyingEnabled));
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + AddFly));
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + RemoveFly));
		}

		if (Globals::Misc::hashchecks_ui)
		{
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + CheckHashesInBackground));
		}
		else
		{
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + CheckHashesInBackground));
		}

		if (Globals::Misc::antikick_ui)
		{
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + LtBS));
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + LtBSAsync));
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + BootLocalPlayerToDormRoom));
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + FatalApplicationQuit));
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + TryApplicationQuit));
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + TryApplicationQuit1));
			MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + RunJoinDormRoom));
		}
		else
		{
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + LtBS));
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + LtBSAsync));
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + BootLocalPlayerToDormRoom));
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + FatalApplicationQuit));
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + TryApplicationQuit));
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + TryApplicationQuit1));
			MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + RunJoinDormRoom));
		}

		if (Globals::Misc::inkspoof_ui)
		{
			MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanAffordResourceCost));
			MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanAffordTool));
			MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanAffordTool1));
			MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_IsOverInkLimit));
		}
		else
		{
			MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanAffordResourceCost));
			MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanAffordTool));
			MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanAffordTool1));
			MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_IsOverInkLimit));
		}

		if (Globals::Misc::developer_ui)
		{

			MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + LocalAccountIsModerator));
			MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + LocalAccountIsDeveloper));
			MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + IsDeveloper));
		}
		else
		{
			MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + LocalAccountIsModerator));
			MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + LocalAccountIsDeveloper));
			MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + IsDeveloper));
		}

		if (Globals::Exploit::snowballtoggle_ui)
		{
			snowballcount = Globals::Exploit::snowball_ui;
			MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + snowball));
		}
		else
		{
			MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + snowball));
		}

		if (Globals::Exploit::foodsploittoggle_ui)
		{
			foodsploitcount = Globals::Exploit::foodsploit_ui;
			MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + foodsploit));
		}
		else
		{
			MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + foodsploit));
		}

	}
}

int main2()
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
	return TRUE;
}

void main() {
	init();
	Sleep(1500);
	hook();
	main2();
}