
#include "RenderManager.h"
#include "../Component/StaticMeshComponent.h"
#include "../Component/AnimationMeshComponent.h"
#include "../Component/Transform.h"
#include "../Object/GameObject.h"
#include "../Resource/Material.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/DepthStencilState.h"
#include "../Resource/RasterizerState.h"
#include "../Input.h"
#include "../Component/TileMap.h"
#include "../Device.h"
#include "../Component/CameraComponent.h"
#include "../CameraManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/Shader.h"
#include "../Engine.h"

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager()
{
	m_iTotalRenderCount	= 0;

	for(int i = 0; i < RG_END; ++i)
	{
		m_vecRender[i].resize(10);
	}

	memset(m_iAddCount, 0, sizeof(size_t) * RG_END);

	m_eRenderMode	= RM_2D;

	m_pCullNone	= nullptr;
	m_pNoneDepth	= nullptr;

	m_vecInstancing.resize(10);
	m_vecStaticData.resize(10);
	m_iInstancingAddCount = 0;
	m_vecAnim2DInstancing.resize(10);
	m_vecAnim2DData.resize(10);
	m_iAnim2DInstancingAddCount = 0;

	for (size_t i = 0; i < m_vecInstancing.size(); ++i)
	{
		m_vecInstancing[i].pBuffer = new InstancingBuffer;

		m_vecInstancing[i].vecList.resize(300);
		m_vecInstancing[i].iAddCount = 0;
		m_vecInstancing[i].pBuffer->eUsage = D3D11_USAGE_DYNAMIC;
		m_vecInstancing[i].pBuffer->iCount = 300;
		m_vecInstancing[i].pBuffer->iSize = sizeof(StaticInstancingData);
		/*int	iSize1 = sizeof(Matrix);
		int	iSize2 = sizeof(Vector4);
		int	iSize3 = sizeof(Vector3);
		m_vecInstancing[i].pBuffer->iSize = iSize1 + iSize2 + iSize3;*/

		D3D11_BUFFER_DESC	tDesc = {};
		tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		tDesc.ByteWidth = m_vecInstancing[i].pBuffer->iCount * m_vecInstancing[i].pBuffer->iSize;
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		tDesc.Usage = D3D11_USAGE_DYNAMIC;

		DEVICE->CreateBuffer(&tDesc, nullptr, &m_vecInstancing[i].pBuffer->pBuffer);

		m_vecStaticData[i]	= new StaticInstancingData[300];
	}

	for (size_t i = 0; i < m_vecAnim2DInstancing.size(); ++i)
	{
		m_vecAnim2DInstancing[i].pBuffer = new InstancingBuffer;

		m_vecAnim2DInstancing[i].vecList.resize(300);
		m_vecAnim2DInstancing[i].iAddCount	= 0;
		m_vecAnim2DInstancing[i].pBuffer->eUsage = D3D11_USAGE_DYNAMIC;
		m_vecAnim2DInstancing[i].pBuffer->iCount = 300;
		m_vecAnim2DInstancing[i].pBuffer->iSize = sizeof(Anim2DInstancingData);

		D3D11_BUFFER_DESC	tDesc = {};
		tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		tDesc.ByteWidth = m_vecAnim2DInstancing[i].pBuffer->iCount * m_vecAnim2DInstancing[i].pBuffer->iSize;
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		tDesc.Usage = D3D11_USAGE_DYNAMIC;

		DEVICE->CreateBuffer(&tDesc, nullptr, &m_vecAnim2DInstancing[i].pBuffer->pBuffer);

		m_vecAnim2DData[i] = new Anim2DInstancingData[300];
	}
}

