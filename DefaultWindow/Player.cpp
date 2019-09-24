#include "StdAfx.h"
#include "Player.h"

/// Classes
#include "BitBmpMgr.h"
#include "HpBar.h"
#include "KeyMgr.h"
#include "NormalBullet.h"
#include "HalfChargedBullet.h"
#include "FullChargedBullet.h"
#include "ObjFactory.h"
#include "ObjMgr.h"

CPlayer::CPlayer(void)
: m_fImageX(120.f)
, m_fImageY(114.f)
{
}

CPlayer::~CPlayer(void)
{
}

void CPlayer::Initialize()
{
	m_bDebug			= false;

	m_tInfo.fX			= 100.f;
	m_tInfo.fY			= 450.f;
	m_tInfo.fCX			= 48.f;
	m_tInfo.fCY			= 56.f;

	m_fSpeed			= 8.4f;
	m_fSec				= 0.f;
	m_fVelocity			= -13.f;
	m_fCacheVel			= 0.f;
	m_fGravity			= m_fVelocity + (9.8f * 2.3f * 2.3f * 0.5f);

	/// < Status > ///
	m_iMaxHP			= 32;
	m_iCurrHP			= 32;
	m_pHpBar			= CObjFactory<CHpBar>::CreateObj();
	CObjMgr::GetInstance()->GetObj(OBJ_HPBAR)->push_back(m_pHpBar);
	

	m_bJump				= false;
	m_bFall				= false;
	m_bDash				= false;
	m_dwKey				= 0;
	m_dwDirection		= m_dwCacheDir = KEY_RIGHT;
	m_dwState			= 0;
	m_dwFrameTime		= GetTickCount();
	
	/// < ATK > ///
	m_iCharged			= 0;
	m_dwATKTime			= GetTickCount();
	m_dwChargedTime		= GetTickCount();
	
	m_bNoCharged		= false;
	m_bHalfCharged		= false;
	m_bFullCharged		= false;
	m_tChargedFrame.iLast = 13;

	/// < Dash > ///
	m_bDash				= false;
	m_bChargedDash		= false;
	m_bChargedDashOn	= false;
	m_dwDashChargedTime	= GetTickCount();
	m_iDashCount		= 0;
	m_iDashCharged		= 0;
	m_iChargedDashCount	= 0;

	/// < OffSet > ///
	m_ptOffSet.x	= int(WINCX * 0.5f);
	m_ptOffSet.y	= int(WINCY - 100.f);
}

int CPlayer::Progress()
{	
	KeyInput();
	SetAction();
	CollisionWithMap();
	CollisionWithBullet();
	SetScene();
	SetSprites();
	MoveFrame();
	ScrollX();
	ScrollY();
	
	return 0;
}

