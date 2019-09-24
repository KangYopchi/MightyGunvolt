#include "StdAfx.h"
#include "Bullet.h"

/// Header
#include "enum.h"
#include "value.h"

/// Class
#include "BitBmpMgr.h"
	
CBullet::CBullet()
: PI(3.141593f)
, DTR(3.141593f * 0.005556f)
, RTD(180.f * 0.318310f)
, m_iHit(0)
{
}

CBullet::~CBullet()
{
}

int CBullet::Progress()
{
	m_tInfo.fX += m_fSpeed;

	int iResult = CollisionWithMap();

	MoveFrame();

	if (m_iHit)
	{
		iResult = m_iHit;
	}

	return iResult;	
}

void CBullet::Release()
{
}

void CBullet::MoveFrame()
{
	if(m_dwFrameTime + m_tFrame.dwTime < GetTickCount())
	{
		m_dwFrameTime = GetTickCount();
		
		++m_tFrame.iStart;
	}

	if(m_tFrame.iStart >= m_tFrame.iLast)
	{
		m_tFrame.iStart = 0;
	}
}

int CBullet::CollisionWithMap()
{
	int iMidIndex = (int(m_tInfo.fX * powf(float(TILECX), -1.f))) + ((int(m_tInfo.fY * powf(float(TILECY), -1.f))) * TILEX);

	int INDEX[4] = { iMidIndex - 1, iMidIndex + 1, iMidIndex - TILEX, iMidIndex + TILEX };

	RECT rcMe =  {
		int(m_tInfo.fX - m_tInfo.fCX * 0.5f),
		int(m_tInfo.fY - m_tInfo.fCY * 0.5f),
		int(m_tInfo.fX + m_tInfo.fCX * 0.5f),
		int(m_tInfo.fY + m_tInfo.fCY * 0.5f) };

	for (int i = 0; i < 4; ++i)
	{
		if (INDEX[i] < 0)
			continue;

		if (UINT(INDEX[i]) >= (*m_pVecMap).size())
			continue;

		if ((*m_pVecMap)[INDEX[i]]->iOption == TILE_BLOCK)
		{
			RECT rcMap = {
				int((*m_pVecMap)[INDEX[i]]->fX - (*m_pVecMap)[INDEX[i]]->fCX * 0.5f),
				int((*m_pVecMap)[INDEX[i]]->fY - (*m_pVecMap)[INDEX[i]]->fCY * 0.5f),
				int((*m_pVecMap)[INDEX[i]]->fX + (*m_pVecMap)[INDEX[i]]->fCX * 0.5f),
				int((*m_pVecMap)[INDEX[i]]->fY + (*m_pVecMap)[INDEX[i]]->fCY * 0.5f) };

			RECT rc = { };
			
			if (IntersectRect(&rc, &rcMe, &rcMap))
			{
				SetRect(&rc, 0, 0, rc.right - rc.left, rc.bottom - rc.top);

				if (rc.right < rc.bottom)
				{
					return 1;
				}
			}
		}
	}
	return 0;
}
