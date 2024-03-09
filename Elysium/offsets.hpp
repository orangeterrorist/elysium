#pragma once

uintptr_t base = (uintptr_t)GetModuleHandle(NULL);
uintptr_t GameAssembly = (uintptr_t)GetModuleHandle("GameAssembly.dll");


// Offsets Below

uintptr_t PlayerIsInvincible = 0x1D9ADD0; // bool true

uintptr_t EnemyDamageSettings = 0x1B18680; // Int32 0

uintptr_t get_PlayerId = 0x2137270; //Int32 1

// Search Term "get_IsLocked"
uintptr_t QuestRoomExitDoor_get_IsLocked = 0x1178080; //bool false
uintptr_t ArenaDoor_get_IsLocked = 0x130FAC0; // bool false
uintptr_t RoomDoorV2_get_IsLocked = 0x278A7E0; // bool false
uintptr_t RoomDoor_get_IsLocked = 0xF0CE60; // bool false
uintptr_t InteractablePortal_get_IsLocked = 0x84ACE0; // bool false
uintptr_t LockableDoor_get_IsLocked = 0x1174040; // bool false

// Search Term "get_IsOnCooldown"
uintptr_t RangedWeapon_get_IsOnCooldown = 0x1F6F220; // bool false
uintptr_t SandboxGunHandle_get_IsOnCooldown = 0x20BDC00;// bool false

uintptr_t AddFlyEnabled = 0x2668F70; // Special (true)

uintptr_t RemoveFlyEnabled = 0x2674740; // NOP

uintptr_t get_IsFlyingEnabled = 0x2678520; // bool true

//uintptr_t OreintationSkip = 0x1DAE820; // bool true RecRoom.Activities.Orientation.OrientationSubScene$$get_IsFinished

// Search Term "get_LocalPlayerCanUse"
uintptr_t CCT_get_LocalPlayerCanUse = 0x23C84F0; // bool true

uintptr_t get_LocalPlayerInBounds = 0x1D8AD60; // bool true

uintptr_t get_WasMakerPenEverOut = 0x1A85450; // bool false

// Search Term "LogoutToBootScene"
uintptr_t LogoutToBootScene = 0x18587A0; // NOP
uintptr_t LogoutToBootSceneAsync = 0x18586A0; // NOP

// Search Term "CheckHashesInBackground"
	//uintptr_t CheckHashesInBackground1 = 0x0; // NOP
	//uintptr_t CheckHashesInBackground2 = 0x0; // NOP
	//uintptr_t CheckHashesInBackground3 = 0x0; // bool false
	//uintptr_t CheckHashesInBackground4 = 0x0; // NOP

uintptr_t get_MaxTeleportHorizontalDistance = 0x2678A90; // float big

uintptr_t get_MaxTeleportDropDistance = 0x26789E0; // float big

uintptr_t get_IsSendChatOnCooldown = 0x2578670; // bool false

uintptr_t get_LocalAccountIsModerator = 0x175FD70; // bool true

uintptr_t get_LocalAccountIsDeveloper = 0x175FBA0; // bool true
uintptr_t get_IsDeveloper = 0x185CA10; // bool true

// Search Term "DoesLocalPlayerOwnKey"
uintptr_t DoesLocalPlayerOwnKey1 = 0x23DB0C0; // bool true
uintptr_t DoesLocalPlayerOwnKey2 = 0x23DAF40; // bool true 
	//uintptr_t DoesLocalPlayerOwnKey3 = 0x1E313F0; // bool true

// Search Term "get_IsGrabbable"
uintptr_t CreationObject_get_IsGrabbable = 0x1A907B0; // bool true
uintptr_t OMCreationObject_get_IsGrabbable = 0x261E8E0; // bool true
uintptr_t Polaroid_get_IsGrabbable = 0x20706E0; // bool true

uintptr_t get_CanUseMakerPen = 0xE96100; // bool true

uintptr_t CanPlayerCreateWithMakerPen = 0x170E9C0; // bool true

