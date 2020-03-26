#include "TileMap.h"
#include "../Resource/StaticMesh.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/Shader.h"
#include "ColliderRect.h"
#include "../Object/GameObject.h"
#include "../Resource/BlendState.h"
#include "../Resource/Material.h"
#include "Tile.h"
#include "../Scene/Scene.h"
#include "../CameraManager.h"
#include "CameraComponent.h"
#include "../Device.h"

CTileMap::CTileMap()
{
	m_bTickUpdate = true;

	SetTypeID<CTileMap>();

	m_pMesh = (CStaticMesh*)GET_SINGLE(CResourceManager)->FindMesh("TexRect");
	m_pLayout = GET_SINGLE(CShaderManager)->FindInputLayout(POS_UV_LAYOUT);
	m_pAlphaBlend = nullptr;
	m_pMaterial	= nullptr;
	m_pTileArray	= nullptr;

	m_iTileCountX = 0;
	m_iTileCountY = 0;
	m_iTileCount = 0;
	m_eTileType	= TT_NONE;

	m_bTileRender	= false;
}

CTileMap::CTileMap(const CTileMap & com) :
	CSceneComponent(com)
{
	m_pMesh = com.m_pMesh;

	if (m_pMesh)
		m_pMesh->AddRef();

	m_pLayout = com.m_pLayout;

	if(m_pMaterial)
		m_pMaterial->AddRef();

	m_pAlphaBlend = nullptr;

	m_iTileCountX = com.m_iTileCountX;
	m_iTileCountY = com.m_iTileCountY;
	m_iTileCount = com.m_iTileCount;

	m_vSize = com.m_vSize;
	m_vTileSize = com.m_vTileSize;
	m_eTileType = com.m_eTileType;
	m_pTileArray	= nullptr;

	m_bTileRender	= com.m_bTileRender;
}

CTileMap::~CTileMap()
{
	SAFE_RELEASE(m_pInstancingBuffer->pBuffer);
	SAFE_DELETE(m_pInstancingBuffer);
	SAFE_DELETE_ARRAY(m_pInstancingData);

	if (m_iTileCount != 0)
	{
		for (unsigned int i = 0; i < m_iTileCountY; ++i)
		{
			for (unsigned int j = 0; j < m_iTileCountX; ++j)
			{
				SAFE_DELETE(m_pTileArray[i * m_iTileCountX + j]);
			}
		}

		SAFE_DELETE_ARRAY(m_pTileArray);
	}

	SAFE_RELEASE(m_pMaterial);
	SAFE_RELEASE(m_pAlphaBlend);
	SAFE_RELEASE(m_pMesh);
}

void CTileMap::SetMesh(const string & strName)
{
	SAFE_RELEASE(m_pMesh);
	m_pMesh = (CStaticMesh*)GET_SINGLE(CResourceManager)->FindMesh(strName);
}

void CTileMap::SetInputLayout(const string & strName)
{
	m_pLayout = GET_SINGLE(CShaderManager)->FindInputLayout(strName);
}

void CTileMap::SetMaterial(const string & strName)
{
	CMaterial*	pMtrl = GET_SINGLE(CResourceManager)->FindMaterial(strName);

	if(pMtrl)
	{
		if (!m_pMaterial)
		{
			m_pMaterial = new CMaterial;

			m_pMaterial->Init();
		}
		m_pMaterial->CopyMaterial(pMtrl);

		m_pMaterial->SetMaterialShaderStyle(MSS_ENVIRONMENT);

		SAFE_RELEASE(pMtrl);
	}
}

void CTileMap::SetAlphaBlend()
{
	if(m_pAlphaBlend)
		return;

	m_pAlphaBlend	= GET_SINGLE(CResourceManager)->FindRenderState(RENDERSTATE_ALPHABLEND);
}

void CTileMap::SetTileMesh(const string & strName)
{
	if(m_iTileCount == 0)
		return;

	for (unsigned int i = 0; i < m_iTileCount; ++i)
	{
		m_pTileArray[i]->SetMesh(strName);
	}
}

