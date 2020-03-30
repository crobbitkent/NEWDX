#include "Bug.h"

#include "EngineGlobals.h"

#include "Resource/Animation2DSequence.h"
#include "Component/Animation2D.h"

#include "Component/ColliderRect.h"
#include "Component/ColliderOBB2D.h"

#include "Sencer.h"

#include "MonsterHitEffect.h"
#include "HollowKnight.h"

Bug::Bug()
	: m_eDir(DIR_LEFT)
	, m_eMoveBackDir(DIR_LEFT)
	, m_pRightSencer(nullptr)
	, m_pLeftSencer(nullptr)
	, m_bMoveBack(false)
	, m_fMoveBackTime(0.f)
	, m_fMoveBackTimeMax(0.1f)
	, m_iHP(3)
{
	m_eState = (BUG_STATE)4;
	m_fCurrentForce = 2.f;
	m_fMoveSpeed = 300.f;

}

Bug::~Bug()
{
	SAFE_RELEASE(m_pBody);
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pMovement);
	SAFE_RELEASE(m_pAnimation);

	SAFE_RELEASE(m_pRightSencer);
	SAFE_RELEASE(m_pLeftSencer);
}

bool Bug::Init()
{
	if (false == CGameObject::Init())
	{
		return false;
	}

	m_pMesh = CGameObject::CreateComponent<CStaticMeshComponent>("Mesh");
	m_pAnimation = CAnimation2D::CreateAnimation2D<CAnimation2D>();

	m_pBody = CGameObject::CreateComponent<CColliderRect>("BugBody");


	CStaticMesh* pMesh = (CStaticMesh*)GET_SINGLE(CResourceManager)->FindMesh("TexRect");
	m_pMesh->SetStaticMesh(pMesh);
	SAFE_RELEASE(pMesh);

	SetRoot(m_pMesh);

	m_pMesh->AddChild(m_pBody, TR_POS | TR_ROT);







	m_pMesh->SetAnimation2D(m_pAnimation);



	m_pMovement = CGameObject::CreateComponent<CCharacterMovementComponent>("Movement");
	m_pMovement->SetUpdateComponent(m_pMesh);


	m_pMovement->SetMoveSpeed(m_fMoveSpeed);


	/////////////////////////////

	CMaterial* pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("BugMaterial");
	m_pMesh->SetMaterial(pMaterial);
	SAFE_RELEASE(pMaterial);


// 	m_pMesh->SetRelativePos(1000.f, 500.f, 0.f);
	m_pMesh->SetRelativeScale(400.f, 400.f, 1.f);
	m_pMesh->SetPivot(0.5f, 0.585f, 0.f);



	m_pBody->SetExtent(150.f, 150.f);
	m_pBody->SetPivot(0.5f, 0.5f, 0.f);
	m_pBody->AddBlockCallback<Bug>(this, &Bug::OnBlock);
	m_pBody->SetCollisionProfile("Monster");

	// 중력 적용
	CGameObject::SetPhysics(true);

	// 센서 장착
	// RIGHT
	m_pRightSencer = m_pScene->SpawnObject<Sencer>();

	CColliderRect* pSencerBody = m_pRightSencer->GetBody();
	m_pMesh->AddChild(pSencerBody, TR_POS);

	pSencerBody->SetRelativePos((m_pBody->GetExtent().x * 0.5f + 1), -(m_pBody->GetExtent().y * 0.5f + 1), 0.f);

	// LEFT
	m_pLeftSencer = m_pScene->SpawnObject<Sencer>();

	pSencerBody = m_pLeftSencer->GetBody();
	m_pMesh->AddChild(pSencerBody, TR_POS);
	
	pSencerBody->SetRelativePos(-(m_pBody->GetExtent().x * 0.5f + 1), -(m_pBody->GetExtent().y * 0.5f + 1), 0.f);

	m_pBody->SetMonster(true);




	SetAnimation();
	SetCurrentState(BS_WALK);

	SetForce(m_fCurrentForce);

	return true;
}

void Bug::Begin()
{
	CGameObject::Begin();
}

void Bug::Update(float fTime)
{
	CGameObject::Update(fTime);



	if (true == m_bOnLand)
	{
		ClearGravity();
	}

	bool anim = m_pAnimation->IsSequenceEnd();

	if (true == m_bDead)
	{
		return;
	}


	if(BS_DEAD == m_eState && true == anim)
	{
		m_bDead = true;

		return;
	}


	if (true == m_bJump)
	{
		JumpBack(fTime);
	}
	else if(BS_DEAD != m_eState)
	{
		CheckFront();

		MoveBack(fTime);
	}


	

	// m_pMovement->AddRotationZ(180.f * fTime * rotationNumber);
}

void Bug::Render(float fTime)
{
	CGameObject::Render(fTime);
}

void Bug::MoveX(float fTime)
{
	/*switch (m_eDir)
	{
	case DIR_LEFT:
		Flip(1);
		break;
	case DIR_RIGHT:
		Flip(0);
		break;
	default:
		break;
	}*/

	Flip(m_eDir);

	m_pMovement->AddMovement(GetWorldAxis(AXIS_X) * m_eDir);
}

