#pragma once

#include "Bug.h"

class DashingBug : public Bug
{
public:
	DashingBug();
	~DashingBug();

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



};

