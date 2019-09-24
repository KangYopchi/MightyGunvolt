#include "StdAfx.h"
#include "MrBullet.h"

/// Class
#include "BitBmpMgr.h"

CMrBullet::CMrBullet()
{
}

CMrBullet::~CMrBullet()
{
}

void CMrBullet::Initialize()
{
	m_iType		= 1;

	m_tInfo.fCY = 40.f;
	m_tInfo.fCX = 40.f;

	m_fAngle = 0.f;
	m_fSec	 = 0.f;
	m_fSpeed = 5.f;

	m_dwFrameTime = GetTickCount();
	
	m_tFrame.iScene = 0;
	m_tFrame.iStart = 0;
	m_tFrame.iLast	= 2;
	m_tFrame.dwTime = 40;
}

int CMrBullet::Progress()
{
	m_tInfo.fX += m_fSpeed * cosf(m_fAngle * DTR);
	m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * DTR);

	int iResult = CollisionWithMap();

	MoveFrame();
	
	return iResult;

}

void CMrBullet::Render(HDC _hdc)
{
	TransparentBlt(_hdc, 
		int(m_tInfo.fX - m_tInfo.fCX * 0.5f + m_ptScroll.x),
		int(m_tInfo.fY - m_tInfo.fCY * 0.5f + m_ptScroll.y),
		int(m_tInfo.fCX),
		int(m_tInfo.fCY),
		CBitBmpMgr::GetInstance()->GetImage("MrBullet")->GetMemDC(), 
		int(m_tInfo.fCX * m_tFrame.iStart),
		0,
		int(m_tInfo.fCX),
		int(m_tInfo.fCY), RGB(0, 255, 80));
}

void CMrBullet::Release()
{
}