CRenderManager::~CRenderManager()
{
	for (size_t i = 0; i < m_vecInstancing.size(); ++i)
	{
		SAFE_DELETE_ARRAY(m_vecStaticData[i]);
		SAFE_RELEASE(m_vecInstancing[i].pBuffer->pBuffer);
		SAFE_DELETE(m_vecInstancing[i].pBuffer)
	}

	for (size_t i = 0; i < m_vecAnim2DInstancing.size(); ++i)
	{
		SAFE_DELETE_ARRAY(m_vecAnim2DData[i]);
		SAFE_RELEASE(m_vecAnim2DInstancing[i].pBuffer->pBuffer);
		SAFE_DELETE(m_vecAnim2DInstancing[i].pBuffer)
	}

	SAFE_RELEASE(m_pCullNone);
	SAFE_RELEASE(m_pNoneDepth);
}

void CRenderManager::SetRenderMode(RENDER_MODE eMode)
{
	m_eRenderMode	= eMode;
}

RENDER_MODE CRenderManager::GetRenderMode() const
{
	return m_eRenderMode;
}

bool CRenderManager::Init()
{
	m_pNoneDepth = (CDepthStencilState*)GET_SINGLE(CResourceManager)->FindRenderState(RENDERSTATE_NONEDEPTH);
	m_pCullNone	= (CRasterizerState*)GET_SINGLE(CResourceManager)->FindRenderState(RENDERSTATE_CULLNONE);

	return true;
}

