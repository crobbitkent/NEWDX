#include "HKMode.h"

#include "Scene/Scene.h"
#include "Resource/ResourceManager.h"
#include "Resource/Material.h"
#include "Resource/Animation2DSequence.h"

#include "CollisionManager.h"

#include "HollowKnight.h"
#include "NewHK.h"
#include "JustBug.h"
#include "DashingBug.h"
#include "HornBug.h"
#include "JumpBug.h"
#include "ShieldBug.h"



#include "TestStage.h"
#include "HKButton.h"

#include "HKAttackEffect.h"

#include "HKMouse.h"

#include "UIHP.h"

#include "../RandomNumber.h"

#include "HKExcelManager.h"
#include "HKTileMap.h"

#include "../Object/TestObject.h"

#include <sstream>

HKMode::HKMode()
{
}

HKMode::~HKMode()
{
}

bool HKMode::Init()
{
	CGameMode::Init();

	RandomNumber::Init();

	SetCollision();

	CreateMaterial();

 	SetHK();
	SetBug();
	SetUI();

	// SetTileMap();

	////////////////////////////////////////// ¿¢¼¿ ·Îµù

	// GET_SINGLE(CExcelManager)->Test();
	// DESTROY_SINGLE(CExcelManager);



	/*CTestObject*	pObj = m_pScene->SpawnObject<CTestObject>();

	SetPlayer(pObj);
	SAFE_RELEASE(pObj);*/

	HollowKnight* hk = m_pScene->SpawnObject<HollowKnight>();
	SetPlayer(hk);
	SAFE_RELEASE(hk);

	/* NewHK* hk = m_pScene->SpawnObject<NewHK>();
	 SetPlayer(hk);
	 SAFE_RELEASE(hk);*/


	// ShieldBug* bug = m_pScene->SpawnObject<ShieldBug>(Vector3(500.f, 500.f, 0.f));
	// SAFE_RELEASE(bug);

	



	SetStage1();





	//HKButton*	pTestButton = m_pScene->SpawnObject<HKButton>();
	//SAFE_RELEASE(pTestButton);

	// HKMouse*	pMouse = m_pScene->SpawnObject<HKMouse>();
	// SAFE_RELEASE(pMouse);



	return true;
}

