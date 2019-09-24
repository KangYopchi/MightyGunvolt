#include "StdAfx.h"
#include "MainGame.h"

/// Headers
#include "function.h"
#include "value.h"

/// Classes
#include "BitBmpMgr.h"
#include "KeyMgr.h"
#include "Obj.h"
#include "ObjFactory.h"
#include "ObjMgr.h"
#include "Player.h"
#include "SceneMgr.h"
#include "WorldEdit.h"

CMainGame::CMainGame()
: m_pWorldEdit(NULL)
{
}

CMainGame::~CMainGame() 
{
	Release();
}

void CMainGame:: Initialize()
{
	m_hdc		= GetDC(g_hWnd);
	m_dwKey		= 0;
	m_dwTime	= GetTickCount();
	m_EditOn	= false;
	
 	
	CBitBmpMgr::GetInstance()->Initialize();
	CObjMgr::GetInstance()->Initialize(); 
	CSceneMgr::GetInstance()->SetScene(SCE_LOGO);
	
	/// Editor
	m_pWorldEdit = new CWorldEdit;
	m_pWorldEdit->Initialize();   	
}

void CMainGame:: Progress()
{
	++m_uFps;
		
	CKeyMgr::GetInstance()->KeyCheck();
	
			
	m_dwKey = CKeyMgr::GetInstance()->GetKey();
	if ((m_dwKey & KEY_E) && (m_dwKey & KEY_CTRL))
	{
		if (m_EditOn)
			m_EditOn = false;
		else if (!m_EditOn)
			m_EditOn = true;	
	}

	if (m_EditOn)
		m_pWorldEdit->Progress();
	
	else if (!m_EditOn)
		CSceneMgr::GetInstance()->Progress();
}

void CMainGame:: Render()
{
	/// Double Buffering
	HDC		MemDC	   = CreateCompatibleDC(m_hdc);
	HBITMAP Bitmap	   = CreateBitmap(WINCX, WINCY, 1, 32, NULL);
	SelectObject(MemDC, Bitmap);
	
	Rectangle(MemDC, 0, 0, WINCX, WINCY);

	if(m_dwTime + 1000 < GetTickCount())
	{
		m_dwTime = GetTickCount();
		wsprintf(m_szFps, L"FPS : %d", m_uFps);
		m_uFps = 0;
	}

	SetWindowText(g_hWnd, m_szFps);
	
	if (m_EditOn)
		m_pWorldEdit->Render(MemDC);
	
	else if (!m_EditOn)
		CSceneMgr::GetInstance()->Render(MemDC);
	
	/// Copy DC
	BitBlt(m_hdc, 0, 0, WINCX, WINCY, MemDC, 0, 0, SRCCOPY);
	/// Delete Temp DC
	DeleteDC(MemDC);
	DeleteObject(Bitmap);
}

void CMainGame:: Release()
{
	ReleaseDC(g_hWnd, m_hdc);
	
	Safe_Delete(m_pWorldEdit);

	/// Detroy Singletons
	CBitBmpMgr::GetInstance()->DestroyInstance();
	CKeyMgr::GetInstance()->DestroyInstance();
	CObjMgr::GetInstance()->DestroyInstance();
	CSceneMgr::GetInstance()->DestroyInstance();
}

