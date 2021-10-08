#ifndef GRAPHICS_H
#define GRAPHICS_H
#include<d3d11.h>
#include"Core.h"
#include"Util.h"


namespace CoreSystems
{
	class Graphics
	{
	public:
		Graphics(const bool& fullscreen);
		~Graphics();
		void Release();
		void BeginDraw();
		void EndDraw();
		void OnResize(WPARAM);
		void Initialize();
		const ID3D11Device& const GetDevice()  { return *p_d3dDevice; }
		const ID3D11DeviceContext& const GetContext()  { return *p_d3dContext; }
	private:
		bool enable_msaa4x;
		UINT msaa4x;
		const ID3D11Device* const p_d3dDevice;
		const ID3D11DeviceContext* const p_d3dContext;
		D3D_FEATURE_LEVEL m_featureLevel;
		UINT m_deviceCreationFlag;
		ID3D11RenderTargetView* p_rtvD3d;
		IDXGISwapChain* p_swapChain;
		ID3D11DepthStencilView* depthStencilView;
		ID3D11RasterizerState* rasterState;
	};
};
#endif