bool HKMode::CreateMaterial()
{
	GET_SINGLE(CResourceManager)->CreateMaterial("PlayerMaterial");
	CMaterial* pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("PlayerMaterial");
	pMaterial->SetSubsetShader(STANDARD_TEX_SHADER);
	pMaterial->SetTexture(0, "Player", TEXT("teemo.png"));
	pMaterial->SetRenderState("AlphaBlend");
	pMaterial->SetMaterialShaderStyle(MSS_ALPHA);
	SAFE_RELEASE(pMaterial);


	GET_SINGLE(CResourceManager)->CreateMaterial("StageMaterial");
	pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("StageMaterial");
	pMaterial->SetSubsetShader(STANDARD_TEX_SHADER);
	pMaterial->SetTexture(0, "TestStage", TEXT("TestCollision.png"));
	pMaterial->SetRenderState("AlphaBlend");
	pMaterial->SetMaterialShaderStyle(MSS_ALPHA);
	SAFE_RELEASE(pMaterial);


	GET_SINGLE(CResourceManager)->CreateMaterial("PlayerAnimMaterial");

	pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("PlayerAnimMaterial");

	pMaterial->SetSubsetShader(STANDARD_ANIM2D_SHADER);
	// pMaterial->SetTexture(0, "Player", TEXT("teemo.png"));
	pMaterial->SetRenderState("AlphaBlend");
	pMaterial->SetMaterialShaderStyle(MSS_ALPHA);
	SAFE_RELEASE(pMaterial);



	GET_SINGLE(CResourceManager)->CreateMaterial("ReversePlayerAnimMaterial");

	pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("ReversePlayerAnimMaterial");

	pMaterial->SetSubsetShader(STANDARD_ANIM2D_SHADER);
	// pMaterial->SetTexture(0, "Player", TEXT("teemo.png"));
	pMaterial->SetRenderState("AlphaBlend");
	pMaterial->SetMaterialShaderStyle(MSS_ALPHA);
	SAFE_RELEASE(pMaterial);
	//////////////////////////////////////////////////////

	GET_SINGLE(CResourceManager)->CreateMaterial("AttackEffectMaterial");

	pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("AttackEffectMaterial");

	pMaterial->SetSubsetShader(STANDARD_ANIM2D_SHADER);
	// pMaterial->SetTexture(0, "Player", TEXT("teemo.png"));
	pMaterial->SetRenderState("AlphaBlend");
	pMaterial->SetMaterialShaderStyle(MSS_ALPHA);
	SAFE_RELEASE(pMaterial);

	//////////////////////////////////////////////////////


	GET_SINGLE(CResourceManager)->CreateMaterial("SideEffectMaterial");

	pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("SideEffectMaterial");

	pMaterial->SetSubsetShader(STANDARD_ANIM2D_SHADER);
	// pMaterial->SetTexture(0, "Player", TEXT("teemo.png"));
	pMaterial->SetRenderState("AlphaBlend");
	pMaterial->SetMaterialShaderStyle(MSS_ALPHA);
	SAFE_RELEASE(pMaterial);

	////////////////////////////////////////////////////////

	GET_SINGLE(CResourceManager)->CreateMaterial("HitEffectMaterial");

	pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("HitEffectMaterial");

	pMaterial->SetSubsetShader(STANDARD_ANIM2D_SHADER);
	// pMaterial->SetTexture(0, "Player", TEXT("teemo.png"));
	pMaterial->SetRenderState("AlphaBlend");
	pMaterial->SetMaterialShaderStyle(MSS_ALPHA);
	SAFE_RELEASE(pMaterial);

	////////////////////////////////////////////////////////

	GET_SINGLE(CResourceManager)->CreateMaterial("MonsterHitEffectMaterial");

	pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("MonsterHitEffectMaterial");

	pMaterial->SetSubsetShader(STANDARD_ANIM2D_SHADER);
	// pMaterial->SetTexture(0, "Player", TEXT("teemo.png"));
	pMaterial->SetRenderState("AlphaBlend");
	pMaterial->SetMaterialShaderStyle(MSS_ALPHA);
	SAFE_RELEASE(pMaterial);





	GET_SINGLE(CResourceManager)->CreateMaterial("BugMaterial");

	pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("BugMaterial");

	pMaterial->SetSubsetShader(STANDARD_ANIM2D_SHADER);
	// pMaterial->SetTexture(0, "Player", TEXT("teemo.png"));
	pMaterial->SetRenderState("AlphaBlend");
	pMaterial->SetMaterialShaderStyle(MSS_ALPHA);
	SAFE_RELEASE(pMaterial);



	GET_SINGLE(CResourceManager)->CreateMaterial("PlayerAnimOutlineMtrl");
	pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("PlayerAnimOutlineMtrl");
	pMaterial->SetSubsetShader(STANDARD_ANIM2D_OUTLINE_SHADER);
	pMaterial->SetRenderState("AlphaBlend");
	pMaterial->SetMaterialShaderStyle(MSS_ALPHA);
	pMaterial->CreateCBufferNode(OUTLINE_CBUFFER, 11, sizeof(OutLineCBuffer));
	SAFE_RELEASE(pMaterial);


	//// SOFT
	GET_SINGLE(CResourceManager)->CreateMaterial("SoftMaterial");
	pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("SoftMaterial");
	pMaterial->SetSubsetShader(STANDARD_TEX_SHADER);
	if (false == pMaterial->SetTexture(0, "Soft", TEXT("HOLLOW/Effect/Soft.png"))) { BOOM; }
	pMaterial->SetRenderState("AlphaBlend");
	pMaterial->SetMaterialShaderStyle(MSS_ALPHA);
	// pMaterial->EnableInstancing();	
	// pMaterial->AddRef();
	SAFE_RELEASE(pMaterial);



	//// BLACK SOFT
	GET_SINGLE(CResourceManager)->CreateMaterial("BlackSoftMaterial");
	pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("BlackSoftMaterial");
	pMaterial->SetSubsetShader(STANDARD_TEX_SHADER);
	if (false == pMaterial->SetTexture(0, "BlackSoft", TEXT("HOLLOW/Effect/BlackSoft.png"))) { BOOM; }
	pMaterial->SetRenderState("AlphaBlend");
	pMaterial->SetMaterialShaderStyle(MSS_ALPHA);
	// pMaterial->EnableInstancing();
	// pMaterial->AddRef();
	SAFE_RELEASE(pMaterial);



	return true;
}






