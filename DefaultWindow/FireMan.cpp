#include "StdAfx.h"
#include "FireMan.h"

/// Header
#include "value.h"

/// Classes
#include "BitBmpMgr.h"
#include "FireShot.h"
#include "ObjFactory.h"
#include "ObjMgr.h"

CFireMan::CFireMan()
{
}

CFireMan::~CFireMan()
{
}


void CFireMan::Initialize()
{
	m_pPlayer = CObjMgr::GetInstance()->GetObj(OBJ_PLAYER)->front();

	m_tInfo.fCX = 64.f;
	m_tInfo.fCY = 56.f;

	m_tInfo.fX = m_pPlayer->GetInfo().fX + 500;
	m_tInfo.fY = m_pPlayer->GetInfo().fY - 200;

	m_bAttack		= false;
	m_bShoot		= false;
	m_bDead			= false;

	m_dwDirection	= m_dwCacheDir = STAT_LEFT;
	
	m_dwState		= 0;
	m_dwFrameTime	= GetTickCount();
	m_dwActionTime	= GetTickCount();

	m_fAngle	= 0.f;
	m_fVelocity	= 10.f;
}

int CFireMan::Progress()
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

void CFireMan::Render(HDC _hdc)
{
	TransparentBlt(_hdc, 
		int(m_tInfo.fX - m_tInfo.fCX * 0.5f + m_ptScroll.x),
		int(m_tInfo.fY - m_tInfo.fCY * 0.5f + m_ptScroll.y),
		int(m_tInfo.fCX),
		int(m_tInfo.fCY),
		CBitBmpMgr::GetInstance()->GetImage("FireMan")->GetMemDC(), 
		int(m_tInfo.fCX * m_tFrame.iStart),
		int(m_tInfo.fCY * m_tFrame.iScene),
		int(m_tInfo.fCX),
		int(m_tInfo.fCY), RGB(0, 255, 80));
}

void CFireMan::Release()
{
}

void CFireMan::SetProcess()
{
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

void CFireMan::SetAction()
{
	m_tInfo.fY += -m_fVelocity + (9.8f * 2.4f * 2.4f * 0.5f);

	float fX;
	float fY;
	float fDIR;


	if (m_bShoot)
	{

		if (m_dwDirection & STAT_RIGHT)
		{
			fX = m_tInfo.fX + 32.f;
			fY = m_tInfo.fY - 20.f;
			fDIR = RIGHT;
		}
		else
		{
			fX = m_tInfo.fX - 32.f;
			fY = m_tInfo.fY - 20.f;
			fDIR = LEFT;
		}

		if (abs(m_tInfo.fY - m_pPlayer->GetInfo().fY) > 50)
		{
			m_fAngle = 45.f * fDIR;
		}
		else 
		{
			m_fAngle = 0.f;
		}


		CObj* pBullet = CObjFactory<CFireShot>::CreateObj(fX, fY)->SetMap(m_pVecMap);
		((CBullet*)pBullet)->SetDIR(fDIR);
		((CBullet*)pBullet)->SetAngle(m_fAngle);
		CObjMgr::GetInstance()->GetObj(OBJ_BULLET)->push_back(pBullet);
		m_bShoot = false;
	}

	if (m_dwState & STAT_ATTACK)
	{
		if (m_bShoot)
			return;

		if (m_dwActionTime + 250 < GetTickCount())
		{
			m_dwActionTime = GetTickCount();
			m_bShoot = true;
		}
	}

}

void CFireMan::SetScene()
{
	if (m_dwState & STAT_STOP_ATTACK)
	{
		if (m_dwDirection & STAT_RIGHT)
		{
			m_szSetScene = L"FireMan_Stand_Right";
		}
		else if (m_dwDirection & STAT_LEFT)
		{
			m_szSetScene = L"FireMan_Stand_Left";
		}
	}
	else if (m_dwState & STAT_ATTACK)
	{
		if (m_dwDirection & STAT_RIGHT)
		{
			m_szSetScene = L"FireMan_ATK_Right";
		}
		else if (m_dwDirection & STAT_LEFT)
		{
			m_szSetScene = L"FireMan_ATK_Left";
		}
	}
}

void CFireMan::SetSprites()
{
	if (m_szSetScene == L"FireMan_Stand_Right")
	{
		if (m_tFrame.iScene != 0)
			m_tFrame.iStart = 0;

		m_tFrame.iScene	= 0;
		m_tFrame.iLast	= 1;
		m_tFrame.dwTime	= 250;
	}

	if (m_szSetScene == L"FireMan_Stand_Left")
	{
		if (m_tFrame.iScene != 1)
			m_tFrame.iStart = 0;

		m_tFrame.iScene	= 1;
		m_tFrame.iLast	= 1;
		m_tFrame.dwTime	= 250;
	}

	if (m_szSetScene == L"FireMan_ATK_Right")
	{
		if (m_tFrame.iScene != 2)
			m_tFrame.iStart = 0;

		m_tFrame.iScene	= 2;
		m_tFrame.iLast	= 1;
		m_tFrame.dwTime	= 250;
	}

	if (m_szSetScene == L"FireMan_ATK_Left")
	{
		if (m_tFrame.iScene != 3)
			m_tFrame.iStart = 0;

		m_tFrame.iScene	= 3;
		m_tFrame.iLast	= 1;
		m_tFrame.dwTime	= 250;
	}
}