#include "StdAfx.h"
#include "FireShot.h"

/// Class
#include "BitBmpMgr.h"

CFireShot::CFireShot()
{
}

CFireShot::~CFireShot()
{
}

void CFireShot::Initialize()
{
	m_iType		= 1;
	
	m_tInfo.fCX = 64.f;
	m_tInfo.fCY = 60.f;

	m_ptStartPoint.x = m_tInfo.fX;
	m_ptStartPoint.y = m_tInfo.fY;

	m_fSpeed = 5.f;

	m_dwFrameTime = GetTickCount();
	
	m_tFrame.iScene = 0;
	m_tFrame.iStart = 0;
	m_tFrame.iLast	= 1;
	m_tFrame.dwTime = 120;
}

int CFireShot::Progress()
{
	m_tInfo.fX += m_fSpeed * cosf(m_fAngle * DTR);
	m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * DTR);

	int iResult = CollisionWithMap();

	if (abs(m_tInfo.fX - m_ptStartPoint.x) > 100)
	{
		iResult = 1;
	}

	MoveFrame();
	
	return iResult;
}

void CFireShot::Render(HDC _hdc)
{
	TransparentBlt(_hdc, 
		int(m_tInfo.fX - m_tInfo.fCX * 0.5f + m_ptScroll.x),
		int(m_tInfo.fY - m_tInfo.fCY * 0.5f + m_ptScroll.y),
		int(m_tInfo.fCX),
		int(m_tInfo.fCY),
		CBitBmpMgr::GetInstance()->GetImage("FireShot")->GetMemDC(), 
		int(m_tInfo.fCX * m_tFrame.iStart),
		int(m_tInfo.fCY * m_tFrame.iScene),
		int(m_tInfo.fCX),
		int(m_tInfo.fCY), RGB(0, 255, 80));
}

void CFireShot::Release()
{
}

void CFireShot::MoveFrame()
{
	if(m_dwFrameTime + m_tFrame.dwTime < GetTickCount())
	{
		m_dwFrameTime = GetTickCount();
		
		++m_tFrame.iStart;
	}

	if(m_tFrame.iStart >= m_tFrame.iLast)
	{

		if (m_tFrame.iScene >= 2)
		{
			m_tFrame.iScene = 2;
			m_tFrame.iStart = 0;
			m_tFrame.iLast	= 3;
			return;
		}

		++m_tFrame.iScene;
		m_tFrame.iStart = 0;
	}
}