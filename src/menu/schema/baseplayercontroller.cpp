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

#include <menu/schema/baseplayercontroller.hpp>

#include <schemasystem/schemasystem.h>

Menu::Schema::CBasePlayerController_Helper::CBasePlayerController_Helper(CSystem *pSchemaSystemHelper)
{
	auto &aCallbacks = m_aClassFieldsClassbacks;

	m_pClass = pSchemaSystemHelper->GetClass(CBASEPLAYERCONTROLLER_CLASS_NAME);

	Assert(m_pClass);

	auto &aFields = m_pClass->GetFields();

	aCallbacks.Insert(m_pClass->GetFieldSymbol("m_hPawn"), {[&](const CUtlSymbolLarge &, SchemaClassFieldData_t *pField)
	{
		m_aOffsets.m_nPawn = pField->m_nSingleInheritanceOffset;
	}});

	aFields.AddListener(&aCallbacks);
}

void Menu::Schema::CBasePlayerController_Helper::Clear()
{
	m_aOffsets = {};
}
