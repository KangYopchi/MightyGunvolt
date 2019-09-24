#include "StdAfx.h"
#include "HpBar.h"

/// Class
#include "BitBmpMgr.h"

CHpBar::CHpBar()
: m_iOnePoint(12)
{
}

CHpBar::~CHpBar()
{
}

void CHpBar::Initialize()
{
	m_tInfo.fCX = 384.f;
	m_tInfo.fCY = 29.f;

	m_tInfo.fX = m_tInfo.fCX * 0.5f;
	m_tInfo.fY = m_tInfo.fCY * 0.5f;

	m_iHp	  = 32;
	m_iDamage = 0;

	m_dwFrameTime = GetTickCount();
}

int CHpBar::Progress()
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

void CHpBar::Render(HDC _hdc)
{
	TransparentBlt(_hdc, 
		int(m_ptScroll.x * 0.0001),
		int(m_ptScroll.y * 0.0001),
		int(m_iHp * m_iOnePoint),
		int(m_tInfo.fCY),
		CBitBmpMgr::GetInstance()->GetImage("HpBar")->GetMemDC(), 
		int(0),
		int(0),
		int(m_iHp * m_iOnePoint),
		int(m_tInfo.fCY), RGB(0, 255, 80));
}

void CHpBar::Release()
{
}
