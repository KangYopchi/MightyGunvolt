#include "StdAfx.h"
#include "PongBullet.h"

/// Class
#include "BitBmpMgr.h"

CPongBullet::CPongBullet()
{
}

CPongBullet::~CPongBullet()
{
}

void CPongBullet::Initialize()
{
	m_iType		= 1;

	m_tInfo.fCY = 34.f;
	m_tInfo.fCX = 36.f;

	m_fSec = 0.f;
	m_fSpeed = 2.f;

	m_dwFrameTime = GetTickCount();
	
	m_tFrame.iScene = 0;
	m_tFrame.iStart = 0;
	m_tFrame.iLast	= 3;
	m_tFrame.dwTime = 40;
}

int CPongBullet::Progress()
{
	if ((m_fAngle == 0) || (m_fAngle == 180))
	{
		m_fSec += 0.1f;
		m_tInfo.fX += ( m_fSpeed *  cosf(m_fAngle * DTR) * m_fSec );
		m_tInfo.fY += ( m_fSpeed * -sinf(m_fAngle * DTR) * m_fSec ) + ( 0.098f * m_fSec * m_fSec * 0.5f );
	}
	else if ((m_fAngle == 45) || (m_fAngle == 135))
	{
		m_fSec += 0.1f;
		m_tInfo.fX += ( m_fSpeed *  cosf(m_fAngle * DTR) * m_fSec );
		m_tInfo.fY += ( m_fSpeed * -sinf(m_fAngle * DTR) * m_fSec ) + ( 0.98f * m_fSec * m_fSec * 0.5f );
	}

	else
	{
		m_fSec += 0.1f;
		m_tInfo.fX += ( m_fSpeed *  cosf(m_fAngle * DTR) * m_fSec );
		m_tInfo.fY += ( m_fSpeed * 1.5f * -sinf(m_fAngle * DTR) * m_fSec ) + ( 0.98f * m_fSec * m_fSec * 0.5f );
	}

	int iResult = CollisionWithMap();

	MoveFrame();
	
	return iResult;
}

void CPongBullet::Render(HDC _hdc)
{
	TransparentBlt(_hdc, 
		int(m_tInfo.fX - m_tInfo.fCX * 0.5f + m_ptScroll.x),
		int(m_tInfo.fY - m_tInfo.fCY * 0.5f + m_ptScroll.y),
		int(m_tInfo.fCX),
		int(m_tInfo.fCY),
		CBitBmpMgr::GetInstance()->GetImage("PongBullet")->GetMemDC(), 
		int(m_tInfo.fCX * m_tFrame.iStart),
		0,
		int(m_tInfo.fCX),
		int(m_tInfo.fCY), RGB(0, 255, 80));
}

void CPongBullet::Release()
{
}