void CPlayer::Render(HDC _hdc)
{
	TransparentBlt(_hdc, 
				int(m_tInfo.fX - m_fImageX * 0.5f + 2.f + m_ptScroll.x),
				int(m_tInfo.fY - m_fImageY * 0.5f - 5.f + m_ptScroll.y),
				int(m_fImageX),
				int(m_fImageY),
				CBitBmpMgr::GetInstance()->GetImage("Beck")->GetMemDC(), 
				int(m_fImageX * m_tFrame.iStart),
				int(m_fImageY * m_tFrame.iScene),
				int(m_fImageX),
				int(m_fImageY), RGB(0, 255, 80));

	if (m_bHalfCharged || m_bFullCharged)
	{
		TransparentBlt(_hdc, 
			int(m_tInfo.fX - m_fImageX * 0.5f + 2.f + m_ptScroll.x),
			int(m_tInfo.fY - m_fImageY * 0.5f - 5.f + m_ptScroll.y),
			int(m_fImageX),
			int(m_fImageY),
			CBitBmpMgr::GetInstance()->GetImage("Beck")->GetMemDC(), 
			int(m_fImageX * m_tChargedFrame.iStart),
			int(m_fImageY * m_tChargedFrame.iScene),
			int(m_fImageX),
			int(m_fImageY), RGB(0, 255, 80));
	}

	
	if (m_bDebug)
	{
		/// DebugMode
		TCHAR	m_szBottomX[128];
		TCHAR	m_szBottomY[128];

		wsprintf(m_szBottomX, L"Bottom X : %d", int(m_tInfo.fX + m_tInfo.fCX * 0.5f ));
		TextOut(_hdc, int(m_tInfo.fX + m_ptScroll.x), int(m_tInfo.fY - m_tInfo.fCY * 0.9f + m_ptScroll.y), m_szBottomX, lstrlen(m_szBottomX));
		wsprintf(m_szBottomY, L"Bottom Y : %d", int(m_tInfo.fY + m_tInfo.fCY * 0.5f));
		TextOut(_hdc, int(m_tInfo.fX + m_ptScroll.x), int(m_tInfo.fY - m_tInfo.fCY * 0.8f + m_ptScroll.y), m_szBottomY, lstrlen(m_szBottomY));

		wsprintf(m_szBottomX, L"OffSet X : %d", m_ptOffSet.x);
		TextOut(_hdc, int(m_tInfo.fX + m_ptScroll.x), int(m_tInfo.fY - m_tInfo.fCY * 1.5f + m_ptScroll.y), m_szBottomX, lstrlen(m_szBottomX));
		wsprintf(m_szBottomY, L"OffSet Y : %d", m_ptOffSet.y);
		TextOut(_hdc, int(m_tInfo.fX + m_ptScroll.x), int(m_tInfo.fY - m_tInfo.fCY * 1.3f + m_ptScroll.y), m_szBottomY, lstrlen(m_szBottomY));

		HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
		HPEN hOldPen = (HPEN)SelectObject(_hdc, hPen);

		Rectangle(_hdc,
			int(m_tInfo.fX - m_tInfo.fCX * 0.5f + m_ptScroll.x),
			int(m_tInfo.fY - m_tInfo.fCY * 0.5f + m_ptScroll.y),
			int(m_tInfo.fX + m_tInfo.fCX * 0.5f + m_ptScroll.x),
			int(m_tInfo.fY + m_tInfo.fCY * 0.5f + m_ptScroll.y));

		SelectObject(_hdc, hOldPen);
		DeleteObject(hPen);
	}
}

void CPlayer::Release()
{
}

void CPlayer::KeyInput()
{
	/// Reset States
	m_dwState		= 0;
	m_dwDirection	= 0;
	m_dwKey = CKeyMgr::GetInstance()->GetKey();

	/// DebugMode
	if ((m_dwKey & KEY_Q) && (m_dwKey & KEY_CTRL))
	{
		m_bDebug	 = !m_bDebug;
	}

	/// Set Progress
	if (!(m_dwKey & KEY_LEFT) && !(m_dwKey & KEY_RIGHT) && !m_bJump && !m_bDash)
	{
		m_dwState		|= STAT_STAND;
	}

	if (m_dwKey & KEY_RIGHT)
	{
		m_dwDirection	|= KEY_RIGHT;
		m_dwState		|= STAT_RUN;
	}

	if (m_dwKey & KEY_LEFT)
	{
		m_dwDirection	|= KEY_LEFT;
		m_dwState		|= STAT_RUN;
	}
	
	if (m_dwKey & KEY_C)
	{
		BeckAtk();
	}
	else if (m_bHalfCharged || m_bFullCharged)
	{
		m_dwState |= STAT_CHARGEDATK;
		m_iCharged = 0;
	}
	else if (m_bNoCharged)
	{
		m_iCharged = 0;
	}
	
	/// Jump
	if (m_dwKey & KEY_X)
	{
		m_bJump = true;
		m_dwState		|= STAT_JUMP;
	}

	/// Dash
	if (m_dwKey & KEY_Z)
	{
		if (m_dwDashChargedTime + 15 < GetTickCount())
		{	
			m_dwDashChargedTime = GetTickCount();

			++m_iDashCharged;

			if (m_iDashCharged > 30)
				m_bChargedDash = true;
		}	
		m_bDash	= true;
	}
	else if (m_bChargedDash)
	{
		m_dwState |= STAT_CHARGEDDASH;
		m_bDash = false;

		++m_iChargedDashCount;

		if (m_iChargedDashCount > 15)
		{
			BeckDashReset();
		}
	}
	else if (m_bDash)
	{
		m_dwState |= STAT_DASH;
		++m_iDashCount;

		if (m_iDashCount > 15)	
		{		
			BeckDashReset();
		}
	}	

	/// < Set PrevDirection > ///
	if (!m_dwDirection)
	{
		m_dwDirection	|= m_dwCacheDir;
	}

	/// < Backcup DIR > ///
	m_dwCacheDir = m_dwDirection;
}

