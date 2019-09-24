#pragma once

/// Header
#include "struct.h"
  
class CKeyMgr;

class CTileTool
{
/// TILEX, Y
const int TILESETX;
const int TILESETY;

private:
	vector<TILE*>			m_vecTile;
	vector<CBitBmp*>		m_vecBmp;
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
	FPOINT		m_tTileDistance[500];		/// TILESETX * TILESETY
/// Tile Options
private:
	int			m_iDrawX;
	int			m_iDrawY;
	int			m_iOption;

public:
	void	MoveTool();
public:
	int		GetDrawX()		{ return m_iDrawX; };
	int		GetDrawY()		{ return m_iDrawY; };
	int		GetOption()		{ return m_iOption; };

public:
	void	Initialize();
	int 	Progress();
	void	Render(HDC _hdc);
private:
	void	Release();

public:
	CTileTool();
	~CTileTool();
};
