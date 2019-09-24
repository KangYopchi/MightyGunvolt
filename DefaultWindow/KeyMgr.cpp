#include "StdAfx.h"
#include "KeyMgr.h"

/// Header
#include "value.h"

CKeyMgr* CKeyMgr::m_pInstance = NULL;

CKeyMgr::CKeyMgr()
: m_dwKey(0)
{
}

CKeyMgr::~CKeyMgr()	
{
}

void CKeyMgr::KeyCheck()
{
	m_dwKey = 0;



	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_dwKey |= KEY_UP;

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_dwKey |= KEY_DOWN;

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_dwKey |= KEY_LEFT;

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_dwKey |= KEY_RIGHT;

	if (GetAsyncKeyState('C') & 0x8000)
		m_dwKey |= KEY_C;

	if (GetAsyncKeyState('X') & 0x8000)
		m_dwKey |= KEY_X;

	if (GetAsyncKeyState('Z') & 0x8000)
		m_dwKey |= KEY_Z;

	if (GetAsyncKeyState('E') & 0x8000)
		m_dwKey |= KEY_E;

	if (GetAsyncKeyState('R') & 0x8000)
		m_dwKey |= KEY_R;

	if (GetAsyncKeyState('Q') & 0x8000)
		m_dwKey |= KEY_Q;

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwKey |= KEY_LBUTTON;

	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		m_dwKey |= KEY_CTRL;

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		m_dwKey |= KEY_RETURN;
}