#pragma once

static UINT OBJFLAG;

template< typename T >
class CObjFactory
{
public:
	static CObj* CreateObj()
	{
		CObj* pObj = new T;
		
		pObj->Initialize();
		pObj->SetFlag( OBJFLAG );
		++OBJFLAG;

		return pObj;
	}

	static CObj* CreateObj( float _fX, float _fY )
	{
		CObj* pObj = new T;
		
		pObj->SetPos( _fX, _fY );
		pObj->Initialize();
		pObj->SetFlag( OBJFLAG );
		++OBJFLAG;

		return pObj;
	}

	CObjFactory()
	{
	}

	~CObjFactory()
	{
	}
};