void HKMode::SetCollision()
{
	GET_SINGLE(CCollisionManager)->CreateChannel("Player", CT_BLOCK);
	GET_SINGLE(CCollisionManager)->CreateChannel("Monster", CT_BLOCK);
	GET_SINGLE(CCollisionManager)->CreateChannel("PlayerProjectile", CT_BLOCK);
	GET_SINGLE(CCollisionManager)->CreateChannel("MonsterProjectile", CT_BLOCK);

	GET_SINGLE(CCollisionManager)->CreateChannel("Stage", CT_BLOCK);
	GET_SINGLE(CCollisionManager)->CreateProfile("Stage", "Stage");

	GET_SINGLE(CCollisionManager)->CreateChannel("Sencer", CT_BLOCK);
	GET_SINGLE(CCollisionManager)->CreateProfile("Sencer", "Sencer");

	GET_SINGLE(CCollisionManager)->CreateProfile("Player", "Player");
	GET_SINGLE(CCollisionManager)->UpdateProfileChannel("Player", "Player", CT_IGNORE);
	//GET_SINGLE(CCollisionManager)->UpdateProfileChannel("Player", "Monster", CT_IGNORE);
	GET_SINGLE(CCollisionManager)->UpdateProfileChannel("Player", "PlayerProjectile", CT_IGNORE);

	GET_SINGLE(CCollisionManager)->CreateProfile("Monster", "Monster");
	GET_SINGLE(CCollisionManager)->UpdateProfileChannel("Monster", "Monster", CT_IGNORE);
	//GET_SINGLE(CCollisionManager)->UpdateProfileChannel("Monster", "Player", CT_IGNORE);
	GET_SINGLE(CCollisionManager)->UpdateProfileChannel("Monster", "MonsterProjectile", CT_IGNORE);

	GET_SINGLE(CCollisionManager)->CreateProfile("PlayerProjectile", "PlayerProjectile");
	GET_SINGLE(CCollisionManager)->UpdateProfileChannel("PlayerProjectile", "Player", CT_IGNORE);
	GET_SINGLE(CCollisionManager)->UpdateProfileChannel("PlayerProjectile", "PlayerProjectile", CT_IGNORE);
	GET_SINGLE(CCollisionManager)->UpdateProfileChannel("PlayerProjectile", "MonsterProjectile", CT_IGNORE);

	GET_SINGLE(CCollisionManager)->CreateProfile("MonsterProjectile", "MonsterProjectile");
	GET_SINGLE(CCollisionManager)->UpdateProfileChannel("MonsterProjectile", "Monster", CT_IGNORE);
	GET_SINGLE(CCollisionManager)->UpdateProfileChannel("MonsterProjectile", "PlayerProjectile", CT_IGNORE);
	GET_SINGLE(CCollisionManager)->UpdateProfileChannel("MonsterProjectile", "MonsterProjectile", CT_IGNORE);



	//{
	//	GET_SINGLE(CResourceManager)->CreateMaterial("PlayerMtrl");

	//	CMaterial*	pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("PlayerMtrl");

	//	pMaterial->SetSubsetShader(STANDARD_TEX_SHADER);
	//	pMaterial->SetTexture(0, "Player", TEXT("teemo.png"));
	//	pMaterial->SetRenderState("AlphaBlend");
	//	pMaterial->SetMaterialShaderStyle(MSS_ALPHA);

	//	SAFE_RELEASE(pMaterial);

	//	GET_SINGLE(CResourceManager)->CreateMaterial("PlayerAnimMtrl");

	//	pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("PlayerAnimMtrl");

	//	pMaterial->SetSubsetShader(STANDARD_ANIM2D_SHADER);
	//	//pMaterial->SetTexture(0, "Player", TEXT("teemo.png"));
	//	pMaterial->SetRenderState("AlphaBlend");
	//	pMaterial->SetMaterialShaderStyle(MSS_ALPHA);

	//	SAFE_RELEASE(pMaterial);

	//	GET_SINGLE(CResourceManager)->CreateMaterial("PlayerAnimOutLineMtrl");

	//	pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("PlayerAnimOutLineMtrl");

	//	pMaterial->SetSubsetShader(STANDARD_ANIM2D_OUTLINE_SHADER);
	//	//pMaterial->SetTexture(0, "Player", TEXT("teemo.png"));
	//	pMaterial->SetRenderState("AlphaBlend");
	//	pMaterial->SetMaterialShaderStyle(MSS_ALPHA);
	//	pMaterial->CreateCBufferNode(OUTLINE_CBUFFER, 11, sizeof(OutLineCBuffer));

	//	SAFE_RELEASE(pMaterial);

	//	GET_SINGLE(CResourceManager)->CreateMaterial("TestPixelMtrl");

	//	pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("TestPixelMtrl");

	//	pMaterial->SetSubsetShader(STANDARD_TEX_SHADER);
	//	pMaterial->SetTexture(0, "TestPixel", TEXT("PixelCollision.png"));

	//	SAFE_RELEASE(pMaterial);

	//	GET_SINGLE(CResourceManager)->CreateMaterial("MainMapTileMaterial");

	//	pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("MainMapTileMaterial");

	//	pMaterial->SetSubsetShader(TILEMAP_SHADER);
	//	pMaterial->SetTexture(0, "MainMapTile", TEXT("Tile.bmp"));

	//	SAFE_RELEASE(pMaterial);

	//	GET_SINGLE(CResourceManager)->CreateMaterial("BulletMaterial");

	//	pMaterial = GET_SINGLE(CResourceManager)->FindMaterial("BulletMaterial");

	//	pMaterial->SetSubsetShader(TILEMAP_SHADER);
	//	pMaterial->SetTexture(0, "Bullet", TEXT("teemo.png"));
	//	pMaterial->SetRenderState("AlphaBlend");
	//	pMaterial->SetMaterialShaderStyle(MSS_ALPHA);
	//	pMaterial->EnableInstancing();
	//	pMaterial->AddRef();

	//	SAFE_RELEASE(pMaterial);
	//}


	//{
	//	GET_SINGLE(CResourceManager)->CreateAnimation2DSequence("MinionIdle", true, 1.f, 7);
	//	GET_SINGLE(CResourceManager)->AddAnimation2DSequenceTexture("MinionIdle", "MinionAtlas",
	//		TEXT("Monster/MinionAtlas.png"));

	//	for (int i = 0; i < 7; ++i)
	//	{
	//		GET_SINGLE(CResourceManager)->AddAnimation2DSequenceFrameInfo("MinionIdle", Vector2(50.f * i, 0.f),
	//			Vector2(50.f * (i + 1), 37.f));
	//	}

	//	GET_SINGLE(CResourceManager)->CreateAnimation2DSequence("MinionWalk", true, 1.f, 6);

	//	for (int i = 0; i <= 5; ++i)
	//	{
	//		TCHAR	strFileName[MAX_PATH] = {};

	//		wsprintf(strFileName, TEXT("Monster/MinionFrame/adventurer-run2-0%d.png"), i);

	//		char	strKey[256] = {};
	//		sprintf_s(strKey, "MinionWalk%d", i);

	//		GET_SINGLE(CResourceManager)->AddAnimation2DSequenceTexture("MinionWalk", strKey, strFileName);
	//	}

	//	GET_SINGLE(CResourceManager)->SetAnimation2DSequenceFrameInfoAll("MinionWalk", Vector2(0.f, 0.f),
	//		Vector2(50.f, 37.f));

	//	GET_SINGLE(CResourceManager)->CreateAnimation2DSequence("MinionKick", false, 1.f, 8);

	//	for (int i = 0; i <= 7; ++i)
	//	{
	//		TCHAR	strFileName[MAX_PATH] = {};

	//		wsprintf(strFileName, TEXT("Monster/MinionFrame/adventurer-kick-0%d.png"), i);

	//		char	strKey[256] = {};
	//		sprintf_s(strKey, "MinionKick%d", i);

	//		GET_SINGLE(CResourceManager)->AddAnimation2DSequenceTexture("MinionKick", strKey, strFileName);
	//	}

	//	GET_SINGLE(CResourceManager)->SetAnimation2DSequenceFrameInfoAll("MinionKick", Vector2(0.f, 0.f),
	//		Vector2(50.f, 37.f));
	//}

}


////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
/*
ÇÒ°Íµé



*/



void HKMode::SetTileMap()
{
	HKTileMap* map = m_pScene->SpawnObject<HKTileMap>();

	HKExcelManager* excel = new HKExcelManager;
	
	excel->LoadStage(TEXT("Stage01.xls"), map);

	// HKExcelManager::LoadStage(TEXT("Stage01.xls"), map);
	
	SAFE_RELEASE(map);

	delete excel;
}
