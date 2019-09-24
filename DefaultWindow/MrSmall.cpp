#include "StdAfx.h"
#include "MrSmall.h"

/// Header
#include "enum.h"

/// Classes
#include "BitBmpMgr.h"
#include "ObjFactory.h"
#include "ObjMgr.h"
#include "SmallBullet.h"

CMrSmall::CMrSmall()
{
}

CMrSmall::~CMrSmall()
{
}

void CMrSmall::Initialize()
{
	m_pPlayer = CObjMgr::GetInstance()->GetObj(OBJ_PLAYER)->front();

	m_tInfo.fCX = 78.f;
	m_tInfo.fCY = 44.f;

	m_ptStartPoint.x = m_tInfo.fX;
	m_ptStartPoint.y = m_tInfo.fY;

	m_bAttack		= false;
	m_bDead			= false;
	m_bShoot		= false;
	m_bJump			= false;
	m_bLand			= false;
	m_bSkill		= false;
	m_bFall			= false;
	m_bReturn		= false;

	m_dwDirection	= 0;
	m_dwState		= 0;
	m_dwActionTime	= GetTickCount();
	m_dwAttackTime	= GetTickCount();
	m_dwFrameTime	= GetTickCount();

	m_fAngle	= 0.f;
	m_fSec		= 0.f;
	m_fVelocity = -15.f;
	m_fCacheVel	= 0.f;
	m_fSpeed	= 10.f;

	m_iCircleCount = 0;
}

int CMrSmall::Progress()
{
	SetProcess();
	SetAction();
	CollisionWithMap();
	CollisionWithBullet();
	SetScene();			
	SetSprites();		
	MoveFrame();

	if (m_bDead)
		return 1;

	return 0;
}

void CMrSmall::Render(HDC _hdc)
{
	TransparentBlt(_hdc, 
		int(m_tInfo.fX - m_tInfo.fCX * 0.5f + m_ptScroll.x),
		int(m_tInfo.fY - m_tInfo.fCY * 0.5f + m_ptScroll.y),
		int(m_tInfo.fCX),
		int(m_tInfo.fCY),
		CBitBmpMgr::GetInstance()->GetImage("MrSmall")->GetMemDC(), 
		int(m_tInfo.fCX * m_tFrame.iStart),
		int(m_tInfo.fCY * m_tFrame.iScene),
		int(m_tInfo.fCX),
		int(m_tInfo.fCY), RGB(0, 255, 80));
}

void CMrSmall::Release()
{
}

void CMrSmall::SetProcess()
{
	/// Direction Reset
	m_dwDirection	= 0;
	m_dwState		= 0;

	/// Set Direction

	m_bReturn = false;

	if (m_bReturn)
	{
		if (m_ptStartPoint.x > m_tInfo.fX)
		{
			m_dwDirection |= STAT_RIGHT;
		}

		else
		{
			m_dwDirection |= STAT_LEFT;
		}
	}

	else
	{
		if (m_pPlayer->GetInfo().fX > m_tInfo.fX)
		{
			m_dwDirection |= STAT_RIGHT;
		}

		else
		{
			m_dwDirection |= STAT_LEFT;
		}
	}

	/// SetAttack Mode
	if (abs(m_pPlayer->GetInfo().fX - m_tInfo.fX) < 400.f)
	{
		m_dwState |= STAT_ATTACK;
	}

	else
	{
		m_dwState |= STAT_STOP_ATTACK;
		m_bReturn = true;
	}

	if (m_dwAttackTime + 300 < GetTickCount())
	{
		m_dwAttackTime = GetTickCount();

		if (m_dwState & STAT_ATTACK)
			m_dwState |= STAT_SHOOT;
	}

	/// if Hp below 50% become Angry
	if (m_dwActionTime + 500 < GetTickCount())
	{
		m_dwActionTime = GetTickCount();
		m_dwState |= STAT_JUMP;
		m_bJump = true;
		++m_iCircleCount;
	}	
	
	if (m_iCircleCount > 30)
	{
		m_dwState |= STAT_USE_SKILL1;
		m_iCircleCount = 0;
	}
}

