
#include "../stdafx.h"


#include "EditGameMode.h"
#include "../Object/EditMapObject.h"
#include "Scene/Scene.h"
#include "Input.h"
#include "Resource/ResourceManager.h"
#include "Resource/Animation2DSequence.h"
#include "Resource/Material.h"

#include "Engine.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

CEditGameMode::CEditGameMode()
{
}

CEditGameMode::~CEditGameMode()
{
}

bool CEditGameMode::Init()
{
	CGameMode::Init();

	CreateMaterial();


	CEditMapObject*	pEditMap = m_pScene->SpawnObject<CEditMapObject>();

	SAFE_RELEASE(pEditMap);

	/*GET_SINGLE(CResourceManager)->CreateAnimation2DSequence("MouseDefault", true, 1.f, 13);

	for (int i = 0; i <= 12; ++i)
	{
		TCHAR	strFileName[MAX_PATH] = {};

		wsprintf(strFileName, TEXT("Mouse\\Default\\%d.png"), i);

		char	strKey[256] = {};
		sprintf_s(strKey, "MouseDefault%d", i);

		GET_SINGLE(CResourceManager)->AddAnimation2DSequenceTexture("MouseDefault", strKey, strFileName);
	}

	GET_SINGLE(CResourceManager)->SetAnimation2DSequenceFrameInfoAll("MouseDefault", Vector2(0.f, 0.f),
		Vector2(32.f, 31.f));

	GET_SINGLE(CInput)->CreateAnim2D();
	GET_SINGLE(CInput)->AddAnim2DSequence("MouseDefault");
	GET_SINGLE(CInput)->AlphaBlendEnable();*/

	return true;
}

bool CEditGameMode::CreateMaterial()
{
	GET_SINGLE(CResourceManager)->CreateMaterial("MainMapTileMaterial");

	CMaterial* pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("MainMapTileMaterial");

	pMaterial->SetSubsetShader(TILEMAP_SHADER);
	pMaterial->SetTexture(0, "MainMapTile", TEXT("Tile.bmp"));

	SAFE_RELEASE(pMaterial);

	return true;
}

bool CEditGameMode::CreateAnimation2DSequence()
{
	return true;
}
