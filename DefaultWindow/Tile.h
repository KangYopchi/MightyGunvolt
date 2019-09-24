#pragma once
#include "obj.h"

class CTile :
	public CObj
{
protected:
	int		m_iIndex;
	int		m_iOption;
	int		m_iDrawX;
	int		m_iDrawY;

public:
	void	SetIndex(int _iIndex)	{ m_iIndex = _iIndex; };
	int		GetIndex()				{ return m_iIndex; };
	void	SetOption(int _iOption) { m_iOption = _iOption; };
	int		GetOption()				{ return m_iOption; };
	void	SetDrawX(int _iDrawX)	{ m_iDrawX = _iDrawX; };
	int		GetDrawX()				{ return m_iDrawX; };
	void	SetDrawY(int _iDrawY)	{ m_iDrawY = _iDrawY; };
	int		GetDrawY()				{ return m_iDrawY; };

public:
	void	Initialize();
	int		Progress();
	void	Render(HDC _hdc);
private:
	void	Release();

public:
	CTile();
	CTile(INFO _tInfo, int _iDrawX, int _iDrawY, int _iIndex, int _iOpiton);
	~CTile();
};
