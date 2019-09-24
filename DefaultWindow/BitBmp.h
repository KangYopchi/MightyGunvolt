#pragma once
#include "define.h"

class CBitBmp
{
private:
	HDC			m_hdc;
	HDC			m_MemDC;
	HBITMAP		m_BitMap;
	HBITMAP		m_OldBmp;

public:
	CBitBmp*	LoadBmp(TCHAR* pFileName);
	HDC			GetMemDC();

private:
	void		Release();

public:
	CBitBmp();
	~CBitBmp();
};
