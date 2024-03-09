#pragma once

//#include "il2cpp.hpp"
//#include "together.hpp"
#include "Globals.hpp"
#include "XorStr.h"

#include <iostream>
#include "polyhook2/Detour/x64Detour.hpp"

#pragma comment (lib, "asmjit.lib")
#pragma comment (lib, "asmtk.lib")
#pragma comment (lib, "PolyHook_2.lib")
#pragma comment (lib, "Zydis.lib")
#pragma comment (lib, "Zycore.lib")

// snowball exploit
void(__fastcall* snowball_o)(DWORD*, DWORD*, float*, DWORD*);
void __stdcall snowball_hook(DWORD* __this, DWORD* BNEEGIEDHBB, float* IMBPOKPKJBF, DWORD* method) {
    int snowball;
    snowball = Globals::Exploit::snowballcount;

    for (int i = 0; i < snowball; ++i) {
        snowball_o(__this, BNEEGIEDHBB, IMBPOKPKJBF, method);
        //std::cout << "snowball: " << snowball << std::endl << "global: " << Globals::Exploit::snowballcount << std::endl;
    }
    return snowball_o(__this, BNEEGIEDHBB, IMBPOKPKJBF, method);
}

// foodsploit
void(__fastcall* foodsploit_o)(DWORD*, DWORD*, DWORD*, DWORD*, DWORD*, float*, INT32*, DWORD*);
void __stdcall foodsploit_hook(DWORD* __this, DWORD* BPMABOMGJLN, DWORD* NCOPCHMDABB, DWORD* DACBJNCMNFL, DWORD* MCFIHGJCDCP, float* BDMGGOCELCK, INT32* BHADODIAHJB, DWORD* method) {
    int foodsploitcountt = Globals::Exploit::foodsploitcount;
    for (int i = 0; i < foodsploitcountt; ++i) {
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

// NOP

void(__fastcall* unop_o)(DWORD*);
void __stdcall unop_hook(DWORD* uh) {
    return;
}


// bools

bool(__fastcall* utrue_o)(DWORD*, DWORD*);
bool __stdcall utrue_hook(DWORD* uh, DWORD* uhh) {
    return true;
}

bool(__fastcall* ufalse_o)(DWORD*);
bool __stdcall ufalse_hook(DWORD* uh) {
    return false;
}

//player scale

float(__fastcall* playersize_o)(DWORD*);
float __stdcall playersize_hook(DWORD* uh) {
    return Globals::Player::PlayerSize;
}

// Floats below

float(__fastcall* ufloatbig_o)(DWORD*);
float __stdcall ufloatbig_hook(DWORD* uh) {
    return 99999999.9f;
}

float(__fastcall* teleport_o)(DWORD*);
float __stdcall teleport_hook(DWORD* uh) {
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

void hook()
{
    // don't make this anymore cancer than it needs to be, whole init func needs to be rewritten
    MH_Initialize();

    MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + AddFlyEnabled), &AddFly_hook, (LPVOID*)&AddFly_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + RemoveFlyEnabled), &unop_hook, (LPVOID*)&unop_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_IsFlyingEnabled), &utrue_hook, (LPVOID*)&utrue_o);

    MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + RangedWeapon_get_IsOnCooldown), &ufalse_hook, (LPVOID*)&ufalse_o);
    MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + SandboxGunHandle_get_IsOnCooldown), &ufalse_hook, (LPVOID*)&ufalse_o);

    MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + CCT_get_LocalPlayerCanUse), &utrue_hook, (LPVOID*)&utrue_o);
    MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_LocalPlayerInBounds), &utrue_hook, (LPVOID*)&utrue_o);
    MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_WasMakerPenEverOut), &ufalse_hook, (LPVOID*)&ufalse_o);
    MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_IsSendChatOnCooldown), &ufalse_hook, (LPVOID*)&ufalse_o);
    MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_LocalAccountIsModerator), &utrue_hook, (LPVOID*)&utrue_o);
    MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_LocalAccountIsDeveloper), &utrue_hook, (LPVOID*)&utrue_o);
    MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + DoesLocalPlayerOwnKey1), &utrue_hook, (LPVOID*)&utrue_o);
    MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + DoesLocalPlayerOwnKey2), &utrue_hook, (LPVOID*)&utrue_o);
    //MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + DoesLocalPlayerOwnKey3), &utrue_hook, (LPVOID*)&utrue_o);

    MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_IsDeveloper), &utrue_hook, (LPVOID*)&utrue_o);
    //MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + LocalRemoveTrialItems), &ufalse_hook, (LPVOID*)&ufalse_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + LogoutToBootScene), &unop_hook, (LPVOID*)&unop_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + LogoutToBootSceneAsync), &unop_hook, (LPVOID*)&unop_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + GoToDorm), &unop_hook, (LPVOID*)&unop_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + LogWasMakerPenEverOut), &unop_hook, (LPVOID*)&unop_o);

    //FatalApplicationQuit
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + FatalApplicationQuit), &unop_hook, (LPVOID*)&unop_o);

    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + TryApplicationQuit), &unop_hook, (LPVOID*)&unop_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + TryApplicationQuit1), &unop_hook, (LPVOID*)&unop_o);

    //MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + RunJoinDormRoom2), &ufalse_hook, (LPVOID*)&ufalse_o);
    //MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + RunJoinDormRoom2), &unop_hook, (LPVOID*)&unop_o);
    //MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + RunJoinDormRoom3), &unop_hook, (LPVOID*)&unop_o);

    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_MagazineAmmunition), &uint1_hook, (LPVOID*)&uint1_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_HasEnoughMagazineAmmunition), &utrue_hook, (LPVOID*)&utrue_o);

    // MakerPen below
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + CanPlayerCreateWithMakerPen), &utrue_hook, (LPVOID*)&utrue_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanUseMakerPen), &utrue_hook, (LPVOID*)&utrue_o);
    // MakerPen above

    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + Weapon_GetEnemyDamage), &uintbig_hook, (LPVOID*)&uintbig_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + WeaponMelee_GetEnemyDamageMelee), &uintbig_hook, (LPVOID*)&uintbig_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + GetPlayerAttackDamage), &uint0_hook, (LPVOID*)&uint0_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_TotalAmmunition), &uintbig_hook, (LPVOID*)&uintbig_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanAnyPlayerSit), &utrue_hook, (LPVOID*)&utrue_o);

    // I don't think that below hook actually does anything
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + Player_get_RespawnDuration), &ufloat0_hook, (LPVOID*)&ufloat0_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + GameCombatManager_get_RespawnDuration), &ufloat0_hook, (LPVOID*)&ufloat0_o);

    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanChangeMovementMode), &utrue_hook, (LPVOID*)&utrue_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + RangedWeapon_Fire), &snowball_hook, (LPVOID*)&snowball_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + SpawnConsumableForLocalPlayer), &foodsploit_hook, (LPVOID*)&foodsploit_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_SelfScaleMaxSize), &ufloatbig_hook, (LPVOID*)&ufloatbig_o);
    MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanAffordResourceCost), &uint0_hook, (LPVOID*)&uint0_o);
    MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanAffordTool1), &uint0_hook, (LPVOID*)&uint0_o);
    MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanAffordTool2), &uint0_hook, (LPVOID*)&uint0_o);
    MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + get_IsOverInkLimit), &ufalse_hook, (LPVOID*)&ufalse_o);