void CTileMap::SetTileInputLayout(const string & strName)
{
	if (m_iTileCount == 0)
		return;

	for (unsigned int i = 0; i < m_iTileCount; ++i)
	{
		m_pTileArray[i]->SetInputLayout(strName);
	}
}

void CTileMap::SetTileMaterial(const string & strName)
{
	if (m_iTileCount == 0)
		return;

	for (unsigned int i = 0; i < m_iTileCount; ++i)
	{
		m_pTileArray[i]->SetMaterial(strName);
	}

	m_bTileRender	= true;
}

void CTileMap::SetTileAlphaBlend()
{
	if (m_iTileCount == 0)
		return;

	for (unsigned int i = 0; i < m_iTileCount; ++i)
	{
		m_pTileArray[i]->SetAlphaBlend();
	}
}

void CTileMap::CreateTile(TILE_TYPE eType, unsigned int iCountX, unsigned int iCountY, 
	const Vector3 & vTileSize, const Vector3& vPos)
{
	if (m_iTileCount != 0)
	{
		for (unsigned int i = 0; i < m_iTileCountY; ++i)
		{
			for (unsigned int j = 0; j < m_iTileCountX; ++j)
			{
				SAFE_DELETE(m_pTileArray[i * m_iTileCountX + j]);
			}
		}

		SAFE_DELETE_ARRAY(m_pTileArray);
	}

	m_iTileCountX = iCountX;
	m_iTileCountY = iCountY;
	m_iTileCount = m_iTileCountX * m_iTileCountY;

	m_eTileType	= eType;
	m_vTileSize	= vTileSize;
	m_vSize	= m_vTileSize * Vector3((float)m_iTileCountX, (float)m_iTileCountY, 0.f);

	SetWorldScale(m_vSize.x, m_vSize.y, 1.f);

	SetRelativePos(vPos);

	m_pTileArray = new CTile*[m_iTileCount];

	for (unsigned int i = 0; i < m_iTileCountY; ++i)
	{
		for (unsigned int j = 0; j < m_iTileCountX; ++j)
		{
			int	iIndex = i * m_iTileCountX + j;

			m_pTileArray[iIndex]	= new CTile;

			m_pTileArray[iIndex]->m_eType	= eType;
			m_pTileArray[iIndex]->m_eOption	= TO_NONE;
			m_pTileArray[iIndex]->m_vPos = vPos + Vector3(j * m_vTileSize.x, i * m_vTileSize.y, 0.f);
			m_pTileArray[iIndex]->m_vSize = m_vTileSize;

			if (!m_vecFrame.empty())
			{
				m_pTileArray[i]->SetFrame(m_vecFrame[0]);
			}
		}
	}

	// 전체 타일 수만큼 인스턴싱 버퍼를 할당해준다.
	m_pInstancingBuffer = new InstancingBuffer;

	m_pInstancingBuffer->iSize	= sizeof(TileMapInstancingData);
	m_pInstancingBuffer->iCount	= m_iTileCount;
	m_pInstancingBuffer->eUsage	= D3D11_USAGE_DYNAMIC;
	D3D11_BUFFER_DESC	tDesc	= {};

	tDesc.Usage	= D3D11_USAGE_DYNAMIC;
	tDesc.ByteWidth	= m_pInstancingBuffer->iSize * m_pInstancingBuffer->iCount;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	DEVICE->CreateBuffer(&tDesc, nullptr, &m_pInstancingBuffer->pBuffer);

	m_pInstancingData	= new TileMapInstancingData[m_iTileCount];
}

void CTileMap::AddFrame(const Vector2 & vStart, const Vector2 & vEnd, const Vector2& vImgSize,
	IMAGE_TYPE eType)
{
	ImageFrame	tFrame = {};

	tFrame.vStart	= vStart;
	tFrame.vEnd	= vEnd;
	tFrame.vImageSize	= vImgSize;
	tFrame.iImageType	= eType;
	tFrame.iFrame	= m_vecFrame.size();

	m_vecFrame.push_back(tFrame);

	if (m_vecFrame.size() == 1 && m_pTileArray)
	{
		for (unsigned int i = 0; i < m_iTileCount; ++i)
		{
			m_pTileArray[i]->SetFrame(tFrame);
		}
	}
}

