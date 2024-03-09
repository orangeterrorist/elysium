#include <ws2tcpip.h> // Include the additional Winsock2 functions
#include <windows.h>
#include <iostream>
#include <Minhook.h>
#include <random>
#include <thread>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cstring>

//auth
#include <curl/curl.h>
#include "main.h"
#include <Lmcons.h>
#include <iomanip>
#include <cstring>
#include "json.hpp"

//discord
#include "Discord/discord_rpc.h"


// fix notif vuln error
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#pragma warning(disable:4244)
#pragma warning(disable:4267)

// snowball exploit variable
int snowballcount = 10;
int foodsploitcount = 10;
float playerScale = 0.0f;
bool notify{ true };
std::string key;

uintptr_t base = (uintptr_t)GetModuleHandle(NULL);
uintptr_t GameAssembly = (uintptr_t)GetModuleHandle("GameAssembly.dll");

// Offsets below

uintptr_t Hysteria = 0x25D29B0; // bool true IsValid (SafeReportResult, Hysteria) get from DnSpy uhhhhhhhhhhhhhggggggg
uintptr_t RapidFire = 0x1836800; // bool false RecRoom.Core.Combat.RangedWeapon$$get_IsOnCooldown
uintptr_t AddFly = 0x2017F60; // special RecRoom.Core.Locomotion.PlayerMovement$$AddFlyEnabled
uintptr_t RemoveFly = 0x2024AE0; // NOP RecRoom.Core.Locomotion.PlayerMovement$$RemoveFlyEnabled
uintptr_t IsFlyEnabled = 0x20285E0; // bool true RecRoom.Core.Locomotion.PlayerMovement$$get_IsFlyingEnabled

uintptr_t OreintationSkip = 0x1033730; // bool true RecRoom.Activities.Orientation.OrientationSubScene$$get_IsFinished

uintptr_t CanStartBroadcasting = 0x1631A60; // bool true
uintptr_t CanStopBroadcasting = 0x1631BB0; // bool true
uintptr_t HasBroadcastingAuthorization = 0x1631FC0; // bool true
uintptr_t get_CanRequestBroadcastingAuthorization = 0x1631790; // bool true
uintptr_t get_CanViewBroadcasters = 0x1631C70; // bool true
uintptr_t get_CanManageBroadcasters = 0x16316C0; // bool true

uintptr_t get_CanRSVP = 0x1631770; // bool true
uintptr_t get_CanJoinEvent = 0x1631630; // bool true
uintptr_t get_CanCreatePrivateEventInstance = 0x16313D0; // bool true
uintptr_t get_CanInviteFriends = 0x16315D0; // bool true
uintptr_t get_EventOrganizerIsLocalPlayer = 0x1631D20; // bool true

uintptr_t CanSpawnConsumable = 0x2049150; // bool true
uintptr_t CanUseSharecam = 0x20493D0; // bool true

uintptr_t CanUseCCT = 0x1CB8B20; // bool true RecRoom.ClothingCustomizerTool$$get_LocalPlayerCanUse

uintptr_t InBounds = 0xFBC2E0; // bool true GameBoundsManager$$get_LocalPlayerInBounds
uintptr_t RapidFire2 = 0x1A9EE80; // bool false RecRoom.Core.Creation.SandboxGunHandle$$get_IsOnCooldown
uintptr_t WasMakerPenEverOut = 0x1F7B440; // bool false
uintptr_t LtBS = 0x13E3FC0; // NOP LogoutToBootScene
uintptr_t LtBSAsync = 0x13E3EE0; // NOP LogoutToBootSceneAsync
uintptr_t CheckHashesInBackground = 0x14028B0; // NOP CheckHashesInBackground_b__0
uintptr_t MaxTeleportHorizontalDistance = 0x2028B40; // float 9999999.9f
uintptr_t MaxTeleportDropDistance = 0x2028A90; // float 9999999.9f
uintptr_t IsSendChatOnCooldown = 0x14F9350; // bool false
uintptr_t LocalAccountIsModerator = 0x1340270; // bool true
uintptr_t LocalAccountIsDeveloper = 0x1340050; // bool true
uintptr_t DLPOKG = 0x16F6B20; // bool true DoesLocalPlayerOwnKey
uintptr_t DLPOKL = 0x16F6B60; // bool true DoesLocalPlayerOwnKey
uintptr_t creationobjectisgrabbable = 0x18E5A70; // bool true RecRoom.Core.Creation.CreationObject$$get_IsGrabbable "TypeSignature": "iii"
uintptr_t get_LocalPlayerCanUseMakerPen = 0x27677872; // bool true
uintptr_t get_CanUseMakerPenRR = 0x20492E0; // bool true
uintptr_t get_CanUseMakerPenGR = 0x14B5ED0; // bool true
uintptr_t get_LocalPlayerCanUseCreationMode = 0x1A65420; // bool true
uintptr_t CanPlayerCreate = 0x1A5D970; // bool true
uintptr_t CanPlayerCreateWithMakerPen = 0x1FB6F10; // bool true
uintptr_t PlayerCanUseMakerPenAccordingToRoles = 0x1A63340; // bool true
uintptr_t creationobjectisfrozen = 0x18DBDA0; // bool false RecRoom.Core.Creation.CreationObject$$get_IsFrozen
uintptr_t IsDeveloper = 0x33DE240; // bool true
uintptr_t RemoveTrialItem = 0x1148F60; // bool false get_IsHeldByPlayer
uintptr_t BootLocalPlayerToDormRoom = 0x13DB300; // NOP
uintptr_t LogWasMakerPenEverOut = 0x1F7B440; // NOP
uintptr_t get_MagazineAmmunition = 0x197FFE0; // Int32 - 2 DWORD
uintptr_t get_HasEnoughMagAmmo = 0x197FE10; // bool true RecRoom.Core.Combat.Weapon$$get_HasEnoughMagazineAmmunition
uintptr_t GetEnemyDamage = 0x197BB30; // int 2147483647 - 3 DWORD
uintptr_t GetPlayerAttackDamage = 0x1FFE6A0; // int 2147483647 - 2 DWORD
uintptr_t get_TotalAmmunition = 0x1980100; // Int32 - 2 DWORD
uintptr_t get_CanAnyPlayerSit = 0x13D36C0; // bool true - 2 DWORD
uintptr_t get_RespawnDuration = 0x10CA140; // float 0.0 - 2 DWORD
uintptr_t snowball = 0x1834260; // void RecRoom.Core.Combat.RangedWeapon$$Fire "TypeSignature": "viifi"
uintptr_t foodsploit = 0x1B68A70; // void SpawnConsumableForLocalPlayer(string BPMABOMGJLN, long? NCOPCHMDABB, Vector3 DACBJNCMNFL, Quaternion MCFIHGJCDCP, float BDMGGOCELCK, PlayerSpawnedEquipment.JIPNAHJEKAE BHADODIAHJB)
uintptr_t DeleteAll = 0x1A64420; // bool true RecRoom.Systems.PlayerRoles.PlayerRoomRoleManager$$get_LocalPlayerCanDeleteAll
uintptr_t get_IsOverInkLimit = 0x18D9B80; // bool false
uintptr_t CanAffordTool = 0x18CF910; // bool true RecRoom.Core.Creation.CreationManager$$CanAffordTool "TypeSignature": "iiiii"
uintptr_t CanAffordTool1 = 0x18CF8E0; // bool true RecRoom.Core.Creation.CreationManager$$CanAffordTool "TypeSignature": "iiii"
uintptr_t RunJoinDormRoom = 0x13E8810; // bool false SessionManager$$RunJoinDormRoom
uintptr_t CanAffordResourceCost = 0x18CF7B0; // bool true
uintptr_t get_SelfScaleMaxSize = 0x18CDC00; // float 99999999.f
uintptr_t get_CanChangeMovementMode = 0x30956096; // bool false
uintptr_t TryApplicationQuit = 0x13EAC10; // NOP void SessionManager__TryApplicationQuit (int32_t OBHNBAGOABK, const MethodInfo* method)
uintptr_t TryApplicationQuit1 = 0x13EAAD0; // NOP "Signature": "void SessionManager__TryApplicationQuit (const MethodInfo* method)"
uintptr_t FatalApplicationQuit = 0x13DE570; // NOP
uintptr_t PlayerSize = 0x10C8340; // special float get_desiredavatarskeletontrackingspacescale

