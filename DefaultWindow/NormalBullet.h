#pragma once
#include "bullet.h"

class CNormalBullet :
	public CBullet
{
public:
	virtual void	Initialize();
	virtual void	Render(HDC _hdc);
private:
	virtual void	Release();

public:
	CNormalBullet();
	~CNormalBullet();
};