bool CTileMap::Init()
{
	if (!CSceneComponent::Init())
		return false;

	return true;
}

void CTileMap::Begin()
{
	CSceneComponent::Begin();

	for (unsigned int i = 0; i < m_iTileCount; ++i)
	{
		m_pTileArray[i]->SetFrame(m_vecFrame[3]);
	}
}

void CTileMap::Update(float fTime)
{
	CSceneComponent::Update(fTime);
}

void CTileMap::PostUpdate(float fTime)
{
	CSceneComponent::PostUpdate(fTime);

	if (m_bTileRender)
	{
		m_iRenderCount	= 0;

		CCameraComponent*	pCamera = m_pScene->GetCameraManager()->GetMainCamera();

		Vector3	vCameraPos = pCamera->GetWorldPos() - GetWorldPos();
		Vector3	vCameraEndPos = vCameraPos + Vector3(_RESOLUTION.iWidth, _RESOLUTION.iHeight, 0.f);

		m_iStartX = (int)(vCameraPos.x / m_vTileSize.x);
		m_iStartY = (int)(vCameraPos.y / m_vTileSize.y);
		m_iEndX = (int)(vCameraEndPos.x / m_vTileSize.x);
		m_iEndY = (int)(vCameraEndPos.y / m_vTileSize.y);

		m_iStartX = m_iStartX < 0 ? 0 : m_iStartX;
		m_iStartY = m_iStartY < 0 ? 0 : m_iStartY;

		m_iEndX = m_iEndX >= m_iTileCountX ? m_iTileCountX - 1 : m_iEndX;
		m_iEndY = m_iEndY >= m_iTileCountY ? m_iTileCountY - 1 : m_iEndY;

		for (int i = m_iStartY; i <= m_iEndY; ++i)
		{
			for (int j = m_iStartX; j <= m_iEndX; ++j)
			{
				m_pTileArray[i * m_iTileCountX + j]->PostUpdate(fTime);
				m_pTileArray[i * m_iTileCountX + j]->SetInstancingData(&m_pInstancingData[m_iRenderCount]);
				++m_iRenderCount;
			}
		}

		D3D11_MAPPED_SUBRESOURCE	tMap	= {};
		CONTEXT->Map(m_pInstancingBuffer->pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);

		memcpy(tMap.pData, m_pInstancingData, sizeof(TileMapInstancingData) * m_iRenderCount);

		CONTEXT->Unmap(m_pInstancingBuffer->pBuffer, 0);

		m_pInstancingBuffer->iCount	= m_iRenderCount;
	}
}

void CTileMap::Render(float fTime)
{
	CSceneComponent::Render(fTime);

	if(m_pMaterial)
	{
		if(m_pAlphaBlend)
			m_pAlphaBlend->SetState();

		size_t	iContainer = m_pMesh->GetContainerCount();

		for (size_t i = 0; i < iContainer; ++i)
		{
			size_t	iSubset = m_pMesh->GetSubsetCount(i);

			for (size_t j = 0; j < iSubset; ++j)
			{
				m_pMaterial->SetMaterial(i, j);
				m_pMesh->Render(i, j);

				m_pMaterial->ResetState();
			}
		}

		if(m_pAlphaBlend)
			m_pAlphaBlend->ResetState();
	}

	if (m_bTileRender)
	{
		m_pTileArray[0]->RenderInstancing(fTime, m_pInstancingBuffer);
		/*for (int i = m_iStartY; i <= m_iEndY; ++i)
		{
			for (int j = m_iStartX; j <= m_iEndX; ++j)
			{
				m_pTileArray[i * m_iTileCountX + j]->Render(fTime);
			}
		}*/
	}
}
