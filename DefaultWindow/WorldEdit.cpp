#include "StdAfx.h"
#include "WorldEdit.h"

/// Headers
#include "enum.h"
#include "function.h"
#include "value.h"

/// Classes
#include "BitBmpMgr.h"
#include "CollisionTool.h"
#include "KeyMgr.h"
#include "TileTool.h"

CWorldEdit::CWorldEdit()
: m_pTileTool(NULL)
, m_fScrollX(0)
, m_fScrollY(0)
, m_fSpeed(10.f)
{
}

CWorldEdit::~CWorldEdit()
{
	Release();
}

void CWorldEdit::Initialize()
{
	m_fSpeed = 140.f;
	m_dwKey	 = 0;

	m_bTileToolOn = false;
	m_iTileDraw	  = 0;
	m_bCOLToolOn  = false;
	m_iCOLDraw	  = 0;
	

	/// Load Data
	ReadData();

	/// If Data is empty, make a new Table
	if (m_vecMap.empty())
	{
		for (int i = 0; i < TILEY; ++i)
		{
			for (int j = 0; j < TILEX; ++j)
			{
				TILE* pTile = new TILE;
				pTile->iIndex = i * TILECX + j;
				pTile->fCX = TILECX;
				pTile->fCY = TILECY;
				pTile->fX  = TILECX * 0.5f + j * TILECX;
				pTile->fY  = TILECY * 0.5f + i * TILECY;
				pTile->iDrawX = 0;
				pTile->iDrawY = 0;
				pTile->iOption = 0;

				m_vecMap.push_back(pTile);
			}
		}
	}

	m_pTileTool = new CTileTool;
	m_pTileTool->Initialize();
	m_pCOLTool = new CCollisionTool;
	m_pCOLTool->Initialize();
}

void CWorldEdit::Progress()
{
	m_dwKey = CKeyMgr::GetInstance()->GetKey();
	
	if (m_dwKey & KEY_E)
	{
		m_bTileToolOn = !m_bTileToolOn;
	}

	if (m_bTileToolOn)
	{		
		if (m_iTileDraw = m_pTileTool->Progress() && (m_dwKey & KEY_LBUTTON))
			Picking();
	}

	if (m_dwKey & KEY_R)
	{
		m_bCOLToolOn = !m_bCOLToolOn;
	}


	if (m_bCOLToolOn)
	{
		if (m_iCOLDraw = m_pCOLTool->Progress() && (m_dwKey & KEY_LBUTTON))
			Picking();
	}
		
	if (m_dwKey & KEY_RETURN)
		SaveData();

	Scroll();
	m_dwKey = 0;

}

void CWorldEdit::Render(HDC _hdc)
{
	
	HFONT hFont;
	HFONT hOldFont;
	hFont = CreateFont(10, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Consolas");
	hOldFont = (HFONT)SelectObject(_hdc, hFont);
	SetBkMode( _hdc, TRANSPARENT );

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			int INDEX = (i * TILEX) + j;

			if ((j < ((WINCX - m_fScrollX) / TILECX) - 51))
				continue;

			if ((j > (WINCX - m_fScrollX) / TILECX) || (i > (WINCY - m_fScrollY) / TILECY))
				continue;
			
			/// Brush
			HBRUSH HBrush;
			HBRUSH OldBrush;
			
			if ( i > 19)
				HBrush=(HBRUSH)CreateSolidBrush(RGB(80,255,255));
			else
				HBrush=(HBRUSH)CreateSolidBrush(RGB(255,255,255));
			OldBrush=(HBRUSH)SelectObject(_hdc, HBrush);
			Rectangle(_hdc,
				int(m_vecMap[INDEX]->fX - TILECX * 0.5f + m_fScrollX),
				int(m_vecMap[INDEX]->fY - TILECY * 0.5f + m_fScrollY),
				int(m_vecMap[INDEX]->fX + TILECX * 0.5f + m_fScrollX),
				int(m_vecMap[INDEX]->fY + TILECY * 0.5f + m_fScrollY));
			
			TransparentBlt(_hdc, 
				int(m_vecMap[INDEX]->fX  - TILECX * 0.5f + m_fScrollX),
				int(m_vecMap[INDEX]->fY - TILECY * 0.5f + m_fScrollY),
				TILECX, TILECY, CBitBmpMgr::GetInstance()->GetImage("Stage1")->GetMemDC(), 
				m_vecMap[INDEX]->iDrawX * TILECX, m_vecMap[INDEX]->iDrawY * TILECY, TILECX, TILECY, RGB(94, 0, 82));

			SelectObject(_hdc, OldBrush);
			DeleteObject(HBrush);
			
			/*TCHAR szBuf[64] = L"";
			wsprintf(szBuf, L" %d", INDEX);
			TextOut(_hdc, 
				int(m_vecMap[INDEX]->fX - TILECX * 0.5f + m_fScrollX),
				int(m_vecMap[INDEX]->fY - TILECY * 0.5f + m_fScrollY),
				szBuf, lstrlen(szBuf));*/


			HPEN hPen;
			HPEN hOldPen;

			switch (m_vecMap[INDEX]->iOption)
			{
			case TILE_BACK:
				hPen	 = CreatePen(PS_SOLID, 1, RGB(0, 255, 255));
				hOldPen = (HPEN)SelectObject(_hdc, hPen);
				SelectObject(_hdc,GetStockObject(NULL_BRUSH));

				Rectangle(_hdc,
					int(m_vecMap[INDEX]->fX - TILECX * 0.5f + m_fScrollX),
					int(m_vecMap[INDEX]->fY - TILECY * 0.5f + m_fScrollY),
					int(m_vecMap[INDEX]->fX + TILECX * 0.5f + m_fScrollX),
					int(m_vecMap[INDEX]->fY + TILECY * 0.5f + m_fScrollY));

				/*Ellipse(_hdc,
					int(m_vecMap[INDEX]->fX - TILECX * 0.5f + m_fScrollX),
					int(m_vecMap[INDEX]->fY - TILECY * 0.5f + m_fScrollY),
					int(m_vecMap[INDEX]->fX + TILECX * 0.5f + m_fScrollX),
					int(m_vecMap[INDEX]->fY + TILECY * 0.5f + m_fScrollY));*/

				SelectObject(_hdc, hOldPen);
				DeleteObject(hPen);
				break;

			case  TILE_BLOCK:
				hPen	 = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
				hOldPen = (HPEN)SelectObject(_hdc, hPen);
				SelectObject(_hdc,GetStockObject(NULL_BRUSH));

				Rectangle(_hdc,
					int(m_vecMap[INDEX]->fX - TILECX * 0.5f + m_fScrollX),
					int(m_vecMap[INDEX]->fY - TILECY * 0.5f + m_fScrollY),
					int(m_vecMap[INDEX]->fX + TILECX * 0.5f + m_fScrollX),
					int(m_vecMap[INDEX]->fY + TILECY * 0.5f + m_fScrollY));

				MoveToEx(_hdc, int(m_vecMap[INDEX]->fX - TILECX * 0.5f + m_fScrollX), 
					int(m_vecMap[INDEX]->fY - TILECY * 0.5f + m_fScrollY), NULL);
				LineTo(_hdc, int(m_vecMap[INDEX]->fX + TILECX *  0.5f + m_fScrollX), 
					int(m_vecMap[INDEX]->fY + TILECY * 0.5f + m_fScrollY));
				MoveToEx(_hdc, int(m_vecMap[INDEX]->fX - TILECX * 0.5f + m_fScrollX), 
					int(m_vecMap[INDEX]->fY + TILECY * 0.5f + m_fScrollY), NULL);
				LineTo(_hdc, int(m_vecMap[INDEX]->fX + TILECX * 0.5f + m_fScrollX), 
					int(m_vecMap[INDEX]->fY - TILECY * 0.5f + m_fScrollY));
				SelectObject(_hdc, hOldPen);
				DeleteObject(hPen);
				break;
			}
		}
	}

	if (m_bTileToolOn)
		m_pTileTool->Render(_hdc);	

	if (m_bCOLToolOn)
		m_pCOLTool->Render(_hdc);
	
	SelectObject(_hdc, hOldFont);
	DeleteObject(hFont);
}

