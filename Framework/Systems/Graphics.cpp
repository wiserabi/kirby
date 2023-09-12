#include "Framework.h"
#include "Graphics.h"

// 스왑체인을 새로 조정하고, 렌더 타겟 뷰와 뷰포트를 업데이트
void Graphics::Resize(const UINT & width, const UINT & height)
{
	// 기존에 생성된 RTV를 제거
	DeleteSurface();
	{
		// 버퍼 크기 변경
		HRESULT hr = swapChain->ResizeBuffers
		(
			0,					 // 백 버퍼의 수
			width,				 // 너비
			height,				 // 높이
			DXGI_FORMAT_UNKNOWN, // 포맷
			0					 // 속성
		);
		assert(SUCCEEDED(hr));
	}
	CreateRenderTargetView();	// 새로운 백 버퍼에 대한 RTV 생성
	SetViewport(width, height);	// 뷰포트를 새로운 크기에 맞게 생성
}

// 뷰포트 설정
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
	// OM 단계에서 사용할 viewport 설정
	deviceContext->RSSetViewports(1, &viewport);
}

// 렌더링을 시작하기 전 피룡한 초기화 작업을 수행
void Graphics::Begin()
{
	deviceContext->OMSetRenderTargets(1, &rtv, nullptr);	// RTV 설정
	deviceContext->RSSetViewports(1, &viewport);			// VIEWPORT 설정
	deviceContext->ClearRenderTargetView(rtv, clearColor);	// RTV를 clearColor로 채운다
}

// 렌더링 끝내고 화면에 결과를 출력
void Graphics::End()
{
	// 백버퍼를 출력하고, 화면 갱신에 대해 대기할지 여부 bVsync로 결정
	HRESULT hr = swapChain->Present(bVsync == true ?  1 : 0, 0);
	assert(SUCCEEDED(hr));
}

void Graphics::GUI()
{
	static bool bOpen = true;
	ImGui::SetNextWindowPos({ 0, 15 });
	ImGui::SetNextWindowSize(ImVec2(200, 30)); // 위젯 창 크기 설정
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

// 스왑체인 생성
void Graphics::CreateSwapchain()
{
	// 이전에 할당한 메모리 해제
	SAFE_RELEASE(device);
	SAFE_RELEASE(deviceContext);
	SAFE_RELEASE(swapChain);

	// 구조체 초기화
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

	// 너비와 높이 설정
	desc.BufferDesc.Width = 0;
	desc.BufferDesc.Height = 0;

	// 수직동기화를 사용할 경우 프레임 레이트 설정
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

	// 버퍼의 쓰임새에 대해서 정의
	// 버퍼 색상 형식 설정
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// 스케일링과 스캔라인 순서
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.ScanlineOrdering
		= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	// 스왑 체인 버퍼 개수, 용도, MSAA 품질 설정
	desc.BufferCount = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	// 출력창 핸들, 창 모드, 스왑 체인 효과 설정
	desc.OutputWindow = handle;
	desc.Windowed = true;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// 사용 가능한 D3D 기능 수준을 나타내는 배열 생성
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

	// 디바이스, 디바이스 컨텍스트, 스왑체인 생성
	HRESULT hr = D3D11CreateDeviceAndSwapChain
	(
		adapterInfos[selectedAdapterIndex]->adapter,	// 최대 vram 용량을 가진 그래픽 장치 선택
		D3D_DRIVER_TYPE_UNKNOWN,						// 드라이버 타입 (자동)
		nullptr,										// 소프트웨어 렌더러 사용 여부 (X)
		0,												// 렌더링시 필요한 플래그 설정
		featureLevel.data(),							// 사용할 Direct3D 기능 레벨
		featureLevel.size(),							// 기능 레벨의 개수
		D3D11_SDK_VERSION,								// DirectX SDK 버전
		&desc,											// 스왑체인을 생성할 때 사용할 구조체
		&swapChain,										// 생성된 스왑 체인 객체 봔환
		&device,										// 디바이스 객체 반환
		nullptr,										// 사용안함
		&deviceContext									// 디바이스 컨텍스트 객체를 반환
	);
	assert(SUCCEEDED(hr));
	// 사이즈 재설정
	Resize(WinMaxWidth, WinMaxHeight);
}

// 스왑체인의 백 버퍼를 가져와서 RTV 생성
void Graphics::CreateRenderTargetView()
{
	// 백 버퍼 가져오기
	ID3D11Texture2D* backbuffer = nullptr;
	HRESULT hr = swapChain->GetBuffer
	(
		0,
		__uuidof(ID3D11Texture2D),
		(void**)&backbuffer
	);
	assert(SUCCEEDED(hr));

	// 백 버퍼로 RTV 생성
	hr = device->CreateRenderTargetView
	(
		backbuffer,
		nullptr,
		&rtv
	);
	assert(SUCCEEDED(hr));

	// 백 버퍼 해제
	SAFE_RELEASE(backbuffer);
}

// rtv 해제
void Graphics::DeleteSurface()
{
	SAFE_RELEASE(rtv);
}

// 어댑터 검색 및 스왑체인 생성
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

// 그래픽 어댑터(그래픽카드) 찾아서 정보 저장
void Graphics::EnumerateAdapters()
{
	// DXGI 객체를 생성하는 함수를 구현한 인터페이스
	IDXGIFactory1* factory;
	if(FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&factory)))
		return;

	UINT index = 0;
	while (true)
	{
		IDXGIAdapter1* adapter = nullptr;
		// 어댑터 나열
		HRESULT hr = factory->EnumAdapters1(index, &adapter);
		
		// 더이상 어댑터가 존재하지 않으면 종료
		if (hr == DXGI_ERROR_NOT_FOUND)
			break;
		// 나열에 실패하면 프로그램 종료
		assert(SUCCEEDED(hr));

		D3DEnumAdapterInfo* adapterInfo = new D3DEnumAdapterInfo();
		ZeroMemory(adapterInfo, sizeof(D3DEnumAdapterInfo));
		adapterInfo->adapterOrdinal = index;
		adapter->GetDesc1(&adapterInfo->adapterDesc);
		adapterInfo->adapter = adapter;

		// 출력장지 나열
		EnumerateAdapterOutput(adapterInfo);
		adapterInfos.push_back(adapterInfo);

		index++;
	}
	// 팩토리 해제
	SAFE_RELEASE(factory);
}

// 어댑터에서 지원하는 출력 모드를 가져오는 역할, 출력 빈도를 저장
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
