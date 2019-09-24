#pragma once
#include "obj.h"

/// Header
#include "enum.h"

class CPlayer :
	public CObj
{
/// Image Size
private:
	const float m_fImageX;
	const float m_fImageY;

/// Debug
private:
	bool	m_bDebug;

/// Status
private:
	CObj*	m_pHpBar;
	int		m_iMaxHP;
	int		m_iCurrHP;
	bool	m_bJump;
	bool	m_bFall;

private:
	DWORD	m_dwDirection;
	DWORD	m_dwCacheDir;
	DWORD	m_dwKey;
	DWORD	m_dwState;
	TCHAR*	m_szSetScene;
	
	/// < Attack > ///
private:
	bool	m_bNoCharged;
	bool	m_bHalfCharged;
	bool	m_bFullCharged;
	DWORD	m_dwATKTime;
	DWORD	m_dwChargedTime;
	DWORD	m_dwFrameTime;
	FRAME	m_tChargedFrame;
	int		m_iCharged;

	/// < Dash> ///
private:               
	bool	m_bDash;
	int		m_iDashCount;
	
	/// < Chaged Dash >///
private:
	bool	m_bChargedDashOn;
	bool	m_bChargedDash;
	DWORD	m_dwDashChargedTime;
	int		m_iChargedDashCount;
	int		m_iDashCharged;

private:
	float	m_fSpeed;
	float	m_fSec;
	float	m_fVelocity;
	float	m_fCacheVel;
	float	m_fGravity;

public:
	void	BeckAtk();
	void	BeckDashReset();

	/// < Scroll> ///
private:
	POINT	m_ptOffSet;
public:
	void	ScrollX();
	void	ScrollY();
	
	///<  Default Function >///
public:
	void	KeyInput();
	void	SetScene();
	void	SetSprites();
	void	SetAction();
	void	CollisionWithMap();
	void	CollisionWithBullet();
	void	MoveFrame();
	
public:
	void	Initialize();
	int		Progress();
	void	Render(HDC _hdc);
private:
	void	Release();

public:
	CPlayer();
	~CPlayer();
};