void CWorldEdit::Release()
{
	int MAPSIZE = m_vecMap.size();
	for (int i = 0; i < MAPSIZE; ++i)
		Safe_Delete(m_vecMap[i]);
	vector<TILE*>().swap(m_vecMap);

	Safe_Delete(m_pTileTool);
	Safe_Delete(m_pCOLTool);
}


void CWorldEdit::Picking()
{
	FPOINT tMouse;

	tMouse.fX = GetMouse().fX;
	tMouse.fY = GetMouse().fY; 

	int iX = int((tMouse.fX - int(m_fScrollX)) * powf(float(TILECX), -1.f));
	int iY = int((tMouse.fY - int(m_fScrollY)) * powf(float(TILECY), -1.f));

	const int INDEX  = iX + iY * TILEX;

	if ((tMouse.fX - m_fScrollX < 0) || (tMouse.fX - m_fScrollX > TILEX * TILECX ))
		return;

	if (m_iCOLDraw)
		m_vecMap[INDEX]->iOption = m_pCOLTool->GetOption();

	if (m_iTileDraw)
	{
		m_vecMap[INDEX]->iOption = m_pTileTool->GetOption();
		m_vecMap[INDEX]->iDrawX  = m_pTileTool->GetDrawX();
		m_vecMap[INDEX]->iDrawY  = m_pTileTool->GetDrawY();
	}

}

void CWorldEdit::SaveData()
{
	HANDLE		hFile = NULL;
	DWORD		dwByte = 0;
	int			iAsk = MessageBox(g_hWnd, L"저장하시겠습니까?", L"저장",MB_OKCANCEL);

	if (iAsk == 1)
	{

		hFile = CreateFile(L"../Resource/Map/Stage1.dat", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			vector<TILE*>::iterator	Tileiter_end = m_vecMap.end();
		for(vector<TILE*>::iterator	Tileiter	 = m_vecMap.begin();
			Tileiter != Tileiter_end; ++Tileiter)
		{
			WriteFile(hFile, (*Tileiter), sizeof(TILE), &dwByte, NULL);
		}


		CloseHandle(hFile);
	}
}

void CWorldEdit::ReadData()
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

void CWorldEdit::Scroll()
{
	if (GetAsyncKeyState(VK_RIGHT))
		m_fScrollX -= m_fSpeed;

	if (GetAsyncKeyState(VK_LEFT))
		m_fScrollX += m_fSpeed;

	if (GetAsyncKeyState(VK_UP))
		m_fScrollY += m_fSpeed;

	if (GetAsyncKeyState(VK_DOWN))
		m_fScrollY -= m_fSpeed;

	if(m_fScrollX > 0)
		m_fScrollX = 0.f;

	if(m_fScrollY < -WINCY)
		m_fScrollY = -WINCY;

	if(m_fScrollX < WINCX - (TILEX * TILECX))
		m_fScrollX = WINCX - (TILEX * TILECX);

	if(m_fScrollY < WINCY - (TILEY * TILECY))
		m_fScrollY = WINCY - (TILEY * TILECY);
}