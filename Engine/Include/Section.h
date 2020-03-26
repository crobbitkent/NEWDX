#pragma once

#include "GameEngine.h"

class ENGINE_DLL CSection
{
	friend class CSectionManager;

private:
	CSection();
	~CSection();

private:
	Vector3		m_vMin;
	Vector3		m_vMax;
	Vector3		m_vSize;
	int			m_iIndex;
	vector<class CColliderBase*>	m_vecCollider;
	unsigned int		m_iColliderCount;

public:
	bool Init(const Vector3& vMin, const Vector3& vMax, int iIndex);
	void AddCollider(class CColliderBase* pCollider);
	void Clear();
	void Collision(float fTime);
	bool Collision(const Vector2& vMousePos, float fTime);

private:
	COLLISION_TYPE FindCollisionType(PCollisionProfile pProfile, const string& strChannelName);
};

