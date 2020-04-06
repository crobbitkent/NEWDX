#include "HKExcelManager.h"

#include "HKTileMap.h"

#include "Component/ColliderRect.h"
#include "Component/ColliderBase.h"







HKExcelManager::HKExcelManager()
{
}

HKExcelManager::~HKExcelManager()
{
}

bool HKExcelManager::LoadStage(const TCHAR * path, class HKTileMap* tileMap)
{
	Book* book = xlCreateBook();

	if (book->load(path))
	{
		Sheet* sheet = book->getSheet(0);

		int totalY = sheet->lastRow();
		int totalX = sheet->lastCol();

		tileMap->SetInit(totalX, totalY);

		if (nullptr != sheet)
		{
			for (int row = 0; row < totalY; ++row)
			{
				for (int col = 0; col < totalX; ++col)
				{
					CellType cellType = sheet->cellType(row, col);

					int index = row * totalX + col;

					switch (cellType)
					{
					case CELLTYPE_EMPTY:
						break;
					case CELLTYPE_NUMBER:
					{
						int num = sheet->readNum(row, col);

						if (1 == num)
						{
							tileMap->CreateOneTile(col, row, 50.f, 50.f, index);
						}
						else
						{
							int a = 0;
						}

						break;
					}
					case CELLTYPE_STRING:
					{
						BOOM;
						break;
					}
					case CELLTYPE_BOOLEAN:
					{
						BOOM;
						break;
					}
					case CELLTYPE_BLANK:

					case CELLTYPE_ERROR:
					{
						BOOM;
						break;
					}
					}
				}			
			}
		}
	}
	else
	{
		BOOM
	}

	book->release();

	return true;
}
