#pragma once

/// Header
#include "struct.h"

class CWorldEdit;

class CMainGame
{
private:
	HDC				m_hdc;
private:
	bool			m_EditOn;
	DWORD			m_dwKey;
	DWORD			m_dwTime;
	TCHAR			m_szFps[64];
	UINT			m_uFps;

private:
	CWorldEdit*			m_pWorldEdit;

public:
	void	Initialize();
	void	Progress();
	void	Render();
private:
	void	Release();

public:
	CMainGame();
	~CMainGame();
};