// Search Term "get_IsFrozen"
uintptr_t Chip_get_IsFrozen = 0x7D63B0; // bool false
uintptr_t CreationObject_get_IsFrozen = 0x1A90700; // bool false
uintptr_t Tool_get_IsFrozen = 0x1DC3680; // bool false

	//uintptr_t LocalRemoveTrialItems = 0x0; // bool false

uintptr_t GoToDorm = 0x2007700; // NOP

uintptr_t LogWasMakerPenEverOut = 0x2AA31B0; // NOP

uintptr_t get_MagazineAmmunition = 0x239D700; // Int32 big

uintptr_t get_HasEnoughMagazineAmmunition = 0x239D520; // bool true

// Search Term "GetEnemyDamage"
uintptr_t Weapon_GetEnemyDamage = 0x2398C30; // Int32 big
uintptr_t WeaponMelee_GetEnemyDamageMelee = 0x26582B0; // Int32 big

uintptr_t GetPlayerAttackDamage = 0x21D40B0; // Int32 big

uintptr_t get_TotalAmmunition = 0x239D820; // Int32

uintptr_t get_CanAnyPlayerSit = 0x184A6C0; // bool true

// Search Term "get_RespawnDuration"
uintptr_t Player_get_RespawnDuration = 0x1D9FE50; // float 0
uintptr_t GameCombatManager_get_RespawnDuration = 0x21376E0; //float 0

uintptr_t get_IsOverInkLimit = 0x1A84E90; // bool false

// Search Term "CanAffordTool"
uintptr_t CanAffordTool1 = 0x1A78EB0; // bool true
uintptr_t CanAffordTool2 = 0x1A78EE0; // bool true

// Search Term "RunJoinDormRoom"
	//uintptr_t RunJoinDormRoom1 = 0x2C1C770; // bool false
	//uintptr_t RunJoinDormRoom2 = 0x2C1C920; // NOP
	//uintptr_t RunJoinDormRoom3 = 0x2C20910; // NOP

uintptr_t CanAffordResourceCost = 0x1A78D80; // bool true

uintptr_t get_SelfScaleMaxSize = 0x1A76D10; // float 99999999.f

uintptr_t get_CanChangeMovementMode = 0x2011140; // bool false

// Search Term "TryApplicationQuit"
uintptr_t TryApplicationQuit = 0x185B1A0; // NOP (int32_t OBHNBAGOABK, const MethodInfo* method)
uintptr_t TryApplicationQuit1 = 0x185B370; // NOP (const MethodInfo* method)

uintptr_t FatalApplicationQuit = 0x1854590; // NOP

uintptr_t get_DesiredAvatarSkeletonTrackingSpaceScale = 0x2135C40; // Special (float)

// Search Term "RecRoom.Core.Combat.RangedWeapon.Fire" (vector3, float = 1f)
uintptr_t RangedWeapon_Fire = 0x1F6C2F0; // Special (void)

uintptr_t SpawnConsumableForLocalPlayer = 0x24F62C0; // Special (void)

// Search Term "CanUseMakerPenInternal"
	//uintptr_t CanUseMakerPenInternal1 = 0x0; // bool true
	//uintptr_t CanUsemakerPenInternal2 = 0x0; // bool true

uintptr_t CreatorRoleCanUseMakerPen = 0x3065FA0; // bool true

#if 0 // Old Broadcasting & Event Perms Modules, client-sided so pointless
uintptr_t CanStartBroadcasting = 0x18D1580; // bool true

uintptr_t CanStopBroadcasting = 0x18D16D0; // bool true

uintptr_t HasBroadcastingAuthorization = 0x18D1AE0; // bool true

uintptr_t get_CanRequestBroadcastingAuthorization = 0x18D12B0; // bool true

uintptr_t get_CanViewBroadcasters = 0x18D1790; // bool true

uintptr_t get_CanManageBroadcasters = 0x18D11E0; // bool true

uintptr_t get_CanRSVP = 0x18D1290; // bool true

uintptr_t get_CanJoinEvent = 0x18D1150; // bool true

uintptr_t get_CanCreatePrivateEventInstance = 0x18D0EF0; // bool true

uintptr_t get_CanInviteFriends = 0x18D10F0; // bool true

uintptr_t get_EventOrganizerIsLocalPlayer = 0x18D1840; // bool true
#endif
