#include "DashingBug.h"

#include "EngineGlobals.h"

#include "Resource/Animation2DSequence.h"
#include "Component/Animation2D.h"

#include "Component/ColliderRect.h"
#include "Component/ColliderOBB2D.h"

#include "Sencer.h"

DashingBug::DashingBug()
{
}


DashingBug::~DashingBug()
{
}

bool DashingBug::Init()
{
	if (false == Bug::Init())
	{
		BOOM
		return false;
	}



	return true;
}

void DashingBug::Begin()
{
	Bug::Begin();
}

void DashingBug::Update(float fTime)
{
	Bug::Update(fTime);
}

void DashingBug::Render(float fTime)
{
	Bug::Render(fTime);
}

void DashingBug::MoveX(float fTime)
{
	Bug::MoveX(fTime);
}

void DashingBug::Reverse()
{
}

void DashingBug::CheckFront()
{
	Bug::CheckFront();
}

void DashingBug::MoveBack(float fTime)
{
	Bug::MoveBack(fTime);
}

void DashingBug::JumpBack(float fTime)
{
	Bug::JumpBack(fTime);
}

void DashingBug::JumpEnd(float fTime)
{
	Bug::JumpEnd(fTime);
}

void DashingBug::SetCurrentState(BUG_STATE eState)
{
	
}

void DashingBug::SetAnimation()
{

}

void DashingBug::OnBlock(CColliderBase * pSrc, CColliderBase * pDest, float fTime)
{
	Bug::OnBlock(pSrc, pDest, fTime);

}