uintptr_t Kiss = 0x5485FF0; // func

// These spoof client sided, maybe set photon master to make it work? cba rn
//uintptr_t GetPlayerHealth = 0x20C1B60; //Int32 9999999 - 3 DWORD
//uintptr_t GetPlayerShield = 0x20C1BF0; //Int32 9999999 - 3 DWORD
//uintptr_t get_MaxHealth = 0x1A0DD50; //Int32 9999999 - 2 DWORD
//uintptr_t HysteriaURL = 0x3FB00D0;//special 3FB00D0
//uintptr_t HysteriaTest = 0x3FAFCF0;//HysteriaTest
//uintptr_t HysteriaVURL = 0x820E260;

// Sample Function:
/*
void(__fastcall* AddFly_o)(DWORD*, bool, DWORD*, INT32, DWORD*);
void __stdcall AddFly_hook(DWORD* __this, bool enable, DWORD* something, INT32 priority, DWORD* methodinfo) {
    bool enable_new = true;
    printf("AddFly Hook Called\n");
    return AddFly_o(__this, enable_new, something, priority, methodinfo);
}
*/

std::string get_device_id() {
    std::string result;

    // Retrieve the processor ID from the registry
    HKEY hKey;
    DWORD dwType;
    DWORD dwSize = 16;
    std::vector<char> szProcessorId(dwSize + 1);
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueExA(hKey, "~MHz", NULL, &dwType, reinterpret_cast<LPBYTE>(szProcessorId.data()), &dwSize) == ERROR_SUCCESS) {
            szProcessorId[dwSize] = '\0';
            result += szProcessorId.data();
        }
        RegCloseKey(hKey);
    }

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\BIOS", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        dwSize = 40;
        std::vector<char> szBaseBoardId(dwSize + 1);
        if (RegQueryValueExA(hKey, "BaseBoardProduct", NULL, &dwType, reinterpret_cast<LPBYTE>(szBaseBoardId.data()), &dwSize) == ERROR_SUCCESS) {
            szBaseBoardId[dwSize] = '\0';
            result += szBaseBoardId.data();
        }
        RegCloseKey(hKey);
    }

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\MountedDevices", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        dwSize = 1024;
        std::vector<BYTE> pData(dwSize);
        if (RegQueryValueExA(hKey, "\\DosDevices\\C:", NULL, &dwType, pData.data(), &dwSize) == ERROR_SUCCESS) {
            std::stringstream ssBootDriveId;
            ssBootDriveId << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(pData[8])
                << std::setw(2) << std::setfill('0') << static_cast<int>(pData[9])
                << std::setw(2) << std::setfill('0') << static_cast<int>(pData[10])
                << std::setw(2) << std::setfill('0') << static_cast<int>(pData[11]);
            result += ssBootDriveId.str();
        }
        RegCloseKey(hKey);
    }

    std::stringstream ss;
    for (int i = 0; i < result.size(); i++) {
        ss << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(result[i]);
    }

    return ss.str();
}

// load notification

void showNotification(const std::string& message, int timeout) {
    if (notify = true)
    {
        const std::string VerNum{ "BETA v0.7" };
        NOTIFYICONDATA nid = {};
        nid.cbSize = sizeof(nid);
        nid.hWnd = NULL;
        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dist(1, 9999999999999999999);
        nid.uID = dist(rng);
        nid.uFlags = NIF_ICON | NIF_INFO;
        nid.hIcon = LoadIcon(NULL, IDI_EXCLAMATION);
        nid.uTimeout = timeout;
        nid.dwInfoFlags = NIIF_INFO;
        std::string full_title = "Elysium " + VerNum + "\nby neptune#1995 & VT#1065";
        strcpy_s(nid.szInfoTitle, full_title.c_str());
        strcpy_s(nid.szInfo, std::string(message).c_str());
        Shell_NotifyIcon(NIM_ADD, &nid);
    }
}

