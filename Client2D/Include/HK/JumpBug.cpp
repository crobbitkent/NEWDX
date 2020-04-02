#include "JumpBug.h"

#include "EngineGlobals.h"

#include "Resource/Animation2DSequence.h"
#include "Component/Animation2D.h"

#include "Component/ColliderRect.h"
#include "Component/ColliderOBB2D.h"

#include "TimerManager.h"

#include "Sencer.h"

JumpBug::JumpBug()
{
}


JumpBug::~JumpBug()
{
	SAFE_RELEASE(m_pPlayerLeftSencer);
	SAFE_RELEASE(m_pPlayerRightSencer);

}

bool JumpBug::Init()
{
	if (false == Bug::Init())
	{
		BOOM
			return false;
	}

	Bug::SetAnimation("JB");
	m_pMesh->SetPivot(0.5f, 0.48f, 0.f);

	// �÷��̾� ã������ ����
	m_pPlayerLeftSencer = m_pScene->SpawnObject<Sencer>();
	m_pSencerBody = m_pPlayerLeftSencer->GetBody();
	m_pMesh->AddChild(m_pSencerBody, TR_POS);
	SAFE_RELEASE(m_pSencerBody);


	m_pPlayerRightSencer = m_pScene->SpawnObject<Sencer>();
	m_pSencerBody = m_pPlayerRightSencer->GetBody();

	m_pMesh->AddChild(m_pSencerBody, TR_POS);
	SAFE_RELEASE(m_pSencerBody);

	m_fMoveSpeed = 100.f;

	// m_pPlayerLeftSencer->SetRelativePos(-300.f, 0.f, 0.f);
	// m_pPlayerRightSencer->SetRelativePos(300.f, 0.f, 0.f);

	m_fGravitySpeed = 16.f;

	m_bDieLand = true;

	m_pBody->AddBlockCallback<JumpBug>(this, &JumpBug::OnBlock);

	return true;
}

void JumpBug::Begin()
{
	Bug::Begin();

	m_pMovement->SetMoveSpeed(m_fMoveSpeed);

	Bug::TurnOff();
}

