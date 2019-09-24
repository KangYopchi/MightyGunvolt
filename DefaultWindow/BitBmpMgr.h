#pragma once

class CBitBmp;

class CBitBmpMgr
{
private:
	static CBitBmpMgr*	m_pInstance;

private:
	map<string, int>	m_mapImages;
	vector<CBitBmp*>	m_vecImages;

public:
	CBitBmp*	GetImage(string _strImageKey) { return m_vecImages[m_mapImages[_strImageKey]]; };

public:
	void	Initialize();
	void	Release();

public:
	static CBitBmpMgr*	GetInstance()
	{
		if (m_pInstance == NULL)
			m_pInstance = new CBitBmpMgr;
		return m_pInstance;
	}

	void	DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}
public:
	CBitBmpMgr();
	~CBitBmpMgr();
};
