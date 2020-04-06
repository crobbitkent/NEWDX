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


void HKMode::SetStage1()
{
	HKTileMap* map = m_pScene->SpawnObject<HKTileMap>();

	HKExcelManager* excel = new HKExcelManager;

	excel->LoadStage(TEXT("Stage1.xls"), m_pScene);

	// HKExcelManager::LoadStage(TEXT("Stage01.xls"), map);

	SAFE_RELEASE(map);

	delete excel;







	//TestStage* stage2 = m_pScene->SpawnObject<TestStage>(Vector3(1500.f, 101.f, 0.f));
	//stage2->SetScale(Vector3(2000.f, 200.f, 0.f));
	//SAFE_RELEASE(stage2);

	//TestStage* stage3 = m_pScene->SpawnObject<TestStage>(Vector3(-2000.f, 301.f, 0.f));
	//stage3->SetScale(Vector3(2000.f, 200.f, 0.f));
	//SAFE_RELEASE(stage3);

	//TestStage* stage4 = m_pScene->SpawnObject<TestStage>(Vector3(2000.f, 301.f, 0.f));
	//stage4->SetScale(Vector3(300.f, 100.f, 0.f));
	//SAFE_RELEASE(stage4);

}