// snowball exploit
void(__fastcall* snowball_o)(DWORD*, DWORD*, float*, DWORD*);
void __stdcall snowball_hook(DWORD* __this, DWORD* BNEEGIEDHBB, float* IMBPOKPKJBF, DWORD* method) {
    for (int i = 0; i < snowballcount; ++i) {
        snowball_o(__this, BNEEGIEDHBB, IMBPOKPKJBF, method);
    }
    return snowball_o(__this, BNEEGIEDHBB, IMBPOKPKJBF, method);
}

// foodsploit
void(__fastcall* foodsploit_o)(DWORD*, DWORD*, DWORD*, DWORD*, DWORD*, float*, INT32*, DWORD*);
void __stdcall foodsploit_hook(DWORD* __this, DWORD* BPMABOMGJLN, DWORD* NCOPCHMDABB, DWORD* DACBJNCMNFL, DWORD* MCFIHGJCDCP, float* BDMGGOCELCK, INT32* BHADODIAHJB, DWORD* method) {
    for (int i = 0; i < foodsploitcount; ++i) {
        foodsploit_o(__this, BPMABOMGJLN, NCOPCHMDABB, DACBJNCMNFL, MCFIHGJCDCP, BDMGGOCELCK, BHADODIAHJB, method);
    }
    return foodsploit_o(__this, BPMABOMGJLN, NCOPCHMDABB, DACBJNCMNFL, MCFIHGJCDCP, BDMGGOCELCK, BHADODIAHJB, method);
}

// fly
void(__fastcall* AddFly_o)(DWORD*, bool, DWORD*, INT32, DWORD*);
void __stdcall AddFly_hook(DWORD* __this, bool enable, DWORD* something, INT32 priority, DWORD* methodinfo) {
    bool enable_new = true;
    //printf("AddFly Hook Called\n");
    return AddFly_o(__this, enable_new, something, priority, methodinfo);
}


bool SendCommandToToy(const std::string& address, int port, const std::string& command) {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed with error: " << result << std::endl;
        return false;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return false;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, address.c_str(), &server_addr.sin_addr.s_addr);

    result = connect(sock, (SOCKADDR*)&server_addr, sizeof(server_addr));
    if (result == SOCKET_ERROR) {
        std::cerr << "Failed to connect to the toy with error: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return false;
    }

    result = send(sock, command.c_str(), command.length(), 0);
    if (result == SOCKET_ERROR) {
        std::cerr << "Failed to send command with error: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return false;
    }

    closesocket(sock);
    WSACleanup();
    return true;
}


// kiss
void(__fastcall* kiss_o)(DWORD*, DWORD*);
void __stdcall kiss_hook(DWORD* __this, DWORD* methodinfo) {
    std::string toy_address = "192.168.0.100";
    int toy_port = 80;
    int intensity = 20;
    std::string vibrate_command = "Vibrate:" + std::to_string(intensity);
    SendCommandToToy(toy_address, toy_port, vibrate_command);
    return kiss_o(__this, methodinfo);
}

// NOP

void(__fastcall* unop_o)(DWORD*);
void __stdcall unop_hook(DWORD* uh) {
    return;
}


// bools

bool(__fastcall* utrue_o)(DWORD*);
bool __stdcall utrue_hook(DWORD* uh) {
    return true;
}

bool(__fastcall* ufalse_o)(DWORD*);
bool __stdcall ufalse_hook(DWORD* uh) {
    return false;
}

// Floats below

//player scale

float(__fastcall* playersize_o)(DWORD*);
float __stdcall playersize_hook(DWORD* uh) {
    return playerScale;
}

float(__fastcall* ufloatbig_o)(DWORD*);
float __stdcall ufloatbig_hook(DWORD* uh) {
    return 99999999.9f;
}


float(__fastcall* ufloat0_o)(DWORD*);
float __stdcall ufloat0_hook(DWORD* uh) {
    return 0.0f;
}

// INT32 below

INT32(__fastcall* uintbig_o)(DWORD*);
INT32 __stdcall uintbig_hook(DWORD* uh) {
    return INT32(999999);
}

INT32(__fastcall* uint1_o)(DWORD*);
INT32 __stdcall uint1_hook(DWORD* uh) {
    return INT32(1);
}

int32_t(__fastcall* uint0_o)(DWORD*);
int32_t __stdcall uint0_hook(DWORD* uh) {
    return int32_t(0);
}