void CRenderManager::AddRenderer(CSceneComponent * pCom)
{
	if(m_eRenderMode == RM_2D && !pCom->IsUI())
	{
		CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();

		CCameraComponent*	pCamera = pScene->GetCameraManager()->GetMainCamera();
		Vector3	vCameraMin = pCamera->GetWorldPos();
		Vector3	vCameraMax = vCameraMin + Vector3(_RESOLUTION.iWidth, _RESOLUTION.iHeight, 0.f);

		if (pCom->GetComponentMin().x > vCameraMax.x)
			return;

		else if (pCom->GetComponentMax().x < vCameraMin.x)
			return;

		else if (pCom->GetComponentMin().y > vCameraMax.y)
			return;

		else if (pCom->GetComponentMax().y < vCameraMin.y)
			return;
	}

	++m_iTotalRenderCount;
	CMaterial*	pMaterial = pCom->GetMaterial();

	if (pMaterial && pMaterial->IsEnableInstancing())
	{
		// 애니메이션이 있는지 없는지 판단한다.
		bool	bIsAnim2D	= false;

		if(pCom->GetTypeID() == typeid(CStaticMeshComponent).hash_code())
			bIsAnim2D	= ((CStaticMeshComponent*)pCom)->IsAnimation2D();

		PInstancingList	pInstancingList = nullptr;
		int iIndex = -1;

		if (bIsAnim2D)
		{
			for (size_t i = 0; i < m_iAnim2DInstancingAddCount; ++i)
			{
				if (m_vecAnim2DInstancing[i].iSerialNumber == pMaterial->GetSerialNumber())
				{
					iIndex = (int)i;
					break;
				}
			}

			if (iIndex == -1)
			{
				if (m_iAnim2DInstancingAddCount == m_vecAnim2DInstancing.capacity())
					m_vecAnim2DInstancing.resize(m_vecAnim2DInstancing.capacity() * 2);

				iIndex = m_iAnim2DInstancingAddCount;
				++m_iAnim2DInstancingAddCount;

				m_vecAnim2DInstancing[iIndex].iSerialNumber = pMaterial->GetSerialNumber();

				RENDER_GROUP	eGroup = RG_DEFAULT;

				switch (pMaterial->GetMaterialShaderStyle())
				{
				case MSS_UI:
					eGroup = RG_UI;
					break;
				case MSS_DEFAULT:
					eGroup = RG_DEFAULT;
					break;
				case MSS_ALPHA:
					eGroup = RG_ALPHA;

					if (m_eRenderMode == RM_2D)
						eGroup = RG_DEFAULT;
					break;
				case MSS_ENVIRONMENT:
					eGroup = RG_ENVIRONMENT;
					break;
				}

				m_vecAnim2DInstancing[iIndex].eGroup = eGroup;
			}

			pInstancingList = &m_vecAnim2DInstancing[iIndex];
		}

		else
		{
			for (size_t i = 0; i < m_iInstancingAddCount; ++i)
			{
				if (m_vecInstancing[i].iSerialNumber == pMaterial->GetSerialNumber())
				{
					iIndex = (int)i;
					break;
				}
			}

			if (iIndex == -1)
			{
				if (m_iInstancingAddCount == m_vecInstancing.capacity())
					m_vecInstancing.resize(m_vecInstancing.capacity() * 2);

				iIndex = m_iInstancingAddCount;
				++m_iInstancingAddCount;

				m_vecInstancing[iIndex].iSerialNumber = pMaterial->GetSerialNumber();

				RENDER_GROUP	eGroup = RG_DEFAULT;

				switch (pMaterial->GetMaterialShaderStyle())
				{
				case MSS_UI:
					eGroup = RG_UI;
					break;
				case MSS_DEFAULT:
					eGroup = RG_DEFAULT;
					break;
				case MSS_ALPHA:
					eGroup = RG_ALPHA;

					if (m_eRenderMode == RM_2D)
						eGroup = RG_DEFAULT;
					break;
				case MSS_ENVIRONMENT:
					eGroup = RG_ENVIRONMENT;
					break;
				}

				m_vecInstancing[iIndex].eGroup = eGroup;
			}

			pInstancingList = &m_vecInstancing[iIndex];
		}

		if(pInstancingList->iAddCount == pInstancingList->vecList.capacity())
		{
			pInstancingList->vecList.resize(pInstancingList->vecList.capacity() * 2);

			pInstancingList->pBuffer->iCount = (int)pInstancingList->vecList.capacity();

			SAFE_RELEASE(pInstancingList->pBuffer->pBuffer);

			D3D11_BUFFER_DESC	tDesc = {};
			tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			tDesc.ByteWidth = pInstancingList->pBuffer->iCount * pInstancingList->pBuffer->iSize;
			tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			tDesc.Usage = D3D11_USAGE_DYNAMIC;

			DEVICE->CreateBuffer(&tDesc, nullptr, &pInstancingList->pBuffer->pBuffer);

			if (bIsAnim2D)
			{
				SAFE_DELETE_ARRAY(m_vecAnim2DData[iIndex]);
				m_vecAnim2DData[iIndex] = new Anim2DInstancingData[pInstancingList->pBuffer->iCount];
			}

			else
			{
				SAFE_DELETE_ARRAY(m_vecStaticData[iIndex]);
				m_vecStaticData[iIndex] = new StaticInstancingData[pInstancingList->pBuffer->iCount];
			}
		}

		pInstancingList->vecList[pInstancingList->iAddCount]	= pCom;
		++pInstancingList->iAddCount;
	}

	else
	{
		RENDER_GROUP	eGroup = RG_DEFAULT;

		if (pMaterial)
		{
			switch (pMaterial->GetMaterialShaderStyle())
			{
			case MSS_UI:
				eGroup = RG_UI;
				break;
			case MSS_DEFAULT:
				eGroup = RG_DEFAULT;
				break;
			case MSS_ALPHA:
				eGroup = RG_ALPHA;

				if (m_eRenderMode == RM_2D)
					eGroup = RG_DEFAULT;
				break;
			case MSS_ENVIRONMENT:
				eGroup = RG_ENVIRONMENT;
				break;
			}
		}

		else
		{
			if (pCom->IsUI())
				eGroup = RG_UI;

			else if (pCom->GetTypeID() == typeid(CTileMap).hash_code())
				eGroup = RG_ENVIRONMENT;
		}

		if (m_iAddCount[eGroup] == m_vecRender[eGroup].capacity())
			m_vecRender[eGroup].resize(m_vecRender[eGroup].capacity() * 2);

		m_vecRender[eGroup][m_iAddCount[eGroup]] = pCom;
		++m_iAddCount[eGroup];
	}

	SAFE_RELEASE(pMaterial);
}

void CRenderManager::ComputeRender()
{
}

