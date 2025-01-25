/**
 * vim: set ts=4 sw=4 tw=99 noet :
 * ======================================================
 * Metamod:Source Menu System
 * Written by Wend4r & komashchenko (Vladimir Ezhikov & Borys Komashchenko).
 * ======================================================

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef _INCLUDE_METAMOD_SOURCE_MENUSYSTEM_PLUGIN_HPP_
#	define _INCLUDE_METAMOD_SOURCE_MENUSYSTEM_PLUGIN_HPP_

#	pragma once

#	include "imenuhandler.hpp"
#	include "imenusystem.hpp"
#	include "ientitymgr.hpp"
#	include "menu.hpp"
#	include "menuallocator.hpp"
#	include "menu/chatsystem.hpp"
#	include "menu/gameeventmanager2system.hpp"
#	include "menu/pathresolver.hpp"
#	include "menu/profilesystem.hpp"
#	include "menu/provider.hpp"
#	include "menu/schema.hpp"
#	include "menu/schema/baseentity.hpp"
#	include "menu/schema/basemodelentity.hpp"
#	include "menu/schema/baseplayercontroller.hpp"
#	include "menu/schema/baseplayerpawn.hpp"
#	include "menu/schema/baseviewmodel.hpp"
#	include "menu/schema/bodycomponent.hpp"
#	include "menu/schema/csplayer_viewmodelservices.hpp"
#	include "menu/schema/csplayerbase_cameraservices.hpp"
#	include "menu/schema/csplayerpawnbase.hpp"
#	include "menu/schema/gamescenenode.hpp"
#	include "menu/schema/pointworldtext.hpp"
#	include "concat.hpp"

#	include <logger.hpp>
#	include <translations.hpp>

#	include <ISmmPlugin.h>

#	include <bitvec.h>
#	include <const.h>
#	include <igameevents.h>
#	include <igamesystem.h>
#	include <igamesystemfactory.h>
#	include <iloopmode.h>
#	include <iserver.h>
#	include <netmessages.h>
#	include <playerslot.h>
#	include <tier0/bufferstring.h>
#	include <tier0/strtools.h>
#	include <tier1/convar.h>
#	include <tier1/utlvector.h>
#	include <tier1/utlmap.h>
#	include <tier1/keyvalues3.h>
#	include <entity2/entitykeyvalues.h>
#	include <network_connection.pb.h>

#	define MENUSYSTEM_LOGGINING_COLOR {127, 255, 0, 191} // Green (Chartreuse)

#	define MENUSYSTEM_GAME_BASE_DIR "addons" CORRECT_PATH_SEPARATOR_S META_PLUGIN_PREFIX
#	define MENUSYSTEM_GAME_EVENTS_FILES "resource" CORRECT_PATH_SEPARATOR_S "*.gameevents"
#	define MENUSYSTEM_GAME_TRANSLATIONS_FILENAMES "*.phrases.*"
#	define MENUSYSTEM_GAME_TRANSLATIONS_FILES "translations" CORRECT_PATH_SEPARATOR_S MENUSYSTEM_GAME_TRANSLATIONS_FILENAMES
#	define MENUSYSTEM_GAME_TRANSLATIONS_COUNTRY_CODES_DIRS "translations" CORRECT_PATH_SEPARATOR_S "*"
#	define MENUSYSTEM_GAME_LANGUAGES_FILES "configs" CORRECT_PATH_SEPARATOR_S "languages.*"
#	define MENUSYSTEM_BASE_PATHID "GAME"

#	define MENUSYSTEM_CLIENT_CVAR_NAME_LANGUAGE "cl_language"

class INetworkMessageInternal;

namespace Menu
{
	class CProfile; // See <menu/profile.hpp>.
};

class MenuSystem_Plugin final : public ISmmPlugin, public IMetamodListener, public IMenuSystem, public IMenuHandler, public CBaseGameSystem, public IEntityManager::IProviderAgent::ISpawnGroupNotifications, // Interfaces.
                                virtual public Menu::Schema::CSystem, virtual public Menu::Schema::CBaseEntity_Helper, virtual public Menu::Schema::CBaseModelEntity_Helper, virtual public Menu::Schema::CBasePlayerController_Helper, virtual public Menu::Schema::CBaseViewModel_Helper, virtual public Menu::Schema::CBodyComponent_Helper, virtual public Menu::Schema::CCSPlayer_ViewModelServices_Helper, virtual public Menu::Schema::CCSPlayerBase_CameraServices_Helper, virtual public Menu::Schema::CCSPlayerPawnBase_Helper, virtual public Menu::Schema::CGameSceneNode_Helper, virtual public Menu::Schema::CPointWorldText_Helper, // Schema helpers.
                                virtual public Logger, public Translations, public Menu::CPathResolver, public Menu::CProvider, // Components.
                                public Menu::CGameEventManager2System, public Menu::CChatSystem, public Menu::CProfileSystem // Subsystems.
{
public:
	using This = MenuSystem_Plugin;

	MenuSystem_Plugin();

public: // ISmmPlugin
	bool Load(PluginId id, ISmmAPI *ismm, char *error = nullptr, size_t maxlen = 0, bool late = true) override;
	bool Unload(char *error, size_t maxlen) override;
	bool Pause(char *error, size_t maxlen) override;
	bool Unpause(char *error, size_t maxlen) override;
	void AllPluginsLoaded() override;

	const char *GetAuthor() override;
	const char *GetName() override;
	const char *GetDescription() override;
	const char *GetURL() override;
	const char *GetLicense() override;
	const char *GetVersion() override;
	const char *GetDate() override;
	const char *GetLogTag() override;

public: // IMetamodListener
	void *OnMetamodQuery(const char *iface, int *ret) override;

public: // IMenuSystem
	CGameEntitySystem **GetGameEntitySystemPointer() const override;
	CBaseGameSystemFactory **GetFirstGameSystemPointer() const override;
	CGameSystemEventDispatcher **GetGameSystemEventDispatcherPointer() const override;
	IGameEventManager2 **GetGameEventManagerPointer() const override;

	class CLanguage : public IMenuSystem::ILanguage
	{
		friend class MenuSystem_Plugin;

	public:
		CLanguage(const CUtlSymbolLarge &sInitName = NULL, const char *pszInitCountryCode = "en");

	public:
		const char *GetName() const override;
		const char *GetCountryCode() const override;

	protected:
		void SetName(const CUtlSymbolLarge &sInitName);
		void SetCountryCode(const char *psz);

	private:
		CUtlSymbolLarge m_sName;
		CUtlString m_sCountryCode;
	}; // MenuPlugin::CLanguage

	class CPlayer : public IPlayer
	{
		friend class MenuSystem_Plugin;

	public:
		CPlayer();

	public: // ISample::IPlayerLanguageCallbacks
		bool AddLanguageListener(IPlayerLanguageListener *pListener) override;
		bool RemoveLanguageListener(IPlayerLanguageListener *pListener) override;

	public: // ISample::IPlayerLanguage
		const ILanguage *GetLanguage() const override;
		void SetLanguage(const ILanguage *pData) override;

	public: // ISample::IPlayerBase
		bool IsConnected() override;
		CServerSideClient *GetServerSideClient() override;

	public: // IMenuSystem::IPlayer
		CUtlVector<MenuData_t> &GetMenus() override;

	public:
		virtual void OnConnected(CServerSideClient *pClient);
		virtual void OnDisconnected(CServerSideClient *pClient, ENetworkDisconnectionReason eReason);

	public:
		virtual void OnLanguageChanged(CPlayerSlot aSlot, CLanguage *pData);

	public: // Menu callbacks.
		virtual bool OnMenuDisplayItem(IMenu *pMenu, CPlayerSlot aSlot, IMenu::ItemPosition_t iItem, IMenu::Item_t &aData);

	public:
		struct TranslatedPhrase_t
		{
			const CPhrase::CFormat *m_pFormat = nullptr;
			const CPhrase::CContent *m_pContent = nullptr;

			bool IsValid() const
			{
				return m_pFormat && m_pContent;
			}
		};

		void TranslatePhrases(const Translations *pTranslations, const CLanguage &aServerLanguage, CUtlVector<CUtlString> &vecMessages);

		const TranslatedPhrase_t &GetYourArgumentPhrase() const
		{
			return m_aYourArgumentPhrase;
		}

		const TranslatedPhrase_t &GetBackItemPhrase() const
		{
			return m_aBackItemPhrase;
		}

		const TranslatedPhrase_t &GetNextItemPhrase() const
		{
			return m_aNextItemPhrase;
		}

		const TranslatedPhrase_t &GetExitItemPhrase() const
		{
			return m_aExitItemPhrase;
		}

	private:
		CServerSideClient *m_pServerSideClient;

	private: // Menus data.
		CUtlVector<MenuData_t> m_vecMenus;

	private:
		const ILanguage *m_pLanguage;
		CUtlVector<IPlayerLanguageListener *> m_vecLanguageCallbacks;

	private:
		TranslatedPhrase_t m_aYourArgumentPhrase;
		TranslatedPhrase_t m_aBackItemPhrase;
		TranslatedPhrase_t m_aNextItemPhrase;
		TranslatedPhrase_t m_aExitItemPhrase;
	}; // MenuPlugin::CPlayerBase

	const ILanguage *GetServerLanguage() const override
	{
		return &m_aServerLanguage;
	}

	const ILanguage *GetLanguageByName(const char *psz) const override;
	IPlayerBase *GetPlayerBase(const CPlayerSlot &aSlot) override;
	IPlayer *GetPlayer(const CPlayerSlot &aSlot) override;
	CPlayer &GetPlayerData(const CPlayerSlot &aSlot);

	IMenuProfileSystem *GetProfiles() override;
	IMenu *CreateMenu(IMenuProfile *pProfile, IMenuHandler *pHandler = nullptr) override;
	bool DisplayMenuToPlayer(IMenu *pMenu, CPlayerSlot aSlot, IMenu::ItemPosition_t iStartItem = MENU_FIRST_ITEM_INDEX, int nManyTimes = MENU_TIME_FOREVER) override;
	bool CloseMenu(IMenu *pMenu) override;

	CMenu *CreateInternalMenu(IMenuProfile *pProfile, IMenuHandler *pHandler = nullptr);
	bool DisplayInternalMenuToPlayer(CMenu *pInternalMenu, CPlayerSlot aSlot, IMenu::ItemPosition_t iStartItem = MENU_FIRST_ITEM_INDEX, int nManyTimes = MENU_TIME_FOREVER);
	IMenuHandler *FindMenuHandler(IMenu *pMenu);
	int DestroyMenuEntities(IMenu *pMenu);
	bool CloseMenuHandler(IMenu *pMenu);

public: // IMenuHandler
	void OnMenuStart(IMenu *pMenu) override;
	void OnMenuDisplay(IMenu *pMenu, CPlayerSlot aSlot) override;
	void OnMenuSelect(IMenu *pMenu, CPlayerSlot aSlot, IMenu::ItemPosition_t iItem) override;
	void OnMenuEnd(IMenu *pMenu, EndReason_t eReason) override;
	void OnMenuDestroy(IMenu *pMenu) override;
	void OnMenuDrawTitle(IMenu *pMenu, CPlayerSlot aSlot, IMenu::Title_t &aTitle) override;
	void OnMenuDisplayItem(IMenu *pMenu, CPlayerSlot aSlot, IMenu::ItemPosition_t iItem, IMenu::Item_t &aData) override;

public: // CBaseGameSystem
	bool Init() override;
	void PostInit() override;
	void Shutdown() override;

	GS_EVENT(GameActivate);
	GS_EVENT(GameDeactivate);
	GS_EVENT(ServerPreEntityThink);
	GS_EVENT(GameFrameBoundary);

public: // IEntityManager::IProviderAgent::ISpawnGroupNotifications
	void OnSpawnGroupAllocated(SpawnGroupHandle_t hSpawnGroup, ISpawnGroup *pSpawnGroup) override;
	void OnSpawnGroupInit(SpawnGroupHandle_t hSpawnGroup, IEntityResourceManifest *pManifest, IEntityPrecacheConfiguration *pConfig, ISpawnGroupPrerequisiteRegistry *pRegistry) override;
	void OnSpawnGroupCreateLoading(SpawnGroupHandle_t hSpawnGroup, CMapSpawnGroup *pMapSpawnGroup, bool bSynchronouslySpawnEntities, bool bConfirmResourcesLoaded, CUtlVector<const CEntityKeyValues *> &vecKeyValues) override;
	void OnSpawnGroupDestroyed(SpawnGroupHandle_t hSpawnGroup) override;

// Schema helpers.
public: // Schema.
	bool InitSchema(char *error = nullptr, size_t maxlen = 0);
	bool LoadSchema(char *error = nullptr, size_t maxlen = 0);
	bool ClearSchema(char *error = nullptr, size_t maxlen = 0);

// Components.
public: // Path resolver.
	bool InitPathResolver(char *error = nullptr, size_t maxlen = 0);
	bool ClearPathResolver(char *error = nullptr, size_t maxlen = 0);

private:
	std::string m_sBaseGameDirectory = MENUSYSTEM_GAME_BASE_DIR;

public: // Utils.
	bool InitProvider(char *error = nullptr, size_t maxlen = 0);
	bool LoadProvider(char *error = nullptr, size_t maxlen = 0);
	bool UnloadProvider(char *error = nullptr, size_t maxlen = 0);

public: // Profiles.
	bool LoadProfiles(char *error = nullptr, size_t maxlen = 0);
	bool ClearProfiles(char *error = nullptr, size_t maxlen = 0);

public: // Entity Manager.
	bool InitEntityManager(char *error = nullptr, size_t maxlen = 0);
	void DumpEntityManager(const CConcatLineString &aConcat, CBufferString &sOutput);
	bool UnloadEntityManager(char *error = nullptr, size_t maxlen = 0);

	bool LoadSpawnGroups(char *error = nullptr, size_t maxlen = 0);
	bool UnloadSpawnGroups(char *error = nullptr, size_t maxlen = 0);

	// Entity keyvalues.
	enum MenuEntityKeyValuesFlags_t : uint8
	{
		MENU_EKV_NONE_FLAGS = 0,
		MENU_EKV_FLAG_DONT_DRAW_BACKGROUND = (1 << 0),
		MENU_EKV_FLAG_IS_ACTIVE = (1 << 1),

		MENU_EKV_INACTIVE_WITHOUT_BACKGROUND = (MENU_EKV_FLAG_DONT_DRAW_BACKGROUND | MENU_EKV_FLAG_IS_ACTIVE)
	};

	void SetMenuKeyValues(CEntityKeyValues *pMenuKV, const Vector &vecOrigin, const QAngle &angRotation);
	void SetViewModelKeyValues(CEntityKeyValues *pViewModelKV, const Vector &vecOrigin, const QAngle &angRotation);

	// Get & calculate positions.
	Vector GetEntityPosition(CBaseEntity *pTarget, QAngle *pRotation = nullptr);
	void CalculateMenuEntitiesPosition(const Vector &vecOrigin, const QAngle &angRotation, const Menu::CProfile *pProfile, Vector &vecBackgroundResult, Vector &vecResult, QAngle &angResult);
	void CalculateMenuEntitiesPositionByEntity(CBaseEntity *pTarget, const Menu::CProfile *pProfile, Vector &vecBackgroundResult, Vector &vecResult, QAngle &angResult);
	void CalculateMenuEntitiesPositionByViewModel(CBaseViewModel *pTarget, const Menu::CProfile *pProfile, Vector &vecBackgroundResult, Vector &vecResult, QAngle &angResult);
	void CalculateMenuEntitiesPositionByCSPlayer(CCSPlayerPawnBase *pTarget, const Menu::CProfile *pProfile, Vector &vecBackgroundResult, Vector &vecResult, QAngle &angResult);

	// Spawn entities.
	void SpawnEntities(const CUtlVector<CEntityKeyValues *> &vecKeyValues, CUtlVector<CEntityInstance *> *pEntities = nullptr, IEntityManager::IProviderAgent::IEntityListener *pListener = nullptr);
	void SpawnMenu(CMenu *pMenu, CPlayerSlot aInitiatorSlot, const Vector &vecBackgroundOrigin, const Vector &vecOrigin, const QAngle &angRotation);
	void SpawnMenuByEntityPosition(CMenu *pMenu, CPlayerSlot aInitiatorSlot, CBaseEntity *pTarget);
	CBaseViewModel *SpawnViewModelEntity(const Vector &vecOrigin, const QAngle &angRotation, CBaseEntity *pOwner, const int nSlot);

	// Menu movement.
	void TeleportMenuInstanceToCSPlayer(CMenu *pInternalMenu, CCSPlayerPawnBase *pTarget);
	void AttachMenuInstanceToEntity(CMenu *pInternalMenu, CBaseEntity *pTarget);
	bool AttachMenuInstanceToCSPlayer(CMenu *pInternalMenu, CCSPlayerPawnBase *pTarget);

	// Setting up the entities.
	bool SettingMenuEntity(CEntityInstance *pEntity);
	bool SettingExtraPlayerViewModelEntity(CBaseViewModel *pEntity, CBaseEntity *pOwner, const int nSlot);

public: // Game Resource.
	bool RegisterGameResource(char *error = nullptr, size_t maxlen = 0);
	bool UnregisterGameResource(char *error = nullptr, size_t maxlen = 0);

public: // Game Factory.
	bool RegisterGameFactory(char *error = nullptr, size_t maxlen = 0);
	bool UnregisterGameFactory(char *error = nullptr, size_t maxlen = 0);

public: // Source 2 Server.
	bool RegisterSource2Server(char *error = nullptr, size_t maxlen = 0);
	bool UnregisterSource2Server(char *error = nullptr, size_t maxlen = 0);

public: // Network Messages.
	bool RegisterNetMessages(char *error = nullptr, size_t maxlen = 0);
	bool UnregisterNetMessages(char *error = nullptr, size_t maxlen = 0);

public: // Languages.
	bool ParseLanguages(char *error = nullptr, size_t maxlen = 0);
	bool ParseLanguages(KeyValues3 *pRoot, CUtlVector<CUtlString> &vecMessages);
	bool ClearLanguages(char *error = nullptr, size_t maxlen = 0);

public: // Translations.
	bool ParseTranslations(char *error = nullptr, size_t maxlen = 0);
	bool ParseTranslations2(char *error = nullptr, size_t maxlen = 0); // Parse country code subfolders.
	bool ClearTranslations(char *error = nullptr, size_t maxlen = 0);

// Subsystems.
public: // Chat system.
	bool LoadChat(char *error = nullptr, size_t maxlen = 0);
	bool ClearChat(char *error = nullptr, size_t maxlen = 0);

public: // Event actions.
	bool HookGameEvents(char *error = nullptr, size_t maxlen = 0);
	bool UnhookGameEvents(char *error = nullptr, size_t maxlen = 0);

private: // Commands.
	CON_COMMAND_MEMBER_F(This, "mm_" META_PLUGIN_PREFIX "_reload_schema", OnReloadSchemaCommand, "Reload schema fields of classes", FCVAR_LINKED_CONCOMMAND);
	CON_COMMAND_MEMBER_F(This, "mm_" META_PLUGIN_PREFIX "_reload_gamedata", OnReloadGameDataCommand, "Reload gamedata configs", FCVAR_LINKED_CONCOMMAND);
	CON_COMMAND_MEMBER_F(This, "mm_" META_PLUGIN_PREFIX "_reload_profiles", OnReloadProfilesCommand, "Reload menu profiles", FCVAR_LINKED_CONCOMMAND);
	CON_COMMAND_MEMBER_F(This, "menuselect", OnMenuSelectCommand, "", FCVAR_LINKED_CONCOMMAND | FCVAR_CLIENT_CAN_EXECUTE);

public: // SourceHooks.
	void OnStartupServerHook(const GameSessionConfiguration_t &config, ISource2WorldSession *pWorldSession, const char *);
	void OnDispatchConCommandHook(ConCommandHandle hCommand, const CCommandContext &aContext, const CCommand &aArgs);
	CServerSideClientBase *OnConnectClientHook(const char *pszName, ns_address *pAddr, void *pNetInfo, C2S_CONNECT_Message *pConnectMsg, const char *pszChallenge, const byte *pAuthTicket, int nAuthTicketLength, bool bIsLowViolence);
	bool OnProcessRespondCvarValueHook(const CCLCMsg_RespondCvarValue_t &aMessage);
	void OnDisconectClientHook(ENetworkDisconnectionReason eReason);

public: // Utils.
	void SendCvarValueQuery(IRecipientFilter *pFilter, const char *pszName, int iCookie);
	void SendChatMessage(IRecipientFilter *pFilter, int iEntityIndex, bool bIsChat, const char *pszChatMessageFormat, const char *pszParam1 = "", const char *pszParam2 = "", const char *pszParam3 = "", const char *pszParam4 = "");
	void SendTextMessage(IRecipientFilter *pFilter, int iDestination, size_t nParamCount, const char *pszParam, ...);
	// void SendVGUIMenuPluginMessage(IRecipientFilter *pFilter, const char *pszName = nullptr, const bool *pIsShow = nullptr, KeyValues3 *pKeys = nullptr);

protected: // Handlers.
	void OnStartupServer(CNetworkGameServerBase *pNetServer, const GameSessionConfiguration_t &config, ISource2WorldSession *pWorldSession);
	void OnConnectClient(CNetworkGameServerBase *pNetServer, CServerSideClientBase *pClient, const char *pszName, ns_address *pAddr, void *pNetInfo, C2S_CONNECT_Message *pConnectMsg, const char *pszChallenge, const byte *pAuthTicket, int nAuthTicketLength, bool bIsLowViolence);
	bool OnProcessRespondCvarValue(CServerSideClientBase *pClient, const CCLCMsg_RespondCvarValue_t &aMessage);
	void OnDisconectClient(CServerSideClientBase *pClient, ENetworkDisconnectionReason eReason);

protected: // ConVar symbols.
	CUtlSymbolLarge GetConVarSymbol(const char *pszName);
	CUtlSymbolLarge FindConVarSymbol(const char *pszName) const;

private: // ConVar (hash)map.
	CUtlSymbolTableLarge_CI m_tableConVars;
	CUtlMap<CUtlSymbolLarge, int> m_mapConVarCookies;

protected: // Language symbols.
	CUtlSymbolLarge GetLanguageSymbol(const char *pszName);
	CUtlSymbolLarge FindLanguageSymbol(const char *pszName) const;

private: // Language (hash)map.
	CUtlSymbolTableLarge_CI m_tableLanguages;
	CUtlMap<CUtlSymbolLarge, CLanguage> m_mapLanguages;

private: // Fields.
	CGameSystemStaticFactory<This> *m_pFactory = NULL;

	// Provide to Entity Manager plugin.
	IEntityManager *m_pEntityManager = nullptr;
	IEntityManager::IProviderAgent *m_pEntityManagerProviderAgent = nullptr;
	IEntityManager::CSpawnGroupProvider *m_pEntityManagerSpawnGroupProvider = nullptr;

	// Run-time things.
	IEntityManager::IProviderAgent::ISpawnGroupInstance *m_pMySpawnGroupInstance = nullptr;

	INetworkMessageInternal *m_pGetCvarValueMessage = NULL;
	INetworkMessageInternal *m_pSayText2Message = NULL;
	INetworkMessageInternal *m_pTextMsgMessage = NULL;
	// INetworkMessageInternal *m_pVGUIMenuMessage = NULL;

	CLanguage m_aServerLanguage;
	CUtlVector<CLanguage> m_vecLanguages;

	CPlayer m_aPlayers[ABSOLUTE_PLAYER_LIMIT]; // Per slot.

	IMenu::Item_t m_aBackControlItem;
	IMenu::Item_t m_aNextControlItem;
	IMenu::Item_t m_aExitControlItem;
	CMenuData_t::ControlItems_t m_aControls;

	CMenuAllocator<sizeof(CMenu) * ABSOLUTE_PLAYER_LIMIT> m_MenuAllocator;
	CUtlMap<const IMenu *, IMenuHandler *> m_mapMenuHandlers;
}; // MenuSystem_Plugin

extern MenuSystem_Plugin *g_pMenuPlugin;

PLUGIN_GLOBALVARS();

#endif //_INCLUDE_METAMOD_SOURCE_MENUSYSTEM_PLUGIN_HPP_
