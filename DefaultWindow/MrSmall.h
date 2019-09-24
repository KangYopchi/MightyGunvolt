#pragma once
#include "enemy.h"

class CMrSmall :
	public CEnemy
{
private:
	bool			m_bLand;
	bool			m_bSkill;
	int				m_iCircleCount;

public:
	virtual	void	SetProcess();
	virtual	void	SetAction();
	virtual void	CollisionWithMap();
	virtual	void	SetScene();			
	virtual	void	SetSprites();		
	virtual void	MoveFrame();

public:
	virtual void	Initialize();	 
	virtual	int		Progress();		 
	virtual void	Render(HDC _hdc);
private:
	virtual void	Release();	

public:
	CMrSmall();
	~CMrSmall();
};
