#pragma once
#include "bullet.h"

class CHalfChargedBullet :
	public CBullet
{
public:
	virtual void Initialize();
	virtual void Render(HDC _hdc);
public:
	CHalfChargedBullet();
	~CHalfChargedBullet();
};
