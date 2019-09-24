#pragma once
#include "enemy.h"

class CBrandish :
	public CEnemy
{
private:
	bool	m_bStopMove;
	int		m_iStopMoveCount;
	int		m_iStopMoveRelease;

	bool	m_bSpinMove;
	int		m_iSpinMoveCount;

	bool	m_bTackle;
	int		m_iChargeTackle;
	int		m_iTackleCount;
	
public:
	void	ActionReset();

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
	CBrandish();
	~CBrandish();
};
