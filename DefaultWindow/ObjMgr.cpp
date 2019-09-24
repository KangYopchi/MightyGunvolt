#include "StdAfx.h"
#include "ObjMgr.h"

/// Headers
#include "function.h"
#include "value.h"

/// Class
#include "ObjFactory.h"
#include "Player.h"

CObjMgr* CObjMgr::m_pInstance = NULL;

CObjMgr::CObjMgr()
{
}

CObjMgr::~CObjMgr()
{
	Release();
}

void CObjMgr::Initialize()
{
	m_vecObj[OBJ_PLAYER].reserve(2);
	m_vecObj[OBJ_ENEMY].reserve(300);
	m_vecObj[OBJ_BULLET].reserve(300000);
	m_vecObj[OBJ_MAP].reserve(30000);
	m_vecObj[OBJ_HPBAR].reserve(10);
}

void CObjMgr::Progress()
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		int iSize = m_vecObj[i].size();
		for (int j = 0; j < iSize; ++j)
		{
			int iResult = m_vecObj[i][j]->Progress();

			if (iResult)
			{
				Safe_Delete(m_vecObj[i][j]);
			}
		}
	}

	for (int i = 0; i < OBJ_END; ++i)
	{
		int iSize = m_vecObj[i].size();
		vector<CObj*>::iterator ObjectIter		= m_vecObj[i].begin();
		vector<CObj*>::iterator	ObjectIterEnd	= m_vecObj[i].end();

		for (ObjectIter; ObjectIter != ObjectIterEnd; )
		{
			if (!(*ObjectIter))
			{
				ObjectIter = m_vecObj[i].erase(ObjectIter);
				ObjectIterEnd	= m_vecObj[i].end();
			}
			else
				++ObjectIter;
		}
	}
}

void CObjMgr::Render(HDC _hdc)
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		int iSize = m_vecObj[i].size();
		for (int j = 0; j < iSize; ++j)
		{
			m_vecObj[i][j]->Render(_hdc);
		}
	}
}

void CObjMgr::Release()
{
	/// Reset Objects
	for (int i = 0; i < OBJ_END; ++i)
	{
		int iSize = m_vecObj[i].size();
		for (int j = 0; j < iSize; ++j)
		{
			Safe_Delete(m_vecObj[i][j]);
		}
		vector<CObj*>().swap(m_vecObj[i]);
	}
}
