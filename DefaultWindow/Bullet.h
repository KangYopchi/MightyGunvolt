#pragma once
#include "obj.h"

class CBullet :
	public CObj
{
protected:
	const float		PI;				
	const float 	DTR;			
	const float 	RTD;

protected:
	int		m_iHit;
	int		m_iType;
	DWORD	m_dwFrameTime;
	float	m_fAngle;
	float	m_fSec;
	float	m_fSpeed;
	FRAME	m_tFrame;
	
public:
	virtual void	SetDIR(float _fDIR)		{ m_fSpeed *= _fDIR; };
	virtual void	SetAngle(float _fAngle) { m_fAngle = _fAngle; };
	virtual void	SetHit(int _iHit)		{ m_iHit = _iHit; };
	virtual int		GetType()				{ return m_iType; };
	virtual void	MoveFrame();
	virtual int		CollisionWithMap();

public:
	virtual void	Initialize()		PURE;
	virtual int		Progress();
	virtual void	Render(HDC _hdc)	PURE;
private:
	virtual void	Release();

public:
	CBullet();
	virtual ~CBullet();
};
