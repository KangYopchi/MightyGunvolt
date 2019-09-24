#pragma once

///Headers
#include "define.h"
#include "enum.h"

class CObj;

class CObjMgr
{
private:
	static CObjMgr*		m_pInstance;

private:
	vector<CObj*>		m_vecObj[OBJ_END];

public:
	vector<CObj*>*		GetObj(OBJID _eID)	{ return &m_vecObj[_eID]; };
	
public:
	void	Initialize();
	void	Progress();
	void	Render(HDC _hdc);
private:
	void	Release();

public:
	static CObjMgr* GetInstance()
	{
		if (m_pInstance == NULL)
			m_pInstance = new CObjMgr;

		return m_pInstance;
	}

	void DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

private:
	CObjMgr();
	~CObjMgr();
};
