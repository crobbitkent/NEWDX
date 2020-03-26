#include "Texture.h"
#include "../PathManager.h"
#include "../Device.h"

CTexture::CTexture()	:
	m_pSRV(nullptr),
	m_eType(IT_ATLAS)
{
}

CTexture::~CTexture()
{
	SAFE_DELETE_NORMAL_VECLIST(m_vecImage);
	SAFE_RELEASE(m_pSRV);
}

unsigned int CTexture::GetWidth() const
{
	return m_iWidth;
}

unsigned int CTexture::GetHeight() const
{
	return m_iHeight;
}

IMAGE_TYPE CTexture::GetImageType() const
{
	return m_eType;
}

bool CTexture::LoadTexture(const string & strName, const TCHAR * pFileName,
	const string & strPathName)
{
	const TCHAR* pPath = GET_SINGLE(CPathManager)->FindPath(strPathName);

	TCHAR	strFullPath[MAX_PATH] = {};

	if (pPath)
		lstrcpy(strFullPath, pPath);

	lstrcat(strFullPath, pFileName);

	return LoadTextureFullPath(strName, strFullPath);
}

bool CTexture::LoadTextureFullPath(const string & strName, const TCHAR * pFullPath)
{
	SetName(strName);

	ScratchImage*	pImage = new ScratchImage;

	// 경로에서 확장자를 얻어온다.
	TCHAR	strExt[_MAX_EXT] = {};

	_wsplitpath_s(pFullPath, nullptr, 0, nullptr, 0, nullptr, 0, strExt, _MAX_EXT);

	char	_strExt[_MAX_EXT] = {};
	
	WideCharToMultiByte(CP_ACP, 0, strExt, -1, _strExt,
		lstrlen(strExt), nullptr, nullptr);
	
	_strupr_s(_strExt);

	if (strcmp(_strExt, ".DDS") == 0)
	{
		if (FAILED(LoadFromDDSFile(pFullPath, DDS_FLAGS_NONE, nullptr, *pImage)))
			return false;
	}

	else if (strcmp(_strExt, ".TGA") == 0)
	{
		if (FAILED(LoadFromTGAFile(pFullPath, nullptr, *pImage)))
			return false;
	}

	else
	{
		if (FAILED(LoadFromWICFile(pFullPath, WIC_FLAGS_NONE, nullptr, *pImage)))
			return false;
	}

	m_vecImage.push_back(pImage);

	return CreateResource();
}

bool CTexture::LoadTexture(const string & strName, const vector<const TCHAR*>& vecFileName,
	const string & strPathName)
{
	return false;
}

bool CTexture::LoadTextureFullPath(const string & strName, const vector<const TCHAR*>& vecFullPath)
{
	return false;
}

void CTexture::SetTexture(int iRegister, int iShaderType)
{
	if(iShaderType & CST_VERTEX)
		CONTEXT->VSSetShaderResources(iRegister, 1, &m_pSRV);

	if(iShaderType & CST_PIXEL)
		CONTEXT->PSSetShaderResources(iRegister, 1, &m_pSRV);
}

bool CTexture::CreateResource()
{
	if (FAILED(CreateShaderResourceView(DEVICE, m_vecImage[0]->GetImages(),
		m_vecImage[0]->GetImageCount(), m_vecImage[0]->GetMetadata(), &m_pSRV)))
		return false;

	m_iWidth = m_vecImage[0]->GetImages()[0].width;
	m_iHeight = m_vecImage[0]->GetImages()[0].height;

	return true;
}
