#include "StdAfx.h"
#include "EHpBar.h"

/// Class
#include "BitBmpMgr.h"

CEHpBar::CEHpBar()
: m_iOnePoint(12)
{
}

CEHpBar::~CEHpBar()
{
}

void CEHpBar::Initialize()
{
	m_tInfo.fCX = 384.f;
	m_tInfo.fCY = 29.f;

	m_tInfo.fX = WINCX - m_tInfo.fCX * 1.5f;
	m_tInfo.fY = WINCY - m_tInfo.fCY * 1.2f;

	m_iHp	  = 32;
	m_iDamage = 0;

	m_dwFrameTime = GetTickCount();
}

int CEHpBar::Progress()
{
	if (m_iDamage)
	{
		if (m_dwFrameTime + 50 < GetTickCount())
		{
			m_dwFrameTime = GetTickCount();
			--m_iHp;
			--m_iDamage;
		}
	}
	return 0;
}

void CEHpBar::Render(HDC _hdc)
{
	TransparentBlt(_hdc, 
		int(m_tInfo.fX + m_tInfo.fCX * 0.5f + m_ptScroll.x * 0.0001),
		int(m_tInfo.fY + m_tInfo.fCY * 0.5f + m_ptScroll.y * 0.0001),
		int(m_iHp * m_iOnePoint),
		int(m_tInfo.fCY),
		CBitBmpMgr::GetInstance()->GetImage("EHpBar")->GetMemDC(), 
		int(0),
		int(0),
		int(m_iHp * m_iOnePoint),
		int(m_tInfo.fCY), RGB(0, 255, 80));
}

void CEHpBar::Release()
{
}
