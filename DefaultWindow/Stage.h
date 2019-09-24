#pragma once
#include "scene.h"

/// Header
#include "struct.h"

class CObj;

class CStage :
	public CScene
{
private:
	CObj*		m_pPlayer;
private:
	bool		m_bDebug;
	DWORD		m_dwDebugKey;
private:
	bool		m_bBoss;
	bool		m_bPong;
	bool		m_bSawBot;
	bool		m_bBrandish;
	bool		m_bFireMan;

	DWORD		m_dwTime;
private:
	vector<TILE*>	m_vecMap;

private:
	POINT	m_ptScroll;

public:
	void	ReadData();
public:
	void	Initialize();
	int		Progress();
	void	Render(HDC _hdc);
private:
	void	Release();

public:
	CStage();
	~CStage();
};
