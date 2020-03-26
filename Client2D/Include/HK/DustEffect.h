#pragma once

#include "Object/GameObject.h"

class DustEffect : public CGameObject
{
	GAMEOBJECT_DEFAULT()

public:
	DustEffect();
	~DustEffect();

private:
	class CStaticMeshComponent*	m_pMesh;
	class CProjectileMovementComponent*	m_pMovement;
	class CAnimation2D* m_pAnimation;

public:
	virtual bool Init();
	virtual void Begin();
	virtual void Update(float fTime);
	virtual void Render(float fTime);

	class CColliderRect* GetBody() const;

	void SetMaxSize(float fSize);

	void SetStaticSize(float fSize);


private:
	float m_fLifeTime;
	float m_fLifeTotalTime;
	bool m_bSizeDecider = false;
	int m_iDir;
	bool m_bDirDecider = false;


};

