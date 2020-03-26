#pragma once

#include "../GameEngine.h"

typedef struct _tagInstancingList
{
	RENDER_GROUP	eGroup;
	size_t	iSerialNumber;
	vector<class CSceneComponent*>	vecList;
	size_t				iAddCount;
	PInstancingBuffer	pBuffer;
}InstancingList, *PInstancingList;

class ENGINE_DLL CRenderManager
{
private:
	vector<class CSceneComponent*>	m_vecRender[RG_END];
	size_t		m_iAddCount[RG_END];
	RENDER_MODE	m_eRenderMode;
	class CDepthStencilState*	m_pNoneDepth;
	class CRasterizerState*		m_pCullNone;
	vector<InstancingList>		m_vecInstancing;
	vector<PStaticInstancingData>	m_vecStaticData;
	size_t		m_iInstancingAddCount;
	vector<InstancingList>		m_vecAnim2DInstancing;
	size_t		m_iAnim2DInstancingAddCount;
	vector<PAnim2DInstancingData>	m_vecAnim2DData;
	int			m_iTotalRenderCount;

public:
	void SetRenderMode(RENDER_MODE eMode);
	RENDER_MODE GetRenderMode()	const;

public:
	bool Init();
	void AddRenderer(class CSceneComponent* pCom);
	void ComputeRender();
	void Render(float fTime);

private:
	void RenderInstancing(float fTime, RENDER_GROUP eGroup);

private:
	static bool SortY(class CSceneComponent* pSrc, class CSceneComponent* pDest);

	DECLARE_SINGLE(CRenderManager)
};

