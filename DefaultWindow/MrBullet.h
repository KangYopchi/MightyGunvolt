#pragma once
#include "bullet.h"

class CMrBullet :
	public CBullet
{
public:
	virtual void	Initialize();	 
	virtual	int		Progress();		 
	virtual void	Render(HDC _hdc);
private:
	virtual void	Release();	

public:
	CMrBullet();
	~CMrBullet();
};
