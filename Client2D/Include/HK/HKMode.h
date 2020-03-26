#pragma once



#include "Scene/GameMode.h"

class HKMode : public CGameMode
{
public:
	HKMode();
	~HKMode();

	virtual bool Init();

private:
	bool CreateMaterial();
	bool CreateAnimation2DSequence();
	void MakePlayerAnimation(const std::string& strName, int iFrameCount, int iFrameSecond, const std::string& strFileName);


	void SetCollision();



	// HOLLOW KNIGHT
	void SetHK();
	void SetBug();
	void SetUI();
};

