#pragma once
#include "enemy.h"

class CMrHappiness :
	public CEnemy
{
private:
	CObj*	m_pEHpBar;
	int		m_iMaxHP;
	int		m_iCurrHP;

private:
	bool			m_bLand;
	bool			m_bSkill;
	int				m_iCircleCount;
	int				m_iSummonCount;

public:
	virtual	void	SetProcess();
	virtual	void	SetAction();
	virtual void	CollisionWithMap();
	virtual void	CollisionWithBullet();
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
	CMrHappiness();
	~CMrHappiness();
};
