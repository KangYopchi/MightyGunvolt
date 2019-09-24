#pragma once

/// Header
#include "enum.h"

class CScene;

class CSceneMgr
{
private:
	static CSceneMgr*	m_pInstance;
	CScene*				m_pScene;

public:
	void SetScene(SCENEID _eID);
	void Progress(void);
	void Render(HDC hdc);
	void Release(void);

public:
	static CSceneMgr* GetInstance(void)
	{
		if(m_pInstance == NULL)
			m_pInstance = new CSceneMgr;

		return m_pInstance;
	}

	void DestroyInstance(void)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}

public:
	CSceneMgr();
	~CSceneMgr();
};
