#include "StdAfx.h"
#include "Brandish.h"

/// Header
#include "enum.h"

/// Classes
#include "BitBmpMgr.h"
#include "ObjMgr.h"

CBrandish::CBrandish()
{
}

CBrandish::~CBrandish()
{
}


void CBrandish::Initialize()
{
	m_pPlayer = CObjMgr::GetInstance()->GetObj(OBJ_PLAYER)->front();

	m_tInfo.fCX = 132.f;
	m_tInfo.fCY = 118.f;
 
	m_tInfo.fX  = m_pPlayer->GetInfo().fX + 100;
	m_ptStartPoint.x = m_tInfo.fX;
	m_ptStartPoint.y = m_tInfo.fY;

	m_bAttack		= false;
	m_bDead			= false;



	m_dwDirection	= m_dwCacheDir = STAT_LEFT;
	
	m_dwState		= 0;
	m_dwFrameTime	= GetTickCount();
	m_dwActionTime	= GetTickCount();
	m_dwAttackTime	= GetTickCount();

	m_fAngle		= 0.f;
	m_fSec			= 0.f;
	m_fVelocity 	= 10.f;
	m_fCacheVel		= 0.f;
	m_fSpeed		= 10.f;

	m_bStopMove			= false;
	m_iStopMoveCount	= 0;
	m_iStopMoveRelease	= 0;

	m_bSpinMove			= false;
	m_iSpinMoveCount	= 0;

	m_bTackle			= false;
	m_iTackleCount		= 0;
	m_iChargeTackle		= 0;
	
}

int CBrandish::Progress()
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

void CBrandish::Render(HDC _hdc)
{
	TransparentBlt(_hdc, 
		int(m_tInfo.fX - m_tInfo.fCX * 0.5f + m_ptScroll.x),
		int(m_tInfo.fY - m_tInfo.fCY * 0.5f + m_ptScroll.y),
		int(m_tInfo.fCX),
		int(m_tInfo.fCY),
		CBitBmpMgr::GetInstance()->GetImage("Brandish")->GetMemDC(), 
		int(m_tInfo.fCX * m_tFrame.iStart),
		int(m_tInfo.fCY * m_tFrame.iScene),
		int(m_tInfo.fCX),
		int(m_tInfo.fCY), RGB(0, 255, 80));
}

void CBrandish::Release()
{
}

void CBrandish::SetProcess()
{
	m_dwDirection = 0;
	m_dwState	  = 0;

	/// if Player's hp left, Turn STAT_ATTACK
	/// if (m_pPlayer->GetHp() >= 1)
	/// m_dwState |= STAT_ATTACK;

	m_dwState |= STAT_ATTACK;

	if (m_dwState & STAT_ATTACK)
	{
		if (m_tInfo.fX < m_pPlayer->GetInfo().fX)
		{
			m_dwDirection |= STAT_RIGHT;
		}
		else
		{
			m_dwDirection |= STAT_LEFT;
		}
	}

	/// Set Direction
	if (m_dwDirection)
	{
		m_dwCacheDir = m_dwDirection;
	}
	else
	{
		m_dwDirection = m_dwCacheDir;
	}
	
	if ((!m_bSpinMove) && (!m_bTackle))
	{
		if (m_dwActionTime + 500 < GetTickCount())
		{
			m_dwActionTime = GetTickCount();

			if (m_iChargeTackle == 4)
			{
				m_bTackle	   = true;
				m_iChargeTackle = 0; 
			}
			else if (!m_bSpinMove)
			{
				m_bSpinMove = true;
				++m_iChargeTackle;
			}

			++m_iStopMoveCount;

			if (m_iStopMoveCount > 6)
			m_bStopMove = true;

		}
		else
		{
			m_dwState |= STAT_STAND;
		}
	}
	/// if HP below 0% 
	/// m_dwState |= STAT_DEAD;
}

void CBrandish::SetAction()
{	
	m_tInfo.fY += -m_fVelocity + (9.8f * 2.4f * 2.4f * 0.5f);

	if (m_bStopMove)
	{
		++m_iStopMoveRelease;

		if (m_iStopMoveRelease > 40)
		{
			ActionReset();
		}
		return;
	}

	if (m_bSpinMove)
	{
		if (m_iSpinMoveCount > 15)
		{
			m_bSpinMove = false;
			m_iSpinMoveCount = 0;
			m_dwActionTime = GetTickCount();
		}
		else
		{
			if (m_dwDirection & STAT_RIGHT)
			{
				m_tInfo.fX += m_fSpeed * 1.2f;
			}
			else if (m_dwDirection & STAT_LEFT)
			{
				m_tInfo.fX -= m_fSpeed * 1.2f;
			}
		}
		++m_iSpinMoveCount;
	}

	if (m_bTackle)
	{
		if (m_iTackleCount > 10)
		{
			m_bTackle = false;
			m_iTackleCount = 0;
			m_dwActionTime = GetTickCount();
		}
		else
		{
			if (m_dwDirection & STAT_RIGHT)
			{
				m_tInfo.fX += m_fSpeed * 2.2f;
			}
			else if (m_dwDirection & STAT_LEFT)
			{
				m_tInfo.fX -= m_fSpeed * 2.2f;
			}
		}
		++m_iTackleCount;
	}	
}

