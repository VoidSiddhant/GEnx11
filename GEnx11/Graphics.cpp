#include "Graphics.h"

namespace CoreSystems
{
	Graphics::Graphics(const bool& fullscreen)
	{
		enable_msaa4x = true;
		//	this->Initialize();
	}

	Graphics::~Graphics()
	{

	}

	void Graphics::Initialize()
	{
		UINT createDeviceFlags = 0;
		const D3D_FEATURE_LEVEL feature = D3D_FEATURE_LEVEL_11_0;
#if defined(DEBUG) || defined(_DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		//Create Device and acquire the context instance...Context us deffered currently(single threaded) for multiple threading this instance should be changed....
		HRESULT hr = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, NULL,createDeviceFlags, 0, 0, D3D11_SDK_VERSION, &p_d3dDevice, &m_featureLevel, &p_d3dContext);
		if (FAILED(hr))
		{
			MessageBox(gp_Core->GetHWND(), L"Failed to Create Device", L"Error", MB_OK | MB_ABORTRETRYIGNORE);
		}
	
		//Check MSAA Quality for 4x support
		p_d3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &msaa4x);
		assert(msaa4x > 0);

		//Create the Swap Chain Desc and Back Buffer Desc
		DXGI_SWAP_CHAIN_DESC sd;
		RECT r;
		GetClientRect(gp_Core->GetHWND(), &r);
		
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.Height = r.bottom;
		sd.BufferDesc.Width = r.right;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		if (enable_msaa4x)
		{
			sd.SampleDesc.Count = 4;
			sd.SampleDesc.Quality = msaa4x - 1;
		}
		else
		{
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
		}
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.Windowed = true;
		sd.OutputWindow = gp_Core->GetHWND();
		sd.Flags = 0;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		//..**** To change multisample quality at runtime destroy and re create the swap chain*****...//

		//Get Device Interface
		IDXGIDevice* dxgiDevice = 0;
		p_d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
		//Get Adapter Interface
		IDXGIAdapter* dxgiAdapter = 0;
		dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

		//Get Factory Interface
		IDXGIFactory* dxgiFactory = 0;
		dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

		//Create Swap Chain and Link it to the back buffer
		hr = dxgiFactory->CreateSwapChain(p_d3dDevice, &sd, &p_swapChain);

		if (FAILED(hr))
		{
			MessageBox(gp_Core->GetHWND(), L"Create Swap Chain Failed", L"Error", MB_OK);
		}
		//Release the acquired COM Interfaces because we dont need them anymore
		ReleaseCOM(dxgiDevice);
		ReleaseCOM(dxgiAdapter);
		ReleaseCOM(dxgiFactory);

		//Create Render Target View

		ID3D11Texture2D* p_backBuffer;
		//Get BackBuffer of Type texture2D from the Swap Chain Interface
		p_swapChain->GetBuffer(0, IID_ID3D11Texture2D, reinterpret_cast<void**>(&p_backBuffer));
		p_d3dDevice->CreateRenderTargetView(p_backBuffer, 0, &p_rtvD3d);
		ReleaseCOM(p_backBuffer);

		//Describe Depth/Stencil Buffer.....its nothing but a 2d Texture
		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Height = r.bottom;
		depthStencilDesc.Width = r.right;
		depthStencilDesc.ArraySize = 1;
		//depthStencilDesc.CPUAccessFlags
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.MipLevels = 1;
		if (msaa4x)
		{
			depthStencilDesc.SampleDesc.Count = 4;
			depthStencilDesc.SampleDesc.Quality = msaa4x - 1;
		}
		else
		{
			depthStencilDesc.SampleDesc.Count = 1;
			depthStencilDesc.SampleDesc.Quality = 0;
		}

		//cpu access flag read verts and data read only
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;
		
		//Create 2D texture with the above description(thereby creating the depthBuffer)
		ID3D11Texture2D *depthStencilBuffer;
		hr = p_d3dDevice->CreateTexture2D(&depthStencilDesc, 0, &depthStencilBuffer);
	
		if (FAILED(hr))
		{
			MessageBox(gp_Core->GetHWND(), L"Failed depthstencilbuffer", L"Failed", MB_OK);
		}

		hr = p_d3dDevice->CreateDepthStencilView(depthStencilBuffer, 0, &depthStencilView);
		if (FAILED(hr))
		{
			MessageBox(gp_Core->GetHWND(), L"failed depthstencilview", L"failed", MB_OK);
		}
		//Binding View to Output Merger Stage
		p_d3dContext->OMSetRenderTargets(1, &p_rtvD3d, depthStencilView);

		//Create ViewPort

		D3D11_VIEWPORT v;
		v.Height = r.bottom;
		v.Width = r.right;
		v.MaxDepth = 1;
		v.MinDepth = 0;
		v.TopLeftX = 0;
		v.TopLeftY = 0;

		p_d3dContext->RSSetViewports(1, &v);

	}

	void Graphics::OnResize(WPARAM wParam)
	{
		//Re-Size the swap chain with updated window

		RECT r;
		GetClientRect(gp_Core->GetHWND(), &r);
		
		p_swapChain->ResizeBuffers(1, r.right, r.bottom, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
		ID3D11Texture2D* p_backBuffer;
		//Get BackBuffer of Type texture2D from the Swap Chain Interface
		p_swapChain->GetBuffer(0, IID_ID3D11Texture2D, reinterpret_cast<void**>(&p_backBuffer));
		p_d3dDevice->CreateRenderTargetView(p_backBuffer, 0, &p_rtvD3d);
		ReleaseCOM(p_backBuffer);

		//Create the depth stencil Buffer

		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Height = r.bottom;
		depthStencilDesc.Width = r.right;
		depthStencilDesc.ArraySize = 1;
		//depthStencilDesc.CPUAccessFlags
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.MipLevels = 1;
		if (msaa4x)
		{
			depthStencilDesc.SampleDesc.Count = 4;
			depthStencilDesc.SampleDesc.Quality = msaa4x - 1;
		}
		else
		{
			depthStencilDesc.SampleDesc.Count = 1;
			depthStencilDesc.SampleDesc.Quality = 0;
		}
		
		//cpu access flag read verts and data read only
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		//Create 2D texture with the above description(thereby creating the depthBuffer)
		ID3D11Texture2D *depthStencilBuffer;
		HRESULT hr;
		hr = p_d3dDevice->CreateTexture2D(&depthStencilDesc, 0, &depthStencilBuffer);

		if (FAILED(hr))
		{
			MessageBox(gp_Core->GetHWND(), L"Failed depthstencilbuffer", L"Failed", MB_OK);
		}

		hr = p_d3dDevice->CreateDepthStencilView(depthStencilBuffer, 0, &depthStencilView);
		if (FAILED(hr))
		{
			MessageBox(gp_Core->GetHWND(), L"failed depthstencilview", L"failed", MB_OK);
		}
		//Binding View to Output Merger Stage
		p_d3dContext->OMSetRenderTargets(1, &p_rtvD3d, depthStencilView);
	
		D3D11_VIEWPORT v;
		v.Height = r.bottom;
		v.Width = r.right;
		v.MaxDepth = 1;
		v.MinDepth = 0;
		v.TopLeftX = 0;
		v.TopLeftY = 0;

		p_d3dContext->RSSetViewports(1, &v);
	}

	void Graphics::BeginDraw() 
	{
		p_d3dContext->ClearRenderTargetView(p_rtvD3d, (const float*)&Colors::Black);
		p_d3dContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);
	
	}

	void Graphics::EndDraw()
	{
		p_swapChain->Present(0, 0);
	}

	void Graphics::Release()
	{
		//Release COM objects in the reverse order of creation
		ReleaseCOM(p_swapChain);
		ReleaseCOM(p_rtvD3d);
		ReleaseCOM(p_d3dContext);
		ReleaseCOM(p_d3dDevice);
		ReleaseCOM(depthStencilView);
	}
};