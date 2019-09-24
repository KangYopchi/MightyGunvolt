#include "StdAfx.h"
#include "Obj.h"

POINT CObj::m_ptScroll = {};

CObj::CObj()
: m_uFlag(0)
, RIGHT(1.f)
, LEFT(-1.f)
{
	memset(&m_tInfo, NULL, sizeof(INFO));
	memset(&m_tFrame, NULL, sizeof(FRAME));
}

CObj::CObj(INFO _tInfo)
: m_uFlag(0), m_tInfo(_tInfo)
, RIGHT(1)
, LEFT(-1)
{
}

CObj::~CObj()
{
}
