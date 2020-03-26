#pragma once

#include "../GameEngine.h"

class ENGINE_DLL CTile
{
	friend class CTileMap;

private:
	CTile();
	~CTile();

private:
	TILE_TYPE	m_eType;
	TILE_OPTION	m_eOption;
	Vector3		m_vPos;
	Vector3		m_vSize;
	class CStaticMesh*	m_pMesh;
	ID3D11InputLayout*	m_pLayout;
	class CMaterial*	m_pMaterial;
	class CRenderState*		m_pAlphaBlend;
	Matrix				m_matWorld;
	Vector2				m_vImgStart;
	Vector2				m_vImgEnd;
	ImageFrame			m_tImgFrame;

public:
	void SetMesh(const string& strName);
	void SetInputLayout(const string& strName);
	void SetMaterial(const string& strName);
	void SetAlphaBlend();
	void SetFrame(const ImageFrame& tFrame);
	void SetInstancingData(PTileMapInstancingData pData);

public:
	void PostUpdate(float fTime);
	void Render(float fTime);
	void RenderInstancing(float fTime, PInstancingBuffer pBuffer);
};

