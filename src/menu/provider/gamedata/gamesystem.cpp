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

#include <menu/provider.hpp>

Menu::CProvider::CGameDataStorage::CGameSystem::CGameSystem()
{
	{
		auto &aCallbacks = m_aAddressCallbacks;

		aCallbacks.Insert(m_aGameConfig.GetSymbol("CBaseGameSystemFactory::sm_pFirst"), GAMEDATA_ADDRESS_SHARED_LAMBDA_CAPTURE(m_ppFirst));
		aCallbacks.Insert(m_aGameConfig.GetSymbol("&IGameSystem::sm_pEventDispatcher"), GAMEDATA_ADDRESS_SHARED_LAMBDA_CAPTURE(m_ppEventDispatcher));

		m_aGameConfig.GetAddresses().AddListener(&aCallbacks);
	}
}

bool Menu::CProvider::CGameDataStorage::CGameSystem::Load(IGameData *pRoot, KeyValues3 *pGameConfig, GameData::CBufferStringVector &vecMessages)
{
	return m_aGameConfig.Load(pRoot, pGameConfig, vecMessages);
}

void Menu::CProvider::CGameDataStorage::CGameSystem::Reset()
{
	m_ppFirst = nullptr;
	m_ppEventDispatcher = nullptr;
}

CBaseGameSystemFactory **Menu::CProvider::CGameDataStorage::CGameSystem::GetFirstPointer() const
{
	return m_ppFirst;
}

CGameSystemEventDispatcher **Menu::CProvider::CGameDataStorage::CGameSystem::GetEventDispatcher() const
{
	return m_ppEventDispatcher;
}
