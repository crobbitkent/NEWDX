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
	 void JumpBack(float fTime);

	 void JumpEnd(float fTime);



	 void SetCurrentState(BUG_STATE  eState);
	 void SetAnimation();

	void OnBlock(class CColliderBase* pSrc, class CColliderBase* pDest, float fTime);





protected:
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

	bool m_bDead = false;

	DIR_TYPE m_eDir;

	DIR_TYPE m_eMoveBackDir;

	bool m_bMoveBack;
	float m_fMoveBackTimeMax;
	float m_fMoveBackTime;

	int m_iHP;

	BUG_STATE m_eState;
	std::vector<std::string> m_vecStateName;

	// 날아가며 죽기
	float m_fJumpTime = 0.f;
	float m_fJumpTotalTime = 1.f;
	bool m_bJump = false;

	float m_fCurrentForce;
	float m_fMoveSpeed;
};
