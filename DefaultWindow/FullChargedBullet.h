#pragma once
#include "bullet.h"

class CFullChargedBullet :
	public CBullet
{
public:
	virtual void Initialize();
	virtual void Render(HDC _hdc);
public:
	CFullChargedBullet();
	virtual ~CFullChargedBullet();
};
