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
	GET_SINGLE(CResourceManager)->CreateAnimation2DSequence("BUG_DIE", false, 0.8f, frameCount);

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


	// BUG DEAD
	frameCount = 2;
	GET_SINGLE(CResourceManager)->CreateAnimation2DSequence("BUG_DEAD", false, 0.2f, frameCount);

	for (int i = 0; i < frameCount; ++i)
	{
		TCHAR	strFileName[MAX_PATH] = {};

		wsprintf(strFileName, TEXT("HOLLOW/Monster/BUG/BUG_DEAD%d.png"), i);

		char strKey[256] = {};
		sprintf_s(strKey, "BUG_DEAD%d", i);

		GET_SINGLE(CResourceManager)->AddAnimation2DSequenceTexture("BUG_DEAD", strKey, strFileName);
	}

	GET_SINGLE(CResourceManager)->SetAnimation2DSequenceFrameInfoAll("BUG_DEAD", Vector2(0.f, 0.f),
		Vector2(400.f, 400.f));

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Dashing Bug
	frameCount = 6;
	GET_SINGLE(CResourceManager)->CreateAnimation2DSequence("DB_WALK", true, 0.8f, frameCount);

	for (int i = 0; i < frameCount; ++i)
	{
		TCHAR	strFileName[MAX_PATH] = {};

		wsprintf(strFileName, TEXT("HOLLOW/Monster/DASHING_BUG/WALK/DB_WALK%d.png"), i);

		char strKey[256] = {};
		sprintf_s(strKey, "DB_WALK%d", i);

		GET_SINGLE(CResourceManager)->AddAnimation2DSequenceTexture("DB_WALK", strKey, strFileName);
	}

	GET_SINGLE(CResourceManager)->SetAnimation2DSequenceFrameInfoAll("DB_WALK", Vector2(0.f, 0.f),
		Vector2(400.f, 400.f));


	// DB Turn
	frameCount = 2;
	GET_SINGLE(CResourceManager)->CreateAnimation2DSequence("DB_TURN", true, 0.1f, frameCount);

	for (int i = 0; i < frameCount; ++i)
	{
		TCHAR	strFileName[MAX_PATH] = {};

		wsprintf(strFileName, TEXT("HOLLOW/Monster/DASHING_BUG/TURN/DB_TURN%d.png"), i);

		char strKey[256] = {};
		sprintf_s(strKey, "DB_TURN%d", i);

		GET_SINGLE(CResourceManager)->AddAnimation2DSequenceTexture("DB_TURN", strKey, strFileName);
	}

	GET_SINGLE(CResourceManager)->SetAnimation2DSequenceFrameInfoAll("DB_TURN", Vector2(0.f, 0.f),
		Vector2(400.f, 400.f));




	// DB DIE
	frameCount = 6;
	GET_SINGLE(CResourceManager)->CreateAnimation2DSequence("DB_DIE", true, 0.8f, frameCount);

	for (int i = 0; i < frameCount; ++i)
	{
		TCHAR	strFileName[MAX_PATH] = {};

		wsprintf(strFileName, TEXT("HOLLOW/Monster/DASHING_BUG/DIE/DB_DIE%d.png"), i);

		char strKey[256] = {};
		sprintf_s(strKey, "DB_DIE%d", i);

		GET_SINGLE(CResourceManager)->AddAnimation2DSequenceTexture("DB_DIE", strKey, strFileName);
	}

	GET_SINGLE(CResourceManager)->SetAnimation2DSequenceFrameInfoAll("DB_DIE", Vector2(0.f, 0.f),
		Vector2(400.f, 400.f));


	// DB DASH
	frameCount = 7;
	GET_SINGLE(CResourceManager)->CreateAnimation2DSequence("DB_DASH", true, 0.4f, frameCount);

	for (int i = 0; i < frameCount; ++i)
	{
		TCHAR	strFileName[MAX_PATH] = {};

		wsprintf(strFileName, TEXT("HOLLOW/Monster/DASHING_BUG/RUN/DB_RUN%d.png"), i);

		char strKey[256] = {};
		sprintf_s(strKey, "DB_DASH%d", i);

		GET_SINGLE(CResourceManager)->AddAnimation2DSequenceTexture("DB_DASH", strKey, strFileName);
	}

	GET_SINGLE(CResourceManager)->SetAnimation2DSequenceFrameInfoAll("DB_DASH", Vector2(0.f, 0.f),
		Vector2(400.f, 400.f));

	// DB DEAD
	frameCount = 2;
	GET_SINGLE(CResourceManager)->CreateAnimation2DSequence("DB_DEAD", false, 0.1f, frameCount);
	for (int i = 0; i < frameCount; ++i)
	{
		TCHAR	strFileName[MAX_PATH] = {};
		wsprintf(strFileName, TEXT("HOLLOW/Monster/DASHING_BUG/DEAD/DB_DEAD%d.png"), i);
		char strKey[256] = {};
		sprintf_s(strKey, "DB_DEAD%d", i);
		GET_SINGLE(CResourceManager)->AddAnimation2DSequenceTexture("DB_DEAD", strKey, strFileName);
	}
	GET_SINGLE(CResourceManager)->SetAnimation2DSequenceFrameInfoAll("DB_DEAD", Vector2(0.f, 0.f),
		Vector2(400.f, 400.f));

	// DB BWALK
	frameCount = 2;
	GET_SINGLE(CResourceManager)->CreateAnimation2DSequence("DB_BWALK", false, 0.2f, frameCount);
	for (int i = 0; i < frameCount; ++i)
	{
		TCHAR	strFileName[MAX_PATH] = {};
		wsprintf(strFileName, TEXT("HOLLOW/Monster/DASHING_BUG/BWALK/DB_BWALK%d.png"), i);
		char strKey[256] = {};
		sprintf_s(strKey, "DB_BWALK%d", i);
		GET_SINGLE(CResourceManager)->AddAnimation2DSequenceTexture("DB_BWALK", strKey, strFileName);
	}
	GET_SINGLE(CResourceManager)->SetAnimation2DSequenceFrameInfoAll("DB_BWALK", Vector2(0.f, 0.f),
		Vector2(400.f, 400.f));

	// DB STAND
	frameCount = 7;
	GET_SINGLE(CResourceManager)->CreateAnimation2DSequence("DB_STAND", true, 1.4f, frameCount);
	for (int i = 0; i < frameCount; ++i)
	{
		TCHAR	strFileName[MAX_PATH] = {};
		wsprintf(strFileName, TEXT("HOLLOW/Monster/DASHING_BUG/STANDING/DB_STANDING%d.png"), i);
		char strKey[256] = {};
		sprintf_s(strKey, "DB_STAND%d", i);
		GET_SINGLE(CResourceManager)->AddAnimation2DSequenceTexture("DB_STAND", strKey, strFileName);
	}
	GET_SINGLE(CResourceManager)->SetAnimation2DSequenceFrameInfoAll("DB_STAND", Vector2(0.f, 0.f),
		Vector2(400.f, 400.f));
}