void init()
{
    // don't make this anymore cancer than it needs to be, whole init func needs to be rewritten
        const std::string VerNum{ "BETA v0.7" };

        MH_Initialize();

        MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + AddFly), &AddFly_hook, (LPVOID*)&AddFly_o);
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + RemoveFly), &unop_hook, (LPVOID*)&unop_o);

        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + CheckHashesInBackground), &unop_hook, (LPVOID*)&unop_o);
        MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + CheckHashesInBackground));

        MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + Hysteria), &utrue_hook, (LPVOID*)&utrue_o);
        MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + RapidFire), &ufalse_hook, (LPVOID*)&ufalse_o);
        //MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + IsFlyEnabled), &utrue_hook, (LPVOID*)&utrue_o);
        MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + OreintationSkip), &utrue_hook, (LPVOID*)&utrue_o);
        MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanStartBroadcasting), &utrue_hook, (LPVOID*)&utrue_o);
        MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanStopBroadcasting), &utrue_hook, (LPVOID*)&utrue_o);
        MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + HasBroadcastingAuthorization), &utrue_hook, (LPVOID*)&utrue_o);
        MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanSpawnConsumable), &utrue_hook, (LPVOID*)&utrue_o);
        MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanUseSharecam), &utrue_hook, (LPVOID*)&utrue_o);
        MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanUseCCT), &utrue_hook, (LPVOID*)&utrue_o);
        MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + InBounds), &utrue_hook, (LPVOID*)&utrue_o);
        MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + RapidFire2), &ufalse_hook, (LPVOID*)&ufalse_o);
        MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + WasMakerPenEverOut), &ufalse_hook, (LPVOID*)&ufalse_o);
        MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + IsSendChatOnCooldown), &ufalse_hook, (LPVOID*)&ufalse_o);
        MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + LocalAccountIsModerator), &utrue_hook, (LPVOID*)&utrue_o);
        MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + LocalAccountIsDeveloper), &utrue_hook, (LPVOID*)&utrue_o);
        MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + DLPOKG), &utrue_hook, (LPVOID*)&utrue_o);
        MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + DLPOKL), &utrue_hook, (LPVOID*)&utrue_o);
        MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + DeleteAll), &utrue_hook, (LPVOID*)&utrue_o);
        MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + IsDeveloper), &utrue_hook, (LPVOID*)&utrue_o);
        MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + RemoveTrialItem), &ufalse_hook, (LPVOID*)&ufalse_o);
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + LtBS), &unop_hook, (LPVOID*)&unop_o);
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + LtBSAsync), &unop_hook, (LPVOID*)&unop_o);
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + BootLocalPlayerToDormRoom), &unop_hook, (LPVOID*)&unop_o);
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + LogWasMakerPenEverOut), &unop_hook, (LPVOID*)&unop_o);

        //FatalApplicationQuit
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + FatalApplicationQuit), &unop_hook, (LPVOID*)&unop_o);

        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + TryApplicationQuit), &unop_hook, (LPVOID*)&unop_o);
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + TryApplicationQuit1), &unop_hook, (LPVOID*)&unop_o);

        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + RunJoinDormRoom), &unop_hook, (LPVOID*)&unop_o);
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + MaxTeleportHorizontalDistance), &ufloatbig_hook, (LPVOID*)&ufloatbig_o);
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + MaxTeleportDropDistance), &ufloatbig_hook, (LPVOID*)&ufloatbig_o);
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_MagazineAmmunition), &uint1_hook, (LPVOID*)&uint1_o);
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_HasEnoughMagAmmo), &utrue_hook, (LPVOID*)&utrue_o);

        // MakerPen below
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + PlayerCanUseMakerPenAccordingToRoles), &utrue_hook, (LPVOID*)&utrue_o);
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + CanPlayerCreateWithMakerPen), &utrue_hook, (LPVOID*)&utrue_o);
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + CanPlayerCreate), &utrue_hook, (LPVOID*)&utrue_o);
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_LocalPlayerCanUseCreationMode), &utrue_hook, (LPVOID*)&utrue_o);
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanUseMakerPenGR), &utrue_hook, (LPVOID*)&utrue_o);
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanUseMakerPenRR), &utrue_hook, (LPVOID*)&utrue_o);
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_LocalPlayerCanUseMakerPen), &utrue_hook, (LPVOID*)&utrue_o);
        // MakerPen above

        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + GetEnemyDamage), &uintbig_hook, (LPVOID*)&uintbig_o);
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + GetPlayerAttackDamage), &uint0_hook, (LPVOID*)&uint0_o);
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_TotalAmmunition), &uintbig_hook, (LPVOID*)&uintbig_o);
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanAnyPlayerSit), &utrue_hook, (LPVOID*)&utrue_o);

        // I don't think that below hook actually does anything
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_RespawnDuration), &ufloat0_hook, (LPVOID*)&ufloat0_o);

        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanChangeMovementMode), &utrue_hook, (LPVOID*)&utrue_o);
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + snowball), &snowball_hook, (LPVOID*)&snowball_o);
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + foodsploit), &foodsploit_hook, (LPVOID*)&foodsploit_o);
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_SelfScaleMaxSize), &ufloatbig_hook, (LPVOID*)&ufloatbig_o);
        MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanAffordResourceCost), &uint0_hook, (LPVOID*)&uint0_o);
        MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanAffordTool), &uint0_hook, (LPVOID*)&uint0_o);
        MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanAffordTool1), &uint0_hook, (LPVOID*)&uint0_o);
        MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_IsOverInkLimit), &ufalse_hook, (LPVOID*)&ufalse_o);

        MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + PlayerSize), &playersize_hook, (LPVOID*)&playersize_o);

        // broadcasting
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanRequestBroadcastingAuthorization), &utrue_hook, (LPVOID*)&utrue_o);
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanViewBroadcasters), &utrue_hook, (LPVOID*)&utrue_o);
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanManageBroadcasters), &utrue_hook, (LPVOID*)&utrue_o);

        // event perms
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanRSVP), &utrue_hook, (LPVOID*)&utrue_o);
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanJoinEvent), &utrue_hook, (LPVOID*)&utrue_o);
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanCreatePrivateEventInstance), &utrue_hook, (LPVOID*)&utrue_o);
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanInviteFriends), &utrue_hook, (LPVOID*)&utrue_o);
        MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_EventOrganizerIsLocalPlayer), &utrue_hook, (LPVOID*)&utrue_o);

        std::cout << "Elysium Client " + VerNum + " Initialized Successfully! \nHello Rec Room! >:^)" << std::endl;
        showNotification("Elysium Client has been initialized!", 2000);
}

static void UpdatePresence()
{
    time_t start_time = time(nullptr);
    time_t elapsed_time = time(nullptr) - start_time;

    int hours = elapsed_time / 3600;
    int minutes = (elapsed_time % 3600) / 60;
    int seconds = elapsed_time % 60;

    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));

    discordPresence.state = "Buy now at Discord.gg/ApeShop";
    discordPresence.details = "Elysium Client BETA v0.7";
    discordPresence.startTimestamp = start_time * 1000; // set the start timestamp to the current time
    //discordPresence.endTimestamp = (start_time + elapsed_time) * 1000; // set the end timestamp to the current time
    discordPresence.largeImageKey = "nword";
    discordPresence.largeImageText = "Made by neptune#1995 & VT#1065";
    //discordPresence.smallImageText = "Rec Room";
    //discordPresence.partyId = "ae488379-351d-4a4f-ad32-2b9b01c91657";
    //discordPresence.partySize = 1;
    //discordPresence.partyMax = 4;
    //discordPresence.joinSecret = "MTI4NzM0OjFpMmhuZToxMjMxMjM= ";
    Discord_UpdatePresence(&discordPresence);
}




// thing for auth
size_t writeCallback(char* buf, size_t size, size_t nmemb, std::string* response) {
    response->append(buf, size * nmemb);
    return size * nmemb;
}

