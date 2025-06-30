#ifndef NEXUS_H
#define NEXUS_H

#include <windows.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif

#define NEXUS_API_VERSION 6

typedef enum ERenderType
{
	ERenderType_PreRender,
	ERenderType_Render,
	ERenderType_PostRender,
	ERenderType_OptionsRender
} ERenderType;

///----------------------------------------------------------------------------------------------------
/// GUI_RENDER:
/// 	Render callback signature.
///----------------------------------------------------------------------------------------------------
typedef void (*GUI_RENDER) (void);
typedef void (*GUI_ADDRENDER) (ERenderType aRenderType, GUI_RENDER aRenderCallback);
typedef void (*GUI_REMRENDER) (GUI_RENDER aRenderCallback);
typedef void (*GUI_REGISTERCLOSEONESCAPE)(const char* aWindowName, bool* aIsVisible);
typedef void (*GUI_DEREGISTERCLOSEONESCAPE)(const char* aWindowName);

typedef void (*UPDATER_REQUESTUPDATE)(signed int aSignature, const char* aUpdateURL);

typedef const char* (*PATHS_GETGAMEDIR)(void);
typedef const char* (*PATHS_GETADDONDIR)(const char* aName);
typedef const char* (*PATHS_GETCOMMONDIR)(void);

typedef enum EMHStatus
{
	MH_UNKNOWN = -1,
	MH_OK = 0,
	MH_ERROR_ALREADY_INITIALIZED,
	MH_ERROR_NOT_INITIALIZED,
	MH_ERROR_ALREADY_CREATED,
	MH_ERROR_NOT_CREATED,
	MH_ERROR_ENABLED,
	MH_ERROR_DISABLED,
	MH_ERROR_NOT_EXECUTABLE,
	MH_ERROR_UNSUPPORTED_FUNCTION,
	MH_ERROR_MEMORY_ALLOC,
	MH_ERROR_MEMORY_PROTECT,
	MH_ERROR_MODULE_NOT_FOUND,
	MH_ERROR_FUNCTION_NOT_FOUND
} EMHStatus;

typedef EMHStatus(__stdcall* MINHOOK_CREATE)(LPVOID pTarget, LPVOID pDetour, LPVOID* ppOriginal);
typedef EMHStatus(__stdcall* MINHOOK_REMOVE)(LPVOID pTarget);
typedef EMHStatus(__stdcall* MINHOOK_ENABLE)(LPVOID pTarget);
typedef EMHStatus(__stdcall* MINHOOK_DISABLE)(LPVOID pTarget);

typedef enum ELogLevel
{
	ELogLevel_OFF = 0,
	ELogLevel_CRITICAL = 1,
	ELogLevel_WARNING = 2,
	ELogLevel_INFO = 3,
	ELogLevel_DEBUG = 4,
	ELogLevel_TRACE = 5,
	ELogLevel_ALL
} ELogLevel;

typedef void (*LOGGER_LOG2)(ELogLevel aLogLevel, const char* aChannel, const char* aStr);

typedef void (*ALERTS_NOTIFY)(const char* aMessage);

///----------------------------------------------------------------------------------------------------
/// EVENT_CONSUME:
/// 	Event consume callback signature.
/// 	aEventArgs is payload and should be known to consumer.
///----------------------------------------------------------------------------------------------------
typedef void (*EVENT_CONSUME)(void* aEventArgs);
typedef void (*EVENTS_RAISE)(const char* aIdentifier, void* aEventData);
typedef void (*EVENTS_RAISENOTIFICATION)(const char* aIdentifier);
typedef void (*EVENTS_RAISE_TARGETED)(signed int aSignature, const char* aIdentifier, void* aEventData);
typedef void (*EVENTS_RAISENOTIFICATION_TARGETED)(signed int aSignature, const char* aIdentifier);
typedef void (*EVENTS_SUBSCRIBE)(const char* aIdentifier, EVENT_CONSUME aConsumeEventCallback);

///----------------------------------------------------------------------------------------------------
/// WNDPROC_CALLBACK:
/// 	Slightly different WndProc signature.
/// 	Return 0 if your addon handled it and you don't want it to be passed to the game.
///----------------------------------------------------------------------------------------------------
typedef UINT(*WNDPROC_CALLBACK)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
typedef void (*WNDPROC_ADDREM)(WNDPROC_CALLBACK aWndProcCallback);
typedef LRESULT(*WNDPROC_SENDTOGAME)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

typedef struct Keybind
{
	unsigned short	Key;
	bool			Alt;
	bool			Ctrl;
	bool			Shift;
} Keybind;

