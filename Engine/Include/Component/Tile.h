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
	float				m_fG;
	float				m_fH;
	float				m_fTotal;
	NAV_INSERT_TYPE		m_eNavType;
	int					m_iIndexX;
	int					m_iIndexY;
	int					m_iIndex;
	CTile*				m_pParent;

public:
	float GetG()	const;
	float GetTotal()	const;
	NAV_INSERT_TYPE GetNavType()	const;
	int GetIndexX()	const;
	int GetIndexY()	const;
	int GetIndex()	const;
	TILE_OPTION GetTileOption()	const;
	Vector3 GetTilePos()	const;
	CTile* GetParent()	const;

public:
	void SetCost(float fG, float fH);
	void ClearNav();
	void SetNavInsertType(NAV_INSERT_TYPE eType);
	void SetParent(CTile* pParent);

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