void logout()
{
    CURL* curl;
    CURLcode res;


    // Get hardware profile info
    HW_PROFILE_INFO hwProfileInfoStruct;
    if (GetCurrentHwProfileA(&hwProfileInfoStruct)) {
        std::string hwProfileInfo = std::string(hwProfileInfoStruct.szHwProfileGuid);
    }
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://google.com/");

        HW_PROFILE_INFO hwProfileInfo;
        GetCurrentHwProfile(&hwProfileInfo);

        char username[UNLEN + 1];
        DWORD username_len = UNLEN + 1;
        GetUserName(username, &username_len);

        char computer_name[MAX_COMPUTERNAME_LENGTH + 1];
        DWORD size = sizeof(computer_name);
        GetComputerNameA(computer_name, &size);

        std::string data = "&pc=" + std::string(computer_name) + "&user=" + std::string(username) + "&hwid=" + std::string(hwProfileInfo.szHwProfileGuid) + "&key=" + key;

        // encode
        const char* data_bytes = data.c_str();
        size_t data_size = strlen(data_bytes);

        std::stringstream hex_stream;
        hex_stream << std::hex << std::setfill('0');
        for (size_t i = 0; i < data_size; i++)
        {
            hex_stream << std::setw(2) << static_cast<int>(static_cast<unsigned char>(data_bytes[i]));
        }
        std::string encoded_data = hex_stream.str();
        std::string enreq = "&data=" + encoded_data;

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, enreq.c_str());

        // send request
        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        return;
    }
    return;
}


