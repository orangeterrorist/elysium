#pragma once

uintptr_t base = (uintptr_t)GetModuleHandle(NULL);
uintptr_t GameAssembly = (uintptr_t)GetModuleHandle("GameAssembly.dll");

// PlayerIsInvincible
uintptr_t playerinvincible = 0xFCC870;

// RoleAllowsSpawningInventions
uintptr_t inventionspawn = 0x1A63710;

//getdamageammount
uintptr_t instakill = 0x15009D0; // int32 RecRoom.Core.Combat.Weapon.EnemyDamageSettings$$EILPKJPEAJO

//get_PlayerId
uintptr_t playerid = 0x10C9CE0; //int32 Player$$get_PlayerId


// New Offsets

uintptr_t doorv1 = 0x8FA410; // bool false RecRoom.Tools.RoomDoor$$get_IsLocked
uintptr_t doorv2 = 0x16E7410; // bool false RecRoom.Tools.RoomDoorV2$$get_IsLocked
uintptr_t arenadoor = 0x78C540; // bool false RecRoom.Activities.Arena.ArenaDoor$$get_IsLocked
uintptr_t portal = 0x8FBAC0; // bool false RecRoom.Core.InteractablePortal$$get_IsLocked
uintptr_t lockabledoor = 0x8FBAD0; // bool false LockableDoor$$get_IsLocked
uintptr_t questexitdoor = 0x8C1DE0; //bool false QuestRoomExitDoor$$get_IsLocked

uintptr_t get_IsFlyingEnabled = 0x20285E0; //bool true get_IsFlyingEnabled

uintptr_t buttonv1 = 0x2445500; //bool false 

// Rec Cheat Offsets below

uintptr_t Hysteria = 0x25D29B0; // bool true IsValid (SafeReportResult, Hysteria) get from DnSpy uhhhhhhhhhhhhhggggggg
uintptr_t RapidFire = 0x25B93D0; // bool false RecRoom.Core.Combat.RangedWeapon$$get_IsOnCooldown

uintptr_t AddFly = 0x2A5F7F0; // special RecRoom.Core.Locomotion.PlayerMovement$$AddFlyEnabled

uintptr_t RemoveFly = 0x2A6AB80; // NOP RecRoom.Core.Locomotion.PlayerMovement$$RemoveFlyEnabled

uintptr_t IsFlyEnabled = 0x2A6E640; // bool true RecRoom.Core.Locomotion.PlayerMovement$$get_IsFlyingEnabled

uintptr_t OreintationSkip = 0x1E0DAC0; // bool true RecRoom.Activities.Orientation.OrientationSubScene$$get_IsFinished

uintptr_t CanStartBroadcasting = 0x20BB250; // bool true

uintptr_t CanStopBroadcasting = 0x20BB3A0; // bool true

uintptr_t HasBroadcastingAuthorization = 0x20BB7B0; // bool true

uintptr_t get_CanRequestBroadcastingAuthorization = 0x20BAF80; // bool true

uintptr_t get_CanViewBroadcasters = 0x20BB460; // bool true

uintptr_t get_CanManageBroadcasters = 0x20BAEA0; // bool true

uintptr_t get_CanRSVP = 0x20BAF50; // bool true

uintptr_t get_CanJoinEvent = 0x20BAE10; // bool true

uintptr_t get_CanCreatePrivateEventInstance = 0x20BABB0; // bool true

uintptr_t get_CanInviteFriends = 0x20BADB0; // bool true

uintptr_t get_EventOrganizerIsLocalPlayer = 0x20BB510; // bool true

//uintptr_t CanSpawnConsumable = 0x2049150; // bool true
uintptr_t CanUseSharecam = 0x2B83B00; // bool true

uintptr_t CanUseCCT = 0x1E98D80; // bool true RecRoom.ClothingCustomizerTool$$get_LocalPlayerCanUse

uintptr_t InBounds = 0x1B6B5B0; // bool true GameBoundsManager$$get_LocalPlayerInBounds

uintptr_t RapidFire2 = 0x22F36D0; // bool false RecRoom.Core.Creation.SandboxGunHandle$$get_IsOnCooldown

uintptr_t WasMakerPenEverOut = 0x1FF2EF0; // bool false

uintptr_t LtBS = 0x1ED2130; // NOP LogoutToBootScene
uintptr_t LtBSAsync = 0x1ED2050; // NOP LogoutToBootSceneAsync

uintptr_t CheckHashesInBackground = 0x2876A70; // NOP CheckHashesInBackground_b__0

uintptr_t MaxTeleportHorizontalDistance = 0x2A6EB90; // float 9999999.9f

uintptr_t MaxTeleportDropDistance = 0x2A6EAE0; // float 9999999.9f