#if 0
    MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + OreintationSkip), &utrue_hook, (LPVOID*)&utrue_o);

    // broadcasting
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanRequestBroadcastingAuthorization), &utrue_hook, (LPVOID*)&utrue_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanViewBroadcasters), &utrue_hook, (LPVOID*)&utrue_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanManageBroadcasters), &utrue_hook, (LPVOID*)&utrue_o);
    MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanStartBroadcasting), &utrue_hook, (LPVOID*)&utrue_o);
    MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanStopBroadcasting), &utrue_hook, (LPVOID*)&utrue_o);
    MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + HasBroadcastingAuthorization), &utrue_hook, (LPVOID*)&utrue_o);

    // event perms
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanRSVP), &utrue_hook, (LPVOID*)&utrue_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanJoinEvent), &utrue_hook, (LPVOID*)&utrue_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanCreatePrivateEventInstance), &utrue_hook, (LPVOID*)&utrue_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanInviteFriends), &utrue_hook, (LPVOID*)&utrue_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_EventOrganizerIsLocalPlayer), &utrue_hook, (LPVOID*)&utrue_o);
#endif

    // teleport
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_MaxTeleportHorizontalDistance), &teleport_hook, (LPVOID*)&teleport_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_MaxTeleportDropDistance), &teleport_hook, (LPVOID*)&teleport_o);

    //MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + CanUseMakerPenInternal1), &utrue_hook, (LPVOID*)&utrue_o);
    //MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + CanUsemakerPenInternal2), &utrue_hook, (LPVOID*)&utrue_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + CreatorRoleCanUseMakerPen), &utrue_hook, (LPVOID*)&utrue_o);

    // doors
    MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + QuestRoomExitDoor_get_IsLocked), &ufalse_hook, (LPVOID*)&ufalse_o);
    MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + ArenaDoor_get_IsLocked), &ufalse_hook, (LPVOID*)&ufalse_o);
    MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + RoomDoorV2_get_IsLocked), &ufalse_hook, (LPVOID*)&ufalse_o);
    MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + RoomDoor_get_IsLocked), &ufalse_hook, (LPVOID*)&ufalse_o);
    MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + InteractablePortal_get_IsLocked), &ufalse_hook, (LPVOID*)&ufalse_o);
    MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + LockableDoor_get_IsLocked), &ufalse_hook, (LPVOID*)&ufalse_o);

    // hash checks
    //MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + CheckHashesInBackground1), &unop_hook, (LPVOID*)&unop_o);
    //MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + CheckHashesInBackground2), &unop_hook, (LPVOID*)&unop_o);
    //MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + CheckHashesInBackground3), &ufalse_hook, (LPVOID*)&ufalse_o);
    //MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + CheckHashesInBackground4), &unop_hook, (LPVOID*)&unop_o);

    // coach
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_PlayerId), &uint1_hook, (LPVOID*)&uint1_o);

    // grab 
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + CreationObject_get_IsGrabbable), &utrue_hook, (LPVOID*)&utrue_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + OMCreationObject_get_IsGrabbable), &utrue_hook, (LPVOID*)&utrue_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + Polaroid_get_IsGrabbable), &utrue_hook, (LPVOID*)&utrue_o);

    MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + Chip_get_IsFrozen), &ufalse_hook, (LPVOID*)&ufalse_o);
    MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + CreationObject_get_IsFrozen), &ufalse_hook, (LPVOID*)&ufalse_o);
    MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + Tool_get_IsFrozen), &ufalse_hook, (LPVOID*)&ufalse_o);
    //MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + 0x2443C20), &utrue_hook, (LPVOID*)&utrue_o);
    //MH_EnableHook(reinterpret_cast<LPVOID*>(GameAssembly + 0x2443C20));

    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_DesiredAvatarSkeletonTrackingSpaceScale), &playersize_hook, (LPVOID*)&playersize_o);

    system(XorStr("cls"));
    std::cout << XorStr("+===================================================================================+") << std::endl;
    std::cout << XorStr("Elysium Client ") << Globals::Version << XorStr(" Initialized Successfully. \nHello Rec Room! >:^)") << std::endl;
    std::cout << XorStr("+-----------------------------------------------------------------------------------+") << std::endl;
    std::cout << XorStr("Welcome to Elysium Client!. \nType 'help' for a list of commands. Press 'home' key to open the click gui.") << std::endl;
    std::cout << XorStr("+===================================================================================+") << std::endl;
}