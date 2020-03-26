#pragma once

#include "GameEngine.h"

class ENGINE_DLL CDevice
{
private:
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pContext;
	IDXGISwapChain*			m_pSwapChain;
	ID3D11RenderTargetView*	m_pTargetView;
	ID3D11DepthStencilView*	m_pDepthView;
	Resolution				m_tRS;
	float					m_fClearColor[4];
	HWND					m_hWnd;

public:
	ID3D11Device* GetDevice()	const;
	ID3D11DeviceContext* GetContext()	const;
	IDXGISwapChain* GetSwapChain()	const;
	Resolution GetResolution()	const;
	Vector2 GetDeviceToWindowRatio()	const;

public:
	bool Init(HWND hWnd, int iWidth, int iHeight, bool bWindowMode);
	void ClearTarget();
	void Present();

	DECLARE_SINGLE(CDevice)
};

