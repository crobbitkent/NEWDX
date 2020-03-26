#include "CharacterMovementComponent.h"

CCharacterMovementComponent::CCharacterMovementComponent()
{
	SetTypeID<CCharacterMovementComponent>();
}

CCharacterMovementComponent::CCharacterMovementComponent(const CCharacterMovementComponent & com) :
	CMovementComponent(com)
{
}

CCharacterMovementComponent::~CCharacterMovementComponent()
{
}

bool CCharacterMovementComponent::Init()
{
	if (!CMovementComponent::Init())
		return false;

	return true;
}

void CCharacterMovementComponent::Begin()
{
	CMovementComponent::Begin();
}

void CCharacterMovementComponent::Update(float fTime)
{
	CMovementComponent::Update(fTime);
}