void CBrandish::CollisionWithMap()
{
	const int MidIndex = (int(m_tInfo.fX * powf(float(TILECX), -1.f))) + ((int(m_tInfo.fY * powf(float(TILECY), -1.f))) * TILEX);

	int INDEX[12] = {
		/// Bottom
		MidIndex + (TILEX * 2),
		MidIndex + TILEX,

		/// Top
		MidIndex - (TILEX * 2),
		MidIndex - TILEX,

		/// Right
		MidIndex + 2,
		MidIndex + 1,

		/// Left
		MidIndex - 2,
		MidIndex - 1,

		/// Bottom Right
		MidIndex + TILEX + 2,
		MidIndex + TILEX + 1,

		/// Bottom Left
		MidIndex + TILEX - 2,
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


void CBrandish::SetScene()
{

	if (m_bStopMove)
	{
		if (m_dwDirection & STAT_RIGHT)
		{
			m_szSetScene = L"Stand_Right";
		}
		else if (m_dwDirection & STAT_LEFT)
		{
			m_szSetScene = L"Stand_Left";
		}
		return;
	}

	if (m_dwState & STAT_STAND)
	{
		if (m_dwDirection & STAT_RIGHT)
		{
			m_szSetScene = L"Stand_Right";
		}
		else if (m_dwDirection & STAT_LEFT)
		{
			m_szSetScene = L"Stand_Left";
		}
	}

	if (m_bSpinMove)
	{
		if (m_dwDirection & STAT_RIGHT)
		{
			m_szSetScene = L"SpinMove_Right";
		}
		else
		{
			m_szSetScene = L"SpinMove_Left";
		}
	}

	if (m_bTackle)
	{
		if (m_dwDirection & STAT_RIGHT)
		{
			m_szSetScene = L"Tackle_Right";
		}
		else
		{
			m_szSetScene = L"Tackle_Left";
		}
	}

}

void CBrandish::SetSprites()
{
	if (m_szSetScene == L"Stand_Right")
	{
		if (m_tFrame.iScene != 0)
			m_tFrame.iStart = 0;

		m_tFrame.iScene	= 0;
		m_tFrame.iLast	= 0;
		m_tFrame.dwTime	= 0;
	}

	if (m_szSetScene == L"Stand_Left")
	{
		if (m_tFrame.iScene != 1)
			m_tFrame.iStart = 0;

		m_tFrame.iScene	= 1;
		m_tFrame.iLast	= 0;
		m_tFrame.dwTime	= 0;
	}

	if (m_szSetScene == L"SpinMove_Right")
	{
		if (m_tFrame.iScene != 6)
			m_tFrame.iStart = 0;

		m_tFrame.iScene	= 6;
		m_tFrame.iLast	= 2;
		m_tFrame.dwTime	= 80;
	}

	if (m_szSetScene == L"SpinMove_Left")
	{
		if (m_tFrame.iScene != 7)
			m_tFrame.iStart = 0;

		m_tFrame.iScene	= 7;
		m_tFrame.iLast	= 2;
		m_tFrame.dwTime	= 80;
	}

	if (m_szSetScene == L"Tackle_Right")
	{
		if (m_tFrame.iScene != 4)
			m_tFrame.iStart = 0;

		m_tFrame.iScene	= 4;
		m_tFrame.iLast	= 2;
		m_tFrame.dwTime	= 80;
	}

	if (m_szSetScene == L"Tackle_Left")
	{
		if (m_tFrame.iScene != 5)
			m_tFrame.iStart = 0;

		m_tFrame.iScene	= 5;
		m_tFrame.iLast	= 2;
		m_tFrame.dwTime	= 80;
	}

}
void CBrandish::ActionReset()
{
	m_bStopMove			= false;
	m_iStopMoveCount	= 0;
	m_iStopMoveRelease	= 0;

	m_bSpinMove			= false;
	m_iSpinMoveCount	= 0;

	m_bTackle			= false;
	m_iTackleCount		= 0;
	m_iChargeTackle		= 0;
}