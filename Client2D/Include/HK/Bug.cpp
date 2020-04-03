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
	m_bJumping = false;
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

	m_pMesh->SetRelativeScale(400.f, 400.f, 1.f);


	m_pBody->SetExtent(150.f, 150.f);
	m_pBody->SetPivot(0.5f, 0.5f, 0.f);



	m_pBody->SetCollisionProfile("Monster");

	// �߷� ����
	CGameObject::SetPhysics(true);

	// ���� ����
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

	// char	strText[256] = {};
	// sprintf_s(strText, "LEFT = %d, RIGHT = %d\n", m_pLeftSencer->IsOverlap(), m_pRightSencer->IsOverlap());
	// OutputDebugStringA(strText);

	

	if (true == m_bOnLand)
	{
		ClearGravity();
	}

	bool anim = m_pAnimation->IsSequenceEnd();

	if (true == m_bDead)
	{
		return;
	}


	if (BS_DEAD == m_eState && true == anim)
	{
		m_bDead = true;

		return;
	}

	////////////////////////////////////////////////////


	if (BS_TURN == m_eState && true == m_pAnimation->IsSequenceEnd())
	{
		// Reverse();
		m_bChildUpdate = true;
		return;
	}

	



	if (true == m_bJump)
	{
		if (false == m_bOnLand)
		{
			bool leftFree = m_pLeftSencer->IsOverlap();
			bool rightFree = m_pRightSencer->IsOverlap();

			// �����߿� �ٴڿ�
			if (false == leftFree && false == rightFree)
			{
				if (DIR_LEFT == m_eDir)
				{
					// �������� ���� �ִµ� ���� ������...
					if (true == leftFree || true == m_bNoLeft)
					{
						// Reverse();
						m_eDir = DIR_RIGHT;
						m_eMoveBackDir = DIR_RIGHT;
						m_bNoLeft = false;
						m_pLeftSencer->ClearOverlap();
						m_pRightSencer->ClearOverlap();

						
					}
				}
				else if (DIR_RIGHT == m_eDir)
				{
					// ���������� ���� �ִµ� ���� ������...
					if (true == rightFree || true == m_bNoRight)
					{
						// Reverse();
						m_eDir = DIR_LEFT;
						m_eMoveBackDir = DIR_LEFT;
						m_bNoRight = false;
						m_pLeftSencer->ClearOverlap();
						m_pRightSencer->ClearOverlap();

					}
				}
			}
			// �������ε� �������� �ɷȴ�.
			else if (true == leftFree && false == rightFree)
			{
				if (true == m_bNoRight)
				{
					// Reverse();
					m_eDir = DIR_LEFT;
					m_eMoveBackDir = DIR_LEFT;
					m_bNoRight = false;
					m_pLeftSencer->ClearOverlap();
					m_pRightSencer->ClearOverlap();

				}
			}
			else if (false == leftFree && true == rightFree)
			{
				if (true == m_bNoLeft)
				{
					// Reverse();
					m_eDir = DIR_RIGHT;
					m_eMoveBackDir = DIR_RIGHT;
					m_bNoLeft = false;
					m_pLeftSencer->ClearOverlap();
					m_pRightSencer->ClearOverlap();

				}
			}			
		}
		JumpBack(fTime);
	}
	else if (BS_DEAD != m_eState)
	{
		if (true == m_bOnLand && false == m_bLandPhysics)
		{
			CheckFront();
		}
		else
		{
			CheckCollision();
		}

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

// false�� REVERSE�ؾ��Ѵ�.
void Bug::CheckFront()
{
	if (DIR_LEFT == m_eDir)
	{
		// �������� ���� �ִµ� ���� ������...
		if (true == m_pLeftSencer->IsOverlap())
		{
			SetCurrentState(BS_TURN);
			m_eDir = DIR_RIGHT;
			m_bNoLeft = false;
			m_pLeftSencer->ClearOverlap();
			m_pRightSencer->ClearOverlap();
			m_bChildUpdate = false;
			return;
		}
	}

	if (DIR_RIGHT == m_eDir)
	{
		// ���������� ���� �ִµ� ���� ������...
		if (true == m_pRightSencer->IsOverlap())
		{
			SetCurrentState(BS_TURN);
			m_eDir = DIR_LEFT;
			m_bNoRight = false;
			m_pLeftSencer->ClearOverlap();
			m_pRightSencer->ClearOverlap();
			m_bChildUpdate = false;
			return;
		}
	}

	CheckCollision();

	return;
}

// false�� REVERSE�ؾ��Ѵ�.
void Bug::CheckCollision()
{
	if (DIR_LEFT == m_eDir)
	{
		// �������� ���� �ִµ� ���� ������...
		if (true == m_bNoLeft)
		{
			SetCurrentState(BS_TURN);
			m_eDir = DIR_RIGHT;
			m_bNoLeft = false;
			m_pLeftSencer->ClearOverlap();
			m_pRightSencer->ClearOverlap();
			m_pMovement->SetMoveSpeed(0.f);
			m_bChildUpdate = false;
			return;
		}
	}

	if (DIR_RIGHT == m_eDir)
	{
		// ���������� ���� �ִµ� ���� ������...
		if (true == m_bNoRight)
		{
			SetCurrentState(BS_TURN);
			m_eDir = DIR_LEFT;
			m_bNoRight = false;
			m_pLeftSencer->ClearOverlap();
			m_pRightSencer->ClearOverlap();
			m_pMovement->SetMoveSpeed(0.f);
			m_bChildUpdate = false;
			return;
		}
	}

	m_pMovement->SetMoveSpeed(m_fMoveSpeed);

	return;
}

void Bug::MoveBack(float fTime)
{
	if (true == m_bNoMoveBack)
	{
		return;
	}

	if (true == m_bMoveBack)
	{
		m_fMoveBackTime += fTime;

		m_pMovement->SetMoveSpeed(2000.f);

		if (true == m_bNoRight && DIR_RIGHT == m_eMoveBackDir)
		{

		}
		else if (true == m_bNoLeft && DIR_LEFT == m_eMoveBackDir)
		{

		}
		else
		{
			m_pMovement->AddMovement(GetWorldAxis(AXIS_X) * m_eMoveBackDir);
		}

		

		if (m_fMoveBackTime >= m_fMoveBackTimeMax)
		{
			m_pMovement->SetMoveSpeed(m_fMoveSpeed);

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

		// ������ �׻� �����ϰ� ���ư���.
		ClearGravity();
		m_fGravitySpeed = 2.f;

		m_fForce = m_fOriginForce * 400;


		// ū ���� ����
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



	std::string stateName = m_strAniName;
	stateName.append("_");
	stateName.append(m_vecStateName[eState]);

	m_pAnimation->ChangeAnimation(stateName);
}

void Bug::SetAnimation(const string& strAniName)
{
	//m_strAniName = strAniName;
	//m_strAniName.append("_WALK");
	//m_pAnimation->AddAnimation2DSequence(m_strAniName);
	//m_strAniName.clear();
	//m_strAniName = strAniName;
	//m_strAniName.append("_TURN");
	//m_pAnimation->AddAnimation2DSequence(m_strAniName);
	//m_strAniName.clear();
	//m_strAniName = strAniName;
	//m_strAniName.append("_DIE");
	//m_pAnimation->AddAnimation2DSequence(m_strAniName);
	//m_strAniName.clear();
	//m_strAniName = strAniName;
	//m_strAniName.append("_DEAD");
	//m_pAnimation->AddAnimation2DSequence(m_strAniName);
	//m_strAniName.clear();
	//m_strAniName = strAniName;
	//m_strAniName.append("_DASH");
	//m_pAnimation->AddAnimation2DSequence(m_strAniName);
	//m_strAniName.clear();
	//m_strAniName = strAniName;
	//m_strAniName.append("_BWALK");
	//m_pAnimation->AddAnimation2DSequence(m_strAniName);
	//m_strAniName.clear();
	//m_strAniName = strAniName;
	//m_strAniName.append("_STAND");
	//m_pAnimation->AddAnimation2DSequence(m_strAniName);
	//m_strAniName.clear();
	//m_strAniName = strAniName;
	//m_strAniName.append("_BDASH");
	//m_pAnimation->AddAnimation2DSequence(m_strAniName);
	//m_strAniName.clear();
	//m_strAniName = strAniName;
	//m_strAniName.append("_BDIE");
	//m_pAnimation->AddAnimation2DSequence(m_strAniName);
	//m_strAniName.clear();
	//m_strAniName = strAniName;
	//m_strAniName.append("_BJUMP");
	//m_pAnimation->AddAnimation2DSequence(m_strAniName);
	//m_strAniName.clear();
	//m_strAniName = strAniName;
	//m_strAniName.append("_JUMP");
	//m_pAnimation->AddAnimation2DSequence(m_strAniName);
	//m_strAniName.clear();
	//m_strAniName = strAniName;
	//m_strAniName.append("_DIELAND");
	//m_pAnimation->AddAnimation2DSequence(m_strAniName);
	//m_strAniName.clear();
	//m_strAniName = strAniName;
	//m_strAniName.append("_ATTACK");
	//m_pAnimation->AddAnimation2DSequence(m_strAniName);



	m_vecStateName.push_back("WALK");
	m_vecStateName.push_back("TURN");
	m_vecStateName.push_back("DIE");
	m_vecStateName.push_back("DEAD");
	m_vecStateName.push_back("DASH");
	m_vecStateName.push_back("BWALK");
	m_vecStateName.push_back("STAND");

	m_vecStateName.push_back("BDASH");
	m_vecStateName.push_back("BDIE");

	m_vecStateName.push_back("BJUMP");
	m_vecStateName.push_back("JUMP");
	m_vecStateName.push_back("DIELAND");
	m_vecStateName.push_back("ATTACK");

	m_vecStateName.push_back("ATTACKA");
	m_vecStateName.push_back("ATTACKB");
	m_vecStateName.push_back("ATTACKC");

	m_vecStateName.push_back("WAIT");

	m_vecStateName.push_back("AATTACK");
	m_vecStateName.push_back("BLOCK");
	m_vecStateName.push_back("BLOCKHIT");


	for (int i = 0; i < (int)BS_OVER; ++i)
	{
		m_strAniName.clear();
		m_strAniName = strAniName;
		m_strAniName.append("_");
		m_strAniName.append(m_vecStateName[i]);
		m_pAnimation->AddAnimation2DSequence(m_strAniName);
	}

	m_strAniName.clear();
	m_strAniName = strAniName;

	SetCurrentState(BS_WALK);
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

		if (true == m_bDead)
		{
			return;
		}

		if (0 >= m_iHP)
		{
			m_bJump = true;
			m_fMoveBackTimeMax = 0.2f;
			m_fMoveSpeed = 500.f;
			m_bLandPhysics = true;
			m_bJumping = false;
			SetCurrentState(BS_DIE);
			return;
		}
	}


	if (true == pDest->IsStage())
	{
		int playerPos = (int)pSrc->GetIntersect().z;

		switch (playerPos)
		{
		case 1: // LEFT
			m_pMovement->AddMovement(Vector3(pSrc->GetIntersect().x * -2.f, 0.f, 0.f));
			m_bNoRight = true;

			break;
		case 2: // TOP
			m_pMovement->AddMovement(Vector3(0.f, pSrc->GetIntersect().y * 2.f, 0.f));
			ClearGravity();
			JumpEnd(fTime);
			m_bOnLand = true;
			m_bNoRight = false;
			m_bNoLeft = false;
			m_pLeftSencer->ClearOverlap();
			m_pRightSencer->ClearOverlap();
			break;
		case 3: // RIGHT
			m_pMovement->AddMovement(Vector3(pSrc->GetIntersect().x * 2.f, 0.f, 0.f));
			m_bNoLeft = true;

			break;
		case 4: // BOTTOM
			m_pMovement->AddMovement(Vector3(0.f, pSrc->GetIntersect().y * -2.f, 0.f));

			// m_bCeiling = true;
			break;
		default:
			BOOM
				break;
		}


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

				if (true == m_bDieLand)
				{
					SetCurrentState(BS_DIELAND);
				}

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



