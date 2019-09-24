#include "StdAfx.h"
#include "Enemy.h"

/// Headers
#include "value.h"
#include "enum.h"

/// Classes
#include "Bullet.h"
#include "ObjMgr.h"

CEnemy::CEnemy()
: PI(3.141593f)
, DTR(3.141593f * 0.005556f)
, RTD(180.f * 0.318310f)
, m_iHitCount(0)
{
	memset(&m_tFrame, NULL, sizeof(FRAME));
}

CEnemy::~CEnemy()
{
}

void CEnemy::CollisionWithMap()
{
	int iMidIndex = (int(m_tInfo.fX * powf(float(TILECX), -1.f))) + ((int(m_tInfo.fY * powf(float(TILECY), -1.f))) * TILEX);

	int INDEX[4] = { iMidIndex - 1, iMidIndex + 1, iMidIndex - TILEX, iMidIndex + TILEX };

	RECT rcObj =  {
		int(m_tInfo.fX - m_tInfo.fCX * 0.5f),
		int(m_tInfo.fY - m_tInfo.fCY * 0.5f),
		int(m_tInfo.fX + m_tInfo.fCX * 0.5f),
		int(m_tInfo.fY + m_tInfo.fCY * 0.5f) };

	for (int i = 0; i < 4; ++i)
	{
		if (INDEX[i] < 0)
			continue;

		if (UINT(INDEX[i]) > (*m_pVecMap).size())
			continue;

		if ((*m_pVecMap)[INDEX[i]]->iOption == TILE_BLOCK)
		{
			RECT rcMap = {
				int((*m_pVecMap)[INDEX[i]]->fX - (*m_pVecMap)[INDEX[i]]->fCX * 0.5f),
				int((*m_pVecMap)[INDEX[i]]->fY - (*m_pVecMap)[INDEX[i]]->fCY * 0.5f),
				int((*m_pVecMap)[INDEX[i]]->fX + (*m_pVecMap)[INDEX[i]]->fCX * 0.5f),
				int((*m_pVecMap)[INDEX[i]]->fY + (*m_pVecMap)[INDEX[i]]->fCY * 0.5f) };

			RECT rc = { };
			
			if (IntersectRect(&rc, &rcObj, &rcMap))
			{
				SetRect(&rc, 0, 0, rc.right - rc.left, rc.bottom - rc.top);

				if (rc.right > rc.bottom)
				{
					if (m_tInfo.fY < (*m_pVecMap)[INDEX[i]]->fY)
					{
						m_tInfo.fY -= rc.bottom;
						m_bJump = false;
						m_fSec = 0;
					}
				}
				else
				{
					if (m_tInfo.fX < (*m_pVecMap)[INDEX[i]]->fX)
					{
						m_tInfo.fX -= rc.right;
					}
					else 
					{
						m_tInfo.fX += rc.right;
					}
				}
			}
		}
	}
}

void CEnemy::CollisionWithBullet()
{
	vector<CObj*>* m_pVecBullet = CObjMgr::GetInstance()->GetObj(OBJ_BULLET);

	int iSize = (*m_pVecBullet).size();

	RECT rcObj =  {
		int(m_tInfo.fX - m_tInfo.fCX * 0.5f),
		int(m_tInfo.fY - m_tInfo.fCY * 0.5f),
		int(m_tInfo.fX + m_tInfo.fCX * 0.5f),
		int(m_tInfo.fY + m_tInfo.fCY * 0.5f) };

	for (int i = 0; i < iSize; ++i)
	{
		if((*m_pVecBullet)[i] == NULL)
			continue;

		if (((CBullet*)((*m_pVecBullet)[i]))->GetType() == 0)
		{
			RECT rcBullet = {
			int((*m_pVecBullet)[i]->GetInfo().fX - (*m_pVecBullet)[i]->GetInfo().fCX * 0.5f),
			int((*m_pVecBullet)[i]->GetInfo().fY - (*m_pVecBullet)[i]->GetInfo().fCY * 0.5f),
			int((*m_pVecBullet)[i]->GetInfo().fX + (*m_pVecBullet)[i]->GetInfo().fCX * 0.5f),
			int((*m_pVecBullet)[i]->GetInfo().fY + (*m_pVecBullet)[i]->GetInfo().fCY * 0.5f) };

			RECT rc = { };

			if (IntersectRect(&rc, &rcObj, &rcBullet))
			{
				((CBullet*)(*m_pVecBullet)[i])->SetHit(1);

				++m_iHitCount;

				if (m_iHitCount > 3)
					m_bDead = true;
			}
		}

	}
}

void CEnemy::MoveFrame()
{
	if(m_dwFrameTime + m_tFrame.dwTime < GetTickCount())
	{
		m_dwFrameTime = GetTickCount();
		
		++m_tFrame.iStart;
	}

	if(m_tFrame.iStart > m_tFrame.iLast)
	{
		m_tFrame.iStart = 0;
	}
}