void CPlayer::SetAction()
{
	/// < RUN > ///
	if (m_dwState & STAT_RUN)
	{
		if (m_dwDirection & KEY_LEFT) 
			m_tInfo.fX -= m_fSpeed;	

		else if (m_dwDirection & KEY_RIGHT)
			m_tInfo.fX += m_fSpeed;
	}		

	if (m_bJump)
	{
		m_fSec += 0.12f;
		m_fCacheVel = m_fVelocity + (9.8f * m_fSec * m_fSec * 0.5f);
		
		if (m_fCacheVel < - 30.f)
			m_fCacheVel = -30.f;
		if (m_fCacheVel >  40.f)
			m_fCacheVel =  40.f;

		m_tInfo.fY += m_fCacheVel; 
	}
	else
	{
		m_tInfo.fY += m_fGravity;
	}

	if ((m_dwState & STAT_DASH) && !(m_dwState & STAT_CHARGEDDASH))
	{
		if (m_dwDirection & KEY_LEFT) 
		{
			m_tInfo.fX -= m_fSpeed * 1.7f;
		}

		else if (m_dwDirection & KEY_RIGHT)
		{
			m_tInfo.fX += m_fSpeed * 1.7f;
		}

	}

	if ((m_dwState & STAT_CHARGEDDASH) && !(m_dwState & STAT_DASH))
	{
		if (m_dwDirection & KEY_LEFT) 
		{
			m_tInfo.fX -= m_fSpeed * 1.7f;
		}

		else if (m_dwDirection & KEY_RIGHT)
		{
			m_tInfo.fX += m_fSpeed * 1.7f;
		}
	}

	if ((m_dwState & STAT_ATTACK) && !(m_dwState & STAT_CHARGEDDASH))
	{
		float fX;
		float fY;
		float fDIR;

		if (m_dwState & STAT_DASH)
		{
			fY = m_tInfo.fY + 2.f;
		}
		else
		{
			fY = m_tInfo.fY - 12.f;
		}

		if (m_dwATKTime + 170 < GetTickCount())
		{
			m_dwATKTime = GetTickCount();


			if (m_dwDirection & KEY_RIGHT)
			{
				fX = m_tInfo.fX + m_tInfo.fCX * 0.5f;
				fDIR = RIGHT;	
			}

			else
			{
				fX = m_tInfo.fX - m_tInfo.fCX * 0.5f;
				fDIR = LEFT;			
			}		

			if (m_bNoCharged)
			{
				CObj* pBullet = CObjFactory<CNormalBullet>::CreateObj(fX, fY);
				((CBullet*)pBullet)->SetDIR(fDIR);
				pBullet->SetMap(m_pVecMap);
				CObjMgr::GetInstance()->GetObj(OBJ_BULLET)->push_back(pBullet);
			}
		}
	}

	if (m_dwState & STAT_CHARGEDATK)
	{
		float fX;  
		float fY = m_tInfo.fY - 12.f;
		float fDIR;

		
		if (m_dwDirection & KEY_RIGHT)
		{
			fX = m_tInfo.fX - m_tInfo.fCX * 0.5f + 70.f;
			fDIR = RIGHT;	
		}

		else
		{
			fX = m_tInfo.fX + m_tInfo.fCX * 0.5f - 70.f;
			fDIR = LEFT;	
		}
	
		if (m_bHalfCharged)
		{
			CObj* pBullet = CObjFactory<CHalfChargedBullet>::CreateObj(fX, fY);
			((CBullet*)pBullet)->SetDIR(fDIR);

			pBullet->SetMap(m_pVecMap);

			CObjMgr::GetInstance()->GetObj(OBJ_BULLET)->push_back(pBullet);
		}

		if (m_bFullCharged)
		{
			CObj* pBullet = CObjFactory<CFullChargedBullet>::CreateObj(fX, fY);
			((CBullet*)pBullet)->SetDIR(fDIR);

			pBullet->SetMap(m_pVecMap);

			CObjMgr::GetInstance()->GetObj(OBJ_BULLET)->push_back(pBullet);
		}

		m_bNoCharged   = true;
		m_bHalfCharged = false;
		m_bFullCharged = false;
	}


	if ((m_tInfo.fX - m_tInfo.fCX * 0.5f) < m_ptScroll.x)
	{	
		m_tInfo.fX = m_tInfo.fCX * 0.5f;
	}

	if ((m_tInfo.fX + m_tInfo.fCX * 0.5f) > WINCX - m_ptScroll.x)
	{	
		m_tInfo.fX = WINCX - m_tInfo.fCX * 0.5f - m_ptScroll.x;
	}
}

