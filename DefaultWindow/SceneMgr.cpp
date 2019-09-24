#include "StdAfx.h"
#include "SceneMgr.h"

/// Header
#include "function.h"

/// Classes
#include "Logo.h"
#include "MainMenu.h"
#include "Stage.h"

CSceneMgr* CSceneMgr::m_pInstance = NULL;

CSceneMgr::CSceneMgr()
: m_pScene(NULL)
{
}

CSceneMgr::~CSceneMgr()
{
	Release();
}

void CSceneMgr::SetScene(SCENEID _eID)
{
	if(m_pScene)
		Safe_Delete(m_pScene);

	switch(_eID)
	{
	case SCE_LOGO:
		m_pScene = new CLogo;
		break;
	case SCE_MENU:
		m_pScene = new CMainMenu;
		break;
	case SCE_EDIT:
		break;
	case SCE_STAGE:
		m_pScene = new CStage;
		break;
	}

	m_pScene->Initialize();
}

void CSceneMgr::Progress()
{
	if(m_pScene)
		m_pScene->Progress();
}

void CSceneMgr::Render(HDC _hdc)
{
	if(m_pScene)
		m_pScene->Render(_hdc);
}

void CSceneMgr::Release()
{
	if(m_pScene)
		::Safe_Delete(m_pScene);
}