void JumpBug::Update(float fTime)
{
	Bug::Update(fTime);

	if (true == m_bWillJump)
	{
		int a = 0;
	}

	// DEBUG
	char	strText[256] = {};
	sprintf_s(strText, (m_vecStateName[m_eState] + "\n").c_str());
	OutputDebugStringA(strText);

	// ���ο��� ����
	m_bSlowAdjust = GET_SINGLE(CTimerManager)->IsSlowMotion();
	if (true == m_bSlowAdjust)
	{
		if (false == m_bSlowDone)
		{
			m_fMoveSpeed *= 2.f;
			m_bSlowDone = true;
		}		
	}
	else
	{
		if (true == m_bSlowDone)
		{
			m_fMoveSpeed *= 0.5f;
			m_bSlowDone = false;
		}
	}
	

	// ����
	if (true == m_bDead || 0 >= m_iHP)
	{
		m_pMesh->SetPivot(0.5f, 0.54f, 0.f);
		return;
	}


	if (BS_STAND == m_eState)
	{
		if (true == m_pAnimation->IsSequenceEnd())
		{
			SetCurrentState(BS_WALK);
			return;
		}
		else
		{
			return;
		}

	}

	// ���Ⱑ ������
	if (BS_TURN == m_eState && true == m_pAnimation->IsSequenceEnd())
	{
		// �����ϱ� ���� ���� �ƴ� ��츸
		if (false == m_bWillJump)
		{
			SetCurrentState(BS_WALK);
			m_bWillJump = false;
			return;
		}
		else
		{
			SetCurrentState(BS_BJUMP);
			RealJump(fTime);
			m_bWillJump = false;
			return;
		}

	}


	//////////////////////////////////////////////////////////////////////////////////////////

	// ����
	if (BS_BJUMP == m_eState && true == m_pAnimation->IsSequenceEnd())
	{
		SetCurrentState(BS_JUMP);
		return;
	}	

	// ������ �ȱ�
	if (BS_ATTACK == m_eState && true == m_pAnimation->IsSequenceEnd())
	{
		SetCurrentState(BS_STAND);
		return;
	}

	//// ���ư��� �������� �ƹ��͵� ���Ѵ�.
	if (false == m_bOnLand || BS_ATTACK == m_eState)
	{
		return;
	}

	// ����
	if (true == m_bIsJumping)
	{
		if (true == m_bOnLand)
		{
			m_bIsJumping = false;
			SetCurrentState(BS_ATTACK);
			m_fMoveSpeed = 100.f;
			m_pMovement->SetMoveSpeed(m_fMoveSpeed);
			m_bWillJump = false;

			return;
		}

		Bug::CheckCollision();

		m_pMovement->AddMovement(GetWorldAxis(AXIS_X) * m_eDir);
		m_fForce -= m_fOriginForce * m_fOriginForce;
		return;
	}

	

	// �÷��̾� ã��
	if (false == m_bWillJump)
	{
		Vector3 playerVec = m_pScene->GetGameMode()->GetPlayer()->GetWorldPos();
		Vector3 monsterVec = GetWorldPos();

		m_fDisX = playerVec.x - monsterVec.x;
		float disY = playerVec.y - monsterVec.y;

		// �÷��̾�� ������ �Ÿ��� ���Ʒ� 250 ����
		if (disY <= 250.f && disY >= -250.f)
		{
			if (m_fDisX <= 400.f && m_fDisX >= -400.f)
			{
				if (m_fDisX <= 400.f && m_fDisX >= 0.f)
				{
					// �÷��̾ ������
					// �ٵ� ���� ����
					if (DIR_LEFT == m_eDir)
					{
						SetCurrentState(BS_TURN);
						Reverse();
						m_eDir = DIR_RIGHT;
						m_bWillJump = true;
						return;
					}
					// ���� ������
					else
					{
						SetCurrentState(BS_BJUMP);
						RealJump(fTime);
						return;
					}
				}
				else
				{
					// �÷��̾ ����
					// ���� ����
					if (DIR_LEFT == m_eDir)
					{
						SetCurrentState(BS_BJUMP);
						RealJump(fTime);
						return;
					}
					else
					{
						SetCurrentState(BS_TURN);
						Reverse();
						m_eDir = DIR_LEFT;
						m_bWillJump = true;
						return;
					}
				}

		
			}
		}
	}
	
	

	
	








}

void JumpBug::Render(float fTime)
{
	Bug::Render(fTime);
}

void JumpBug::MoveX(float fTime)
{
	Bug::MoveX(fTime);
}

void JumpBug::Reverse()
{
	Bug::Reverse();
}

void JumpBug::CheckFront()
{
	Bug::CheckFront();
}

void JumpBug::MoveBack(float fTime)
{
	Bug::MoveBack(fTime);
}

void JumpBug::JumpBack(float fTime)
{
	Bug::JumpBack(fTime);
}

void JumpBug::JumpEnd(float fTime)
{
	Bug::JumpEnd(fTime);
}



void JumpBug::RealJump(float fTime)
{
	// parent
	m_bJumping = true;
	m_bOnLand = false;


	// JB
	m_bIsJumping = true;

	m_fForce = m_fOriginForce * 60;

	m_fMoveSpeed = m_fDisX;

	if (m_fMoveSpeed <= 0)
	{
		m_fMoveSpeed *= -1.f;
	}

	m_pMovement->SetMoveSpeed(m_fMoveSpeed);

}

void JumpBug::SetCurrentState(BUG_STATE eState)
{
	Bug::SetCurrentState(eState);
}


void JumpBug::OnBlock(CColliderBase * pSrc, CColliderBase * pDest, float fTime)
{
	Bug::OnBlock(pSrc, pDest, fTime);
}

