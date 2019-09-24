#pragma once

/// Headers
#include "define.h"
#include "struct.h"

class CCollisionTool;
class CTileTool;

class CWorldEdit
{
private:
	vector<TILE*>		m_vecMap;
private:
	CTileTool*			m_pTileTool;
	CCollisionTool*		m_pCOLTool;
private:
	DWORD	m_dwKey;
	bool	m_bTileToolOn;
	int		m_iTileDraw;
	bool	m_bCOLToolOn;
	int		m_iCOLDraw;
	float	m_fSpeed;
	float	m_fScrollX;
	float	m_fScrollY;

private:
	void	Picking();
	void	SaveData();
	void	ReadData();
	void	Scroll();

public:
	void	Initialize();
	void	Progress();
	void	Render(HDC _hdc);
private:	
	void	Release();

public:
	CWorldEdit();
	~CWorldEdit();
};
