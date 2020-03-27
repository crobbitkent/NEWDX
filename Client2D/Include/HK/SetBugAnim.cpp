#include "HKMode.h"

#include "Scene/Scene.h"
#include "Resource/ResourceManager.h"
#include "Resource/Material.h"
#include "Resource/Animation2DSequence.h"

#include "CollisionManager.h"

#include "HollowKnight.h"
#include "Bug.h"

#include "HKAttackEffect.h"



void HKMode::SetBug()
{
	// Bug
	int frameCount = 4;
	GET_SINGLE(CResourceManager)->CreateAnimation2DSequence("BUG_WALK", true, 0.5f, frameCount);

	for (int i = 0; i < frameCount; ++i)
	{
		TCHAR	strFileName[MAX_PATH] = {};

		wsprintf(strFileName, TEXT("HOLLOW/Monster/BUG/BUG%d.png"), i);

		char strKey[256] = {};
		sprintf_s(strKey, "BUG_WALK%d", i);

		GET_SINGLE(CResourceManager)->AddAnimation2DSequenceTexture("BUG_WALK", strKey, strFileName);
	}

	GET_SINGLE(CResourceManager)->SetAnimation2DSequenceFrameInfoAll("BUG_WALK", Vector2(0.f, 0.f),
		Vector2(400.f, 400.f));


	// Bug Turn
	frameCount = 2;
	GET_SINGLE(CResourceManager)->CreateAnimation2DSequence("BUG_TURN", true, 0.1f, frameCount);

	for (int i = 0; i < frameCount; ++i)
	{
		TCHAR	strFileName[MAX_PATH] = {};

		wsprintf(strFileName, TEXT("HOLLOW/Monster/BUG/BUG_TURN%d.png"), i);

		char strKey[256] = {};
		sprintf_s(strKey, "BUG_TURN%d", i);

		GET_SINGLE(CResourceManager)->AddAnimation2DSequenceTexture("BUG_TURN", strKey, strFileName);
	}

	GET_SINGLE(CResourceManager)->SetAnimation2DSequenceFrameInfoAll("BUG_TURN", Vector2(0.f, 0.f),
		Vector2(400.f, 400.f));




	// BUG DIE
	frameCount = 4;
	GET_SINGLE(CResourceManager)->CreateAnimation2DSequence("BUG_DIE", false, 0.5f, frameCount);

	for (int i = 0; i < frameCount; ++i)
	{
		TCHAR	strFileName[MAX_PATH] = {};

		wsprintf(strFileName, TEXT("HOLLOW/Monster/BUG/BUG_DIE%d.png"), i);

		char strKey[256] = {};
		sprintf_s(strKey, "BUG_DIE%d", i);

		GET_SINGLE(CResourceManager)->AddAnimation2DSequenceTexture("BUG_DIE", strKey, strFileName);
	}

	GET_SINGLE(CResourceManager)->SetAnimation2DSequenceFrameInfoAll("BUG_DIE", Vector2(0.f, 0.f),
		Vector2(400.f, 400.f));





}


