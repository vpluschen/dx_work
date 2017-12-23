#include "dx_util.h"

IDirect3D9 *m_d3d9 = nullptr;
IDirect3DDevice9 * m_device = nullptr;

bool DxUtil::Init(HWND hWnd, bool fullScreen)
{
	HRESULT hr = 0;

	/* get com interface*/
	m_d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (!m_d3d9)
	{
		::OutputDebugString(L"create d3d interface failed");
		return false;
	}

	/* check device caps*/
	D3DCAPS9 caps;
	hr = m_d3d9->GetDeviceCaps(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		&caps
	);
	if (FAILED(hr))
	{
		::OutputDebugString(L"check device caps failed");
		return false;
	}

	int32_t vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	//���ز�����������ʵ������Ϸ��Ŀ����
	/*���¾�ݵ�ԭ���ǣ�ÿ�����صĻ����������Ƿ���ȫλ�ڶ�����ڲ��������ġ�������ڶ�����ڲ�������Ⱦ����������Ⱦ��
	 *����Ȼ�����ǲ�׼ȷ�ġ���Ϊ��һЩ����ǡ��λ�ڱ�Ե���档
	 *�����������һ��������λ�ڶ�����ڲ�������Ĵ�С������������Ⱦ����ô��õ�Ч��Ҫ�úܶࡣ
	 *�������ص���ɫ���Ƕ������ɫ�����ⲿ����ɫ�Ļ�ϡ�
	*/
	DWORD qualityLevel = 0;
	hr = m_d3d9->CheckDeviceMultiSampleType(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		D3DFMT_A8R8G8B8,
		!fullScreen,
		D3DMULTISAMPLE_4_SAMPLES,
		&qualityLevel
	);

	D3DMULTISAMPLE_TYPE ms = D3DMULTISAMPLE_NONE;
	if (SUCCEEDED(hr) && qualityLevel > 0)
	{
		ms = D3DMULTISAMPLE_4_SAMPLES;
	}

	/* create d3d device*/
	RECT rect;
	if (fullScreen)
	{
		::GetWindowRect(hWnd, &rect);
	}
	else
	{
		::GetClientRect(hWnd, &rect);
	}

	D3DPRESENT_PARAMETERS params = {0};
	params.BackBufferWidth = rect.right - rect.left;
	params.BackBufferHeight = rect.bottom - rect.top;
	params.BackBufferFormat = D3DFMT_A8R8G8B8;
	params.BackBufferCount = 1;
	params.MultiSampleType = ms;
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	params.hDeviceWindow = hWnd;
	params.Windowed = fullScreen;
	params.EnableAutoDepthStencil = true;
	params.AutoDepthStencilFormat = D3DFMT_D24S8;
	params.Flags = 0;
	params.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	hr = m_d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		vp,
		&params,
		&m_device
	);

	if (FAILED(hr) || !m_device)
	{
		::OutputDebugString(L"create d3d device failed\n");
		return false;
	}

	m_d3d9->Release();
	m_d3d9 = nullptr;

	::OutputDebugString(L"d3d device init sucess");

	return true;
}

IDirect3DDevice9 * DxUtil::GetDevice()
{
	return nullptr;
}

void DxUtil::Clear()
{
	if (m_device)
	{
		m_device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0, 0);
		m_device->Present(0, 0, 0, 0);
	}
}

void DxUtil::Release()
{
	if (m_device)
	{
		m_device->Release();
		m_device = nullptr;
	}
}
