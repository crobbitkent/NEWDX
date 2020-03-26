#pragma once


#include "Object/GameObject.h"
#include "../Client.h"



class Sencer : public CGameObject
{
	GAMEOBJECT_DEFAULT();

public:
	Sencer();
	~Sencer();

	virtual bool Init();
	virtual void Begin();
	virtual void Update(float fTime);
	virtual void Render(float fTime);

	class CColliderRect* GetBody() const { return m_pBody;  }

	bool IsOverlap() const;

	void OnBlock(class CColliderBase* pSrc, class CColliderBase* pDest, float fTime);
	void OffBlock(class CColliderBase* pSrc, class CColliderBase* pDest, float fTime);


private:
	class CCharacterMovementComponent*	m_pMovement;
	class CColliderRect* m_pBody;

	bool m_bFree;
};

