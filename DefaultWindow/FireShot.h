#pragma once
#include "bullet.h"

class CFireShot :
	public CBullet
{
private:
	POINTFLOAT		m_ptStartPoint;

public:
	virtual void	MoveFrame();

public:
	virtual void	Initialize();	 
	virtual	int		Progress();		 
	virtual void	Render(HDC _hdc);
private:
	virtual void	Release();	

public:
	CFireShot();
	~CFireShot();
};
