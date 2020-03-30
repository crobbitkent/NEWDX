
#include "Navigation2D.h"
#include "../Component/TileMap.h"
#include "../Component/Tile.h"
#include <stack>

CNavigation2D::CNavigation2D() :
	m_pTileMap(nullptr),
	m_iOpenCount(0),
	m_iUseCount(0)
{
}

CNavigation2D::~CNavigation2D()
{
}

void CNavigation2D::SetTileMap(CTileMap * pTileMap)
{
	m_pTileMap = pTileMap;

	m_vecOpen.clear();
	m_vecUse.clear();

	m_vecOpen.resize(m_pTileMap->GetTileCount());
	m_vecUse.resize(m_pTileMap->GetTileCount());
}

bool CNavigation2D::FindPath(const Vector3 & vStart, const Vector3 & vEnd)
{
	if (!m_pTileMap)
		return false;

	CTile*	pStart = m_pTileMap->GetTile(vStart);

	if (!pStart)
		return false;

	CTile*	pEnd = m_pTileMap->GetTile(vEnd);

	if (!pEnd)
		return false;

	m_PathList.clear();

	m_iOpenCount = 0;
	m_iUseCount = 0;

	CTile*	pNode = nullptr;

	m_vecOpen[m_iOpenCount] = pStart;
	++m_iOpenCount;

	m_vecUse[m_iUseCount] = pStart;
	++m_iUseCount;
	m_vecUse[m_iUseCount] = pEnd;
	++m_iUseCount;

	while (m_iOpenCount > 0)
	{
		// 열린목록에서 비용이 가장 작은 노드를 찾는다.
		pNode = m_vecOpen[0];

		int	iIndex = 0;

		for (int i = 1; i < m_iOpenCount; ++i)
		{
			if (pNode->GetG() > m_vecOpen[i]->GetG())
			{
				pNode = m_vecOpen[i];
				iIndex = i;
				break;
			}
		}

		pNode->SetNavInsertType(NIT_CLOSE);

		--m_iOpenCount;
		m_vecOpen[iIndex] = m_vecOpen[m_iOpenCount];

		// 기준노드를 중심으로 주변 노드를 탐색해서 넣어둔다.
		if (FindNode(pNode, pEnd, vEnd))
		{
			break;
		}
	}

	for (int i = 0; i < m_iUseCount; ++i)
	{
		m_vecUse[i]->ClearNav();
	}

	return !m_PathList.empty();
}

bool CNavigation2D::FindNode(CTile * pNode, CTile * pEnd, const Vector3& vEnd)
{
	CTile*	pFindNode = nullptr;
	Vector3	vTileSize = m_pTileMap->GetTileSize();

	// 8방향 노드를 찾는다.
	for (int i = 0; i < NND_END; ++i)
	{
		float	fG = 0.f;
		switch (i)
		{
		case NND_LT:
			pFindNode = m_pTileMap->GetTileFromIndex(pNode->GetIndexX() - 1, pNode->GetIndexY() + 1);
			fG = sqrtf(vTileSize.x * vTileSize.x + vTileSize.y * vTileSize.y);
			break;
		case NND_T:
			pFindNode = m_pTileMap->GetTileFromIndex(pNode->GetIndexX(), pNode->GetIndexY() + 1);
			fG = vTileSize.y;
			break;
		case NND_RT:
			pFindNode = m_pTileMap->GetTileFromIndex(pNode->GetIndexX() + 1, pNode->GetIndexY() + 1);
			fG = sqrtf(vTileSize.x * vTileSize.x + vTileSize.y * vTileSize.y);
			break;
		case NND_R:
			pFindNode = m_pTileMap->GetTileFromIndex(pNode->GetIndexX() + 1, pNode->GetIndexY());
			fG = vTileSize.x;
			break;
		case NND_RB:
			pFindNode = m_pTileMap->GetTileFromIndex(pNode->GetIndexX() + 1, pNode->GetIndexY() - 1);
			fG = sqrtf(vTileSize.x * vTileSize.x + vTileSize.y * vTileSize.y);
			break;
		case NND_B:
			pFindNode = m_pTileMap->GetTileFromIndex(pNode->GetIndexX(), pNode->GetIndexY() - 1);
			fG = vTileSize.y;
			break;
		case NND_LB:
			pFindNode = m_pTileMap->GetTileFromIndex(pNode->GetIndexX() - 1, pNode->GetIndexY() - 1);
			fG = sqrtf(vTileSize.x * vTileSize.x + vTileSize.y * vTileSize.y);
			break;
		case NND_L:
			pFindNode = m_pTileMap->GetTileFromIndex(pNode->GetIndexX() - 1, pNode->GetIndexY());
			fG = vTileSize.x;
			break;
		}

		if (!pFindNode)
			continue;

		else if (pFindNode->GetTileOption() == TO_NOMOVE)
			continue;

		else if (pFindNode->GetNavType() == NIT_CLOSE)
			continue;

		Vector3	vNodeCenter = pFindNode->GetTilePos() + vTileSize / 2.f;

		// 도착점을 찾았다면 도착점까지의 경로를 넣어주고 종료한다.
		if (pFindNode == pEnd)
		{
			stack<Vector3>	Path;

			Path.push(vEnd);

			CTile*	pPathNode = pNode;

			while (pPathNode)
			{
				Vector3	vPathNodeCenter = pPathNode->GetTilePos() + vTileSize / 2.f;
				Path.push(vPathNodeCenter);

				pPathNode = pPathNode->GetParent();
			}

			// 스택에 들어간 경로를 정방향으로 바꿔준다.
			while (!Path.empty())
			{
				m_PathList.push_back(Path.top());
				Path.pop();
			}

			m_PathList.pop_front();

			return true;
		}

		float	fH = vEnd.Distance(vNodeCenter);
		fG += pNode->GetG();

		if (pFindNode->GetNavType() == NIT_OPEN)
		{
			if (pFindNode->GetG() > fG)
			{
				pFindNode->SetCost(fG, fH);
				pFindNode->SetParent(pNode);
			}
		}

		else
		{
			pFindNode->SetCost(fG, fH);
			pFindNode->SetNavInsertType(NIT_OPEN);
			pFindNode->SetParent(pNode);

			m_vecOpen[m_iOpenCount] = pFindNode;
			++m_iOpenCount;

			m_vecUse[m_iUseCount] = pFindNode;
			++m_iUseCount;
		}
	}

	return false;
}
