#pragma once

class CBridge
{
protected:
	const float		PI;						/// PI
	const float 	DTR;					/// Degree to Radian
	const float 	RTD;					/// Radian to Degree

	//
//	float			m_fSpeed;				/// Unit's Original force
//	float			m_fAngle;				
//	float			m_fDistance;			
//	float			m_fVelocity;	
//	UINT			m_uAimFlag;				/// Aimed Object's flag
//
//public:
//	void	SetAimFlag(UINT _uFlag)					 { m_uAimFlag = _uFlag; };
//	UINT	GetAimFlag()							 { return m_uAimFlag; };
//	virtual void		SetAngle(float _fAngle)		 { m_fAngle = _fAngle; };
//	virtual float		GetAngle()					 { return m_fAngle; };
//	void	KeyInput();

public:
	virtual void	Initialize()	 PURE;
	virtual int		Progress()		 PURE;
	virtual void	Render(HDC _hdc) PURE;
private:
	virtual void	Release()		 PURE;

public:
	CBridge();
	~CBridge();
};