///----------------------------------------------------------------------------------------------------
/// KEYBINDS_PROCESS:
/// 	KeybindHandler callback signature.
///		aIsRelease will be true if the keybind is released.
///----------------------------------------------------------------------------------------------------
typedef void (*KEYBINDS_PROCESS)(const char* aIdentifier, bool aIsRelease);
typedef void (*KEYBINDS_INVOKE)(const char* aIdentifier, bool aIsRelease);
typedef void (*KEYBINDS_REGISTERWITHSTRING)(const char* aIdentifier, KEYBINDS_PROCESS aKeybindHandler, const char* aKeybind);
typedef void (*KEYBINDS_REGISTERWITHSTRUCT)(const char* aIdentifier, KEYBINDS_PROCESS aKeybindHandler, Keybind aKeybind);
typedef void (*KEYBINDS_DEREGISTER)(const char* aIdentifier);

typedef enum EGameBinds
{
	// Movement
	EGameBinds_MoveForward = 0,
	EGameBinds_MoveBackward = 1,
	EGameBinds_MoveLeft = 2,
	EGameBinds_MoveRight = 3,
	EGameBinds_MoveTurnLeft = 4,
	EGameBinds_MoveTurnRight = 5,
	EGameBinds_MoveDodge = 6,
	EGameBinds_MoveAutoRun = 7,
	EGameBinds_MoveWalk = 8,
	EGameBinds_MoveJump = 9,
	EGameBinds_MoveSwimUp = 10,
	EGameBinds_MoveSwimDown = 11,
	EGameBinds_MoveAboutFace = 12,

	// Skills
	EGameBinds_SkillWeaponSwap = 17,
	EGameBinds_SkillWeapon1 = 18,
	EGameBinds_SkillWeapon2 = 19,
	EGameBinds_SkillWeapon3 = 20,
	EGameBinds_SkillWeapon4 = 21,
	EGameBinds_SkillWeapon5 = 22,
	EGameBinds_SkillHeal = 23,
	EGameBinds_SkillUtility1 = 24,
	EGameBinds_SkillUtility2 = 25,
	EGameBinds_SkillUtility3 = 26,
	EGameBinds_SkillElite = 27,
	EGameBinds_SkillProfession1 = 28,
	EGameBinds_SkillProfession2 = 29,
	EGameBinds_SkillProfession3 = 30,
	EGameBinds_SkillProfession4 = 31,
	EGameBinds_SkillProfession5 = 79,
	EGameBinds_SkillProfession6 = 201,
	EGameBinds_SkillProfession7 = 202,
	EGameBinds_SkillSpecialAction = 82,

	// Targeting
	EGameBinds_TargetAlert = 131,
	EGameBinds_TargetCall = 32,
	EGameBinds_TargetTake = 33,
	EGameBinds_TargetCallLocal = 199,
	EGameBinds_TargetTakeLocal = 200,
	EGameBinds_TargetEnemyNearest = 34,
	EGameBinds_TargetEnemyNext = 35,
	EGameBinds_TargetEnemyPrev = 36,
	EGameBinds_TargetAllyNearest = 37,
	EGameBinds_TargetAllyNext = 38,
	EGameBinds_TargetAllyPrev = 39,
	EGameBinds_TargetLock = 40,
	EGameBinds_TargetSnapGroundTarget = 80,
	EGameBinds_TargetSnapGroundTargetToggle = 115,
	EGameBinds_TargetAutoTargetingDisable = 116,
	EGameBinds_TargetAutoTargetingToggle = 117,
	EGameBinds_TargetAllyTargetingMode = 197,
	EGameBinds_TargetAllyTargetingModeToggle = 198,

	// UI Binds
	EGameBinds_UiCommerce = 41, // TradingPost
	EGameBinds_UiContacts = 42,
	EGameBinds_UiGuild = 43,
	EGameBinds_UiHero = 44,
	EGameBinds_UiInventory = 45,
	EGameBinds_UiKennel = 46, // Pets
	EGameBinds_UiLogout = 47,
	EGameBinds_UiMail = 71,
	EGameBinds_UiOptions = 48,
	EGameBinds_UiParty = 49,
	EGameBinds_UiPvp = 73,
	EGameBinds_UiPvpBuild = 75,
	EGameBinds_UiScoreboard = 50,
	EGameBinds_UiSeasonalObjectivesShop = 209, // Wizard's Vault
	EGameBinds_UiInformation = 51,
	EGameBinds_UiChatToggle = 70,
	EGameBinds_UiChatCommand = 52,
	EGameBinds_UiChatFocus = 53,
	EGameBinds_UiChatReply = 54,
	EGameBinds_UiToggle = 55,
	EGameBinds_UiSquadBroadcastChatToggle = 85,
	EGameBinds_UiSquadBroadcastChatCommand = 83,
	EGameBinds_UiSquadBroadcastChatFocus = 84,

	// Camera
	EGameBinds_CameraFree = 13,
	EGameBinds_CameraZoomIn = 14,
	EGameBinds_CameraZoomOut = 15,
	EGameBinds_CameraReverse = 16,
	EGameBinds_CameraActionMode = 78,
	EGameBinds_CameraActionModeDisable = 114,

	// Screenshots
	EGameBinds_ScreenshotNormal = 56,
	EGameBinds_ScreenshotStereoscopic = 57,

	// Map
	EGameBinds_MapToggle = 59,
	EGameBinds_MapFocusPlayer = 60,
	EGameBinds_MapFloorDown = 61,
	EGameBinds_MapFloorUp = 62,
	EGameBinds_MapZoomIn = 63,
	EGameBinds_MapZoomOut = 64,

	// Mounts
	EGameBinds_SpumoniToggle = 152,
	EGameBinds_SpumoniMovement = 130,
	EGameBinds_SpumoniSecondaryMovement = 153,
	EGameBinds_SpumoniMAM01 = 155, // Raptor
	EGameBinds_SpumoniMAM02 = 156, // Springer
	EGameBinds_SpumoniMAM03 = 157, // Skimmer
	EGameBinds_SpumoniMAM04 = 158, // Jackal
	EGameBinds_SpumoniMAM05 = 159, // Griffon
	EGameBinds_SpumoniMAM06 = 161, // RollerBeetle
	EGameBinds_SpumoniMAM07 = 169, // Warclaw
	EGameBinds_SpumoniMAM08 = 170, // Skyscale
	EGameBinds_SpumoniMAM09 = 203, // SiegeTurtle

	// Spectator Binds
	EGameBinds_SpectatorNearestFixed = 102,
	EGameBinds_SpectatorNearestPlayer = 103,
	EGameBinds_SpectatorPlayerRed1 = 104,
	EGameBinds_SpectatorPlayerRed2 = 105,
	EGameBinds_SpectatorPlayerRed3 = 106,
	EGameBinds_SpectatorPlayerRed4 = 107,
	EGameBinds_SpectatorPlayerRed5 = 108,
	EGameBinds_SpectatorPlayerBlue1 = 109,
	EGameBinds_SpectatorPlayerBlue2 = 110,
	EGameBinds_SpectatorPlayerBlue3 = 111,
	EGameBinds_SpectatorPlayerBlue4 = 112,
	EGameBinds_SpectatorPlayerBlue5 = 113,
	EGameBinds_SpectatorFreeCamera = 120,
	EGameBinds_SpectatorFreeCameraMode = 127,
	EGameBinds_SpectatorFreeMoveForward = 121,
	EGameBinds_SpectatorFreeMoveBackward = 122,
	EGameBinds_SpectatorFreeMoveLeft = 123,
	EGameBinds_SpectatorFreeMoveRight = 124,
	EGameBinds_SpectatorFreeMoveUp = 125,
	EGameBinds_SpectatorFreeMoveDown = 126,

	// Squad Markers
	EGameBinds_SquadMarkerPlaceWorld1 = 86, // Arrow
	EGameBinds_SquadMarkerPlaceWorld2 = 87, // Circle
	EGameBinds_SquadMarkerPlaceWorld3 = 88, // Heart
	EGameBinds_SquadMarkerPlaceWorld4 = 89, // Square
	EGameBinds_SquadMarkerPlaceWorld5 = 90, // Star
	EGameBinds_SquadMarkerPlaceWorld6 = 91, // Swirl
	EGameBinds_SquadMarkerPlaceWorld7 = 92, // Triangle
	EGameBinds_SquadMarkerPlaceWorld8 = 93, // Cross
	EGameBinds_SquadMarkerClearAllWorld = 119,
	EGameBinds_SquadMarkerSetAgent1 = 94, // Arrow
	EGameBinds_SquadMarkerSetAgent2 = 95, // Circle
	EGameBinds_SquadMarkerSetAgent3 = 96, // Heart
	EGameBinds_SquadMarkerSetAgent4 = 97, // Square
	EGameBinds_SquadMarkerSetAgent5 = 98, // Star
	EGameBinds_SquadMarkerSetAgent6 = 99, // Swirl
	EGameBinds_SquadMarkerSetAgent7 = 100, // Triangle
	EGameBinds_SquadMarkerSetAgent8 = 101, // Cross
	EGameBinds_SquadMarkerClearAllAgent = 118,

	// Mastery Skills
	EGameBinds_MasteryAccess = 196,
	EGameBinds_MasteryAccess01 = 204, // Fishing
	EGameBinds_MasteryAccess02 = 205, // Skiff
	EGameBinds_MasteryAccess03 = 206, // Jade Bot Waypoint
	EGameBinds_MasteryAccess04 = 207, // Rift Scan
	EGameBinds_MasteryAccess05 = 208, // Skyscale
	EGameBinds_MasteryAccess06 = 211, // Homestead Doorway

	// Miscellaneous Binds
	EGameBinds_MiscAoELoot = 74,
	EGameBinds_MiscInteract = 65,
	EGameBinds_MiscShowEnemies = 66,
	EGameBinds_MiscShowAllies = 67,
	EGameBinds_MiscCombatStance = 68, // Stow/Draw
	EGameBinds_MiscToggleLanguage = 69,
	EGameBinds_MiscTogglePetCombat = 76,
	EGameBinds_MiscToggleFullScreen = 160,
	EGameBinds_MiscToggleDecorationMode = 210, // DecorateMode

	// Toys/Novelties
	EGameBinds_ToyUseDefault = 162,
	EGameBinds_ToyUseSlot1 = 163, // Chair
	EGameBinds_ToyUseSlot2 = 164, // Instrument
	EGameBinds_ToyUseSlot3 = 165, // Held Item
	EGameBinds_ToyUseSlot4 = 166, // Toy
	EGameBinds_ToyUseSlot5 = 167, // Tonic
	//ToyUseSlot6 unused

	// Build Templates
	EGameBinds_Loadout1 = 171,
	EGameBinds_Loadout2 = 172,
	EGameBinds_Loadout3 = 173,
	EGameBinds_Loadout4 = 174,
	EGameBinds_Loadout5 = 175,
	EGameBinds_Loadout6 = 176,
	EGameBinds_Loadout7 = 177,
	EGameBinds_Loadout8 = 178,

	// Equipment Templates
	EGameBinds_GearLoadout1 = 182,
	EGameBinds_GearLoadout2 = 183,
	EGameBinds_GearLoadout3 = 184,
	EGameBinds_GearLoadout4 = 185,
	EGameBinds_GearLoadout5 = 186,
	EGameBinds_GearLoadout6 = 187,
	EGameBinds_GearLoadout7 = 188,
	EGameBinds_GearLoadout8 = 189
} EGameBinds;

