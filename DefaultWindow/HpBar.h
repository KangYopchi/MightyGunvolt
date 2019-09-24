#pragma once
#include "obj.h"

class CHpBar :
	public CObj
{
private:
	const int m_iOnePoint;
private:
	int		m_iHp;
	int		m_iDamage;
	DWORD	m_dwFrameTime;

public:
	void	SetHp(int _iHp)			{ m_iHp = _iHp; };
	void	SetDamage(int _iDamage) { m_iDamage += _iDamage; };

public:
	void	Initialize();
	int		Progress();
	void	Render(HDC _hdc);
private:
	void	Release();

public:
	CHpBar();
	~CHpBar();
};
