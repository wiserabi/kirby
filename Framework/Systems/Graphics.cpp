#include "Framework.h"
#include "Graphics.h"

// ����ü���� ���� �����ϰ�, ���� Ÿ�� ��� ����Ʈ�� ������Ʈ
void Graphics::Resize(const UINT & width, const UINT & height)
{
	// ������ ������ RTV�� ����
	DeleteSurface();
	{
		// ���� ũ�� ����
		HRESULT hr = swapChain->ResizeBuffers
		(
			0,					 // �� ������ ��
			width,				 // �ʺ�
			height,				 // ����
			DXGI_FORMAT_UNKNOWN, // ����
			0					 // �Ӽ�
		);
		assert(SUCCEEDED(hr));
	}
	CreateRenderTargetView();	// ���ο� �� ���ۿ� ���� RTV ����
	SetViewport(width, height);	// ����Ʈ�� ���ο� ũ�⿡ �°� ����
}

// ����Ʈ ����
void Graphics::SetViewport(const UINT & width, const UINT & height)
{
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
}

void Graphics::InitViewport()
{
	// OM �ܰ迡�� ����� viewport ����
	deviceContext->RSSetViewports(1, &viewport);
}

// �������� �����ϱ� �� �Ƿ��� �ʱ�ȭ �۾��� ����
void Graphics::Begin()
{
	deviceContext->OMSetRenderTargets(1, &rtv, nullptr);	// RTV ����
	deviceContext->RSSetViewports(1, &viewport);			// VIEWPORT ����
	deviceContext->ClearRenderTargetView(rtv, clearColor);	// RTV�� clearColor�� ä���
}

// ������ ������ ȭ�鿡 ����� ���
void Graphics::End()
{
	// ����۸� ����ϰ�, ȭ�� ���ſ� ���� ������� ���� bVsync�� ����
	HRESULT hr = swapChain->Present(bVsync == true ?  1 : 0, 0);
	assert(SUCCEEDED(hr));
}

void Graphics::GUI()
{
	static bool bOpen = true;
	ImGui::SetNextWindowPos({ 0, 15 });
	ImGui::SetNextWindowSize(ImVec2(200, 30)); // ���� â ũ�� ����
	ImGui::Begin
	(
		"Vstnc", 
		&bOpen,
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar
	);
	{
		ImGui::Checkbox("##Vsync", &bVsync);
	}
	ImGui::End();
}

// ����ü�� ����
void Graphics::CreateSwapchain()
{
	// ������ �Ҵ��� �޸� ����
	SAFE_RELEASE(device);
	SAFE_RELEASE(deviceContext);
	SAFE_RELEASE(swapChain);

	// ����ü �ʱ�ȭ
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

	// �ʺ�� ���� ����
	desc.BufferDesc.Width = 0;
	desc.BufferDesc.Height = 0;

	// ��������ȭ�� ����� ��� ������ ����Ʈ ����
	if (bVsync)
	{
		desc.BufferDesc.RefreshRate.Numerator = adapterInfos[0]->outputInfo->numerator;
		desc.BufferDesc.RefreshRate.Denominator = adapterInfos[0]->outputInfo->denominator;
	}
	else
	{
		desc.BufferDesc.RefreshRate.Numerator = 0;
		desc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// ������ ���ӻ��� ���ؼ� ����
	// ���� ���� ���� ����
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// �����ϸ��� ��ĵ���� ����
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.ScanlineOrdering
		= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	// ���� ü�� ���� ����, �뵵, MSAA ǰ�� ����
	desc.BufferCount = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	// ���â �ڵ�, â ���, ���� ü�� ȿ�� ����
	desc.OutputWindow = handle;
	desc.Windowed = true;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// ��� ������ D3D ��� ������ ��Ÿ���� �迭 ����
	vector<D3D_FEATURE_LEVEL> featureLevel
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};


	UINT maxVRam = 0;
	for (UINT i = 0; i < adapterInfos.size(); i++)
	{
		if (adapterInfos[i]->adapterDesc.DedicatedVideoMemory > maxVRam)
		{
			selectedAdapterIndex = i;
			maxVRam = adapterInfos[i]->adapterDesc.DedicatedVideoMemory;
		}
	}

	for (int i = 0; i < adapterInfos.size(); i++)
	{
		gpuName = adapterInfos[i]->adapterDesc.Description;
		wcout << "GPU Name : " << adapterInfos[i]->adapterDesc.Description << endl;
		cout << "VRAM : " << adapterInfos[i]->adapterDesc.DedicatedVideoMemory << endl;
		cout << endl;
	}

	wcout << "Selected GPU Name : " 
		<< adapterInfos[selectedAdapterIndex]->
		adapterDesc.Description << endl;

	// ����̽�, ����̽� ���ؽ�Ʈ, ����ü�� ����
	HRESULT hr = D3D11CreateDeviceAndSwapChain
	(
		adapterInfos[selectedAdapterIndex]->adapter,	// �ִ� vram �뷮�� ���� �׷��� ��ġ ����
		D3D_DRIVER_TYPE_UNKNOWN,						// ����̹� Ÿ�� (�ڵ�)
		nullptr,										// ����Ʈ���� ������ ��� ���� (X)
		0,												// �������� �ʿ��� �÷��� ����
		featureLevel.data(),							// ����� Direct3D ��� ����
		featureLevel.size(),							// ��� ������ ����
		D3D11_SDK_VERSION,								// DirectX SDK ����
		&desc,											// ����ü���� ������ �� ����� ����ü
		&swapChain,										// ������ ���� ü�� ��ü ��ȯ
		&device,										// ����̽� ��ü ��ȯ
		nullptr,										// ������
		&deviceContext									// ����̽� ���ؽ�Ʈ ��ü�� ��ȯ
	);
	assert(SUCCEEDED(hr));
	// ������ �缳��
	Resize(WinMaxWidth, WinMaxHeight);
}

