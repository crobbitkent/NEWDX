#pragma once

#include "GameEngine.h"

class ENGINE_DLL CTimerManager
{
private:
	unordered_map<size_t, class CTimer*>	m_mapTimer;

public:
	bool Init();
	void Update();

	bool CreateTimer(const string& strName);
	class CTimer* FindTimer(const string& strName);

	void CreateSlowMotion(float fSetTime);
	void StopSlowMotion();


	DECLARE_SINGLE(CTimerManager)

private:
	float m_fSlowTime = 0.f;
	float m_fSlowTotalTime = 0.f;
	bool m_bSlowMotion = false;
};