void CPlayer::CollisionWithMap()
{
	m_bFall = true; 
	const int MidIndex = (int(m_tInfo.fX * powf(float(TILECX), -1.f))) + ((int(m_tInfo.fY * powf(float(TILECY), -1.f))) * TILEX);

	int INDEX[4] = {
		/// Bottom
		MidIndex + TILEX,

		/// Top
		MidIndex - TILEX,

		/// Right
		MidIndex + 1,

		/// Left
		MidIndex - 1,
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
						m_bFall = false;
					}
					else
					{
						m_tInfo.fY -= rc.bottom;
						m_bJump = false;
						m_bFall = false;
						m_fSec	= 0.f;
					}
				}
				else
				{
					if (m_tInfo.fX > (*m_pVecMap)[INDEX[i]]->fX)
					{
						m_tInfo.fX += rc.right;
						m_bFall = false;
					}
					else 
					{
						m_tInfo.fX -= rc.right;
						m_bFall = false;
					}
				}
			}
		}
	}
}

void CPlayer::CollisionWithBullet()
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

		if (((CBullet*)((*m_pVecBullet)[i]))->GetType() == 1)
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

				--m_iCurrHP;
				((CHpBar*)m_pHpBar)->SetDamage(1);

				/*if (m_iCurrHP < 0)
					m_bDead = true;*/
			}
		}

	}
}

void CPlayer::SetScene()
{
	if (m_dwState & STAT_STAND)
	{		
		if (m_dwDirection & KEY_RIGHT)
			m_szSetScene = L"Beck_Stand_Right";

		else if (m_dwDirection & KEY_LEFT)
			m_szSetScene = L"Beck_Stand_Left";
	}

	if (m_dwState & STAT_RUN)
	{
		if (m_dwDirection & KEY_RIGHT)
			m_szSetScene = L"Beck_Run_Right";

		else if (m_dwDirection & KEY_LEFT)
			m_szSetScene = L"Beck_Run_Left";
	}

	if ((m_dwState & STAT_JUMP) || (m_bFall))
	{
		if (m_dwDirection & KEY_RIGHT)
			m_szSetScene = L"Beck_Jump_Right";

		else if (m_dwDirection & KEY_LEFT)
			m_szSetScene = L"Beck_Jump_Left";
	}

	if (m_dwState & STAT_ATTACK)
	{
		if (m_dwState & STAT_STAND)
		{
			if (m_dwDirection & KEY_RIGHT)
				m_szSetScene = L"Beck_ATK_Right";

			else if (m_dwDirection & KEY_LEFT)
				m_szSetScene = L"Beck_ATK_Left";
		}

		else if ((m_dwState & STAT_RUN) && !m_bJump)
		{
			if (m_dwDirection & KEY_RIGHT)
				m_szSetScene = L"Beck_ATK_Run_Right";

			else if (m_dwDirection & KEY_LEFT)
				m_szSetScene = L"Beck_ATK_Run_Left";
		}

		else if (m_dwState & STAT_DASH)
		{
			if (m_dwDirection & KEY_RIGHT)
				m_szSetScene = L"Beck_ATK_Dash_Right";

			else if (m_dwDirection & KEY_LEFT)
				m_szSetScene = L"Beck_ATK_Dash_Left";
		}

		else if (m_dwState & STAT_JUMP)
		{
			if (m_dwDirection & KEY_RIGHT)
				m_szSetScene = L"Beck_ATK_Jump_Right";

			else if (m_dwDirection & KEY_LEFT)
				m_szSetScene = L"Beck_ATK_Jump_Left";
		}	
	}

	if ((m_dwState & STAT_DASH) && !(m_dwState & STAT_ATTACK))
	{
		if (m_dwDirection & KEY_RIGHT)
			m_szSetScene = L"Beck_Dash_Right";

		else if (m_dwDirection & KEY_LEFT)
			m_szSetScene = L"Beck_Dash_Left";
	}

	if ((m_dwState & STAT_CHARGEDDASH) && !(m_dwState & STAT_ATTACK))
	{
		if (m_dwDirection & KEY_RIGHT)
			m_szSetScene = L"Beck_ChargedDash_Right";

		else if (m_dwDirection & KEY_LEFT)
			m_szSetScene = L"Beck_ChargedDash_Left";
	}	
}