typedef void (*GAMEBINDS_PRESSASYNC)(EGameBinds aGameBind);
typedef void (*GAMEBINDS_RELEASEASYNC)(EGameBinds aGameBind);
typedef void (*GAMEBINDS_INVOKEASYNC)(EGameBinds aGameBind, int aDuration);
typedef void (*GAMEBINDS_PRESS)(EGameBinds aGameBind);
typedef void (*GAMEBINDS_RELEASE)(EGameBinds aGameBind);
typedef bool (*GAMEBINDS_ISBOUND)(EGameBinds aGameBind);

typedef void* (*DATALINK_GETRESOURCE)(const char* aIdentifier);
typedef void* (*DATALINK_SHARERESOURCE)(const char* aIdentifier, size_t aResourceSize);

typedef struct Texture
{
	unsigned Width;
	unsigned Height;
	void* Resource; // ID3D11ShaderResourceView*
} Texture;

///----------------------------------------------------------------------------------------------------
/// TEXTURES_RECEIVECALLBACK:
/// 	TextureReceiver callback signature.
///----------------------------------------------------------------------------------------------------
typedef void		(*TEXTURES_RECEIVECALLBACK)(const char* aIdentifier, Texture* aTexture);
typedef Texture* (*TEXTURES_GET)(const char* aIdentifier);
typedef Texture* (*TEXTURES_GETORCREATEFROMFILE)(const char* aIdentifier, const char* aFilename);
typedef Texture* (*TEXTURES_GETORCREATEFROMRESOURCE)(const char* aIdentifier, unsigned aResourceID, HMODULE aModule);
typedef Texture* (*TEXTURES_GETORCREATEFROMURL)(const char* aIdentifier, const char* aRemote, const char* aEndpoint);
typedef Texture* (*TEXTURES_GETORCREATEFROMMEMORY)(const char* aIdentifier, void* aData, size_t aSize);
typedef void		(*TEXTURES_LOADFROMFILE)(const char* aIdentifier, const char* aFilename, TEXTURES_RECEIVECALLBACK aCallback);
typedef void		(*TEXTURES_LOADFROMRESOURCE)(const char* aIdentifier, unsigned aResourceID, HMODULE aModule, TEXTURES_RECEIVECALLBACK aCallback);
typedef void		(*TEXTURES_LOADFROMURL)(const char* aIdentifier, const char* aRemote, const char* aEndpoint, TEXTURES_RECEIVECALLBACK aCallback);
typedef void		(*TEXTURES_LOADFROMMEMORY)(const char* aIdentifier, void* aData, size_t aSize, TEXTURES_RECEIVECALLBACK aCallback);

