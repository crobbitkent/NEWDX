#pragma once

#include "GameEngine.h"
#include "libxl.h"
#include <sstream>


using namespace libxl;


class HKExcelManager
{
public:
	HKExcelManager();
	~HKExcelManager();

	// �ݶ��̴� �ϳ��ϳ�
	bool LoadStage(const TCHAR* path, class HKTileMap* tileMap);

	// ū �ݶ��̴�
	bool LoadStage(const TCHAR* path, class CScene* pScene);




	
};

