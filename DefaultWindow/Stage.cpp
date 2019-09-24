#include "StdAfx.h"
#include "Stage.h"

/// Headers
//#include "enum.h"
#include "function.h"
#include "struct.h"
#include "value.h"

/// Classes
#include "BitBmpMgr.h"
#include "KeyMgr.h"
#include "Obj.h"
#include "ObjFactory.h"
#include "ObjMgr.h"
#include "Player.h"

#include "RenderMgr.h"
#include "Tile.h"

#include "FireMan.h"
#include "Pong.h"
#include "MrHappiness.h"
#include "SawBot.h"
#include "Brandish.h"

CStage::CStage()
{
}

CStage::~CStage()
{
	Release();
}

void CStage::Initialize(void)
{
	m_bDebug		= false;
	m_dwDebugKey	= 0;

	m_bBoss = false;
	m_bPong = false;
	m_bSawBot = false;
	m_bBrandish = false;
	m_bFireMan = false;


	m_dwTime = GetTickCount();

	ReadData();
	/// Change this sentence to Char select menu;
	
	CObjMgr::GetInstance()->GetObj(OBJ_PLAYER)->push_back(CObjFactory<CPlayer>::CreateObj()->SetMap(&m_vecMap));
	m_pPlayer = CObjMgr::GetInstance()->GetObj(OBJ_PLAYER)->front();
}

int CStage::Progress(void)
{
	m_dwDebugKey = CKeyMgr::GetInstance()->GetKey();

	if ((m_dwDebugKey & KEY_Q) && (m_dwDebugKey & KEY_CTRL))
	{
		m_bDebug = !m_bDebug;
	}

	CObjMgr::GetInstance()->Progress();
	m_ptScroll.x = CObjMgr::GetInstance()->GetObj(OBJ_PLAYER)->front()->GetScroll().x;
	m_ptScroll.y = CObjMgr::GetInstance()->GetObj(OBJ_PLAYER)->front()->GetScroll().y;

	if (abs(m_ptScroll.x) < 290)
		return 0;



	
	if ( (abs(m_ptScroll.x) - 300)  < 1)
	{
		if (!m_bPong)
		{
			CObjMgr::GetInstance()->GetObj(OBJ_ENEMY)->push_back(CObjFactory<CPong>::CreateObj(m_pPlayer->GetInfo().fX + 100, m_pPlayer->GetInfo().fY)->SetMap(&m_vecMap));
			m_bPong = true;
		}
	}

	if ( (abs(m_ptScroll.x) - 1600)  < 1)
	{
		if (!m_bFireMan)
		{
			CObjMgr::GetInstance()->GetObj(OBJ_ENEMY)->push_back(CObjFactory<CFireMan>::CreateObj(m_pPlayer->GetInfo().fX + 100, m_pPlayer->GetInfo().fY)->SetMap(&m_vecMap));
			m_bFireMan = true;
		}
	}


	if ( (abs(m_ptScroll.x) - 2900)  < 1)
	{
		if (!m_bSawBot)
		{
			CObjMgr::GetInstance()->GetObj(OBJ_ENEMY)->push_back(CObjFactory<CSawBot>::CreateObj(m_pPlayer->GetInfo().fX + 100, m_pPlayer->GetInfo().fY)->SetMap(&m_vecMap));
			m_bSawBot = true;
		}
	}

	if ( (abs(m_ptScroll.x) - 3200)  < 1)
	{
		if (!m_bBrandish)
		{
			CObjMgr::GetInstance()->GetObj(OBJ_ENEMY)->push_back(CObjFactory<CBrandish>::CreateObj(m_pPlayer->GetInfo().fX + 100, m_pPlayer->GetInfo().fY - 400)->SetMap(&m_vecMap));
			m_bBrandish  = true;
		}
	}

	if (abs(m_ptScroll.x) > 12900)
	{
		if (!m_bBoss)
		{
			CObjMgr::GetInstance()->GetObj(OBJ_ENEMY)->push_back(CObjFactory<CMrHappiness>::CreateObj(m_pPlayer->GetInfo().fX + 100, m_pPlayer->GetInfo().fY)->SetMap(&m_vecMap));
			m_bBoss = true;
		}
	}

	
	
	return 0;
}

