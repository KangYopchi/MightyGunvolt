#include "StdAfx.h"
#include "TileTool.h"

/// Headers
#include "function.h"
#include "value.h"

/// Classes
#include "BitBmpMgr.h"
#include "KeyMgr.h"

CTileTool::CTileTool()
: TILESETX(20)
, TILESETY(25)

{
}

CTileTool::~CTileTool()
{
	Release();
}

void CTileTool::Initialize()
{
	m_vecTile.reserve(300);

	m_bToolMove = false;

	m_iDrawX	= 0;
	m_iDrawY	= 0;
	m_iOption	= 0;
	m_dwKey		= 0;

	m_tInfo.fX	= 320.f;
	m_tInfo.fY	= 430.f;
	m_tInfo.fCX = 640.f;
	m_tInfo.fCY = 800.f;

	m_tTitle.fCX = m_tInfo.fCX;
	m_tTitle.fCY = 10.f;
	m_tTitle.fX  = m_tInfo.fX;
	m_tTitle.fY  = m_tInfo.fY - m_tInfo.fCY * 0.5f - m_tTitle.fCY * 0.5f;
	
	m_tStandard.fX = m_tInfo.fX - m_tInfo.fCX * 0.5f;
	m_tStandard.fY = m_tInfo.fY - m_tInfo.fCY * 0.5f;

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
			pTile->iOption		= 0;
			pTile->iDrawX		= j;
			pTile->iDrawY		= i;

			m_vecTile.push_back(pTile);
		}
	}
}

int CTileTool::Progress()
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

						m_tTileDistance[INDEX].fX = m_vecTile[INDEX]->fX - m_tStart.fX;
						m_tTileDistance[INDEX].fY = m_vecTile[INDEX]->fY - m_tStart.fY;

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

				m_vecTile[INDEX]->fX = m_tCurrent.fX + m_tTileDistance[INDEX].fX;
				m_vecTile[INDEX]->fY = m_tCurrent.fY + m_tTileDistance[INDEX].fY;

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

		m_iDrawX  = m_vecTile[INDEX]->iDrawX;
		m_iDrawY  = m_vecTile[INDEX]->iDrawY;
		m_iOption = m_vecTile[INDEX]->iOption;
	}

	return 0;
}

void CTileTool::Render(HDC _hdc)
{

	//if (!m_bToolMove)
	//	return;

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

			TransparentBlt(_hdc, 
				int(m_vecTile[INDEX]->fX - TILECX * 0.5f),
				int(m_vecTile[INDEX]->fY - TILECY * 0.5f),
				TILECX, TILECY, CBitBmpMgr::GetInstance()->GetImage("Stage1")->GetMemDC(), 
				m_vecTile[INDEX]->iDrawX * TILECX, m_vecTile[INDEX]->iDrawY * TILECY, TILECX, TILECY, RGB(94, 0, 82));

			HBRUSH MyBrush,OldBrush;
			MyBrush=(HBRUSH)GetStockObject(NULL_BRUSH);
			OldBrush=(HBRUSH)SelectObject(_hdc,MyBrush);
			Rectangle(_hdc,
				int(m_vecTile[INDEX]->fX - TILECX * 0.5f),
				int(m_vecTile[INDEX]->fY - TILECY * 0.5f),
				int(m_vecTile[INDEX]->fX + TILECX * 0.5f),
				int(m_vecTile[INDEX]->fY + TILECY * 0.5f));
			SelectObject(_hdc,OldBrush);			
		}
	}
}

void CTileTool::Release()
{
	int TILESIZE = m_vecTile.size();
	for (int i = 0; i < TILESIZE; ++i)
		Safe_Delete(m_vecTile[i]);

	int BMPSIZE = m_vecBmp.size();
	for (int i = 0; i < BMPSIZE; ++i)
		Safe_Delete(m_vecBmp[i]);
}