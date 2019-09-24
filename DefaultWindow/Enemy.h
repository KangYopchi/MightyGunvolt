#pragma once
#include "obj.h"

class CEnemy :
	public CObj
{
protected:
	const float		PI;				
	const float 	DTR;			
	const float 	RTD;
protected:
	CObj*		m_pPlayer;
	DWORD		m_dwDirection;
	DWORD		m_dwCacheDir;
	DWORD		m_dwState;
	TCHAR*		m_szSetScene;
	
	bool		m_bAttack;
	bool		m_bFall;
	bool		m_bDead;
	bool		m_bJump;
	bool		m_bShoot;
	bool		m_bReturn;

	int			m_iHitCount;

	DWORD		m_dwActionTime;
	DWORD		m_dwAttackTime;
	float		m_fAngle;
	float		m_fSec;
	float		m_fSpeed;
	float		m_fVelocity;
	float		m_fCacheVel;

	FRAME		m_tFrame;
	DWORD		m_dwFrameTime;	

	POINTFLOAT	m_ptStartPoint;

public:
	virtual	void	SetProcess()		PURE;
	virtual	void	SetScene()			PURE;
	virtual	void	SetSprites()		PURE;
	virtual	void	SetAction()			PURE;
	virtual	void	CollisionWithMap();
	virtual void	CollisionWithBullet();
	virtual void	MoveFrame();

public:
	virtual void	Initialize()	 PURE;	 
	virtual	int		Progress()		 PURE;		 
	virtual void	Render(HDC _hdc) PURE;
private:
	virtual void	Release()		 PURE;	

public:
	CEnemy();
	virtual ~CEnemy();
};
