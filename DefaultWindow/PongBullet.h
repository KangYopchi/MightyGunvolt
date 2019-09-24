#pragma once
#include "bullet.h"

class CPongBullet :
	public CBullet
{
public:
	virtual void	Initialize();	 
	virtual	int		Progress();		 
	virtual void	Render(HDC _hdc);
private:
	virtual void	Release();	

public:
	CPongBullet();
	~CPongBullet();
};