// ����ü���� �� ���۸� �����ͼ� RTV ����
void Graphics::CreateRenderTargetView()
{
	// �� ���� ��������
	ID3D11Texture2D* backbuffer = nullptr;
	HRESULT hr = swapChain->GetBuffer
	(
		0,
		__uuidof(ID3D11Texture2D),
		(void**)&backbuffer
	);
	assert(SUCCEEDED(hr));

	// �� ���۷� RTV ����
	hr = device->CreateRenderTargetView
	(
		backbuffer,
		nullptr,
		&rtv
	);
	assert(SUCCEEDED(hr));

	// �� ���� ����
	SAFE_RELEASE(backbuffer);
}

// rtv ����
void Graphics::DeleteSurface()
{
	SAFE_RELEASE(rtv);
}

// ����� �˻� �� ����ü�� ����
Graphics::Graphics()
{
	EnumerateAdapters();
	CreateSwapchain();
}

Graphics::~Graphics()
{
	SAFE_RELEASE(rtv);
	SAFE_RELEASE(swapChain);
	SAFE_RELEASE(deviceContext);
	SAFE_RELEASE(device);
}

// �׷��� �����(�׷���ī��) ã�Ƽ� ���� ����
void Graphics::EnumerateAdapters()
{
	// DXGI ��ü�� �����ϴ� �Լ��� ������ �������̽�
	IDXGIFactory1* factory;
	if(FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&factory)))
		return;

	UINT index = 0;
	while (true)
	{
		IDXGIAdapter1* adapter = nullptr;
		// ����� ����
		HRESULT hr = factory->EnumAdapters1(index, &adapter);
		
		// ���̻� ����Ͱ� �������� ������ ����
		if (hr == DXGI_ERROR_NOT_FOUND)
			break;
		// ������ �����ϸ� ���α׷� ����
		assert(SUCCEEDED(hr));

		D3DEnumAdapterInfo* adapterInfo = new D3DEnumAdapterInfo();
		ZeroMemory(adapterInfo, sizeof(D3DEnumAdapterInfo));
		adapterInfo->adapterOrdinal = index;
		adapter->GetDesc1(&adapterInfo->adapterDesc);
		adapterInfo->adapter = adapter;

		// ������� ����
		EnumerateAdapterOutput(adapterInfo);
		adapterInfos.push_back(adapterInfo);

		index++;
	}
	// ���丮 ����
	SAFE_RELEASE(factory);
}

// ����Ϳ��� �����ϴ� ��� ��带 �������� ����, ��� �󵵸� ����
bool Graphics::EnumerateAdapterOutput(D3DEnumAdapterInfo * adapterInfo)
{
	IDXGIOutput* output = nullptr;
	HRESULT hr = adapterInfo->adapter->EnumOutputs(0, &output);

	if (DXGI_ERROR_NOT_FOUND == hr)
		return false;
	assert(SUCCEEDED(hr));

	D3DEnumOutputInfo* outputInfo = new D3DEnumOutputInfo();
	ZeroMemory(outputInfo, sizeof(D3DEnumOutputInfo));

	output->GetDesc(&outputInfo->outputDesc);
	outputInfo->output = output;

	UINT numModes = 0;
	DXGI_MODE_DESC* displayModes = nullptr;
	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

	hr = output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
	assert(SUCCEEDED(hr));

	displayModes = new DXGI_MODE_DESC[numModes];
	hr = output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModes);
	assert(SUCCEEDED(hr));

	for (UINT i = 0; i < numModes; i++)
	{
		bool bCheck = true;
		bCheck &= displayModes[i].Width == WinMaxWidth;
		bCheck &= displayModes[i].Height == WinMaxHeight;

		if (bCheck == true)
		{
			outputInfo->numerator = displayModes[i].RefreshRate.Numerator;
			outputInfo->denominator = displayModes[i].RefreshRate.Denominator;
		}
	}

	adapterInfo->outputInfo = outputInfo;
	SAFE_DELETE_ARRAY(displayModes);
	return true;
}

D3DEnumAdapterInfo::~D3DEnumAdapterInfo()
{
	SAFE_RELEASE(adapter);
	SAFE_DELETE(outputInfo);
}

D3DEnumOutputInfo::~D3DEnumOutputInfo()
{
	SAFE_RELEASE(output);
}