void JumpBug::ClearState()
{

}






//// �ȴٰ� �Ǵ� ���ִٰ� �÷��̾ ����

//if (BS_WALK == m_eState)
//{
//	// ���� ���� �ִµ� ���ʿ� �ִ� ���
//	if (true == m_pPlayerLeftSencer->IsPlayer() && DIR_LEFT == m_eDir)
//	{
//		SetCurrentState(BS_BDASH);
//		m_fWalkTime = 0.f;
//		m_fDashTime = 0.f;
//		m_pMovement->SetMoveSpeed(0.f);
//		return;
//	}
//	// ���� ���� �ִµ� �����ʿ� �ִ� ���
//	else if (true == m_pPlayerRightSencer->IsPlayer() && DIR_LEFT == m_eDir)
//	{
//		SetCurrentState(BS_TURN);
//		// ���� �ൿ ����
//		m_fWalkTime = 0.f;
//		m_fDashTime = 0.f;
//		m_bWillDash = true;
//		Reverse();
//		m_eDir = DIR_RIGHT;
//		return;
//	}
//	// ������ ���� �ִµ� ���ʿ� �ִ� ���
//	else if (true == m_pPlayerLeftSencer->IsPlayer() && DIR_RIGHT == m_eDir)
//	{
//		SetCurrentState(BS_TURN);
//		m_fWalkTime = 0.f;
//		m_fDashTime = 0.f;
//		// ���� �ൿ ����
//		m_bWillDash = true;
//		Reverse();
//		m_eDir = DIR_LEFT;
//		return;
//	}
//	// ������ ���� �ִµ� �����ʿ� �ִ� ���
//	else if (true == m_pPlayerRightSencer->IsPlayer() && DIR_RIGHT == m_eDir)
//	{
//		SetCurrentState(BS_BDASH);
//		m_fWalkTime = 0.f;
//		m_fDashTime = 0.f;
//		m_pMovement->SetMoveSpeed(0.f);

//		return;
//	}
//}




//// �غ��� ������ �޸���
//if (BS_BWALK == m_eState && m_pAnimation->IsSequenceEnd())
//{
//	SetCurrentState(BS_DASH);
//	m_pMovement->SetMoveSpeed(500.f);
//	return;
//}

//// �뽬�ϰ� �ִٰ� ���ư���
//if (BS_DASH == m_eState)
//{
//	m_fDashTime += fTime;

//	SetCurrentState(BS_DASH);

//	if (m_fDashTime >= m_fDashTotalTime)
//	{
//		m_fDashTime = 0.f;
//		SetCurrentState(BS_WALK);
//		m_fWalkTime = 0.f;
//		return;
//	}
//}

//// �ȱ� ���ư���
//if (BS_WALK == m_eState)
//{
//	m_fWalkTime += fTime;
//	SetCurrentState(BS_WALK);
//	m_pMovement->SetMoveSpeed(m_fMoveSpeed);

//	// �ȿ����̱�
//	if (m_fWalkTime >= m_fWalkTotalTime)
//	{
//		SetCurrentState(BS_WALK);
//		m_pMovement->SetMoveSpeed(0.f);
//		m_fWalkTime = 0.f;
//		return;
//	}

//	return;
//}

// ���ִٰ� ���ư���
//if (BS_STAND == m_eState)
//{
//	m_fStandTime += fTime;

//	SetCurrentState(BS_STAND);
//	// �ȿ����̱�
//	m_pMovement->SetMoveSpeed(0.f);

//	if (m_fStandTime >= m_fStandTotalTime)
//	{
//		// �ٽ� �����̱�
//		m_fStandTime = 0.f;
//		SetCurrentState(BS_WALK);
//		m_pMovement->SetMoveSpeed(m_fMoveSpeed);
//		return;
//	}
//}

// �ȱⰡ �����ٸ�
// if(BS_WALK == m_eSate)