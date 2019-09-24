#include "StdAfx.h"
#include "Pong.h"

/// Header
#include "value.h"

/// Classes
#include "BitBmpMgr.h"
#include "ObjFactory.h"
#include "ObjMgr.h"
#include "PongBullet.h"

CPong::CPong()
{
}

CPong::~CPong()
{
}

void CPong::Initialize()
{
	m_pPlayer = CObjMgr::GetInstance()->GetObj(OBJ_PLAYER)->front();

	m_tInfo.fCX = 68.f;
	m_tInfo.fCY = 68.f;

	m_tInfo.fX = m_pPlayer->GetInfo().fX + 500;
	m_tInfo.fY = m_pPlayer->GetInfo().fY - 200;

	m_bAttack	= false;
	m_bShoot	= false;
	m_bDead		= false;
	
	m_dwDirection = 0;
	m_dwState	  = 0;
	m_dwFrameTime  = GetTickCount();
	m_dwActionTime = GetTickCount();

	m_fAngle	= 0.f;
}

int CPong::Progress()
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

void CPong::Render(HDC _hdc)
{

	TransparentBlt(_hdc, 
		int(m_tInfo.fX - m_tInfo.fCX * 0.5f + m_ptScroll.x),
		int(m_tInfo.fY - m_tInfo.fCY * 0.5f + m_ptScroll.y),
		int(m_tInfo.fCX),
		int(m_tInfo.fCY),
		CBitBmpMgr::GetInstance()->GetImage("Pong")->GetMemDC(), 
		int(m_tInfo.fCX * m_tFrame.iStart),
		int(m_tInfo.fCY * m_tFrame.iScene),
		int(m_tInfo.fCX),
		int(m_tInfo.fCY), RGB(0, 255, 80));
}

void CPong::Release()
{
}

void CPong::SetProcess()
{
	m_dwDirection = 0;


	/// Set Direction
	if (m_pPlayer->GetInfo().fX < m_tInfo.fX)
		m_dwDirection |= KEY_LEFT;

	else
		m_dwDirection |= STAT_RIGHT;


	/// Set AttackMode
	if (abs(m_pPlayer->GetInfo().fX - m_tInfo.fX) <=  270.f)
		m_bAttack = true;

	else
		m_bAttack = false;


	if (m_bAttack)
	{
		if (m_dwActionTime + 500 < GetTickCount())
		{
			m_dwActionTime = GetTickCount();
			m_bShoot = true;
		}
	}
	else if(m_dwActionTime)
	{
		m_bShoot = false;
		m_dwActionTime = GetTickCount();
	}
}

void CPong::SetAction()
{
	/// Gravity
	m_tInfo.fY += -10.f + (9.8f * 2.4f * 2.4f * 0.5f);

	float fX, fY;

	if (m_dwDirection & STAT_RIGHT)
	{
		fX = m_tInfo.fX + 10.f;
	}
	else
	{
		fX = m_tInfo.fX - 20.f;
	}

	if ((m_fAngle == 0) || (m_fAngle == 180))
	{
		fY = m_tInfo.fY - 15.f;
	}

	else if (((m_fAngle >= 45) && (m_fAngle <= 60)) || ((m_fAngle >= 120) && (m_fAngle <= 135)))
	{
		fY = m_tInfo.fY - 25.f;
	}

	else if ((m_fAngle >= 80) && (m_fAngle <= 100))
	{
		fY = m_tInfo.fY - 40.f;
	}

	/// 공격모드 일 때 
	if (m_bShoot)
	{
		CObj* pBullet = CObjFactory<CPongBullet>::CreateObj(fX, fY)->SetMap(m_pVecMap);
		((CPongBullet*)pBullet)->SetAngle(m_fAngle);
		CObjMgr::GetInstance()->GetObj(OBJ_BULLET)->push_back(pBullet);
		m_bShoot = false;
	}
}

void CPong::SetScene()
{
	/// Set Angle by Player's Position
	
	if ((abs(m_pPlayer->GetInfo().fX - m_tInfo.fX) >= 270.f) && (abs(m_pPlayer->GetInfo().fX - m_tInfo.fX) > 200.f))
	{
		if (m_dwDirection & STAT_RIGHT)
		{
			m_fAngle = 0.f;
			m_szSetScene = L"Pong_0_Degree";
		}
		else 
		{
			m_fAngle = 180.f;
			m_szSetScene = L"Pong_180_Degree";
		}			
	}

	else if ((abs(m_pPlayer->GetInfo().fX - m_tInfo.fX) <= 200.f) && (abs(m_pPlayer->GetInfo().fX - m_tInfo.fX) > 130.f))
	{
		if (m_dwDirection & STAT_RIGHT)
		{
			m_fAngle = 45.f;
			m_szSetScene = L"Pong_45_Degree";
		}
		else 
		{
			m_fAngle = 135.f;
			m_szSetScene = L"Pong_135_Degree";
		}			
	}

	else if ((abs(m_pPlayer->GetInfo().fX - m_tInfo.fX) <= 130.f) && (abs(m_pPlayer->GetInfo().fX - m_tInfo.fX) > 60.f))
	{
		if (m_dwDirection & STAT_RIGHT)
		{
			m_fAngle = 60.f;
			m_szSetScene = L"Pong_60_Degree";
		}
		else 
		{
			m_fAngle = 120.f;
			m_szSetScene = L"Pong_120_Degree";
		}		
	}

	else if ((abs(m_pPlayer->GetInfo().fX - m_tInfo.fX) <= 60.f) && (abs(m_pPlayer->GetInfo().fX - m_tInfo.fX) > 30.f))
	{
		if (m_dwDirection & STAT_RIGHT)
		{
			m_fAngle = 80.f;
			m_szSetScene = L"Pong_80_Degree";
		}
		else 
		{
			m_fAngle = 100.f;
			m_szSetScene = L"Pong_100_Degree";
		}
	}

	else if (abs(m_pPlayer->GetInfo().fX - m_tInfo.fX) <= 30.f)
	{
		m_fAngle = 90.f;
		m_szSetScene = L"Pong_90_Degree";
	}	
}