void CPlayer::SetSprites()
{
	if (m_szSetScene == L"Beck_Stand_Right")
	{
		if (m_tFrame.iScene != 2)
			m_tFrame.iStart = 0;

		m_tFrame.iScene	= 2;
		m_tFrame.iLast	= 1;
		m_tFrame.dwTime	= 200;
	}

	if (m_szSetScene == L"Beck_Stand_Left")
	{
		if (m_tFrame.iScene != 3)
			m_tFrame.iStart = 0;

		m_tFrame.iScene = 3;
		m_tFrame.iLast	= 1;
		m_tFrame.dwTime = 200;
	}


	if (m_szSetScene == L"Beck_Run_Right")
	{
		if (m_tFrame.iScene != 6)
			m_tFrame.iStart = 0;

		m_tFrame.iScene = 6;
		m_tFrame.iLast	= 3;
		m_tFrame.dwTime = 200;
	}

	if (m_szSetScene == L"Beck_Run_Left")
	{
		if (m_tFrame.iScene != 7)
			m_tFrame.iStart = 0;

		m_tFrame.iScene = 7;
		m_tFrame.iLast	= 3;
		m_tFrame.dwTime = 200;
	}

	if (m_szSetScene == L"Beck_Jump_Right")
	{
		if (m_tFrame.iScene != 8)
			m_tFrame.iStart = 0;

		m_tFrame.iScene = 8;
		m_tFrame.iLast	= 1;
		m_tFrame.dwTime = 200;
	}

	if (m_szSetScene == L"Beck_Jump_Left")
	{
		if (m_tFrame.iScene != 9)
			m_tFrame.iStart = 0;

		m_tFrame.iScene = 9;
		m_tFrame.iLast	= 1;
		m_tFrame.dwTime = 200;
	}

	if (m_szSetScene == L"Beck_ATK_Right")
	{
		if (m_tFrame.iScene != 20)
			m_tFrame.iStart = 0;

		m_tFrame.iScene = 20;
		m_tFrame.iLast	= 2;
		m_tFrame.dwTime = 80;
	}

	if (m_szSetScene == L"Beck_ATK_Left")
	{
		if (m_tFrame.iScene != 21) 
			m_tFrame.iStart = 0;

		m_tFrame.iScene = 21;
		m_tFrame.iLast	= 2;
		m_tFrame.dwTime = 80;
	}

	if (m_szSetScene == L"Beck_ATK_Run_Right")
	{
		if (m_tFrame.iScene != 22)
			m_tFrame.iStart = 0;

		m_tFrame.iScene = 22;
		m_tFrame.iLast	= 7;
		m_tFrame.dwTime = 40;
	}

	if (m_szSetScene == L"Beck_ATK_Run_Left")
	{
		if (m_tFrame.iScene != 23)
			m_tFrame.iStart = 0;

		m_tFrame.iScene = 23;
		m_tFrame.iLast	= 7;
		m_tFrame.dwTime = 40;
	}

	if (m_szSetScene == L"Beck_ATK_Jump_Right")
	{
		if (m_tFrame.iScene != 24)
			m_tFrame.iStart = 0;

		m_tFrame.iScene = 24;
		m_tFrame.iLast	= 2;
		m_tFrame.dwTime = 80;
	}

	if (m_szSetScene == L"Beck_ATK_Jump_Left")
	{
		if (m_tFrame.iScene != 25)
			m_tFrame.iStart = 0;

		m_tFrame.iScene = 25;
		m_tFrame.iLast	= 2;
		m_tFrame.dwTime = 80;
	}

	if (m_szSetScene == L"Beck_ATK_Dash_Right")
	{
		if (m_tFrame.iScene != 32)
			m_tFrame.iStart = 0;

		m_tFrame.iScene = 32;
		m_tFrame.iLast	= 2;
		m_tFrame.dwTime = 80;
	}

	if (m_szSetScene == L"Beck_ATK_Dash_Left")
	{
		if (m_tFrame.iScene != 33)
			m_tFrame.iStart = 0;

		m_tFrame.iScene = 33;
		m_tFrame.iLast	= 2;
		m_tFrame.dwTime = 80;
	}

	if (m_szSetScene == L"Beck_Dash_Right")
	{
		if (m_tFrame.iScene != 28)
			m_tFrame.iStart = 0;

		m_tFrame.iScene = 28;
		m_tFrame.iLast	= 2;
		m_tFrame.dwTime = 80;
	}

	if (m_szSetScene == L"Beck_Dash_Left")
	{
		if (m_tFrame.iScene != 29)
			m_tFrame.iStart = 0;

		m_tFrame.iScene = 29;
		m_tFrame.iLast	= 2;
		m_tFrame.dwTime = 80;
	}

	if (m_szSetScene == L"Beck_ChargedDash_Right")
	{
		if (m_tFrame.iScene != 30)
			m_tFrame.iStart = 0;

		m_tFrame.iScene = 30;
		m_tFrame.iLast	= 2;
		m_tFrame.dwTime = 80;
	}

	if (m_szSetScene == L"Beck_ChargedDash_Left")
	{
		if (m_tFrame.iScene != 31)
			m_tFrame.iStart = 0;

		m_tFrame.iScene = 31;
		m_tFrame.iLast	= 2;
		m_tFrame.dwTime = 80;
	}
}
void CPlayer::MoveFrame()
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

