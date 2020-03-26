#include "TestStage.h"

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

TestStage::TestStage()
	: m_pMaterial(nullptr)
	, m_pBody(nullptr)
	, m_pMesh(nullptr)
	, m_pMovement(nullptr)
{
}

TestStage::~TestStage()
{
	SAFE_RELEASE(m_pBody);
	SAFE_RELEASE(m_pMaterial);
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pMovement);
}

bool TestStage::Init()
{
	if (false == CGameObject::Init())
	{
		return false;
	}

	int scaleX = 2000.f;
	int scaleY = 500.f;

	m_pBody = CreateComponent<CColliderRect>("StageBody");
	m_pBody->SetExtent(scaleX, scaleY);
	m_pBody->SetPivot(0.5f, 0.5f, 0.f);
	m_pBody->AddBlockCallback<TestStage>(this, &TestStage::OnBlock);
	// m_pBody->SetCollisionProfile("Monster");

	m_pMesh = CreateComponent<CStaticMeshComponent>("Mesh");
	CStaticMesh* pMesh = (CStaticMesh*)GET_SINGLE(CResourceManager)->FindMesh("TexRect");
	m_pMesh->SetStaticMesh(pMesh);
	SAFE_RELEASE(pMesh);

	m_pMesh->AddChild(m_pBody, TR_POS);

	m_pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("StageMaterial");
	m_pMesh->SetMaterial(m_pMaterial);

	SetRoot(m_pMesh);

	m_pMesh->SetRelativePos(0.f, 0.f, 0.f);
	m_pMesh->SetRelativeScale(scaleX, scaleY, 1.f);
	m_pMesh->SetPivot(0.5f, 0.5f, 0.f);

	m_pBody->SetStage(true);

	m_pMovement = CGameObject::CreateComponent<CCharacterMovementComponent>("Movement");
	m_pMovement->SetUpdateComponent(m_pMesh);

	return true;
}

void TestStage::Begin()
{
	CGameObject::Begin();
}

void TestStage::Update(float fTime)
{
	CGameObject::Update(fTime);
}

void TestStage::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void TestStage::SetScale(const Vector3 & vScale)
{
	m_pMesh->SetRelativeScale(Vector3(vScale.x, vScale.y, 1.f));
	m_pBody->SetExtent(vScale.x, vScale.y);
}

void TestStage::OnBlock(CColliderBase * pSrc, CColliderBase * pDest, float fTime)
{
	int a = 0;
}
