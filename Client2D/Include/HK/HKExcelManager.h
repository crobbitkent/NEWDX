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

	bool LoadStage(const TCHAR* path, class HKTileMap* tileMap);




	
};

