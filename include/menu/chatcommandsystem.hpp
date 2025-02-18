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

#ifndef _INCLUDE_METAMOD_SOURCE_MENU_CHATCOMMANDSYSTEM_HPP_
#	define _INCLUDE_METAMOD_SOURCE_MENU_CHATCOMMANDSYSTEM_HPP_

#	pragma once

#	include "systembase.hpp"

#	include <playerslot.h>
#	include <tier0/utlstring.h>
#	include <tier1/utlvector.h>

#	include <logger.hpp>

#	define MENU_CHATCOMMANDSYSTEM_LOGGINING_COLOR {0, 127, 255, 191}

namespace Menu
{
	using CChatCommandSystemBase = CSystemBase<CPlayerSlot, bool, const CUtlVector<CUtlString> &>;

	class CChatCommandSystem : public CChatCommandSystemBase
	{
	public:
		using Base = CChatCommandSystemBase;

	public:
		CChatCommandSystem();

	public:
		const char *GetName() override;
		const char *GetHandlerLowercaseName() override;

	public:
		static char GetPublicTrigger();
		static char GetSilentTrigger();

	public:
		bool Handle(const char *pszName, CPlayerSlot aSlot, bool bIsSilent, const CUtlVector<CUtlString> &vecArgs) override;
	}; // Menu::CChatCommandSystem
}; // Menu

#endif // _INCLUDE_METAMOD_SOURCE_MENU_CHATCOMMANDSYSTEM_HPP_
