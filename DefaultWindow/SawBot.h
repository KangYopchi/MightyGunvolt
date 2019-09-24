#pragma once
#include "enemy.h"

class CSawBot :
	public CEnemy
{
private:
	int	m_iChargeCount;
	int	m_iDashCount;
	int	m_iMeleeCount;
public:
	virtual	void	SetProcess();
	virtual	void	SetScene();
	virtual	void	SetSprites();
	virtual	void	SetAction();
	virtual void	CollisionWithMap();

public:
	virtual void	Initialize();
	virtual	int		Progress();
	virtual void	Render(HDC _hdc);
private:
	virtual void	Release();

public:
	CSawBot();
	~CSawBot();
};
