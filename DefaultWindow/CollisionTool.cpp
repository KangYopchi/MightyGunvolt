#include "StdAfx.h"
#include "CollisionTool.h"

/// Headers
#include "enum.h"
#include "function.h"
#include "value.h"

/// Class
#include "KeyMgr.h"

CCollisionTool::CCollisionTool()
: TILESETX(4)
, TILESETY(1)
{
}

CCollisionTool::~CCollisionTool()
{
	Release();
}

void CCollisionTool::Initialize()
{
	m_vecCOLs.reserve(300);

	m_bToolMove = false;

	m_iOption	= 0;
	m_dwKey		= 0;

	m_tInfo.fX	= 400.f;
	m_tInfo.fY	= 400.f;
	m_tInfo.fCX = 128.f;
	m_tInfo.fCY = 32.f;

	m_tTitle.fCX = m_tInfo.fCX;
	m_tTitle.fCY = 10.f;
	m_tTitle.fX  = m_tInfo.fX;
	m_tTitle.fY  = m_tInfo.fY - m_tInfo.fCY * 0.5f - m_tTitle.fCY * 0.5f;
	
	m_tStandard.fX = m_tInfo.fX - m_tInfo.fCX * 0.5f;
	m_tStandard.fY = m_tInfo.fY - m_tInfo.fCY * 0.5f;

	int iOptionValue = 101; /// Option value Set
	for (int i = 0; i < TILESETY; ++i) 
	{
		for(int j = 0; j < TILESETX; ++j)
		{
			int INDEX = (i * TILESETX) + j;
			
			TILE* pTile			= new TILE;
			pTile->fX			= m_tStandard.fX + (j * TILECX) + (TILECX * 0.5f);
			pTile->fY			= m_tStandard.fY + (i * TILECY) + (TILECY * 0.5f);
			pTile->fCX			= TILECX;
			pTile->fCY			= TILECY;
			pTile->iIndex		= INDEX;
			if (j == 0)
				pTile->iOption = 0;
			else
			{
				pTile->iOption = iOptionValue;
				++iOptionValue;
			}
			
			m_vecCOLs.push_back(pTile);
			
		}
	}
}

int CCollisionTool::Progress()
{
	m_dwKey = CKeyMgr::GetInstance()->GetKey();
	
	/// Move Tool
	if (m_dwKey & KEY_LBUTTON)
	{
		m_tStart.fX = GetMouse().fX;
		m_tStart.fY = GetMouse().fY;

		/// Collision Title bar
		if ((m_tStart.fX > m_tInfo.fX - m_tInfo.fCX * 0.5f) && (m_tStart.fX < m_tInfo.fX + m_tInfo.fCX * 0.5f) &&
			(m_tStart.fY > m_tInfo.fY - m_tInfo.fCY * 0.5f - m_tTitle.fCY) && (m_tStart.fY < m_tInfo.fY - m_tInfo.fCY * 0.5f))
		{
			if ( m_bToolMove == false )
			{
				m_bToolMove = true;

				m_tDistance.fX = m_tInfo.fX - m_tStart.fX;
				m_tDistance.fY = m_tInfo.fY - m_tStart.fY;

				m_tTitleDistance.fX = m_tTitle.fX - m_tStart.fX;
				m_tTitleDistance.fY = m_tTitle.fY - m_tStart.fY;

				for (int i = 0; i < TILESETY; ++i) 
				{
					for(int j = 0; j < TILESETX; ++j)
					{
						int INDEX = (i * TILESETX) + j;

						m_tTileDistance[INDEX].fX = m_vecCOLs[INDEX]->fX - m_tStart.fX;
						m_tTileDistance[INDEX].fY = m_vecCOLs[INDEX]->fY - m_tStart.fY;

					}
				}
			}
		}
	}
	else 
		m_bToolMove = false;

	if (m_bToolMove)
	{
		m_tCurrent.fX = GetMouse().fX;
		m_tCurrent.fY = GetMouse().fY;

		m_tInfo.fX = m_tCurrent.fX + m_tDistance.fX;
		m_tInfo.fY = m_tCurrent.fY + m_tDistance.fY;


		m_tTitle.fX = m_tCurrent.fX + m_tTitleDistance.fX;
		m_tTitle.fY = m_tCurrent.fY + m_tTitleDistance.fY ;

		for (int i = 0; i < TILESETY; ++i) 
		{
			for(int j = 0; j < TILESETX; ++j)
			{
				int INDEX = (i * TILESETX) + j;

				m_vecCOLs[INDEX]->fX = m_tCurrent.fX + m_tTileDistance[INDEX].fX;
				m_vecCOLs[INDEX]->fY = m_tCurrent.fY + m_tTileDistance[INDEX].fY;

			}
		}
		return 0;
	}

	/// Picking Tile
	if (m_dwKey & KEY_LBUTTON)
	{
		FPOINT	tMouse;

		tMouse.fX = GetMouse().fX;
		tMouse.fY = GetMouse().fY;	

		if ((tMouse.fX < m_tInfo.fX - m_tInfo.fCX * 0.5f) || (tMouse.fY < m_tInfo.fY - m_tInfo.fCY * 0.5f) 
		 || (tMouse.fX > m_tInfo.fX + m_tInfo.fCX * 0.5f) || (tMouse.fY > m_tInfo.fY + m_tInfo.fCY * 0.5f))
			return 1;
		
		

		float tempi =  powf(float(TILECX), -1.f);
		tMouse.fX = (tMouse.fX - (m_tInfo.fX - m_tInfo.fCX * 0.5f)) * powf(float(TILECX), -1.f);
		tMouse.fY = (tMouse.fY - (m_tInfo.fY - m_tInfo.fCY * 0.5f)) * powf(float(TILECY), -1.f);
	
		int INDEX = int(tMouse.fX) + int(tMouse.fY) * TILESETX;

		m_iOption = m_vecCOLs[INDEX]->iOption;
	}

	return 0;
}

