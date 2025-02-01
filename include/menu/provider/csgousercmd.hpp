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

#ifndef _INCLUDE_METAMOD_SOURCE_MENU_PROVIDER_CSGOUSERCMD_HPP_
#	define _INCLUDE_METAMOD_SOURCE_MENU_PROVIDER_CSGOUSERCMD_HPP_

#	pragma once

#	include <basetypes.h>

#	include <cs_usercmd.pb.h>

class bf_write;

enum InputBitMask_t : int64
{
	IN_NONE = 0,
	IN_ALL = (~0),

	IN_ATTACK = (1 << 0),
	IN_JUMP = (1 << 1),
	IN_DUCK = (1 << 2),
	IN_FORWARD = (1 << 3),
	IN_BACK = (1 << 4),
	IN_USE = (1 << 5),
	IN_TURNLEFT = (1 << 7),
	IN_TURNRIGHT = (1 << 8),
	IN_MOVELEFT = (1 << 9),
	IN_MOVERIGHT = (1 << 10),
	IN_ATTACK2 = (1 << 11),
	IN_RELOAD = (1 << 13),
	IN_SPEED = (1 << 16),
	IN_JOYAUTOSPRINT = (1 << 17),

	IN_FIRST_MOD_SPECIFIC_BIT = (1ll << 32),
	IN_USEORRELOAD = (1ll << 32),
	IN_SCORE = (1ll << 33),
	IN_ZOOM = (1ll << 34),
	IN_LOOK_AT_WEAPON = (1ll << 35),
}; // InputBitMask_t

struct CInButtonState
{
public:
	// virtual CInButtonState *GetPrev();

public:
	uint64 m_nValue = 0;
	uint64 m_nValueChanged = 0;
	uint64 m_nValueScroll = 0;
};

class CUserCmdBase
{
public:
	virtual ~CUserCmdBase() = default;

public:
	virtual void unk1() {};
	virtual void unk2() {};
	virtual void unk3() {};
	virtual int &GetCmdNum() { return m_cmdNum; };
	virtual void unk5() {};
	virtual void unk6() {};
	virtual void unk7() {};
	virtual void unk8() {};
	virtual bool DeltaDecode(bf_write &sPacket, CUserCmdBase *pPrev, void *&pMarginController, double margin) { return false; };

public:
	int m_cmdNum;
};

template<class T>
class CUserCmdBaseHost : public CUserCmdBase, public T
{
public:
	// ...
};

class CCSGOUserCmd : public CUserCmdBaseHost<CSGOUserCmdPB>
{
public:
	CInButtonState m_ButtonStates;

private:
	// Not part of the player message.
	uint32 unknown[4];

public:
	CCSGOUserCmd *m_pPrev;
	CCSGOUserCmd *m_pNext;
};

#endif // _INCLUDE_METAMOD_SOURCE_MENU_PROVIDER_CSGOUSERCMD_HPP_
