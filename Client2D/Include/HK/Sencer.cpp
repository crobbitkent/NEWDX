#include "Sencer.h"

#include "EngineGlobals.h"
#include "Input.h"

#include "Scene/Scene.h"

#include "Device.h"

#include "Component/CameraComponent.h"
#include "Resource/Animation2DSequence.h"
#include "Component/Animation2D.h"



#include "Resource/ResourceManager.h"
#include "Resource/Material.h"
#include "Component/ColliderSphere2D.h"
#include "Component/ColliderRect.h"

Sencer::Sencer()
	: m_pBody(nullptr)
	, m_pMovement(nullptr)
{
}

Sencer::~Sencer()
{
	SAFE_RELEASE(m_pBody);
	SAFE_RELEASE(m_pMovement);
}

bool Sencer::Init()
{
	if (false == CGameObject::Init())
	{
		return false;
	}

	m_pBody = CreateComponent<CColliderRect>("SencerBody");
	m_pBody->SetExtent(2.f, 2.f);
	m_pBody->SetPivot(0.5f, 0.5f, 0.f);
	m_pBody->AddBlockCallback<Sencer>(this, &Sencer::OnBlock);
	m_pBody->AddEndOverlapCallback<Sencer>(this, &Sencer::OffBlock);
	m_pBody->SetCollisionProfile("Player");

	m_pBody->EnableOverlap(true);

	m_pMovement = CGameObject::CreateComponent<CCharacterMovementComponent>("Movement");
	m_pMovement->SetUpdateComponent(m_pBody);

	SetRoot(m_pBody);

	return true;
}

void Sencer::Begin()
{
	CGameObject::Begin();
}

void Sencer::Update(float fTime)
{
	CGameObject::Update(fTime);


}

void Sencer::Render(float fTime)
{
	CGameObject::Render(fTime);
}

bool Sencer::IsOverlap() const
{
	return m_bFree;
}

void Sencer::OnBlock(CColliderBase * pSrc, CColliderBase * pDest, float fTime)
{
	m_bFree = false;
}

void Sencer::OffBlock(CColliderBase * pSrc, CColliderBase * pDest, float fTime)
{
	m_bFree = true;
}

