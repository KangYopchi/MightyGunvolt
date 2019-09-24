#pragma once
#include "enemy.h"

class CFireMan :
	public CEnemy
{
public:
	virtual	void	SetProcess();		
	virtual	void	SetScene();			
	virtual	void	SetSprites();		
	virtual	void	SetAction();

public:
	virtual void	Initialize();	 
	virtual	int		Progress();		 
	virtual void	Render(HDC _hdc);
private:
	virtual void	Release();	

public:
	CFireMan();
	~CFireMan();
};