uintptr_t IsSendChatOnCooldown = 0x1EBC260; // bool false

uintptr_t LocalAccountIsModerator = 0x1DDBC50; // bool true

uintptr_t LocalAccountIsDeveloper = 0x1DDBA30; // bool true

uintptr_t DLPOKG = 0x216DA80; // bool true DoesLocalPlayerOwnKey

uintptr_t DLPOKL = 0x216DAC0; // bool true DoesLocalPlayerOwnKey

uintptr_t creationobjectisgrabbable = 0x1F5BF40; // bool true RecRoom.Core.Creation.CreationObject$$get_IsGrabbable "TypeSignature": "iii"

//uintptr_t get_LocalPlayerCanUseMakerPen = 0x27677872; // bool true

uintptr_t get_CanUseMakerPenRR = 0x7F4B50; // bool true

//uintptr_t get_CanUseMakerPenGR = 0x14B5ED0; // bool true

//uintptr_t get_LocalPlayerCanUseCreationMode = 0x1A65420; // bool true
//uintptr_t CanPlayerCreate = 0x1A5D970; // bool true
uintptr_t CanPlayerCreateWithMakerPen = 0x23151D0; // bool true

//uintptr_t PlayerCanUseMakerPenAccordingToRoles = 0x1A63340; // bool true
uintptr_t creationobjectisfrozen = 0x1F59F80; // bool false RecRoom.Core.Creation.CreationObject$$get_IsFrozen

uintptr_t IsDeveloper = 0x1EDAC80; // bool true

uintptr_t RemoveTrialItem = 0x1D623F0; // bool false get_IsHeldByPlayer

uintptr_t BootLocalPlayerToDormRoom = 0x1EC94A0; // NOP

uintptr_t LogWasMakerPenEverOut = 0x2A13570; // NOP

uintptr_t get_MagazineAmmunition = 0x25231D0; // Int32 - 2 DWORD

uintptr_t get_HasEnoughMagAmmo = 0x2523010; // bool true RecRoom.Core.Combat.Weapon$$get_HasEnoughMagazineAmmunition

uintptr_t GetEnemyDamage = 0x251EDA0; // int 2147483647 - 3 DWORD

uintptr_t GetPlayerAttackDamage = 0x28DDB00; // int 2147483647 - 2 DWORD

uintptr_t get_TotalAmmunition = 0x25232F0; // Int32 - 2 DWORD

uintptr_t get_CanAnyPlayerSit = 0x1EC1FF0; // bool true - 2 DWORD

uintptr_t get_RespawnDuration = 0x23BB0E0; // float 0.0 - 2 DWORD

uintptr_t snowball = 0x25B68E0; // void RecRoom.Core.Combat.RangedWeapon$$Fire "TypeSignature": "viifi"

uintptr_t foodsploit = 0x2653490; // void SpawnConsumableForLocalPlayer(string BPMABOMGJLN, long? NCOPCHMDABB, Vector3 DACBJNCMNFL, Quaternion MCFIHGJCDCP, float BDMGGOCELCK, PlayerSpawnedEquipment.JIPNAHJEKAE BHADODIAHJB)

//uintptr_t DeleteAll = 0x1A64420; // bool true RecRoom.Systems.PlayerRoles.PlayerRoomRoleManager$$get_LocalPlayerCanDeleteAll
uintptr_t get_IsOverInkLimit = 0x1FF2920; // bool false

uintptr_t CanAffordTool = 0x1FE8D50; // bool true RecRoom.Core.Creation.CreationManager$$CanAffordTool "TypeSignature": "iiiii"
uintptr_t CanAffordTool1 = 0x1FE8D20; // bool true RecRoom.Core.Creation.CreationManager$$CanAffordTool "TypeSignature": "iiii"

uintptr_t RunJoinDormRoom = 0x1ED6FB0; // bool false SessionManager$$RunJoinDormRoom

uintptr_t CanAffordResourceCost = 0x1FE8BF0; // bool true

uintptr_t get_SelfScaleMaxSize = 0x1FE69E0; // float 99999999.f

uintptr_t get_CanChangeMovementMode = 0x2FAD600; // bool false

uintptr_t TryApplicationQuit = 0x1ED93B0; // NOP void SessionManager__TryApplicationQuit (int32_t OBHNBAGOABK, const MethodInfo* method)

uintptr_t TryApplicationQuit1 = 0x1ED9270; // NOP "Signature": "void SessionManager__TryApplicationQuit (const MethodInfo* method)"
uintptr_t FatalApplicationQuit = 0x1ECCFA0; // NOP
uintptr_t PlayerSize = 0x23B9450; // special float get_desiredavatarskeletontrackingspacescale