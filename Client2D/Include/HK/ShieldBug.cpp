#include "ShieldBug.h"

#include "EngineGlobals.h"

#include "Resource/Animation2DSequence.h"
#include "Component/Animation2D.h"

#include "Component/ColliderRect.h"
#include "Component/ColliderOBB2D.h"

#include "Sencer.h"

#include "../RandomNumber.h"

ShieldBug::ShieldBug()
{
}


ShieldBug::~ShieldBug()
{
	SAFE_RELEASE(m_pPlayerLeftSencer);
	SAFE_RELEASE(m_pPlayerRightSencer);

}

bool ShieldBug::Init()
{
	if (false == Bug::Init())
	{
		BOOM
			return false;
	}

	Bug::SetAnimation("SB");
	m_pMesh->SetPivot(0.5f, 0.52f, 0.f);

	// �÷��̾� ã������ ����
	m_pPlayerLeftSencer = m_pScene->SpawnObject<Sencer>();
	m_pSencerBody = m_pPlayerLeftSencer->GetBody();
	m_pMesh->AddChild(m_pSencerBody, TR_POS);
	SAFE_RELEASE(m_pSencerBody);


	m_pPlayerRightSencer = m_pScene->SpawnObject<Sencer>();
	m_pSencerBody = m_pPlayerRightSencer->GetBody();

	m_pMesh->AddChild(m_pSencerBody, TR_POS);
	SAFE_RELEASE(m_pSencerBody);

	m_fMoveSpeed = 150.f;

	m_pMesh->SetRelativeScale(600.f, 600.f, 1.f);

	m_pPlayerLeftSencer->SetRelativePos(-300.f, 0.f, 0.f);
	m_pPlayerRightSencer->SetRelativePos(300.f, 0.f, 0.f);

	m_bDieLand = true;

	m_pBody->AddBlockCallback<ShieldBug>(this, &ShieldBug::OnBlock);

	return true;
}

void ShieldBug::Begin()
{
	Bug::Begin();

	m_pMovement->SetMoveSpeed(m_fMoveSpeed);

	// Bug::TurnOff();
}

