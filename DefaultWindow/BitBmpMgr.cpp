#include "StdAfx.h"
#include "BitBmpMgr.h"

/// Header
#include "function.h"

CBitBmpMgr* CBitBmpMgr::m_pInstance = NULL;

CBitBmpMgr::CBitBmpMgr()
{
	m_vecImages.reserve(100);
}

CBitBmpMgr::~CBitBmpMgr()
{
	Release();
}

void CBitBmpMgr::Initialize()
{
	///< Maps >///
	m_mapImages.insert(map<string, int>::value_type("Stage1", m_vecImages.size()));
	m_vecImages.push_back((new CBitBmp)->LoadBmp(L"../Resource/Texture/Stage1.bmp"));

	///< Logo >///
	m_mapImages.insert(map<string, int>::value_type("Logo1", m_vecImages.size()));
	m_vecImages.push_back((new CBitBmp)->LoadBmp(L"../Resource/Main/Logo01.bmp"));

	m_mapImages.insert(map<string, int>::value_type("Logo2", m_vecImages.size()));
	m_vecImages.push_back((new CBitBmp)->LoadBmp(L"../Resource/Main/Logo02.bmp"));

	///< HpBar >///
	m_mapImages.insert(map<string, int>::value_type("HpBar", m_vecImages.size()));
	m_vecImages.push_back((new CBitBmp)->LoadBmp(L"../Resource/HpBar/HpBar.bmp"));

	m_mapImages.insert(map<string, int>::value_type("EHpBar", m_vecImages.size()));
	m_vecImages.push_back((new CBitBmp)->LoadBmp(L"../Resource/HpBar/EHpBar.bmp"));

	///< Beck >///
	m_mapImages.insert(map<string, int>::value_type("Beck", m_vecImages.size()));
	m_vecImages.push_back((new CBitBmp)->LoadBmp(L"../Resource/Player/Beck/Beck.bmp"));

	m_mapImages.insert(map<string, int>::value_type("NormalBullet", m_vecImages.size()));
	m_vecImages.push_back((new CBitBmp)->LoadBmp(L"../Resource/Player/Beck/Bullet/NormalBullet.bmp"));

	m_mapImages.insert(map<string, int>::value_type("HalfChargedBullet", m_vecImages.size()));
	m_vecImages.push_back((new CBitBmp)->LoadBmp(L"../Resource/Player/Beck/Bullet/HalfChargedBullet.bmp"));

	m_mapImages.insert(map<string, int>::value_type("FullChargedBullet", m_vecImages.size()));
	m_vecImages.push_back((new CBitBmp)->LoadBmp(L"../Resource/Player/Beck/Bullet/FullChargedBullet.bmp"));


	/// Enemies

	///< Pong >///
	m_mapImages.insert(map<string, int>::value_type("Pong", m_vecImages.size()));
	m_vecImages.push_back((new CBitBmp)->LoadBmp(L"../Resource/Enemies/Pong/Pong.bmp"));

	m_mapImages.insert(map<string, int>::value_type("PongBullet", m_vecImages.size()));
	m_vecImages.push_back((new CBitBmp)->LoadBmp(L"../Resource/Enemies/Pong/PongBullet.bmp"));

	///< SawBot >///
	m_mapImages.insert(map<string, int>::value_type("SawBot", m_vecImages.size()));
	m_vecImages.push_back((new CBitBmp)->LoadBmp(L"../Resource/Enemies/SawBot/SawBot.bmp"));


	///< FireMan >///
	m_mapImages.insert(map<string, int>::value_type("FireMan", m_vecImages.size()));
	m_vecImages.push_back((new CBitBmp)->LoadBmp(L"../Resource/Enemies/FireMan/FireMan.bmp"));
	m_mapImages.insert(map<string, int>::value_type("FireShot", m_vecImages.size()));
	m_vecImages.push_back((new CBitBmp)->LoadBmp(L"../Resource/Enemies/FireMan/FireShot.bmp"));

	/// < Brandish >///
	m_mapImages.insert(map<string, int>::value_type("Brandish", m_vecImages.size()));
	m_vecImages.push_back((new CBitBmp)->LoadBmp(L"../Resource/Enemies/Brandish/Brandish.bmp"));

	/// Bosses

	///< MrHappiness >///
	m_mapImages.insert(map<string, int>::value_type("MrHappiness", m_vecImages.size()));
	m_vecImages.push_back((new CBitBmp)->LoadBmp(L"../Resource/Bosses/MrHappiness/MrHappiness.bmp"));

	m_mapImages.insert(map<string, int>::value_type("MrBullet", m_vecImages.size()));
	m_vecImages.push_back((new CBitBmp)->LoadBmp(L"../Resource/Bosses/MrHappiness/MrBullet.bmp"));

	///< SmallHappiness >>///
	m_mapImages.insert(map<string, int>::value_type("MrSmall", m_vecImages.size()));
	m_vecImages.push_back((new CBitBmp)->LoadBmp(L"../Resource/Bosses/MrHappiness/MrSmall.bmp"));

	m_mapImages.insert(map<string, int>::value_type("SmallBullet", m_vecImages.size()));
	m_vecImages.push_back((new CBitBmp)->LoadBmp(L"../Resource/Bosses/MrHappiness/SmallBullet.bmp"));
}

void CBitBmpMgr::Release()
{
	if (!m_mapImages.empty())
		m_mapImages.clear();

	int iSIZE = m_vecImages.size();
	for (int i = 0; i < iSIZE; ++i)
	{
		Safe_Delete(m_vecImages[i]);
	}
	vector<CBitBmp*>().swap(m_vecImages);
}
