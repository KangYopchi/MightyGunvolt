#pragma once
#include "scene.h"

/// Header
#include "struct.h"

class CLogo :
	public CScene
{
private:
	INFO	m_tInfo;
	FRAME	m_tFrame;
	DWORD	m_dwTime;

public:
	void	Initialize();
	int		Progress();
	void	Render(HDC _hdc);
private:
	void	Release();


public:
	CLogo();
	virtual ~CLogo();
};
