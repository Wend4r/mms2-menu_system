/**
 * vim: set ts=4 sw=4 tw=99 noet :
 * ======================================================
 * Metamod:Source Menu System
 * Written by komashchenko & Wend4r (Borys Komashchenko & Vladimir Ezhikov).
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

#ifndef _INCLUDE_METAMOD_SOURCE_MENU_SYSTEM_PROVIDER_HPP_
#	define _INCLUDE_METAMOD_SOURCE_MENU_SYSTEM_PROVIDER_HPP_

#	pragma once

#	include <stddef.h>
#	include <stdint.h>

#	include <tier0/dbg.h>
#	include <tier0/platform.h>
#	include <tier0/utlscratchmemory.h>
#	include <tier1/utldelegateimpl.h>
#	include <tier1/utlmap.h>
#	include <entity2/entitykeyvalues.h>
#	include <variant.h>

#	include <gamedata.hpp> // GameData

#	define MENU_SYSTEM_GAMECONFIG_FOLDER_DIR "gamedata"
#	define MENU_SYSTEM_GAMECONFIG_BASEENTITY_FILENAME "baseentity.games.*"
#	define MENU_SYSTEM_GAMECONFIG_GAMERESOURCE_FILENAME "gameresource.games.*"
#	define MENU_SYSTEM_GAMECONFIG_GAMESYSTEM_FILENAME "gamesystem.games.*"
#	define MENU_SYSTEM_GAMECONFIG_SOURCE2SERVER_FILENAME "source2server.games.*"

class CBaseGameSystemFactory;
class CGameEventManager;
namespace MenuSystem
{
	class Provider : public IGameData
	{
	public:
		Provider();

	public:
		bool Init(GameData::CBufferStringVector &vecMessages);
		bool Load(const char *pszBaseDir, const char *pszPathID, GameData::CBufferStringVector &vecMessages);
		bool Destroy(GameData::CBufferStringVector &vecMessages);

	protected:
		CUtlSymbolLarge GetSymbol(const char *pszText);
		CUtlSymbolLarge FindSymbol(const char *pszText) const;

	public: // IGameData
		const DynLibUtils::CModule *FindLibrary(const char *pszName) const;

	protected:
		bool LoadGameData(const char *pszBaseDir, const char *pszPathID, GameData::CBufferStringVector &vecMessages);

	public:
		class GameDataStorage
		{
		public:
			bool Load(IGameData *pRoot, const char *pszBaseConfigDir, const char *pszPathID, GameData::CBufferStringVector &vecMessages);

		protected:
			bool LoadBaseEntity(IGameData *pRoot, KeyValues3 *pGameConfig, GameData::CBufferStringVector &vecMessages);
			bool LoadGameResource(IGameData *pRoot, KeyValues3 *pGameConfig, GameData::CBufferStringVector &vecMessages);
			bool LoadGameSystem(IGameData *pRoot, KeyValues3 *pGameConfig, GameData::CBufferStringVector &vecMessages);
			bool LoadSource2Server(IGameData *pRoot, KeyValues3 *pGameConfig, GameData::CBufferStringVector &vecMessages);

		public:
			class CBaseEntity
			{
			public:
				CBaseEntity();

			public:
				bool Load(IGameData *pRoot, KeyValues3 *pGameConfig, GameData::CBufferStringVector &vecMessages);
				void Reset();

			public:
				void AcceptInput(CEntityInstance *pInstance, const char *pInputName, CEntityInstance *pActivator, CEntityInstance *pCaller, variant_t *pValue, int nOutputID) const;
				void Teleport(CEntityInstance *pInstance, const Vector &vecPosition = {}, const QAngle &angRotation = {}, const Vector &velocity = {}) const;

			private:
				GameData::Config::Addresses::ListenerCallbacksCollector m_aAddressCallbacks;
				GameData::Config::Offsets::ListenerCallbacksCollector m_aOffsetCallbacks;
				GameData::Config m_aGameConfig;

			private: // Addresses.
				using AcceptInput_t = void (CEntityInstance *, const char *, CEntityInstance *, CEntityInstance *, variant_t *, int);

				AcceptInput_t *m_pAcceptInputMethod = nullptr;
				ptrdiff_t m_nTeleportOffset;
			}; // CBaseEntity

			class CGameResource
			{
			public:
				CGameResource();

			public:
				bool Load(IGameData *pRoot, KeyValues3 *pGameConfig, GameData::CBufferStringVector &vecMessages);
				void Reset();

			public:
				ptrdiff_t GetEntitySystemOffset() const;

			private:
				GameData::Config::Offsets::ListenerCallbacksCollector m_aOffsetCallbacks;
				GameData::Config m_aGameConfig;

			private: // Offsets.
				ptrdiff_t m_nEntitySystemOffset = -1;
			}; // CGameResource

			class CGameSystem
			{
			public:
				CGameSystem();

			public:
				bool Load(IGameData *pRoot, KeyValues3 *pGameConfig, GameData::CBufferStringVector &vecMessages);
				void Reset();

			public:
				CBaseGameSystemFactory **GetFirstointer() const;

			private:
				GameData::Config::Addresses::ListenerCallbacksCollector m_aAddressCallbacks;
				GameData::Config m_aGameConfig;

			private: // Addresses.
				CBaseGameSystemFactory **m_ppFirst = nullptr;
			}; // CGameSystem

			class CSource2Server
			{
			public:
				CSource2Server();

			public:
				bool Load(IGameData *pRoot, KeyValues3 *pGameConfig, GameData::CBufferStringVector &vecMessages);
				void Reset();

			public:
				CGameEventManager **GetGameEventManagerPointer() const;

			private:
				GameData::Config::Addresses::ListenerCallbacksCollector m_aAddressCallbacks;
				GameData::Config m_aGameConfig;

			private: // Addresses.
				CGameEventManager **m_ppGameEventManager = nullptr;
			}; // CSource2Server

			const CBaseEntity &GetBaseEntity() const;
			const CGameResource &GetGameResource() const;
			const CGameSystem &GetGameSystem() const;
			const CSource2Server &GetSource2Server() const;

		private:
			CBaseEntity m_aBaseEntity;
			CGameResource m_aGameResource;
			CGameSystem m_aGameSystem;
			CSource2Server m_aSource2Server;
		}; // GameDataStorage

		const GameDataStorage &GetGameDataStorage() const;

	private:
		CUtlSymbolTableLarge_CI m_aSymbolTable;
		CUtlMap<CUtlSymbolLarge, DynLibUtils::CModule *> m_mapLibraries;

	private:
		GameDataStorage m_aStorage;

	private:
		DynLibUtils::CModule m_aEngine2Library, 
		                     m_aFileSystemSTDIOLibrary, 
		                     m_aServerLibrary;
	}; // Provider
}; // MenuSystem

#endif // _INCLUDE_METAMOD_SOURCE_MENU_SYSTEM_PROVIDER_HPP_