void CMrSmall::SetAction()
{
	if (m_bJump)
	{
		m_fSec += 0.2f;
		m_fCacheVel = m_fVelocity + (9.8f * m_fSec * m_fSec * 0.5f);
		m_tInfo.fY += m_fCacheVel; 
	}
	else
	{
		m_tInfo.fY += m_fVelocity + (9.8f * 2.1f * 2.1f * 0.5f);
	}		

	if (m_dwDirection & STAT_RIGHT)
	{
		m_tInfo.fX += 3.f;
	}
	else
	{
		m_tInfo.fX -= 3.f;
	}


	if (m_dwState & STAT_SHOOT)
	{
		CObj* pBullet = CObjFactory<CSmallBullet>::CreateObj(m_tInfo.fX, m_tInfo.fY)->SetMap(m_pVecMap);
		((CSmallBullet*)pBullet)->SetAngle(m_fAngle);
		if (m_dwDirection & STAT_RIGHT)
		{
			((CBullet*)pBullet)->SetDIR(RIGHT);
		}
		else
		{
			((CBullet*)pBullet)->SetDIR(LEFT);
		}
		CObjMgr::GetInstance()->GetObj(OBJ_BULLET)->push_back(pBullet);
	} 

	if (m_dwState & STAT_USE_SKILL1)
	{
		for (int i = 0; i <= 360; i += 10)
		{
			CObj* pBullet = CObjFactory<CSmallBullet>::CreateObj(m_tInfo.fX, m_tInfo.fY)->SetMap(m_pVecMap);
			((CBullet*)pBullet)->SetAngle(float(i));
			CObjMgr::GetInstance()->GetObj(OBJ_BULLET)->push_back(pBullet);
		}
	}
}


void CMrSmall::CollisionWithMap()
{
	const int MidIndex = (int(m_tInfo.fX * powf(float(TILECX), -1.f))) + ((int(m_tInfo.fY * powf(float(TILECY), -1.f))) * TILEX);

	int INDEX[6] = {
		/// Bottom
		MidIndex + TILEX,

		/// Top
		MidIndex - TILEX,

		/// Right
		MidIndex + 1,

		/// Left
		MidIndex - 1,

		/// Bottom Right
		MidIndex + TILEX + 1,

		/// Bottom Left
		MidIndex + TILEX - 1,
	};

	RECT rcObj =  {
		int(m_tInfo.fX - m_tInfo.fCX * 0.5f),
		int(m_tInfo.fY - m_tInfo.fCY * 0.5f),
		int(m_tInfo.fX + m_tInfo.fCX * 0.5f),
		int(m_tInfo.fY + m_tInfo.fCY * 0.5f) };

	int iSize = sizeof(INDEX) / sizeof(int);

	for (int i = 0; i < iSize; ++i)
	{
		if ((INDEX[i] < 0) || (UINT(INDEX[i]) >= (*m_pVecMap).size()))
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
					if (m_tInfo.fY > (*m_pVecMap)[INDEX[i]]->fY)
					{
						m_tInfo.fY += rc.bottom;
					}
					else
					{
						m_tInfo.fY -= rc.bottom;
						m_bJump = false;
						m_bLand	= true;
						m_fSec	= 0.f;
					}
				}
				else
				{
					if (m_tInfo.fX > (*m_pVecMap)[INDEX[i]]->fX)
					{
						m_tInfo.fX += rc.right;
					}
					else 
					{
						m_tInfo.fX -= rc.right;
					}
				}
			}
		}
	}
}


void CMrSmall::SetScene()
{
	m_szSetScene = NULL;

	/// Jump Motion
	if ((m_dwState & STAT_JUMP) && m_bJump)
	{
		m_szSetScene = L"MrHappiness_Jump_Ready";
	}
	else if (m_bJump)
	{
		m_szSetScene = L"MrHappiness_Jump_Ing";
	}
	else if (m_bLand)
	{
		m_szSetScene = L"MrHappiness_Jump_Ready";
		m_bLand = false;
	}
				
	if (m_bDead)
	{
		m_szSetScene = L"MrHappiness_Dead";
	}
	
}

void CMrSmall::SetSprites()
{
	if (m_szSetScene == L"MrHappiness_Jump_Ready")
	{
		m_tFrame.iScene	= 1;
		m_tFrame.iStart = 0;
		m_tFrame.iLast	= 0;
		m_tFrame.dwTime	= 0;
	}

	if (m_szSetScene == L"MrHappiness_Jump_Ing")
	{
		m_tFrame.iScene	= 4;
		m_tFrame.iStart = 0;
		m_tFrame.iLast	= 0;
		m_tFrame.dwTime	= 0;
	}
}


void CMrSmall::MoveFrame()
{
	if (!m_bDead)
		return;

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

