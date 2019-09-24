#pragma once

/// Header
#include "struct.h"

class CCollisionTool
{
const int TILESETX;
const int TILESETY;

private:
	vector<TILE*>			m_vecCOLs;

private:
	INFO		m_tInfo;
	TILE		m_tTitle;
	bool		m_bToolMove;
	DWORD		m_dwKey;
/// Mouse Coodernate
private:
	FPOINT		m_tStandard;
	FPOINT		m_tStart;
	FPOINT		m_tCurrent;
	FPOINT		m_tDistance;
	FPOINT		m_tTitleDistance;
	FPOINT		m_tTileDistance[500];
/// Tile Options
private:
	int			m_iOption;
public:
	int		GetOption()		{ return m_iOption; };

public:
	void	Initialize();
	int 	Progress();
	void	Render(HDC _hdc);
private:
	void	Release();

public:
	CCollisionTool();
	~CCollisionTool();
};
