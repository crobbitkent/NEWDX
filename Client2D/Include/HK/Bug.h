#pragma once

#include "Object/GameObject.h"
#include "../Client.h"

class Bug : public CGameObject
{
	GAMEOBJECT_DEFAULT();

public:
	Bug();
	~Bug();

	virtual bool Init();
	virtual void Begin();
	virtual void Update(float fTime);
	virtual void Render(float fTime);

	void MoveX(float fTime);
	void Reverse();
	void CheckFront();
	void MoveBack(float fTime);

	void JumpEnd(float fTime);

	void OnBlock(class CColliderBase* pSrc, class CColliderBase* pDest, float fTime);


private:
	class CStaticMeshComponent* m_pMesh;

	class CCharacterMovementComponent* m_pMovement;
	class CAnimation2D* m_pAnimation;
	float m_fFireTime;

	class CColliderRect* m_pBody;
	int rotationNumber;

	// SENCER
	class Sencer* m_pRightSencer;
	class Sencer* m_pLeftSencer;
	// class MonsterEffect* m_pEffect;

	DIR_TYPE m_eDir;

	DIR_TYPE m_eMoveBackDir;

	bool m_bMoveBack;
	float m_fMoveBackTimeMax;
	float m_fMoveBackTime;

	int m_iHP;
};