typedef void (*QUICKACCESS_ADDSHORTCUT)	(const char* aIdentifier, const char* aTextureIdentifier, const char* aTextureHoverIdentifier, const char* aKeybindIdentifier, const char* aTooltipText);
typedef void (*QUICKACCESS_ADDSIMPLE)	(const char* aIdentifier, GUI_RENDER aShortcutRenderCallback);
typedef void (*QUICKACCESS_ADDSIMPLE2)	(const char* aIdentifier, const char* aTargetShortcutIdentifier, GUI_RENDER aShortcutRenderCallback);
typedef void (*QUICKACCESS_GENERIC)		(const char* aIdentifier);

typedef const char* (*LOCALIZATION_TRANSLATE)(const char* aIdentifier);
typedef const char* (*LOCALIZATION_TRANSLATETO)(const char* aIdentifier, const char* aLanguageIdentifier);
typedef void (*LOCALIZATION_SET)(const char* aIdentifier, const char* aLanguageIdentifier, const char* aString);

///----------------------------------------------------------------------------------------------------
/// FONTS_RECEIVECALLBACK:
/// 	FontReceiver callback signature.
/// 	aFont = ImFont*
///----------------------------------------------------------------------------------------------------
typedef void (*FONTS_RECEIVECALLBACK)(const char* aIdentifier, void* aFont);
///----------------------------------------------------------------------------------------------------
/// FONTS_GETRELEASE:
/// 	Signature to get and release fonts.
///----------------------------------------------------------------------------------------------------
typedef void (*FONTS_GETRELEASE)(const char* aIdentifier, FONTS_RECEIVECALLBACK aCallback);
typedef void (*FONTS_ADDFROMFILE)(const char* aIdentifier, float aFontSize, const char* aFilename, FONTS_RECEIVECALLBACK aCallback, void* aConfig);
typedef void (*FONTS_ADDFROMRESOURCE)(const char* aIdentifier, float aFontSize, unsigned aResourceID, HMODULE aModule, FONTS_RECEIVECALLBACK aCallback, void* aConfig);
typedef void (*FONTS_ADDFROMMEMORY)(const char* aIdentifier, float aFontSize, void* aData, size_t aSize, FONTS_RECEIVECALLBACK aCallback, void* aConfig);
typedef void (*FONTS_RESIZE)(const char* aIdentifier, float aFontSize);

