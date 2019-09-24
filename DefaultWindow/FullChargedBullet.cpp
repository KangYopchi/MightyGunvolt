#include "StdAfx.h"
#include "FullChargedBullet.h"

///Class
#include "BitBmpMgr.h"

CFullChargedBullet::CFullChargedBullet()
{
}

CFullChargedBullet::~CFullChargedBullet()
{
}

void CFullChargedBullet::Initialize()
{
	m_iType		= 0;
	m_tInfo.fCX = 68.f;
	m_tInfo.fCY = 66.f;

	m_dwFrameTime = GetTickCount();

	m_tFrame.iStart = 0;
	m_tFrame.iLast	= 2;
	m_tFrame.dwTime = 30;

	m_fSpeed = 15.f;
}

void CFullChargedBullet::Render(HDC _hdc)
{
	TransparentBlt(_hdc, 
		int(m_tInfo.fX - m_tInfo.fCX * 0.5f + m_ptScroll.x),
		int(m_tInfo.fY - m_tInfo.fCY * 0.5f + m_ptScroll.y),
		int(m_tInfo.fCX),
		int(m_tInfo.fCY),
		CBitBmpMgr::GetInstance()->GetImage("FullChargedBullet")->GetMemDC(), 
		int(m_tInfo.fCX * m_tFrame.iStart),
		0,
		int(m_tInfo.fCX),
		int(m_tInfo.fCY), RGB(0, 255, 80));
}