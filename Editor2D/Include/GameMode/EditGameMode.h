#pragma once

#include "Scene/GameMode.h"

class CEditGameMode :
	public CGameMode
{
public:
	CEditGameMode();
	~CEditGameMode();

public:
	virtual bool Init();

private:
	bool CreateMaterial();
	bool CreateAnimation2DSequence();
};

