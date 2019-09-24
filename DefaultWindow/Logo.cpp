#include "StdAfx.h"
#include "Logo.h"

#include <Vfw.h>
#include "SceneMgr.h"
#pragma comment(lib, "vfw32.lib")

/// Header
#include "value.h"

/// Classes
#include "BitBmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"

CLogo::CLogo()
{
}

CLogo::~CLogo()
{
	Release();
}

void CLogo::Initialize()
{
	m_tInfo.fX = WINCX * 0.5f;
	m_tInfo.fY = WINCY * 0.5f;
	m_tInfo.fCX = 800.f;
	m_tInfo.fCY = 600.f;
	m_tFrame.iStart = 0;
	m_tFrame.iLast  = 1;
	m_tFrame.dwTime = 300;

	m_dwTime = GetTickCount();
}

int CLogo::Progress()
{
	if (m_dwTime + m_tFrame.dwTime < GetTickCount())
	{
		m_dwTime = GetTickCount();

		++m_tFrame.iStart;

		if (m_tFrame.iStart > 1)
			m_tFrame.iStart = 0;
	}

	if (CKeyMgr::GetInstance()->GetKey() & KEY_RETURN)
	{
		CSceneMgr::GetInstance()->SetScene(SCE_STAGE);
		return 0;
	}


	return 0;
}

void CLogo::Render(HDC _hdc)
{

	if (m_tFrame.iStart == 0)
	{
		TransparentBlt(_hdc, 
					int(m_tInfo.fX - m_tInfo.fCX * 0.5f),
					int(m_tInfo.fY - m_tInfo.fCY * 0.5f),
					int(m_tInfo.fCX),
					int(m_tInfo.fCY),
					CBitBmpMgr::GetInstance()->GetImage("Logo1")->GetMemDC(), 
					0,
					0,
					int(m_tInfo.fCX),
					int(m_tInfo.fCY), RGB(0, 255, 80));
	}

	else if (m_tFrame.iStart == 1)
	{
		TransparentBlt(_hdc, 
					int(m_tInfo.fX - m_tInfo.fCX * 0.5f),
					int(m_tInfo.fY - m_tInfo.fCY * 0.5f),
					int(m_tInfo.fCX),
					int(m_tInfo.fCY),
					CBitBmpMgr::GetInstance()->GetImage("Logo2")->GetMemDC(), 
					0,
					0,
					int(m_tInfo.fCX),
					int(m_tInfo.fCY), RGB(0, 255, 80));
	}
}

void CLogo::Release()
{
	
}
