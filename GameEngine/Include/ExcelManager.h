#pragma once

#include "GameEngine.h"
#include "libxl.h"

using namespace libxl;

class ENGINE_DLL ExcelManager
{
public:


	bool CreateBook();

	DECLARE_SINGLE(ExcelManager);
};