void CStage::Render(HDC _hdc)
{
	HFONT hFont;
	HFONT hOldFont;

	hFont = CreateFont(10, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Consolas");
	hOldFont = (HFONT)SelectObject(_hdc, hFont);
	SetBkMode( _hdc, TRANSPARENT );

	HPEN hPen;
	HPEN hOldPen;

	/// Map
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			int INDEX = (i * TILEX) + j;

			if ((j < ((WINCX -m_ptScroll.x) / TILECX) - 51))
				continue;


			if ( ( j > (WINCX - m_ptScroll.x) / TILECX ) || ( i > (WINCY - m_ptScroll.y ) / TILECY ) )
				continue;
	
			TransparentBlt(_hdc, 
				int(m_vecMap[INDEX]->fX - TILECX * 0.5f + m_ptScroll.x),
				int(m_vecMap[INDEX]->fY - TILECY * 0.5f + m_ptScroll.y),
				TILECX, TILECY, CBitBmpMgr::GetInstance()->GetImage("Stage1")->GetMemDC(), 
				m_vecMap[INDEX]->iDrawX * TILECX, m_vecMap[INDEX]->iDrawY * TILECY, TILECX, TILECY, RGB(94, 0, 82));


			if (m_bDebug)
			{ /// Debug
				
				/*TCHAR szBuf[64] = L"";
				wsprintf(szBuf, L" %d", INDEX);
				TextOut(_hdc, 
					int(m_vecMap[INDEX]->fX - TILECX * 0.5f + m_ptScroll.x),
					int(m_vecMap[INDEX]->fY - TILECY * 0.5f + m_ptScroll.y),
					szBuf, lstrlen(szBuf));*/

				if (m_vecMap[INDEX]->iOption == TILE_BLOCK)
				{			

					hPen    = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
					hOldPen = (HPEN)SelectObject(_hdc, hPen);
					SelectObject(_hdc,GetStockObject(NULL_BRUSH));

					Rectangle(_hdc,
						int(m_vecMap[INDEX]->fX - TILECX * 0.5f + m_ptScroll.x),
						int(m_vecMap[INDEX]->fY - TILECY * 0.5f + m_ptScroll.y),
						int(m_vecMap[INDEX]->fX + TILECX * 0.5f + m_ptScroll.x),
						int(m_vecMap[INDEX]->fY + TILECY * 0.5f + m_ptScroll.y));

					MoveToEx(_hdc, int(m_vecMap[INDEX]->fX - TILECX * 0.5f + m_ptScroll.x), 
						int(m_vecMap[INDEX]->fY - TILECY * 0.5f + m_ptScroll.y), NULL);
					LineTo(_hdc,   int(m_vecMap[INDEX]->fX + TILECX * 0.5f + m_ptScroll.x), 
						int(m_vecMap[INDEX]->fY + TILECY * 0.5f + m_ptScroll.y));
					MoveToEx(_hdc, int(m_vecMap[INDEX]->fX - TILECX * 0.5f + m_ptScroll.x), 
						int(m_vecMap[INDEX]->fY + TILECY * 0.5f + m_ptScroll.y), NULL);
					LineTo(_hdc,   int(m_vecMap[INDEX]->fX + TILECX * 0.5f + m_ptScroll.x), 
						int(m_vecMap[INDEX]->fY - TILECY * 0.5f + m_ptScroll.y));

					SelectObject(_hdc, hOldPen);
					DeleteObject(hPen);
				}
			}/// Debug
		}
	}

	/// Obj
	CObjMgr::GetInstance()->Render(_hdc);

	SelectObject(_hdc, hOldFont);
	DeleteObject(hFont);
}

void CStage::Release(void)
{
	int iSize = m_vecMap.size();

	for (int i = 0; i < iSize; ++i)
	{
		Safe_Delete(m_vecMap[i]);
	}
	vector<TILE*>().swap(m_vecMap);

	
}

void CStage::ReadData()
{

	HANDLE	hFile = NULL;
	DWORD	dwByte = 0;

	hFile = CreateFile(L"../Resource/Map/Stage1.dat", GENERIC_READ, 0, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	while (true)
	{
		TILE* pTile = new TILE;

		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);

		if(dwByte == 0)
		{
			Safe_Delete(pTile);
			break;
		}

		m_vecMap.push_back(pTile);

	}
	CloseHandle(hFile);
}