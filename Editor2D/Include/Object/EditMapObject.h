#pragma once

#include "Object/GameObject.h"

class CEditMapObject :
	public CGameObject
{
	GAMEOBJECT_DEFAULT()

public:
	CEditMapObject();
	~CEditMapObject();

private:
	class CTileMap*	m_pTileMap;

public:
	virtual bool Init();
	virtual void Begin();
	virtual void Update(float fTime);
	virtual void Render(float fTime);
};

