#pragma once

class CKeyMgr
{
private:
	static CKeyMgr* m_pInstance;
private:
	DWORD	m_dwKey;

public:
	void	KeyCheck();
	DWORD	GetKey() const { return m_dwKey; };

/// Instance function
public:
	static CKeyMgr*	GetInstance()
	{
		if (m_pInstance == NULL)
			m_pInstance = new CKeyMgr;

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
	CKeyMgr();
	~CKeyMgr();
};