void consoleThread() {
    const std::string VerNum{ "BETA v0.7" };
    AllocConsole();

    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    std::string title = "Elysium Client Authentication | by neptune#1995 & VT#1065";
    SetConsoleTitleA(title.c_str());

    // discord stuff
    DiscordEventHandlers Handle;
    memset(&Handle, 0, sizeof(Handle));
    Discord_Initialize("1079687619773988865", &Handle, 1, NULL);
    std::thread(UpdatePresence).detach();
    

    // Initalizing all bools as false
    bool flytoggled{ false };
    bool rapidfire{ false };
    bool makerpen{ false };
    bool devtoggle{ false };
    bool snowballtoggle{ false };
    bool foodsploittoggle{ false };
    bool playersizescale{ false };
    bool grabtoggle{ false };
    bool antikicktoggle{ false };
    bool teleporttoggle{ false };
    bool alltoggle{ false };
    bool inkspooftoggle{ false };
    bool broacasting{ false };
    bool eventperms{ false };

    bool CCT{ false };
    bool bounds{ false };
    bool coolchat{ false };
    bool jew{ false };
    bool ammo{ false };
    bool ai{ false };
    bool hijack{ false };
    bool respawn{ false };
    bool bigger{ false };
    bool move{ false };

    bool exit{ false };

    std::string device_id = get_device_id();
    std::cout << "Device ID: " << device_id << std::endl;

    while (true)
    {
        const char* appdataPath = std::getenv("LOCALAPPDATA");
        const char* filePath = "\\..\\LocalLow\\Against Gravity\\Rec Room\\Player.log";
        char fullPath[MAX_PATH];
        std::strcpy(fullPath, appdataPath);
        std::strcat(fullPath, filePath);
        std::ofstream file(fullPath);
        file.close();


        std::string cmd;
        std::cout << " > ";
        std::getline(std::cin, cmd);
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), [](unsigned char c) { return std::tolower(c); });

        // toggles
        if (cmd == "fly")
        {
            flytoggled = !flytoggled;

            if (flytoggled == true)
            {
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + AddFly));
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + RemoveFly));
                std::cout << "[Elysium] Toggled Fly On." << std::endl;
                showNotification("Fly Toggled On.", 500);
            }
            else
            {
                MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + AddFly));
                MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + RemoveFly));
                std::cout << "[Elysium] Toggled Fly Off." << std::endl;
                showNotification("Fly Toggled Off.", 500);
            }
        }
        if (cmd == "rapidfire")
        {
            rapidfire = !rapidfire;

            if (rapidfire == true)
            {
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + RapidFire));
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + RapidFire2));
                std::cout << "[Elysium] Toggled Rapid Fire On." << std::endl;
                showNotification("Rapid Fire Toggled On.", 500);
            }
            else
            {
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + RapidFire));
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + RapidFire2));
                std::cout << "[Elysium] Toggled Rapid Fire Off." << std::endl;
                showNotification("Rapid Fire Toggled Off.", 500);
            }
        }
        if (cmd == "makerpen")
        {
            makerpen = !makerpen;

            if (makerpen == true)
            {
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + PlayerCanUseMakerPenAccordingToRoles));
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + CanPlayerCreateWithMakerPen));
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + CanPlayerCreate));
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_LocalPlayerCanUseCreationMode));
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanUseMakerPenGR));
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanUseMakerPenRR));
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_LocalPlayerCanUseMakerPen));
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + WasMakerPenEverOut));
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + LogWasMakerPenEverOut));
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + DeleteAll));
                std::cout << "[Elysium] Toggled Maker Pen On." << std::endl;
                showNotification("Maker Pen Toggled On.", 500);
            }
            else
            {
                MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + PlayerCanUseMakerPenAccordingToRoles));
                MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + CanPlayerCreateWithMakerPen));
                MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + CanPlayerCreate));
                MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_LocalPlayerCanUseCreationMode));
                MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanUseMakerPenGR));
                MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanUseMakerPenRR));
                MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_LocalPlayerCanUseMakerPen));
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + WasMakerPenEverOut));
                MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + LogWasMakerPenEverOut));
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + DeleteAll));
                std::cout << "[Elysium] Toggled Maker Pen Off." << std::endl;
                showNotification("Maker Pen Toggled Off.", 500);
            }
        }
        if (cmd == "broadcast")
        {
            broacasting = !broacasting;

            if (broacasting == true)
            {
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanStartBroadcasting));
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanStopBroadcasting));
                //MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_CanRequestBroadcastingAuthorization));
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_CanViewBroadcasters));
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_CanManageBroadcasters));
                std::cout << "[Elysium] Toggled Broadcasting On." << std::endl;
                showNotification("Broacasting Toggled On.", 500);
            }
            else
            {
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanStartBroadcasting));
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanStopBroadcasting));
                //MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_CanRequestBroadcastingAuthorization));
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_CanViewBroadcasters));
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_CanManageBroadcasters));
                std::cout << "[Elysium] Toggled Broacasting Off." << std::endl;
                showNotification("Broacasting Toggled Off.", 500);
            }
        }
        if (cmd == "eventperms")
        {
            eventperms = !eventperms;

            if (eventperms == true)
            {
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_CanRSVP));
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_CanJoinEvent));
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_CanCreatePrivateEventInstance));
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_CanInviteFriends));
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_EventOrganizerIsLocalPlayer));
                std::cout << "[Elysium] Toggled Event Permissions On." << std::endl;
                showNotification("Event Permissions Toggled On.", 500);
            }
            else
            {
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_CanRSVP));
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_CanJoinEvent));
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_CanCreatePrivateEventInstance));
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_CanInviteFriends));
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_EventOrganizerIsLocalPlayer));
                std::cout << "[Elysium] Toggled Event Permissions Off." << std::endl;
                showNotification("Event Permissions Toggled Off.", 500);
            }
        }
        if (cmd == "dev")
        {
            devtoggle = !devtoggle;

            if (devtoggle == true)
            {
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + LocalAccountIsModerator));
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + LocalAccountIsDeveloper));
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + IsDeveloper));
                std::cout << "[Elysium] Toggled Developer Mode On." << std::endl;
                showNotification("Developer Mode Toggled On.", 500);
            }
            else
            {
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + LocalAccountIsModerator));
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + LocalAccountIsDeveloper));
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + IsDeveloper));
                std::cout << "[Elysium] Toggled Developer Mode Off." << std::endl;
                showNotification("Developer Mode Toggled Off.", 500);
            }
        }
        if (cmd == "snowball")
        {
            snowballtoggle = !snowballtoggle;

            if (snowballtoggle == true)
            {
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + snowball));
                std::cout << "[Elysium] Toggled Snowball Exploit On." << std::endl;
                showNotification("Snowball Exploit Toggled On.", 500);
            }
            else
            {
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + snowball));
                std::cout << "[Elysium] Toggled Snowball Exploit Off." << std::endl;
                showNotification("Snowball Exploit Toggled Off.", 500);
            }
        }
        if (cmd == "foodsploit")
        {
            foodsploittoggle = !foodsploittoggle;

            if (foodsploittoggle == true)
            {
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + foodsploit));
                std::cout << "[Elysium] Toggled Foodsploit On." << std::endl;
                showNotification("Foodsploit Toggled On.", 500);
            }
            else
            {
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + foodsploit));
                std::cout << "[Elysium] Toggled Foodsploit Off." << std::endl;
                showNotification("Foodsploit Toggled Off.", 500);
            }
        }
        if (cmd == "playerscale")
        {
            playersizescale = !playersizescale;

            if (playersizescale == true)
            {
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + PlayerSize));
                std::cout << "[Elysium] Toggled Player Scale Override On." << std::endl;
                showNotification("Player Scale Override Toggled On.", 500);
            }
            else
            {
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + PlayerSize));
                std::cout << "[Elysium] Toggled Player Scale Override Off." << std::endl;
                showNotification("Player Scale Override Toggled Off.", 500);
            }
        }
        if (cmd == "grab")
        {
            grabtoggle = !grabtoggle;

            if (grabtoggle == true)
            {
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + DLPOKG));
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + DLPOKL));
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + creationobjectisgrabbable));
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + creationobjectisfrozen));
                std::cout << "[Elysium] Toggled Force Grab On." << std::endl;
                showNotification("Force Grab Toggled On.", 500);
            }
            else
            {
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + DLPOKG));
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + DLPOKL));
                MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + creationobjectisgrabbable));
                MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + creationobjectisfrozen));
                std::cout << "[Elysium] Toggled Force Grab Off." << std::endl;
                showNotification("Force Grab Toggled Off.", 500);
            }
        }
        if (cmd == "antikick")
        {
            antikicktoggle = !antikicktoggle;

            if (antikicktoggle == true)
            {
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + LtBS));
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + LtBSAsync));
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + BootLocalPlayerToDormRoom));
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + FatalApplicationQuit));
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + TryApplicationQuit));
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + TryApplicationQuit1));
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + RunJoinDormRoom));
                std::cout << "[Elysium] Toggled Anti Kick On." << std::endl;
                showNotification("Anti-Kick Toggled On.", 500);
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
                std::cout << "[Elysium] Toggled Anti Kick Off." << std::endl;
                showNotification("Anti-Kick Toggled Off.", 500);
            }
        }
        if (cmd == "teleport")
        {
            teleporttoggle = !teleporttoggle;

            if (teleporttoggle == true)
            {
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + MaxTeleportHorizontalDistance));
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + MaxTeleportDropDistance));
                std::cout << "[Elysium] Toggled Teleport On." << std::endl;
                showNotification("Teleport Override Toggled On.", 500);
            }
            else
            {
                MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + MaxTeleportHorizontalDistance));
                MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + MaxTeleportDropDistance));
                std::cout << "[Elysium] Toggled Teleport Off." << std::endl;
                showNotification("Teleport Override Toggled Off.", 500);
            }
        }
        if (cmd == "inkspoof")
        {
            inkspooftoggle = !inkspooftoggle;

            if (inkspooftoggle == true)
            {
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanAffordResourceCost));
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanAffordTool));
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanAffordTool1));
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_IsOverInkLimit));
                std::cout << "[Elysium] Toggled Ink Spoof On." << std::endl;
                showNotification("Ink Spoof Toggled On.", 500);
            }
            else
            {
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanAffordResourceCost));
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanAffordTool));
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanAffordTool1));
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_IsOverInkLimit));
                std::cout << "[Elysium] Toggled Ink Spoof Off." << std::endl;
                showNotification("Ink Spoof Toggled Off.", 500);
            }
        }
        if (cmd == "cct")
        {
            CCT = !CCT;

            if (CCT == true)
            {
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanUseCCT));
                std::cout << "[Elysium] Toggled Clothing Customizer On." << std::endl;
                showNotification("Clothing Customizer Toggled On.", 500);
            }
            else
            {
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanUseCCT));
                std::cout << "[Elysium] Toggled Clothing Customizer Off." << std::endl;
                showNotification("Clothing Customizer Toggled Off.", 500);
            }
        }
        if (cmd == "bounds")
        {
            bounds = !bounds;

            if (bounds == true)
            {
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + InBounds));
                std::cout << "[Elysium] Toggled Bounds On." << std::endl;
                showNotification("Bounds Toggled On.", 500);
            }
            else
            {
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + InBounds));
                std::cout << "[Elysium] Toggled Bounds Off." << std::endl;
                showNotification("Bounds Toggled Off.", 500);
            }
        }
        if (cmd == "jew")
        {
            jew = !jew;

            if (jew == true)
            {
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + RemoveTrialItem));
                std::cout << "[Elysium] Toggled Trial Item Override On." << std::endl;
                showNotification("Trial Item Override Toggled On.", 500);
            }
            else
            {
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + RemoveTrialItem));
                std::cout << "[Elysium] Toggled Trial Item Override Off." << std::endl;
                showNotification("Trial Item Override Toggled Off.", 500);
            }
        }
        if (cmd == "ammo")
        {
            ammo = !ammo;

            if (ammo == true)
            {
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_MagazineAmmunition));
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_HasEnoughMagAmmo));
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_TotalAmmunition));
                std::cout << "[Elysium] Toggled Infinite Ammo On." << std::endl;
                showNotification("Infinite Ammo Toggled On.", 500);
            }
            else
            {
                MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_MagazineAmmunition));
                MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_HasEnoughMagAmmo));
                MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_TotalAmmunition));
                std::cout << "[Elysium] Toggled Infinite Ammo Off." << std::endl;
                showNotification("Infinite Ammo Toggled Off.", 500);
            }
        }
        if (cmd == "ai")
        {
            ai = !ai;

            if (ai == true)
            {
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + GetEnemyDamage));
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + GetPlayerAttackDamage));
                std::cout << "[Elysium] Toggled AI Damage On." << std::endl;
                showNotification("AI Damage Toggled On.", 500);
            }
            else
            {
                MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + GetEnemyDamage));
                MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + GetPlayerAttackDamage));
                std::cout << "[Elysium] Toggled AI Damage Off." << std::endl;
                showNotification("AI Damage Toggled Off.", 500);
            }
        }
        if (cmd == "respawn")
        {
            respawn = !respawn;

            if (respawn == true)
            {
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_RespawnDuration));
                std::cout << "[Elysium] Toggled Slightly Faster Respawn On." << std::endl;
                showNotification("Slightly Faster Respawn Toggled On.", 500);
            }
            else
            {
                MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_RespawnDuration));
                std::cout << "[Elysium] Toggled Slightly Faster Respawn Off." << std::endl;
                showNotification("Slightly Faster Respawn Toggled Off.", 500);
            }
        }
        if (cmd == "hijack")
        {
            hijack = !hijack;

            if (hijack == true)
            {
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanAnyPlayerSit));
                std::cout << "[Elysium] Toggled Hijack On." << std::endl;
                showNotification("Hijack Toggled On.", 500);
            }
            else
            {
                MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanAnyPlayerSit));
                std::cout << "[Elysium] Toggled Hijack Off." << std::endl;
                showNotification("Hijack Toggled Off.", 500);
            }
        }
        if (cmd == "chat")
        {
            coolchat = !coolchat;

            if (coolchat == true)
            {
                MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + IsSendChatOnCooldown));
                std::cout << "[Elysium] Toggled Chat Cooldown On." << std::endl;
                showNotification("Chat Cooldown Toggled On.", 500);
            }
            else
            {
                MH_DisableHook(reinterpret_cast<LPBOOL*>(GameAssembly + IsSendChatOnCooldown));
                std::cout << "[Elysium] Toggled Chat Cooldown Off." << std::endl;
                showNotification("Chat Cooldown Toggled Off.", 500);
            }
        }
        if (cmd == "bigger")
        {
            bigger = !bigger;

            if (bigger == true)
            {
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_SelfScaleMaxSize));
                std::cout << "[Elysium] Toggled Infinite Self Scale On." << std::endl;
                showNotification("Infinite Self Scale Toggled On.", 500);
            }
            else
            {
                MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_SelfScaleMaxSize));
                std::cout << "[Elysium] Toggled Infinite Self Scale Off." << std::endl;
                showNotification("Infinite Self Scale Toggled Off.", 500);
            }
        }
        if (cmd == "move")
        {
            move = !move;

            if (move == true)
            {
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanChangeMovementMode));
                std::cout << "[Elysium] Toggled Movement Setting Override On." << std::endl;
                showNotification("Movement Setting Override Toggled On.", 500);
            }
            else
            {
                MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanChangeMovementMode));
                std::cout << "[Elysium] Toggled Movement Setting Override Toggled Off." << std::endl;
                showNotification("Movement Setting Override Toggled Off.", 500);
            }
        }
        if (cmd == "alloff")
        {
            //MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + Hysteria));
            /*
            * 8706-5 shadowban cause role anti-cheat
            * 
            MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanSpawnConsumable));
            MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanUseSharecam));
            */
            MH_DisableHook(MH_ALL_HOOKS);
            flytoggled = false;
            rapidfire = false;
            devtoggle = false;
            snowballtoggle = false;
            foodsploittoggle = false;
            grabtoggle = false;
            antikicktoggle = false;
            teleporttoggle = false;
            inkspooftoggle = false;
            broacasting = false;
            eventperms = false;
            CCT = false;
            bounds = false;;
            coolchat = false;
            jew = false;
            ammo = false;
            ai = false;
            hijack = false;
            respawn = false;
            bigger = false;
            move = false;
            std::cout << "[Elysium] Toggled Everything Off." << std::endl;
            showNotification("All Modules Toggled Off.", 500);
        }
        // exploit stuff
        if (cmd == "snowball#")
        {
            std::cout << "[Elysium] Enter snowball count (bullet amount)." << std::endl;
            while (!(std::cin >> snowballcount)) {
                std::cout << "[Elysium] Invalid input, please enter an integer." << std::endl;
                std::cin.clear();
                std::cin.ignore(cmd.size(), '\n');
            }
            std::cout << "[Elysium] Set snowball count to '" << snowballcount << "'." << std::endl;
            showNotification("Foodsploit Amount Set.", 500);
        }
        if (cmd == "foodsploit#")
        {
            std::cout << "[Elysium] Enter foodsploitcount (food amount)." << std::endl;
            while (!(std::cin >> foodsploitcount)) {
                std::cout << "[Elysium] Invalid input, please enter an integer." << std::endl;
                std::cin.clear();
                std::cin.ignore(cmd.size(), '\n');
            }
            std::cout << "[Elysium] Set foodsploitcount to '" << foodsploitcount << "'." << std::endl;
            showNotification("Foodsploit Amount Set.", 500);
        }
        if (cmd == "playerscale#") {
            std::cout << "[Elysium] Enter Player Scale Size (size float)." << std::endl;
            while (!(std::cin >> playerScale)) {
                std::cout << "[Elysium] Invalid input, please enter an float." << std::endl;
                std::cin.clear();
                std::cin.ignore(cmd.size(), '\n >');
            }
            std::cout << "[Elysium] Set Player Scale to '" << playerScale << "'." << std::endl;
            showNotification("Player Scale Size Set.", 500);
        }
        else if (cmd == "quit")
        {
            MH_DisableHook(MH_ALL_HOOKS);
            MH_Uninitialize();
            Discord_Shutdown();
            logout();
            std::cout << "[Elysium] Unloading DLL, you can now close the console." << std::endl;
            showNotification("Elysium Client has Unloaded.", 1000);
            break;
        }
        else if (cmd == "notif")
        {
            notify = !notify;

            if (notify == true)
            {
                MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanChangeMovementMode));
                std::cout << "[Elysium] Toggled Notifications On." << std::endl;
                showNotification("Notifications On.", 500);
            }
            else
            {
                MH_DisableHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanChangeMovementMode));
                std::cout << "[Elysium] Toggled Notifications Off." << std::endl;
            }
        }
        else if (cmd == "exit")
        {
            logout();
            MH_DisableHook(MH_ALL_HOOKS);
            MH_Uninitialize();
            Discord_Shutdown();
            exit = true;
            std::cout << "[Elysium] Exiting." << std::endl;
            showNotification("Elysium Client Exited.", 1000);
            break;
        }
        else if (cmd == "help")
        {
            std::cout << "Available commands:" << std::endl;
            std::cout << "- fly: toggles fly" << std::endl;
            std::cout << "- rapidfire: Toggles rapid fire" << std::endl;
            std::cout << "- dev: Toggles developer mode" << std::endl;
            std::cout << "- grab: Toggles making all objects grabbable" << std::endl;
            std::cout << "- antikick: Toggles anti kick" << std::endl;
            std::cout << "- teleport: Toggles teleport" << std::endl;
            std::cout << "- inkspoof: Toggles inkspoof" << std::endl;

            std::cout << "- broadcast: Toggles broacasting permissions" << std::endl;
            std::cout << "- eventperms: Toggles event permissions" << std::endl;
            std::cout << "- cct: Toggles clothing customizer" << std::endl;
            std::cout << "- bounds: Toggles bounds restrictions" << std::endl;
            std::cout << "- chat: Toggles chat cooldown" << std::endl;
            std::cout << "- jew: Toggles trial item override" << std::endl;
            std::cout << "- ammo: Toggles infinite ammo" << std::endl;
            std::cout << "- ai: Toggles insta-kill ai & no ai damage" << std::endl;
            std::cout << "- hijack: Toggles hijack/force sit" << std::endl;
            std::cout << "- respawn: Toggles slightly faster respawn" << std::endl;
            std::cout << "- bigger: Toggles infinite self scale" << std::endl;
            std::cout << "- move: Toggles movement override" << std::endl;


            std::cout << "- alloff: Toggles all modules off" << std::endl;


            std::cout << "- snowball: Toggles snowball exploit" << std::endl;
            std::cout << "- snowball#: Configure snowball exploit (enter valid int)" << std::endl;
            std::cout << "- foodsploit: Toggle foodsploit" << std::endl;
            std::cout << "- foodsploit#: Configure foodsploit (enter valid int)" << std::endl;
            std::cout << "- playerscale: Toggle playerscale override" << std::endl;
            std::cout << "- playerscale#: Configure playerscale override (enter valid float)" << std::endl;
            //std::cout << "- init: Initializes Elysium" << std::endl;

            std::cout << "- notif: Toggles notifications" << std::endl;
            std::cout << "- quit: Unloads Elysium" << std::endl;
            std::cout << "- exit: Unloads Elysium & quits Rec Room" << std::endl;
            std::cout << "- help: Shows this text" << std::endl;
        }
        /*
        * having init command make's it so if auth fails the user can 
        * manually init which should only be done on auth success so
        * if user attempts to jmp auth, menu will be disfunctional
        * 
        {
            std::cout << "( ^ - ^)/ Initializing..." << std::endl;
            init();
        }
        *
        * command example ig, keeping it for now
        * 
        if (cmd == "testbool")
        {
        }
        *
        * the following code is example code for calling a func, kinda useless
        * since args are usually required which crashes game if invalid
        *
        if (cmd == "")
        {
            typedef void(__fastcall* KillAllLivingEnemies)(std::string, std::string);
            KillAllLivingEnemies killAllEnemiesFunc = reinterpret_cast<KillAllLivingEnemies>(0x1354270);
            killAllEnemiesFunc();
        }
        */
        else
        {
            // commented out cause this is called for some reason (probably no return but i cba testing rn)
            //std::cout << "[Elysium] Command not found, type 'help' for a list of commands." << std::endl;
        }
        std::cin.clear();
        //std::cin.ignore(cmd.size(), '\n');
    }
    FreeConsole();
    DLL_PROCESS_DETACH;
    if (exit)
    {
        logout();
        std::exit(0);
    }
    ExitThread;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        std::thread(consoleThread).detach();
        break;
    case DLL_PROCESS_DETACH:
        break;
    default:
        break;
    }
    switch (ul_reason_for_call)
        return TRUE;
}