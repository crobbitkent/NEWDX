#include "ExcelManager.h"



DEFINITION_SINGLE(ExcelManager);

ExcelManager::ExcelManager()
{
}


ExcelManager::~ExcelManager()
{
}

bool ExcelManager::CreateBook()
{
	Book* book = xlCreateBook();

	return true;
}
