#pragma once

/// Header
#include "define.h"

class CBitBmp;

class CScene
{
public:
	virtual void Initialize()		PURE;
	virtual int  Progress()			PURE;
	virtual void Render(HDC _hdc)	PURE;
private:
	virtual void Release()			PURE;

public:
	CScene();
	virtual ~CScene();
};
