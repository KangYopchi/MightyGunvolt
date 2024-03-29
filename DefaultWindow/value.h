#pragma once

extern	HWND  g_hWnd;

const int WINCX = 800;
const int WINCY = 600;

const int TILEX = 2500;
const int TILEY = 76;

const int TILECX = 32;
const int TILECY = 32;

/// KEY VALUE
const DWORD		KEY_UP		= 0x00000001;
const DWORD		KEY_DOWN	= 0x00000002;
const DWORD		KEY_LEFT	= 0x00000004;
const DWORD		KEY_RIGHT	= 0x00000008;
const DWORD		KEY_C		= 0x00000010;
const DWORD		KEY_X		= 0x00000020;
const DWORD		KEY_Z		= 0x00000040;
const DWORD		KEY_RETURN	= 0x00000080;
const DWORD		KEY_LBUTTON	= 0x00000100;
const DWORD		KEY_E		= 0x00000200;	
const DWORD		KEY_R		= 0x00000400;	
const DWORD		KEY_Q		= 0x00000800;	
const DWORD		KEY_CTRL	= 0x00001000;

/// PLAYER & MONSTER STATEMENT
const DWORD		STAT_STAND			= 0x00000001;
const DWORD		STAT_RUN			= 0x00000002;
const DWORD		STAT_JUMP			= 0x00000004;
const DWORD		STAT_JUMP_ATK		= 0x00000008;	
const DWORD		STAT_ATTACK			= 0x00000010;	
const DWORD		STAT_DASH			= 0x00000020;	
const DWORD		STAT_CLIMB			= 0x00000040;	
const DWORD		STAT_CLIMB_ATK		= 0x00000080;	
const DWORD		STAT_CLIMB_FIN		= 0x00000100;	
const DWORD		STAT_DEAD			= 0x00000200;
const DWORD		STAT_CHARGEDATK		= 0x00000400;
const DWORD		STAT_CHARGEDDASH	= 0x00000800;
const DWORD		STAT_STOP_ATTACK	= 0x00001000;
const DWORD		STAT_RIGHT			= 0x00002000;
const DWORD		STAT_LEFT			= 0x00004000;
const DWORD		STAT_RETURN			= 0x00008000;
const DWORD		STAT_SHOOT			= 0x00010000;
const DWORD		STAT_HOLD			= 0x00020000;
const DWORD		STAT_USE_SKILL1		= 0x00040000;
const DWORD		STAT_USE_SKILL2		= 0x00080000;
const DWORD		STAT_USE_SKILL3		= 0x00100000;
const DWORD		STAT_USE_SKILL4		= 0x00200000;