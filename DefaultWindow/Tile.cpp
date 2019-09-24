#include "StdAfx.h"
#include "Tile.h"

/// Header
#include "enum.h"
#include "value.h"

/// Class
#include "BitBmpMgr.h"

CTile::CTile()
{
	m_tInfo.fCX = TILECX;
	m_tInfo.fCY = TILECY;
}

CTile::CTile(INFO _tInfo, int _iDrawX, int _iDrawY, int _iIndex, int _iOption)
: CObj(_tInfo), m_iDrawX(_iDrawX), m_iDrawY(_iDrawY), m_iIndex(_iIndex), m_iOption(_iOption)
{
}

CTile::~CTile()
{
}

void CTile::Initialize()
{
	
}

int CTile::Progress()
{
	return 0;
}

void CTile::Render(HDC _hdc)
{
	TransparentBlt(_hdc, 
		int(m_tInfo.fX - TILECX * 0.5f),
		int(m_tInfo.fY - TILECY * 0.5f),
		TILECX, TILECY, CBitBmpMgr::GetInstance()->GetImage("Stage1")->GetMemDC(), 
		m_iDrawX * TILECX, m_iDrawY * TILECY, TILECX, TILECY, RGB(94, 0, 82));

	HPEN hPen;
	HPEN hOldPen;

	switch (m_iOption)
	{
	case TILE_BACK:
		hPen	 = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
		hOldPen = (HPEN)SelectObject(_hdc, hPen);

		Rectangle(_hdc,
			int(m_tInfo.fX - TILECX * 0.5f),
			int(m_tInfo.fY - TILECY * 0.5f),
			int(m_tInfo.fX + TILECX * 0.5f),
			int(m_tInfo.fY + TILECY * 0.5f));

		/*Ellipse(_hdc,
			int(m_tInfo.fX - TILECX * 0.5f),
			int(m_tInfo.fY - TILECY * 0.5f),
			int(m_tInfo.fX + TILECX * 0.5f),
			int(m_tInfo.fY + TILECY * 0.5f));*/

		SelectObject(_hdc, hOldPen);
		DeleteObject(hPen);
		break;

	case  TILE_BLOCK:
		hPen	 = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		hOldPen = (HPEN)SelectObject(_hdc, hPen);

		Rectangle(_hdc,
			int(m_tInfo.fX - TILECX * 0.5f),
			int(m_tInfo.fY - TILECY * 0.5f),
			int(m_tInfo.fX + TILECX * 0.5f),
			int(m_tInfo.fY + TILECY * 0.5f));

		MoveToEx(_hdc, int(m_tInfo.fX - TILECX * 0.5f), int(m_tInfo.fY - TILECY * 0.5f), NULL);
		LineTo(_hdc, int(m_tInfo.fX + TILECX * 0.5f), int(m_tInfo.fY + TILECY * 0.5f));
		MoveToEx(_hdc, int(m_tInfo.fX - TILECX * 0.5f), int(m_tInfo.fY + TILECY * 0.5f), NULL);
		LineTo(_hdc, int(m_tInfo.fX + TILECX * 0.5f), int(m_tInfo.fY - TILECY * 0.5f));
		SelectObject(_hdc, hOldPen);
		DeleteObject(hPen);
		break;
	}
}

void CTile::Release()
{
}