void CPlayer::ScrollX(void)
{
	if(m_tInfo.fX < m_ptOffSet.x)
	{
		if(m_ptScroll.x > 0 - m_fSpeed)
		{
			if(m_tInfo.fX < 0)
			{
				m_tInfo.fX = 0;
				
			}
			return;
		}
		m_ptScroll.x += (long)m_fSpeed;
		m_ptOffSet.x -= (long)m_fSpeed;
	}

	if(m_tInfo.fX > m_ptOffSet.x)
	{
		if(m_ptScroll.x < WINCX - (TILECX * TILEX) + m_fSpeed)
		{
			if(m_tInfo.fX > (TILECX * TILEX) + m_fSpeed)
			{
				m_tInfo.fX = (TILECX * TILEX);
			}
			return;
		}

		m_ptScroll.x -= (long)m_fSpeed;
		m_ptOffSet.x += (long)m_fSpeed;
	}
}

void CPlayer::ScrollY(void)
{
	if(m_tInfo.fY < m_ptOffSet.y)
	{
		if(m_ptScroll.y > 0 - m_fGravity)
		{
			if(m_tInfo.fY < 0)
				m_tInfo.fY = 0;

			return;
		}

		m_ptScroll.y += (long)m_fGravity;
		m_ptOffSet.y -= (long)m_fGravity;
	}

	/// Scroll problem (Window keep going down)

	if(m_tInfo.fY > m_ptOffSet.y)
	{
		if(m_ptScroll.y < WINCY - (TILEY * TILECY) + m_fGravity)
		{
			if(m_tInfo.fY > (TILEY * TILECY) - WINCY * 0.5f)
				m_tInfo.fY = (TILEY * TILECY) - WINCY * 0.5f;
			
			return;
		}

		m_ptScroll.y -= (long)m_fGravity;
		m_ptOffSet.y += (long)m_fGravity;
	}
}

void CPlayer::BeckAtk()
{	
	m_dwState		|= STAT_ATTACK;

	if (m_dwChargedTime + 15 < GetTickCount())
	{	
		m_dwChargedTime = GetTickCount();

		++m_iCharged;

		if (m_bHalfCharged || m_bFullCharged)
		{
			m_tChargedFrame.iScene = 34;

			++m_tChargedFrame.iStart;

			if (m_tChargedFrame.iStart >= m_tChargedFrame.iLast)
			{
				m_tChargedFrame.iStart = 0;
			}
		}

		if (m_iCharged < 3)
		{
			m_bNoCharged	 = true;
			m_bHalfCharged	 = false;
			m_bFullCharged	 = false;
		}
		else if ((m_iCharged > 15 ) && (m_iCharged < 60))
		{
			m_bNoCharged   = false;
			m_bHalfCharged = true;
			m_bFullCharged = false;
		}
		else if (m_iCharged > 60)
		{
			m_bNoCharged   = false;
			m_bHalfCharged = false;
			m_bFullCharged = true;
		}
	}
}

void CPlayer::BeckDashReset()
{
	m_bDash = false;
	m_iDashCount = 0;
	m_iDashCharged = 0;
	m_bChargedDash = false;
	m_iChargedDashCount = 0;
}