void CCollisionTool::Render(HDC _hdc)
{
	/// Titile
	Rectangle(_hdc,
		int(m_tTitle.fX - m_tTitle.fCX * 0.5f),
		int(m_tTitle.fY - m_tTitle.fCY * 0.5f),
		int(m_tTitle.fX + m_tTitle.fCX * 0.5f),
		int(m_tTitle.fY + m_tTitle.fCY * 0.5f));

	for (int i = 0; i < TILESETY; ++i)
	{
		for (int j = 0; j < TILESETX; ++j)
		{
			const int INDEX = i * TILESETX + j;

			if(0 > INDEX || INDEX >= TILESETX * TILESETY)
				break;

			HPEN hPen;
			HPEN hOldPen;

			switch (m_vecCOLs[INDEX]->iOption)
			{
			case TILE_BACK:
				hPen	 = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
				hOldPen = (HPEN)SelectObject(_hdc, hPen);

				Rectangle(_hdc,
					int(m_vecCOLs[INDEX]->fX - TILECX * 0.5f),
					int(m_vecCOLs[INDEX]->fY - TILECY * 0.5f),
					int(m_vecCOLs[INDEX]->fX + TILECX * 0.5f),
					int(m_vecCOLs[INDEX]->fY + TILECY * 0.5f));

			/*	Ellipse(_hdc,
					int(m_vecCOLs[INDEX]->fX - TILECX * 0.5f),
					int(m_vecCOLs[INDEX]->fY - TILECY * 0.5f),
					int(m_vecCOLs[INDEX]->fX + TILECX * 0.5f),
					int(m_vecCOLs[INDEX]->fY + TILECY * 0.5f));*/

				SelectObject(_hdc, hOldPen);
				DeleteObject(hPen);
				break;

			case  TILE_BLOCK:
				hPen	 = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
				hOldPen = (HPEN)SelectObject(_hdc, hPen);

				Rectangle(_hdc,
					int(m_vecCOLs[INDEX]->fX - TILECX * 0.5f),
					int(m_vecCOLs[INDEX]->fY - TILECY * 0.5f),
					int(m_vecCOLs[INDEX]->fX + TILECX * 0.5f),
					int(m_vecCOLs[INDEX]->fY + TILECY * 0.5f));

				MoveToEx(_hdc, int(m_vecCOLs[INDEX]->fX - TILECX * 0.5f), int(m_vecCOLs[INDEX]->fY - TILECY * 0.5f), NULL);
				LineTo(_hdc, int(m_vecCOLs[INDEX]->fX + TILECX * 0.5f), int(m_vecCOLs[INDEX]->fY + TILECY * 0.5f));
				MoveToEx(_hdc, int(m_vecCOLs[INDEX]->fX - TILECX * 0.5f), int(m_vecCOLs[INDEX]->fY + TILECY * 0.5f), NULL);
				LineTo(_hdc, int(m_vecCOLs[INDEX]->fX + TILECX * 0.5f), int(m_vecCOLs[INDEX]->fY - TILECY * 0.5f));
				SelectObject(_hdc, hOldPen);
				DeleteObject(hPen);
				break;
			}
		}
	}
}

void CCollisionTool::Release()
{
	int iSize = m_vecCOLs.size();
	for (int i = 0; i < iSize; ++i)
		Safe_Delete(m_vecCOLs[i]);
}
