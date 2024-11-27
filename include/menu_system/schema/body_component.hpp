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

#ifndef _INCLUDE_METAMOD_SOURCE_MENU_SYSTEM_SCHEMA_BODY_COMPONENT_HPP_
#	define _INCLUDE_METAMOD_SOURCE_MENU_SYSTEM_SCHEMA_BODY_COMPONENT_HPP_

#	pragma once

#	include <menu_system/schema.hpp>

#	include <tier0/dbg.h>
#	include <tier0/platform.h>

#	define CBODYCOMPONENT_CLASS_NAME "CBodyComponent"

class QAngle;
class Vector;
class CBodyComponent;
class CGameSceneNode;

namespace MenuSystem
{
	namespace Schema
	{
		class CBodyComponent_Helper
		{
		public:
			CBodyComponent_Helper(CSchemaSystem_Helper *pSchemaSystemHelper);

		public:
			void Clear();

		public:
			FORCEINLINE CGameSceneNode **GetSceneNode(CBodyComponent *pInstance);

		private:
			CSchemaSystem_Helper::CClass *m_pClass;
			CSchemaSystem_Helper::CClass::Fields::ListenerCallbacksCollector m_aClassFieldsClassbacks;

			struct
			{
				int m_nSceneNode = INVALID_SCHEMA_FIELD_OFFSET;
			} m_aOffsets;
		}; // MenuSystem::Schema::CBodyComponent_Helper
	}; // MenuSystem::Schema
}; // MenuSystem

FORCEINLINE CGameSceneNode **MenuSystem::Schema::CBodyComponent_Helper::GetSceneNode(CBodyComponent *pInstance)
{
	Assert(m_aOffsets.m_nSceneNode != INVALID_SCHEMA_FIELD_OFFSET);

	return reinterpret_cast<CGameSceneNode **>(reinterpret_cast<uintp>(pInstance) + m_aOffsets.m_nSceneNode);
}

#endif // _INCLUDE_METAMOD_SOURCE_MENU_SYSTEM_SCHEMA_BODY_COMPONENT_HPP_
