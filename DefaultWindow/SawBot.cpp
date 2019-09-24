#include "StdAfx.h"
#include "SawBot.h"

/// Header
#include "enum.h"

/// Classes
#include "BitBmpMgr.h"
#include "ObjMgr.h"


CSawBot::CSawBot()
{
}

CSawBot::~CSawBot()
{
}

void CSawBot::Initialize()
{
	m_pPlayer = CObjMgr::GetInstance()->GetObj(OBJ_PLAYER)->front();

	m_tInfo.fCX = 130.f;
	m_tInfo.fCY = 126.f;

	m_tInfo.fX = m_pPlayer->GetInfo().fX + 500;
	m_tInfo.fY = m_pPlayer->GetInfo().fY - 200;

	m_ptStartPoint.x = m_tInfo.fX;
	m_ptStartPoint.y = m_tInfo.fY;

	m_bAttack		= false;
	m_bDead			= false;

	m_dwDirection	= m_dwCacheDir = STAT_LEFT;
	
	m_dwState		= 0;
	m_dwFrameTime	= GetTickCount();
	m_dwActionTime	= GetTickCount();

	m_iChargeCount  = 0;
	m_iDashCount	= 0;
	m_iMeleeCount	= 0;

	m_fAngle	= 0.f;
	m_fSec		= 0.f;
	m_fVelocity = 10.f;
	m_fCacheVel	= 0.f;
	m_fSpeed	= 2.f;
}

int CSawBot::Progress()
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

void CSawBot::Render(HDC _hdc)
{
	TransparentBlt(_hdc, 
		int(m_tInfo.fX - m_tInfo.fCX * 0.5f + m_ptScroll.x),
		int(m_tInfo.fY - m_tInfo.fCY * 0.5f + m_ptScroll.y),
		int(m_tInfo.fCX),
		int(m_tInfo.fCY),
		CBitBmpMgr::GetInstance()->GetImage("SawBot")->GetMemDC(), 
		int(m_tInfo.fCX * m_tFrame.iStart),
		int(m_tInfo.fCY * m_tFrame.iScene),
		int(m_tInfo.fCX),
		int(m_tInfo.fCY), RGB(0, 255, 80));
}

void CSawBot::Release()
{
}

void CSawBot::SetProcess()
{
	if (m_bAttack)
		return;

	m_dwDirection = 0;
	m_dwState	  = 0;

	if (abs(m_tInfo.fX - m_pPlayer->GetInfo().fX) < 190)
	{
		m_dwState |= STAT_ATTACK;
	}
	else
	{
		m_dwState |= STAT_STOP_ATTACK;
	}

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
	else if (m_dwState & STAT_STOP_ATTACK)
	{
		if (abs(m_tInfo.fX - m_ptStartPoint.x) > 250)
		{
			if (m_tInfo.fX < m_ptStartPoint.x)
			{
				m_dwDirection |= STAT_RIGHT;
			}
			else
			{
				m_dwDirection |= STAT_LEFT;
			}
		}
	}

	if (m_dwState & STAT_ATTACK)
	{
		if (abs(m_tInfo.fX - m_pPlayer->GetInfo().fX) < 160)
		{
			m_dwState |= STAT_HOLD;
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
}

void CSawBot::SetAction()
{	
	m_tInfo.fY += -m_fVelocity + (9.8f * 2.4f * 2.4f * 0.5f);
	
	if (m_dwState & STAT_HOLD) 
	{
		m_bAttack = true;		
	}
	else
	{
		if (m_dwState & STAT_ATTACK)
		{
			if (m_dwDirection & STAT_RIGHT)
			{
				m_tInfo.fX += m_fSpeed;
			}
			else
			{
				m_tInfo.fX -= m_fSpeed;
			}
		}
		else if (m_dwState & STAT_STOP_ATTACK)
		{
			if (m_dwDirection & STAT_RIGHT)
			{
				m_tInfo.fX += m_fSpeed * 2.f;
			}
			else
			{
				m_tInfo.fX -= m_fSpeed * 2.f;
			}
		}
	}	

	if (m_bAttack)
	{
		if (m_iChargeCount > 15)
		{
			m_dwState |= STAT_USE_SKILL1;

			if (m_iDashCount < 8)
			{
				if (m_dwDirection & STAT_RIGHT)
				{
					m_tInfo.fX += m_fVelocity * 1.7f;
				}
				else
				{
					m_tInfo.fX -= m_fVelocity * 1.7f;
				}
			}
			else
			{
				m_iDashCount = 0;
				m_iChargeCount = 0;
				m_bAttack	 = false;
			}
			++m_iDashCount;
		}
		++ m_iChargeCount;
		
	}
}

void CSawBot::CollisionWithMap()
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


void CSawBot::SetScene()
{
	if (m_dwState & STAT_STOP_ATTACK)
	{
		if (m_dwDirection & STAT_RIGHT)
		{
			m_szSetScene = L"SawBot_Walk_Right";
		}
		else if (m_dwDirection & STAT_LEFT)
		{
			m_szSetScene = L"SawBot_Walk_Left";
		}
	}
	else if (m_dwState & STAT_ATTACK)
	{
		if (m_dwDirection & STAT_RIGHT)
		{
			m_szSetScene = L"SawBot_Dash_Right";
		}
		else if (m_dwDirection & STAT_LEFT)
		{
			m_szSetScene = L"SawBot_Dash_Left";
		}
	}

	if (m_dwState & STAT_USE_SKILL1)
	{
		if (m_dwDirection & STAT_RIGHT)
		{
			m_szSetScene = L"SawBot_ATK_Right";
		}
		else if (m_dwDirection & STAT_LEFT)
		{
			m_szSetScene = L"SawBot_ATK_Left";
		}
	}
}

void CSawBot::SetSprites()
{
	if (m_szSetScene == L"SawBot_Walk_Right")
	{
		if (m_tFrame.iScene != 2)
			m_tFrame.iStart = 0;

		m_tFrame.iScene	= 2;
		m_tFrame.iLast	= 3;
		m_tFrame.dwTime	= 80;
	}

	else if (m_szSetScene == L"SawBot_Walk_Left")
	{
		if (m_tFrame.iScene != 3)
			m_tFrame.iStart = 0;

		m_tFrame.iScene	= 3;
		m_tFrame.iLast	= 3;
		m_tFrame.dwTime	= 80;
	}

	else if (m_szSetScene == L"SawBot_Dash_Right")
	{
		if (m_tFrame.iScene != 4)
			m_tFrame.iStart = 0;

		m_tFrame.iScene	= 4;
		m_tFrame.iLast	= 1;
		m_tFrame.dwTime	= 80;
	}

	else if (m_szSetScene == L"SawBot_Dash_Left")
	{
		if (m_tFrame.iScene != 5)
			m_tFrame.iStart = 0;

		m_tFrame.iScene	= 5;
		m_tFrame.iLast	= 1;
		m_tFrame.dwTime	= 80;
	}

	else if (m_szSetScene == L"SawBot_ATK_Right")
	{
		if (m_tFrame.iScene != 6)
			m_tFrame.iStart = 0;

		m_tFrame.iScene	= 6;
		m_tFrame.iLast	= 2;
		m_tFrame.dwTime	= 130;
	}

	else if (m_szSetScene == L"SawBot_ATK_Left")
	{
		if (m_tFrame.iScene != 7)
			m_tFrame.iStart = 0;

		m_tFrame.iScene	= 7;
		m_tFrame.iLast	= 2;
		m_tFrame.dwTime	= 130;
	}
}