typedef struct NexusLinkData
{
	unsigned	Width;
	unsigned	Height;
	float		Scaling;

	bool		IsMoving;
	bool		IsCameraMoving;
	bool		IsGameplay;

	void* Font;		// ImFont*
	void* FontBig;	// ImFont*
	void* FontUI;		// ImFont*
} NexusLinkData;

typedef struct AddonAPI
{
	/* Renderer */
	void* SwapChain;
	void* ImguiContext;
	void* ImguiMalloc;
	void* ImguiFree;

	struct RendererVT
	{
		///----------------------------------------------------------------------------------------------------
		/// Register:
		/// 	Registers a render callback, ERenderType is either Pre, Present, Post or Options,
		/// 	callback should be of void func().
		///----------------------------------------------------------------------------------------------------
		GUI_ADDRENDER						Register;
		///----------------------------------------------------------------------------------------------------
		/// Deregister:
		/// 	Removes the registered render callback that is passed.
		///----------------------------------------------------------------------------------------------------
		GUI_REMRENDER						Deregister;
	};
	RendererVT								Renderer;

	///----------------------------------------------------------------------------------------------------
	/// RequestUpdate:
	/// 	Downloads the addon available at remote without checking its version.
	/// 	The addon already did that.
	/// 	I hope.
	///----------------------------------------------------------------------------------------------------
	UPDATER_REQUESTUPDATE					RequestUpdate;

	/* Logging */
	LOGGER_LOG2								Log;

	/* User Interface */
	struct UIVT
	{
		///----------------------------------------------------------------------------------------------------
		/// SendAlert:
		/// 	Sends a text alert to the user visible for a short amount of time.
		///----------------------------------------------------------------------------------------------------
		ALERTS_NOTIFY						SendAlert;
		///----------------------------------------------------------------------------------------------------
		/// RegisterCloseOnEscape:
		/// 	Registers a window name to get its bool toggled when escape is pressed.
		///----------------------------------------------------------------------------------------------------
		GUI_REGISTERCLOSEONESCAPE			RegisterCloseOnEscape;
		///----------------------------------------------------------------------------------------------------
		/// DeregisterCloseOnEscape:
		/// 	Deregisters a window name to listen to on escape.
		///----------------------------------------------------------------------------------------------------
		GUI_DEREGISTERCLOSEONESCAPE			DeregisterCloseOnEscape;
	};
	UIVT									UI;

	/* Paths */
	struct PathsVT
	{
		PATHS_GETGAMEDIR					GetGameDirectory;
		PATHS_GETADDONDIR					GetAddonDirectory;
		PATHS_GETCOMMONDIR					GetCommonDirectory;
	};
	PathsVT									Paths;

	/* Minhook */
	struct MinHookVT
	{
		MINHOOK_CREATE						Create;
		MINHOOK_REMOVE						Remove;
		MINHOOK_ENABLE						Enable;
		MINHOOK_DISABLE						Disable;
	};
	MinHookVT								MinHook;

	struct EventsVT
	{
		///----------------------------------------------------------------------------------------------------
		/// Raise:
		/// 	Raises an event to all subscribing addons.
		/// 	aEventData is a pointer to the data you want to share or nullptr.
		/// 	Addons are responsible for knowing how to interpret this data.
		///----------------------------------------------------------------------------------------------------
		EVENTS_RAISE						Raise;
		///----------------------------------------------------------------------------------------------------
		/// RaiseNotification:
		/// 	Raises an event without a payload.
		/// 	Synonymous with RaiseEvent("EV_FOO", nullptr);
		///----------------------------------------------------------------------------------------------------
		EVENTS_RAISENOTIFICATION			RaiseNotification;
		///----------------------------------------------------------------------------------------------------
		/// RaiseTargeted:
		/// 	Raises an event targeted at a specific subscriber.
		///----------------------------------------------------------------------------------------------------
		EVENTS_RAISE_TARGETED				RaiseTargeted;
		///----------------------------------------------------------------------------------------------------
		/// RaiseNotificationTargeted:
		/// 	Raises a notification (event without payload) targeted at a specific subscriber.
		///----------------------------------------------------------------------------------------------------
		EVENTS_RAISENOTIFICATION_TARGETED	RaiseNotificationTargeted;
		///----------------------------------------------------------------------------------------------------
		/// Subscribe:
		/// 	Registers an event callback.
		///----------------------------------------------------------------------------------------------------
		EVENTS_SUBSCRIBE					Subscribe;
		///----------------------------------------------------------------------------------------------------
		/// Unsubscribe:
		/// 	Deregisters an event callback.
		///----------------------------------------------------------------------------------------------------
		EVENTS_SUBSCRIBE					Unsubscribe;
	};
	EventsVT								Events;

	/* WndProc */
	struct WndProcVT
	{
		///----------------------------------------------------------------------------------------------------
		/// Register:
		/// 	Registers/Deregisters a WndProc callback.
		///----------------------------------------------------------------------------------------------------
		WNDPROC_ADDREM						Register;
		///----------------------------------------------------------------------------------------------------
		/// Deregister:
		/// 	Registers/Deregisters a WndProc callback.
		///----------------------------------------------------------------------------------------------------
		WNDPROC_ADDREM						Deregister;
		///----------------------------------------------------------------------------------------------------
		/// SendToGameOnly:
		/// 	Sends a WndProc to the game only and bypasses all other hooks.
		///----------------------------------------------------------------------------------------------------
		WNDPROC_SENDTOGAME					SendToGameOnly;
	};
	WndProcVT								WndProc;

	/* InputBinds */
	struct InputBindsVT
	{
		///----------------------------------------------------------------------------------------------------
		/// Invoke:
		/// 	Trigger a keybind programmatically.
		///----------------------------------------------------------------------------------------------------
		KEYBINDS_INVOKE						Invoke;
		///----------------------------------------------------------------------------------------------------
		/// RegisterWithString:
		/// 	Registers a KeybindHandler callback for a given named keybind.
		/// 	aKeybind is the default if not yet defined. Use as "ALT+CTRL+SHIFT+Q", "ALT+SHIFT+T", etc.
		///----------------------------------------------------------------------------------------------------
		KEYBINDS_REGISTERWITHSTRING			RegisterWithString;
		///----------------------------------------------------------------------------------------------------
		/// RegisterWithStruct:
		/// 	Same as KEYBINDS_REGISTERWITHSTRING except you pass a Nexus Keybind struct as a default bind.
		///----------------------------------------------------------------------------------------------------
		KEYBINDS_REGISTERWITHSTRUCT			RegisterWithStruct;
		///----------------------------------------------------------------------------------------------------
		/// Deregister:
		/// 	Deregisters a KeybindHandler callback.
		///----------------------------------------------------------------------------------------------------
		KEYBINDS_DEREGISTER					Deregister;
	};
	InputBindsVT							InputBinds;

	/* GameBinds */
	struct GameBindsVT
	{
		///----------------------------------------------------------------------------------------------------
		/// Deregister:
		/// 	Presses the keys of a given bind
		///----------------------------------------------------------------------------------------------------
		GAMEBINDS_PRESSASYNC				PressAsync;
		///----------------------------------------------------------------------------------------------------
		/// ReleaseAsync:
		/// 	Releases the keypresses of a given bind.
		///----------------------------------------------------------------------------------------------------
		GAMEBINDS_RELEASEASYNC				ReleaseAsync;
		///----------------------------------------------------------------------------------------------------
		/// InvokeAsync:
		/// 	Sends the keys of a given bind and then releases them after a given duration.
		///----------------------------------------------------------------------------------------------------
		GAMEBINDS_INVOKEASYNC				InvokeAsync;
		///----------------------------------------------------------------------------------------------------
		/// Press:
		/// 	Presses the keys of a given bind
		///----------------------------------------------------------------------------------------------------
		GAMEBINDS_PRESS						Press;
		///----------------------------------------------------------------------------------------------------
		/// Release:
		/// 	Releases the keypresses of a given bind.
		///----------------------------------------------------------------------------------------------------
		GAMEBINDS_RELEASE					Release;
		///----------------------------------------------------------------------------------------------------
		/// IsBound:
		/// 	Returns if a given game bind is set.
		///----------------------------------------------------------------------------------------------------
		GAMEBINDS_ISBOUND					IsBound;
	};
	GameBindsVT								GameBinds;

	/* DataLink */
	struct DataLinkVT
	{
		///----------------------------------------------------------------------------------------------------
		/// Get:
		/// 	Returns a pointer to the requested resource or nullptr if not existing.
		///----------------------------------------------------------------------------------------------------
		DATALINK_GETRESOURCE				Get;
		///----------------------------------------------------------------------------------------------------
		/// Share:
		/// 	Allocates a shared resource of given size and returns a pointer to it for writing.
		///----------------------------------------------------------------------------------------------------
		DATALINK_SHARERESOURCE				Share;
	};
	DataLinkVT								DataLink;

	/* Textures */
	struct TexturesVT
	{
		///----------------------------------------------------------------------------------------------------
		/// Get:
		/// 	Returns a Texture* or nullptr if it doesn't exist.
		///----------------------------------------------------------------------------------------------------
		TEXTURES_GET						Get;
		///----------------------------------------------------------------------------------------------------
		/// GetOrCreateFromFile:
		/// 	Returns a Texture* or if it doesn't exist yet creates it from file.
		///----------------------------------------------------------------------------------------------------
		TEXTURES_GETORCREATEFROMFILE		GetOrCreateFromFile;
		///----------------------------------------------------------------------------------------------------
		/// GetOrCreateFromResource:
		/// 	Returns a Texture* or if it doesn't exist yet creates it from resource.
		///----------------------------------------------------------------------------------------------------
		TEXTURES_GETORCREATEFROMRESOURCE	GetOrCreateFromResource;
		///----------------------------------------------------------------------------------------------------
		/// GetOrCreateFromURL:
		/// 	Returns a Texture* or if it doesn't exist yet creates it from URL.
		///----------------------------------------------------------------------------------------------------
		TEXTURES_GETORCREATEFROMURL			GetOrCreateFromURL;
		///----------------------------------------------------------------------------------------------------
		/// GetOrCreateFromMemory:
		/// 	Returns a Texture* or if it doesn't exist yet creates it from memory.
		///----------------------------------------------------------------------------------------------------
		TEXTURES_GETORCREATEFROMMEMORY		GetOrCreateFromMemory;
		///----------------------------------------------------------------------------------------------------
		/// LoadFromFile:
		/// 	Creates a texture from file and passes it to the TextureReceiver callback when finished.
		///----------------------------------------------------------------------------------------------------
		TEXTURES_LOADFROMFILE				LoadFromFile;
		///----------------------------------------------------------------------------------------------------
		/// LoadFromResource:
		/// 	Creates a texture from resource and passes it to the TextureReceiver callback when finished.
		///----------------------------------------------------------------------------------------------------
		TEXTURES_LOADFROMRESOURCE			LoadFromResource;
		///----------------------------------------------------------------------------------------------------
		/// LoadFromURL:
		/// 	Creates a texture from URL and passes it to the TextureReceiver callback when finished.
		///----------------------------------------------------------------------------------------------------
		TEXTURES_LOADFROMURL				LoadFromURL;
		///----------------------------------------------------------------------------------------------------
		/// LoadFromMemory:
		/// 	Creates a texture from memory and passes it to the TextureReceiver callback when finished.
		///----------------------------------------------------------------------------------------------------
		TEXTURES_LOADFROMMEMORY				LoadFromMemory;
	};
	TexturesVT								Textures;

	/* Shortcuts */
	struct QuickAccessVT
	{
		///----------------------------------------------------------------------------------------------------
		/// Add:
		/// 	Adds a shortcut icon to the QuickAccess with given texture identifiers.
		/// 	When clicked aKeybindIdentifier will be invoked.
		///----------------------------------------------------------------------------------------------------
		QUICKACCESS_ADDSHORTCUT				Add;
		///----------------------------------------------------------------------------------------------------
		/// Remove:
		/// 	Removes a shortcut with the given identifier from Quick Access.
		///----------------------------------------------------------------------------------------------------
		QUICKACCESS_GENERIC					Remove;
		///----------------------------------------------------------------------------------------------------
		/// Notify:
		/// 	Sends a notification icon to the given shortcut.
		///----------------------------------------------------------------------------------------------------
		QUICKACCESS_GENERIC					Notify;
		///----------------------------------------------------------------------------------------------------
		/// AddContextMenu:
		/// 	Appends ImGui callback when right-clicking a shortcut icon.
		///----------------------------------------------------------------------------------------------------
		QUICKACCESS_ADDSIMPLE2				AddContextMenu;
		///----------------------------------------------------------------------------------------------------
		/// RemoveContextMenu:
		/// 	Removes a simple shortcut / context item with the given identifier from Quick Access.
		///----------------------------------------------------------------------------------------------------
		QUICKACCESS_GENERIC					RemoveContextMenu;
	};
	QuickAccessVT							QuickAccess;

	/* Localization */
	struct LocalizationVT
	{
		///----------------------------------------------------------------------------------------------------
		/// Translate:
		/// 	Translates aIdentifier into current active language or returns aIdentifier if not available.
		///----------------------------------------------------------------------------------------------------
		LOCALIZATION_TRANSLATE				Translate;
		///----------------------------------------------------------------------------------------------------
		/// TranslateTo:
		/// 	Same as Translate except you can pass which language you want to translate to.
		///----------------------------------------------------------------------------------------------------
		LOCALIZATION_TRANSLATETO			TranslateTo;
		///----------------------------------------------------------------------------------------------------
		/// Set:
		/// 	Set a translated string at runtime.
		///----------------------------------------------------------------------------------------------------
		LOCALIZATION_SET					Set;
	};
	LocalizationVT							Localization;

	/* Fonts */
	struct FontsVT
	{
		///----------------------------------------------------------------------------------------------------
		/// Get:
		/// 	Requests a font to be sent to the given callback/receiver.
		///----------------------------------------------------------------------------------------------------
		FONTS_GETRELEASE					Get;
		///----------------------------------------------------------------------------------------------------
		/// Release:
		/// 	Releases a callback/receiver from a specific font.
		///----------------------------------------------------------------------------------------------------
		FONTS_GETRELEASE					Release;
		///----------------------------------------------------------------------------------------------------
		/// AddFromFile:
		/// 	Adds a font from disk and sends updates to the callback.
		///----------------------------------------------------------------------------------------------------
		FONTS_ADDFROMFILE					AddFromFile;
		///----------------------------------------------------------------------------------------------------
		/// AddFromResource:
		/// 	Adds a font from an embedded resource and sends updates to the callback.
		///----------------------------------------------------------------------------------------------------
		FONTS_ADDFROMRESOURCE				AddFromResource;
		///----------------------------------------------------------------------------------------------------
		/// AddFromMemory:
		/// 	Adds a font from memory and sends updates to the callback.
		///----------------------------------------------------------------------------------------------------
		FONTS_ADDFROMMEMORY					AddFromMemory;
		///----------------------------------------------------------------------------------------------------
		/// Resize:
		/// 	Resizes a font and sends updates to the callback.
		///----------------------------------------------------------------------------------------------------
		FONTS_RESIZE						Resize;
	};
	FontsVT									Fonts;
} AddonAPI;

