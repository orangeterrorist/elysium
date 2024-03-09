#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <cstdio>

#include "kiero/kiero.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#include "Encryption.hpp"

typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define CONSOLE_LOGGING true
#if CONSOLE_LOGGING
#define Log(Arg, ...) (printf("[>] " Arg "\n", __VA_ARGS__))
#else
#define Log()
#endif

static Present oPresent = nullptr;
static HWND window = nullptr;
static WNDPROC oWndProc = nullptr;
static ID3D11Device* pDevice = nullptr;
static ID3D11DeviceContext* pContext = nullptr;
static ID3D11RenderTargetView* mainRenderTargetView = nullptr;

extern bool auth;


namespace Globals
{
	static void* DllInstance = nullptr;
	static bool IsClosing = false;
	static bool Open = false;
	static int Tab = 0;

	namespace Combat
	{
		static bool bounds_ui = false;
		static bool ammo_ui = false;
		static bool rapidfire_ui = false;
		static bool ai_ui = false;
		static bool hijack_ui = false;
		static bool respawn_ui = false;
		static bool infhealth_ui = false;
		static bool instakill_ui = false;
	}

	namespace Player
	{
		static bool broadcast_ui = false;
		static bool eventperms_ui = false;
		static bool cct_ui = false;
		static bool fly_ui = false;
		static bool teleporttoggle_ui = false;
		static float teleport_ui = 1;
		static bool movement_ui = false;
		static bool grab_ui = false;
		static bool trialitems_ui = false;
		static bool scalespoof_ui = false;
		static bool antichatcooldown_ui = false;
		static bool makerpen_ui = false;

		static bool scalegui = false;
		static float bodysizegui = 1;

	}

	namespace Misc
	{
		static bool hashchecks_ui = false;
		static bool antikick_ui = false;
		static bool inkspoof_ui = false;
		static bool developer_ui = false;
	}
	
	namespace Exploit
	{
		static bool snowballtoggle_ui = false;
		static int snowball_ui = 10;
		static bool foodsploittoggle_ui = false;
		static int foodsploit_ui = 1;
	}
}