void ShieldBug::Update(float fTime)
{
	Bug::Update(fTime);

	// ���ư��� �������� �ƹ��͵� ���Ѵ�.
	if (false == m_bOnLand)
	{
		return;
	}

	// ����
	if (true == m_bDead)
	{
		// m_pMesh->SetPivot(0.5f, 0.54f, 0.f);
		return;
	}

	// DEBUG
	char	strText[256] = {};
	sprintf_s(strText, (m_vecStateName[m_eState] + "\n").c_str());
	OutputDebugStringA(strText);


	if (false == m_bChildUpdate)
	{
		return;
	}



	// ���Ⱑ ������
	if (BS_TURN == m_eState && true == m_pAnimation->IsSequenceEnd())
	{

		// �����ϱ� ���� ���� �ƴ� ��츸
		if (false == m_bWillBlock)
		{
			SetCurrentState(BS_WALK);
			m_bWillBlock = false;
			return;
		}
		else
		{
			SetCurrentState(BS_BLOCK);
			m_bBlocking = true;
			m_bWillBlock= false;
			return;
		}


		return;
	}


	///////////////////////////////////////////////////////////////
	// ��ٷ�
	if (true == m_bWaiting)
	{
		m_fWaitingTime += fTime;

		if (m_fWaitingTime >= m_fWaitingTotalTime)
		{
			m_fWaitingTime = 0.f;
			m_bWaiting = false;
		}
		else
		{
			m_pMovement->SetMoveSpeed(0.f);
			m_fMoveSpeed = 0.f;
			return;
		}
	}


	// �غ��� ������ �޸���
	if (BS_BLOCK == m_eState && m_pAnimation->IsSequenceEnd())
	{
		m_bBlocking = false;
		int ran = RandomNumber::GetRandomNumber(0, 2);
		m_pMovement->SetMoveSpeed(600.f);
		m_fMoveSpeed = 600.f;
		if (0 == ran)
		{
			SetCurrentState(BS_ATTACKB);	
			return;
		}
		else
		{
			SetCurrentState(BS_ATTACKA);
			return;
		}
		/*SetCurrentState(BS_ATTACKB);
		m_pMovement->SetMoveSpeed(600.f);
		m_fMoveSpeed = 600.f;*/

		return;
	}

	// �����ϰ� �ִٰ� ���ư���
	if (BS_ATTACKA == m_eState && true == m_pAnimation->IsSequenceEnd())
	{
		SetCurrentState(BS_AATTACK);
		return;
	}
	// �����ϰ� �ִٰ� ���ư���
	if (BS_ATTACKB == m_eState && true == m_pAnimation->IsSequenceEnd())
	{
		SetCurrentState(BS_WAIT);
		m_pMovement->SetMoveSpeed(0.f);
		m_fMoveSpeed = 0.f;

		return;
	}
	// �����ϰ� �ִٰ� ���ư���
	if (BS_ATTACKC == m_eState && true == m_pAnimation->IsSequenceEnd())
	{
		SetCurrentState(BS_AATTACK);

		return;
	}
	// ���ڸ�
	if (BS_AATTACK == m_eState && true == m_pAnimation->IsSequenceEnd())
	{
		SetCurrentState(BS_STAND);

		return;
	}
	// B -> C ����
	if (BS_WAIT == m_eState && true == m_pAnimation->IsSequenceEnd())
	{
		SetCurrentState(BS_ATTACKC);
		m_pMovement->SetMoveSpeed(600.f);
		m_fMoveSpeed = 600.f;
		return;
	}




	// �ȴٰ� �Ǵ� ���ִٰ� �÷��̾ ����

	if (BS_WALK == m_eState)
	{
		// ���� ���� �ִµ� ���ʿ� �ִ� ���
		if (true == m_pPlayerLeftSencer->IsPlayer() && DIR_LEFT == m_eDir)
		{
			SetCurrentState(BS_BLOCK);
			m_fMoveSpeed = 0.f;
			m_pMovement->SetMoveSpeed(0.f);
			m_bBlocking = true;
			return;
		}
		// ���� ���� �ִµ� �����ʿ� �ִ� ���
		else if (true == m_pPlayerRightSencer->IsPlayer() && DIR_LEFT == m_eDir)
		{
			SetCurrentState(BS_TURN);
			m_bWillBlock = true;
			Reverse();
			m_eDir = DIR_RIGHT;
			return;
		}
		// ������ ���� �ִµ� ���ʿ� �ִ� ���
		else if (true == m_pPlayerLeftSencer->IsPlayer() && DIR_RIGHT == m_eDir)
		{
			SetCurrentState(BS_TURN);
			m_bWillBlock = true;
			Reverse();
			m_eDir = DIR_LEFT;
			return;
		}
		// ������ ���� �ִµ� �����ʿ� �ִ� ���
		else if (true == m_pPlayerRightSencer->IsPlayer() && DIR_RIGHT == m_eDir)
		{
			SetCurrentState(BS_BLOCK);
			m_fMoveSpeed = 0.f;
			m_pMovement->SetMoveSpeed(0.f);
			m_bBlocking = true;
			return;
		}
	}




	// �ȱ� ���ư���
	if (BS_STAND == m_eState)
	{
		SetCurrentState(BS_WALK);
		m_fMoveSpeed = 150.f;
		m_pMovement->SetMoveSpeed(m_fMoveSpeed);

		return;
	}




}

void ShieldBug::Render(float fTime)
{
	Bug::Render(fTime);
}

void ShieldBug::MoveX(float fTime)
{
	Bug::MoveX(fTime);
}

void ShieldBug::Reverse()
{
	Bug::Reverse();
}

void ShieldBug::CheckFront()
{
	Bug::CheckFront();
}

void ShieldBug::MoveBack(float fTime)
{
	Bug::MoveBack(fTime);
}

void ShieldBug::JumpBack(float fTime)
{
	Bug::JumpBack(fTime);
}

void ShieldBug::JumpEnd(float fTime)
{
	Bug::JumpEnd(fTime);
}

void ShieldBug::AttackA(float fTime)
{

}

void ShieldBug::AttackB(float fTime)
{

}




void ShieldBug::SetCurrentState(BUG_STATE eState)
{
	Bug::SetCurrentState(eState);
}


void ShieldBug::OnBlock(CColliderBase * pSrc, CColliderBase * pDest, float fTime)
{
	if (true == m_bBlocking)
	{

	}
	else
	{
		Bug::OnBlock(pSrc, pDest, fTime);
	}

}

void ShieldBug::ClearState()
{

}
