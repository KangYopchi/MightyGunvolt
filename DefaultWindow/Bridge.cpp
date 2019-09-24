#include "StdAfx.h"
#include "Bridge.h"

CBridge::CBridge()
: PI(3.141593f)
, DTR(3.141593f * 0.005556f)
, RTD(180.f * 0.318310f)
//: m_uAimFlag(0)
//, m_fSpeed(0.f)		
//, m_fAngle(0.f)		
//, m_fDistance(0.f)	
//, m_fVelocity(0.f)
{
}

CBridge::~CBridge()
{
}

void CBridge::Initialize()
{
}

int CBridge::Progress()
{
	return 0;
}

void CBridge::Render(HDC _hdc)
{
}

void CBridge::Release()
{
}
