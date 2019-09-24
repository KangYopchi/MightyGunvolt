#include "StdAfx.h"
#include "NormalBullet.h"

/// Class
#include "BitBmpMgr.h"

CNormalBullet::CNormalBullet()
{
}

CNormalBullet::~CNormalBullet()
{
}


void CNormalBullet::Initialize()
{
	m_iType		= 0;

	m_tInfo.fCX = 20.f;
	m_tInfo.fCY = 18.f;

	m_dwFrameTime = GetTickCount();

	m_tFrame.iScene = 0;
	m_tFrame.iStart = 0;
	m_tFrame.iLast	= 3;
	m_tFrame.dwTime = 30;

	m_fSpeed = 12.f;
}

void CNormalBullet::Render(HDC _hdc)
{
	TransparentBlt(_hdc, 
		int(m_tInfo.fX - m_tInfo.fCX * 0.5f + m_ptScroll.x),
		int(m_tInfo.fY - m_tInfo.fCY * 0.5f + m_ptScroll.y),
		int(m_tInfo.fCX),
		int(m_tInfo.fCY),
		CBitBmpMgr::GetInstance()->GetImage("NormalBullet")->GetMemDC(), 
		int(m_tInfo.fCX * m_tFrame.iStart),
		0,
		int(m_tInfo.fCX),
		int(m_tInfo.fCY), RGB(0, 255, 80));
}

void CNormalBullet::Release()
{
}