void Bug::Reverse()
{

	Flip(m_eDir);

}

// false면 REVERSE해야한다.
void Bug::CheckFront()
{
	if (DIR_LEFT == m_eDir)
	{
		// 왼쪽으로 가고 있는데 왼쪽 센서가...
		if (true == m_pLeftSencer->IsOverlap())
		{
			Reverse();
			m_eDir = DIR_RIGHT;
			return;
		}
	}

	if(DIR_RIGHT == m_eDir)
	{
		// 오른쪽으로 가고 있는데 왼쪽 센서가...
		if (true == m_pRightSencer->IsOverlap())
		{
			Reverse();
			m_eDir = DIR_LEFT;
			return;
		}
	}

	return;
}

void Bug::MoveBack(float fTime)
{
	if (true == m_bMoveBack)
	{
		m_fMoveBackTime += fTime;

		m_pMovement->SetMoveSpeed(2000.f);

		m_pMovement->AddMovement(GetWorldAxis(AXIS_X) * m_eMoveBackDir);

		if (m_fMoveBackTime >= m_fMoveBackTimeMax)
		{
			m_pMovement->SetMoveSpeed(300.f);

			m_bMoveBack = false;
			m_fMoveBackTime = 0.f;
		}	
	}
	else
	{
		MoveX(fTime);
	}
}

void Bug::JumpBack(float fTime)
{
	if (false == m_bJumping)
	{
		m_pMovement->SetMoveSpeed(m_fMoveSpeed);

		m_fForce = m_fOriginForce * 400;
		
		if (BS_DIE != m_eState)
		{
			SetCurrentState(BS_DIE);
		}


		// 큰 먼지 생성
		// DustEffect* dust = m_pScene->SpawnObject<DustEffect>(
		//	GetWorldPos() - Vector3(0.f, 400.f * 0.2f, 0.f));
		// dust->SetStaticSize(200.f);
		// SAFE_RELEASE(dust);
		m_bJumping = true;

		m_bOnLand = false;
	}

	m_pMovement->AddMovement(GetWorldAxis(AXIS_X) * m_eMoveBackDir);
	m_fForce -= m_fOriginForce * m_fOriginForce;
}

void Bug::JumpEnd(float fTime)
{
	m_fForce = 0.f;
}







void Bug::SetCurrentState(BUG_STATE eState)
{
	if (eState == m_eState)
	{
		return;
	}

	m_eState = eState;

	

	std::string stateName = "BUG_";
	stateName.append(m_vecStateName[eState]);

	m_pAnimation->ChangeAnimation(stateName);
}

void Bug::SetAnimation()
{
	m_pAnimation->AddAnimation2DSequence("BUG_WALK");
	m_pAnimation->AddAnimation2DSequence("BUG_TURN");
	m_pAnimation->AddAnimation2DSequence("BUG_DIE");
	m_pAnimation->AddAnimation2DSequence("BUG_DEAD");

	m_vecStateName.push_back("WALK");
	m_vecStateName.push_back("TURN");
	m_vecStateName.push_back("DIE");
	m_vecStateName.push_back("DEAD");

}




void Bug::OnBlock(CColliderBase * pSrc, CColliderBase * pDest, float fTime)
{
	int a = 0;

	if (nullptr == pDest)
	{
		return;
	}

	if ("PlayerProjectile" == pDest->GetCollisionProfile()->strName)
	{
		m_bMoveBack = true;

		MonsterHitEffect* attack = m_pScene->SpawnObject<MonsterHitEffect>(GetWorldPos());

		SAFE_RELEASE(attack);

		HollowKnight* player = (HollowKnight*)(m_pScene->GetGameMode()->GetPlayer());

		m_eMoveBackDir = player->GetDirection();


		m_iHP -= 1;

		if (0 >= m_iHP)
		{	
			m_bJump = true;
			m_fMoveBackTimeMax = 0.2f;
			m_fMoveSpeed = 500.f;
		}
	}


	if (true == pDest->IsStage())
	{
		if (true == m_bJump)
		{
			m_fCurrentForce = m_fCurrentForce * 0.05;
			m_fMoveSpeed = m_fMoveSpeed * 0.25;

			if (m_fCurrentForce <= 0.05f)
			{
				SetCurrentState(BS_DEAD);
				m_bJump = false;
				m_bMoveBack = false;
			}
			else
			{
				ClearGravity();
				m_pMovement->AddMovement(Vector3(0.f, pSrc->GetIntersect().y * 2.f, 0.f));
				SetForce(m_fCurrentForce);
				m_bJump = true;
				m_bJumping = false;
				return;
			}

			
		}

		ClearGravity();
		JumpEnd(fTime);

		m_bOnLand = true;

		m_pMovement->AddMovement(Vector3(0.f, pSrc->GetIntersect().y * 2.f, 0.f));
		return;
	}
}



