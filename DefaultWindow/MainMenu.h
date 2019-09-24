#pragma once
#include "scene.h"

class CMainMenu :
	public CScene
{
public:
	void	Initialize();
	int		Progress();
	void	Render(HDC _hdc);
private:
	void	Release();


public:
	CMainMenu();
	~CMainMenu();
};
