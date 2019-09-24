#pragma once

/// Headers
#include "struct.h"
#include "value.h"

static FPOINT GetMouse()
{
	POINT pt;

	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	return FPOINT(float(pt.x), float(pt.y));
}

template<typename T>
static void Safe_Delete(T& _rTemp)
{
	if (_rTemp)
	{
		delete _rTemp;
		_rTemp = NULL;
	}
}