typedef void (*ADDON_LOAD) (AddonAPI* aAPI);
typedef void (*ADDON_UNLOAD) (void);

typedef struct AddonVersion
{
	signed short	Major;
	signed short	Minor;
	signed short	Build;
	signed short	Revision;
} AddonVersion;

typedef enum EAddonFlags
{
	EAddonFlags_None = 0,
	EAddonFlags_IsVolatile = 1,				/* is hooking functions or doing anything else that's volatile and game build dependant */
	EAddonFlags_DisableHotloading = 2,			/* prevents unloading at runtime, aka. will require a restart if updated, etc. */
	EAddonFlags_OnlyLoadDuringGameLaunchSequence = 4	/* prevents loading the addon later than the initial character select */
} EAddonFlags;

typedef enum EUpdateProvider
{
	EUpdateProvider_None = 0,	/* Does not support auto updating */
	EUpdateProvider_Raidcore = 1,	/* Provider is Raidcore (via API) */
	EUpdateProvider_GitHub = 2,	/* Provider is GitHub Releases */
	EUpdateProvider_Direct = 3,	/* Provider is direct file link */
	EUpdateProvider_Self = 4		/* Provider is self check, addon has to request manually and version will not be verified */
} EUpdateProvider;

typedef struct AddonDefinition
{
	/* required */
	signed int      Signature;      /* Raidcore Addon ID, set to random unqiue negative integer if not on Raidcore */
	signed int		APIVersion;		/* Determines which AddonAPI struct revision the Loader will pass, use the NEXUS_API_VERSION define from Nexus.h */
	const char* Name;           /* Name of the addon as shown in the library */
	AddonVersion	Version;
	const char* Author;         /* Author of the addon */
	const char* Description;    /* Short description */
	ADDON_LOAD      Load;           /* Pointer to Load Function of the addon */
	ADDON_UNLOAD    Unload;         /* Pointer to Unload Function of the addon. Not required if EAddonFlags::DisableHotloading is set. */
	EAddonFlags     Flags;          /* Information about the addon */

	/* update fallback */
	EUpdateProvider Provider;       /* What platform is the the addon hosted on */
	const char* UpdateLink;     /* Link to the update resource */
} AddonDefinition;

#endif