void CPong::SetSprites()
{
	if (m_bAttack)
	{ /// if

		if (m_szSetScene == L"Pong_0_Degree")
		{
			if (m_tFrame.iScene != 9)
				m_tFrame.iStart = 0;

			m_tFrame.iScene	= 9;
			m_tFrame.iLast	= 2;
			m_tFrame.dwTime	= 40;
		}

		else if (m_szSetScene == L"Pong_180_Degree")
		{
			if (m_tFrame.iScene != 10)
				m_tFrame.iStart = 0;

			m_tFrame.iScene	= 10;
			m_tFrame.iLast	= 2;
			m_tFrame.dwTime	= 40;
		}


		else if (m_szSetScene == L"Pong_45_Degree")
		{
			if (m_tFrame.iScene != 11)
				m_tFrame.iStart = 0;

			m_tFrame.iScene	= 11;
			m_tFrame.iLast	= 2;
			m_tFrame.dwTime	= 40;
		}

		else if (m_szSetScene == L"Pong_135_Degree")
		{
			if (m_tFrame.iScene != 12)
				m_tFrame.iStart = 0;

			m_tFrame.iScene	= 12;
			m_tFrame.iLast	= 2;
			m_tFrame.dwTime	= 40;
		}


		else if (m_szSetScene == L"Pong_60_Degree")
		{
			if (m_tFrame.iScene != 13)
				m_tFrame.iStart = 0;

			m_tFrame.iScene	= 13;
			m_tFrame.iLast	= 2;
			m_tFrame.dwTime	= 40;
		}

		else if (m_szSetScene == L"Pong_120_Degree")
		{
			if (m_tFrame.iScene != 14)
				m_tFrame.iStart = 0;

			m_tFrame.iScene	= 14;
			m_tFrame.iLast	= 2;
			m_tFrame.dwTime	= 40;
		}

		else if (m_szSetScene == L"Pong_80_Degree")
		{
			if (m_tFrame.iScene != 15)
				m_tFrame.iStart = 0;

			m_tFrame.iScene	= 15;
			m_tFrame.iLast	= 2;
			m_tFrame.dwTime	= 40;
		}

		else if (m_szSetScene == L"Pong_100_Degree")
		{
			if (m_tFrame.iScene != 16)
				m_tFrame.iStart = 0;

			m_tFrame.iScene	= 16;
			m_tFrame.iLast	= 2;
			m_tFrame.dwTime	= 40;
		}


		else if (m_szSetScene == L"Pong_90_Degree")
		{
			if (m_dwDirection & STAT_RIGHT)
				m_tFrame.iScene = 17;
			else
				m_tFrame.iScene = 18;

			if ((m_tFrame.iScene != 17) || (m_tFrame.iScene != 18))
				m_tFrame.iStart = 0;

			m_tFrame.iLast	= 2;
			m_tFrame.dwTime	= 40;
		}		
	  /// if
	} 

	else
	{ /// else
		if (m_szSetScene == L"Pong_0_Degree")
		{
			m_tFrame.iStart = 0;
			m_tFrame.iScene	= 0;
			m_tFrame.iLast	= 0;
			m_tFrame.dwTime	= 0;
		}

		else if (m_szSetScene == L"Pong_180_Degree")
		{
			m_tFrame.iStart = 0;
			m_tFrame.iScene	= 1;
			m_tFrame.iLast	= 0;
			m_tFrame.dwTime	= 200;
		}

		else if (m_szSetScene == L"Pong_45_Degree")
		{
			m_tFrame.iStart = 0;
			m_tFrame.iScene	= 2;
			m_tFrame.iLast	= 0;
			m_tFrame.dwTime	= 200;
		}

		else if (m_szSetScene == L"Pong_135_Degree")
		{
			m_tFrame.iStart = 0;
			m_tFrame.iScene	= 3;
			m_tFrame.iLast	= 0;
			m_tFrame.dwTime	= 200;
		}

		else if (m_szSetScene == L"Pong_60_Degree")
		{
			m_tFrame.iStart = 0;
			m_tFrame.iScene	= 4;
			m_tFrame.iLast	= 0;
			m_tFrame.dwTime	= 200;
		}

		else if (m_szSetScene == L"Pong_120_Degree")
		{
			m_tFrame.iStart = 0;
			m_tFrame.iScene	= 5;
			m_tFrame.iLast	= 0;
			m_tFrame.dwTime	= 200;
		}

		else if (m_szSetScene == L"Pong_80_Degree")
		{
			m_tFrame.iStart = 0;
			m_tFrame.iScene	= 6;
			m_tFrame.iLast	= 0;
			m_tFrame.dwTime	= 200;
		}

		else if (m_szSetScene == L"Pong_100_Degree")
		{
			m_tFrame.iStart = 0;
			m_tFrame.iScene	= 7;
			m_tFrame.iLast	= 0;
			m_tFrame.dwTime	= 200;
		}
		
		else if (m_szSetScene == L"Pong_90_Degree")
		{
			m_tFrame.iStart = 0;
			m_tFrame.iScene	= 8;
			m_tFrame.iLast	= 0;
			m_tFrame.dwTime	= 200;
		}	
	 /// else
	}
}