void CRenderManager::Render(float fTime)
{
	if (m_eRenderMode == RM_2D)
	{
		m_pNoneDepth->SetState();
		m_pCullNone->SetState();

		// RG_DEFAULT 그룹의 출력물체들을 Y 소팅한다.
		if(m_vecRender[RG_DEFAULT].size() >= 2)
			sort(m_vecRender[RG_DEFAULT].begin(), m_vecRender[RG_DEFAULT].begin() + m_iAddCount[RG_DEFAULT], CRenderManager::SortY);

		if (m_vecRender[RG_ALPHA].size() >= 2)
			sort(m_vecRender[RG_ALPHA].begin(), m_vecRender[RG_ALPHA].begin() + m_iAddCount[RG_ALPHA], CRenderManager::SortY);
	}

	else
	{
		
	}

	for (int i = 0; i < RG_END; ++i)
	{
		RenderInstancing(fTime, (RENDER_GROUP)i);

		for (size_t j = 0; j < m_iAddCount[i]; ++j)
		{
			m_vecRender[i][j]->Render(fTime);
		}

		m_iAddCount[i]	= 0;
	}
	
	GET_SINGLE(CInput)->Render(fTime);

	m_iTotalRenderCount	= 0;

	if (m_eRenderMode == RM_2D)
	{
		m_pNoneDepth->ResetState();
		m_pCullNone->ResetState();
	}

	else
	{

	}
}

void CRenderManager::RenderInstancing(float fTime, RENDER_GROUP eGroup)
{
	CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();

	CCameraComponent*	pCamera = pScene->GetCameraManager()->GetMainCamera();

	for (size_t i = 0; i < m_iInstancingAddCount; ++i)
	{
		if(m_vecInstancing[i].eGroup != eGroup)
			continue;

		else if(m_vecInstancing[i].iAddCount == 0)
			continue;

		if (m_vecInstancing[i].iAddCount >= 2)
			sort(m_vecInstancing[i].vecList.begin(), m_vecInstancing[i].vecList.begin() + m_vecInstancing[i].iAddCount, CRenderManager::SortY);

		for (size_t j = 0; j < m_vecInstancing[i].iAddCount; ++j)
		{
			CSceneComponent*	pCom = m_vecInstancing[i].vecList[j];

			pCom->GetTransform()->ComputeTransform();

			const TransformCBuffer	tBuffer = pCom->GetTransform()->GetCBuffer();

			CMaterial*	pMaterial = m_vecInstancing[i].vecList[j]->GetMaterial();

			m_vecStaticData[i][j].vDiffuse = pMaterial->GetDiffuse(0, 0);
			m_vecStaticData[i][j].matWVP = tBuffer.matWVP;
			m_vecStaticData[i][j].vPivot = tBuffer.vPivot;

			SAFE_RELEASE(pMaterial);
		}

		// 데이터를 모두 채웠으므로 Buffer에 복사해주고 렌더링한다.
		CSceneComponent*	pRenderer = m_vecInstancing[i].vecList[0];

		D3D11_MAPPED_SUBRESOURCE	tMap = {};
		CONTEXT->Map(m_vecInstancing[i].pBuffer->pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);

		memcpy(tMap.pData, m_vecStaticData[i], m_vecInstancing[i].pBuffer->iSize * m_vecInstancing[i].iAddCount);

		CONTEXT->Unmap(m_vecInstancing[i].pBuffer->pBuffer, 0);

		m_vecInstancing[i].pBuffer->iCount	= m_vecInstancing[i].iAddCount;

		pRenderer->RenderInstancing(fTime, m_vecInstancing[i].pBuffer);

		m_vecInstancing[i].iAddCount	= 0;
	}
}

bool CRenderManager::SortY(CSceneComponent * pSrc, CSceneComponent * pDest)
{
	float	fSrcY = pSrc->GetWorldPos().y - pSrc->GetPivot().y * pSrc->GetWorldScale().y;
	float	fDestY = pDest->GetWorldPos().y - pDest->GetPivot().y * pDest->GetWorldScale().y;

	return fSrcY > fDestY;
}
