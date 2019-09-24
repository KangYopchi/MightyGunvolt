#include "StdAfx.h"
#include "BitBmp.h"

/// Header
#include "value.h"

CBitBmp::CBitBmp()
{
}

CBitBmp::~CBitBmp()
{
	Release();
}

CBitBmp* CBitBmp::LoadBmp(TCHAR* pFileName)
{
	m_hdc = GetDC(g_hWnd);

	m_MemDC = CreateCompatibleDC(m_hdc);

	ReleaseDC(g_hWnd, m_hdc);

	m_BitMap = (HBITMAP)LoadImage(NULL, pFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	m_OldBmp = (HBITMAP)SelectObject(m_MemDC, m_BitMap);

	return this;
}

void CBitBmp::Release()
{
	SelectObject(m_MemDC, m_OldBmp);
	DeleteObject(m_BitMap);
	DeleteDC(m_MemDC);
}

HDC CBitBmp::GetMemDC()
{
	return m_MemDC;
}
