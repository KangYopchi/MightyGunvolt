#pragma once

typedef struct tagINFO
{
	float	fX;
	float	fY;
	float	fCX;
	float	fCY;

	tagINFO() { }
	tagINFO(float _fX, float _fY, float _fCX, float _fCY)
		: fX(_fX), fY(_fY), fCX(_fCX), fCY(_fCY)
	{ }
}INFO;

typedef struct tagFPOINT
{
	float	fX;
	float	fY;

	tagFPOINT() { }
	tagFPOINT(float _fX, float _fY)
		: fX(_fX), fY(_fY)
	{ }

}FPOINT;

typedef struct tagTILE
{
	float	fX;
	float	fY;
	float	fCX;
	float	fCY;

	int		iIndex;
	int		iOption;
	int		iDrawX;
	int		iDrawY;

	tagTILE() { }
	tagTILE(float _fX, float _fY, float _fCX, float _fCY) 
		: fX(_fX), fY(_fY), fCX(_fCX), fCY(_fCY)
	{ }
}TILE;

typedef struct tagFRAME
{
	UINT		iStart;
	UINT		iLast;
	UINT		iScene;
	DWORD		dwTime;

	tagFRAME() { }
	tagFRAME(UINT _iStart, UINT _iLast, UINT _iScene, DWORD _dwTime)
		:iStart(_iStart), iLast(_iLast), iScene(_iScene), dwTime(_dwTime)
	{ }

}FRAME;