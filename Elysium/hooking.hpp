#pragma once

#include "il2cpp.hpp"
#include "together.hpp"

float teleport = 1.0f;

int snowballcount = 10;
int foodsploitcount = 10;
float playerScale = 0.0f;

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

// kiss
void(__fastcall* kiss_o)(DWORD*, DWORD*);
void __stdcall kiss_hook(DWORD* __this, DWORD* methodinfo) {
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

float(__fastcall* teleport_o)(DWORD*);
float __stdcall teleport_hook(DWORD* uh) {
    return teleport;
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
    //MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanSpawnConsumable), &utrue_hook, (LPVOID*)&utrue_o);
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
    //MH_CreateHook(reinterpret_cast<LPBOOL*>(GameAssembly + DeleteAll), &utrue_hook, (LPVOID*)&utrue_o);
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

    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_MagazineAmmunition), &uint1_hook, (LPVOID*)&uint1_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_HasEnoughMagAmmo), &utrue_hook, (LPVOID*)&utrue_o);

    // MakerPen below
    //MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + PlayerCanUseMakerPenAccordingToRoles), &utrue_hook, (LPVOID*)&utrue_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + CanPlayerCreateWithMakerPen), &utrue_hook, (LPVOID*)&utrue_o);
    //MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + CanPlayerCreate), &utrue_hook, (LPVOID*)&utrue_o);
    //MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_LocalPlayerCanUseCreationMode), &utrue_hook, (LPVOID*)&utrue_o);
    //MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanUseMakerPenGR), &utrue_hook, (LPVOID*)&utrue_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_CanUseMakerPenRR), &utrue_hook, (LPVOID*)&utrue_o);
    //MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + get_LocalPlayerCanUseMakerPen), &utrue_hook, (LPVOID*)&utrue_o);
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

    // teleport
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + MaxTeleportHorizontalDistance), &teleport_hook, (LPVOID*)&teleport_o);
    MH_CreateHook(reinterpret_cast<LPVOID*>(GameAssembly + MaxTeleportDropDistance), &teleport_hook, (LPVOID*)&teleport_o);

    std::cout << "Elysium Client " + VerNum + " Initialized Successfully! \nHello Rec Room! >:^)" << std::endl;
}