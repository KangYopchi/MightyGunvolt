#pragma once

/// Headers
#include "define.h"
#include "struct.h"
#include "value.h"

class CObj
{
protected:
	const float	RIGHT;
	const float	LEFT;
protected:
	vector<TILE*>*	m_pVecMap;
protected:
	UINT			m_uFlag;
	INFO			m_tInfo;
	FRAME			m_tFrame;

protected:
	static POINT		m_ptScroll;

public:
	INFO	GetInfo()				{ return m_tInfo;	};
	UINT	GetFlag()				{ return m_uFlag;	};
	void	SetFlag(UINT _uFlag)	{ m_uFlag = _uFlag; };
	POINT	GetScroll()				{ return m_ptScroll; };
	
///	 Bridge
public:
	virtual void	SetPos(float _fX, float _fY)	{ m_tInfo.fX = _fX, m_tInfo.fY = _fY; };	
	virtual CObj*	SetMap(vector<TILE*>* _pVecMap) { m_pVecMap = _pVecMap; return this; };

public:
	virtual void	Initialize()	 PURE;
	virtual int		Progress()		 PURE;
	virtual void	Render(HDC _hdc) PURE;
private:
	virtual void	Release()		 PURE;

public:
	CObj();
	CObj(INFO _tInfo);
	virtual ~CObj();
};
