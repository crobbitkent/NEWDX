#include "Scene.h"
#include "GameMode.h"
#include "../Object/GameObject.h"
#include "../CameraManager.h"

CScene::CScene()	:
	m_pGameMode(nullptr)
{
	SetTypeID<CScene>();

	m_pCameraManager = new CCameraManager;

	m_pCameraManager->m_pScene = this;

	m_pCameraManager->Init();
}

CScene::~CScene()
{
	SAFE_DELETE(m_pCameraManager);
	SAFE_RELEASE_VECLIST(m_BeginList);
	SAFE_DELETE(m_pGameMode);
	SAFE_RELEASE_VECLIST(m_ObjList);
}

CGameMode * CScene::GetGameMode() const
{
	return m_pGameMode;
}

CCameraManager * CScene::GetCameraManager() const
{
	return m_pCameraManager;
}

bool CScene::Init()
{
	m_pGameMode = new CGameMode;

	m_pGameMode->m_pScene = this;

	m_pGameMode->Init();

	return true;
}

void CScene::Begin()
{
	if (!m_BeginList.empty())
	{
		auto iter = m_BeginList.begin();
		auto iterEnd = m_BeginList.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->Begin();
			SAFE_RELEASE((*iter));
		}

		m_BeginList.clear();
	}
}

void CScene::Update(float fTime)
{
	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	while (iter != iterEnd)
	{
		CGameObject*	pObj = *iter;

		if (!pObj->IsActive())
		{
			SAFE_RELEASE(pObj);
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!pObj->IsEnable())
		{
			++iter;
			continue;
		}

		pObj->Update(fTime);

		++iter;
	}

	if (!m_BeginList.empty())
	{
		iter = m_BeginList.begin();
		iterEnd = m_BeginList.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->Begin();
			SAFE_RELEASE((*iter));
		}

		m_BeginList.clear();
	}
}

void CScene::PostUpdate(float fTime)
{
	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	while (iter != iterEnd)
	{
		CGameObject*	pObj = *iter;

		if (!pObj->IsActive())
		{
			SAFE_RELEASE(pObj);
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!pObj->IsEnable())
		{
			++iter;
			continue;
		}

		pObj->PostUpdate(fTime);

		++iter;
	}

	if (!m_BeginList.empty())
	{
		iter = m_BeginList.begin();
		iterEnd = m_BeginList.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->Begin();
			SAFE_RELEASE((*iter));
		}

		m_BeginList.clear();
	}
}

void CScene::Render(float fTime)
{
	auto	iter = m_ObjList.begin();
	auto	iterEnd = m_ObjList.end();

	while (iter != iterEnd)
	{
		CGameObject*	pObj = *iter;

		if (!pObj->IsActive())
		{
			SAFE_RELEASE(pObj);
			iter = m_ObjList.erase(iter);
			iterEnd = m_ObjList.end();
			continue;
		}

		else if (!pObj->IsEnable())
		{
			++iter;
			continue;
		}

		pObj->Render(fTime);

		++iter;
	}
}

bool CScene::AddCamera(CCameraComponent * pCamera)
{
	return m_pCameraManager->AddCamera(pCamera);
}

bool CScene::DeleteCamera(CCameraComponent * pCamera)
{
	return m_pCameraManager->DeleteCamera(pCamera);
}
