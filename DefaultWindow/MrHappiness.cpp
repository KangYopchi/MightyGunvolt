#include "StdAfx.h"
#include "MrHappiness.h"

/// Header
#include "enum.h"

/// Classes
#include "BitBmpMgr.h"
#include "EHpBar.h"
#include "MrBullet.h "
#include "MrSmall.h"
#include "ObjFactory.h"
#include "ObjMgr.h"

CMrHappiness::CMrHappiness()
{
}

CMrHappiness::~CMrHappiness()
{
}

void CMrHappiness::Initialize()
{
	m_pPlayer = CObjMgr::GetInstance()->GetObj(OBJ_PLAYER)->front();


	m_pEHpBar = CObjFactory<CEHpBar>::CreateObj();
	CObjMgr::GetInstance()->GetObj(OBJ_HPBAR)->push_back(m_pEHpBar);

	m_tInfo.fCX = 130.f;
	m_tInfo.fCY = 108.f;

	/// Temporary Coordinate  (Change later)
	m_tInfo.fX = m_pPlayer->GetInfo().fX + 500;
	m_tInfo.fY = m_pPlayer->GetInfo().fY - 100;

	m_ptStartPoint.x = m_tInfo.fX;
	m_ptStartPoint.y = m_tInfo.fY;

	m_iMaxHP			= 35;
	m_iCurrHP			= 35;

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
	m_iSummonCount = 0;
}

int CMrHappiness::Progress()
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

void CMrHappiness::Render(HDC _hdc)
{
	TransparentBlt(_hdc, 
		int(m_tInfo.fX - m_tInfo.fCX * 0.5f + m_ptScroll.x),
		int(m_tInfo.fY - m_tInfo.fCY * 0.5f + m_ptScroll.y),
		int(m_tInfo.fCX),
		int(m_tInfo.fCY),
		CBitBmpMgr::GetInstance()->GetImage("MrHappiness")->GetMemDC(), 
		int(m_tInfo.fCX * m_tFrame.iStart),
		int(m_tInfo.fCY * m_tFrame.iScene),
		int(m_tInfo.fCX),
		int(m_tInfo.fCY), RGB(0, 255, 80));
}

void CMrHappiness::Release()
{
}

void CMrHappiness::SetProcess()
{
	/// Direction Reset
	m_dwDirection	= 0;
	m_dwState		= 0;

	/// Set Direction

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
		++m_iSummonCount;
	}	
	
	if (m_iCircleCount > 30)
	{
		m_dwState |= STAT_USE_SKILL1;
		m_iCircleCount = 0;
	}

	if (m_iSummonCount > 10)
	{
		m_dwState |= STAT_USE_SKILL2;
		m_iSummonCount = 0;
	}
}

void CMrHappiness::SetAction()
{
	if (m_bJump)
	{
		m_fSec += 0.1f;
		m_fCacheVel = m_fVelocity + (9.8f * m_fSec * m_fSec * 0.5f);

		m_tInfo.fY += m_fCacheVel; 
	}
	else
	{
		m_tInfo.fY += m_fVelocity + (9.8f * 2.4f * 2.4f * 0.5f);
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
		CObj* pBullet = CObjFactory<CMrBullet>::CreateObj(m_tInfo.fX, m_tInfo.fY)->SetMap(m_pVecMap);
		((CBullet*)pBullet)->SetAngle(m_fAngle);
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
			CObj* pBullet = CObjFactory<CMrBullet>::CreateObj(m_tInfo.fX, m_tInfo.fY)->SetMap(m_pVecMap);
			((CBullet*)pBullet)->SetAngle(float(i));
			CObjMgr::GetInstance()->GetObj(OBJ_BULLET)->push_back(pBullet);
		}
	}

	if (m_dwState & STAT_USE_SKILL2)
	{
		CObjMgr::GetInstance()->GetObj(OBJ_ENEMY)->push_back(CObjFactory<CMrSmall>::CreateObj(m_tInfo.fX + 100, m_tInfo.fY - 10)->SetMap(m_pVecMap) ); 
	}
}


void CMrHappiness::CollisionWithMap()
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

void CMrHappiness::CollisionWithBullet()
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
				
				((CEHpBar*)m_pEHpBar)->SetDamage(1);
				
				--m_iCurrHP;

				if (m_iCurrHP <= 0)
					m_bDead = true;
			}
		}

	}
}



void CMrHappiness::SetScene()
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

void CMrHappiness::SetSprites()
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
		m_tFrame.iScene	= 1;
		m_tFrame.iStart = 1;
		m_tFrame.iLast	= 0;
		m_tFrame.dwTime	= 0;
	}

	if (m_szSetScene == L"Mrhappiness_Skill_Use")
	{
		m_tFrame.iScene	= 2;
		m_tFrame.iStart = 1;
		m_tFrame.iLast	= 0;
		m_tFrame.dwTime	= 0;
	}
}


void CMrHappiness::MoveFrame()
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

	