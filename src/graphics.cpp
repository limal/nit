/*
Copyright (C) 2010 Lukasz Wolnik

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

Lukasz Wolnik lukasz.wolnik@o2.pl
*/

#include "StdAfx.h"
#include "graphics.h"
#include "font.h"
#include "mesh.h"
#include "textures.h"

using namespace nit;

IDirect3DVertexDeclaration9* nitVERTEX::decl = 0;
IDirect3DVertexDeclaration9* nitVERTEXCOLOR::decl = 0;
IDirect3DVertexDeclaration9* nitVERTEXSCREEN::decl = 0;

Graphics::Graphics(bool windowed) :
	windowed(windowed)
{
}

Graphics::~Graphics()
{
	ReleaseCOM(device);
	ReleaseCOM(d3d);
}

void Graphics::Release()
{
}

void Graphics::InitializeVertexDeclarations(Graphics* gfx)
{
	IDirect3DDevice9* device = gfx->GetDevice();

	// nitVERTEXCOLOR
	D3DVERTEXELEMENT9 elements_d3dvertexcolor[] =
	{
		{0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		D3DDECL_END()
	};
	device->CreateVertexDeclaration(elements_d3dvertexcolor, &nitVERTEXCOLOR::decl);

	// nitVERTEXSCREEN
	D3DVERTEXELEMENT9 elements_d3dvertexscreen[] =
	{
		{0,  0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0},
		{0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		D3DDECL_END()
	};
	device->CreateVertexDeclaration(elements_d3dvertexscreen, &nitVERTEXSCREEN::decl);

	// nitVERTEX
	D3DVERTEXELEMENT9 elements_d3dvertex[] =
	{
		{0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};
	device->CreateVertexDeclaration(elements_d3dvertex, &nitVERTEX::decl);
}

void Graphics::SetD3DVERTEXDeclaration()
{
	device->SetVertexDeclaration(nitVERTEX::decl);
}

void Graphics::SetD3DVERTEXCOLORDeclaration()
{
	device->SetVertexDeclaration(nitVERTEXCOLOR::decl);
}

void Graphics::AddOnLostDevice(void* delegate)
{
	on_lost_device += *((LostQueueDelegate*)delegate);
}

void Graphics::AddOnResetDevice(void* delegate)
{
	on_reset_device += *((LostQueueDelegate*)delegate);
}

void Graphics::AddOnRender(void* delegate, unsigned int priority)
{
	render.Add(*((RenderQueueDelegate*)delegate), priority);
}

HRESULT Graphics::Initialize(unsigned int width, unsigned int height, HWND hWnd)
{
	return CreateD3D9(width, height, hWnd);
}

void Graphics::BeginRTT(std::string texname)
{
	//render_texture->BeginScene();
}

HRESULT Graphics::CreateD3D9(unsigned int width, unsigned int height, HWND hWnd)
{
	D3DMULTISAMPLE_TYPE multisample_type = D3DMULTISAMPLE_4_SAMPLES;
	//D3DMULTISAMPLE_TYPE multisample_type = D3DMULTISAMPLE_NONE;
	DWORD multisampling_quality = 0;

	//d3d = shared_ptr<IDirect3D9>(Direct3DCreate9(D3D_SDK_VERSION), mem_fn(&IDirect3D9::Release));
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DDISPLAYMODE mode;
	d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);
	d3d->CheckDeviceType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mode.Format, mode.Format, true);

	D3DCAPS9 caps;
	d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	DWORD devBehaviorFlags = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		devBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
	}
	else
	{
		devBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	if (caps.DevCaps & D3DDEVCAPS_PUREDEVICE && devBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING)
	{
		devBehaviorFlags |= D3DCREATE_PUREDEVICE;
	}

	/*if ((caps.VertexShaderVersion < D3DVS_VERSION(3, 0)) && (caps.PixelShaderVersion < D3DPS_VERSION(3, 0)))
	{
      return E_FAIL;
	}*/

	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = windowed;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24X8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (SetMultisampling(multisample_type, d3dpp.BackBufferFormat, d3dpp.AutoDepthStencilFormat, multisampling_quality))
	{
		d3dpp.MultiSampleType = multisample_type;
		d3dpp.MultiSampleQuality = multisampling_quality - 1;
	}
	else
	{
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
		d3dpp.MultiSampleQuality = 0;
	}

	IDirect3DDevice9* raw_device = NULL;
	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, devBehaviorFlags, &d3dpp, &device);
	//device = shared_ptr<IDirect3DDevice9>(raw_device, mem_fn(&IDirect3DDevice9::Release));

	InitializeVertexDeclarations(this);

	textures = shared_ptr<Textures>(new Textures(this));

	font = shared_ptr<Font>(new Font(this, "data/calibri14px.fnt", L"data/calibri14px_0.dds"));
	//render_texture = shared_ptr<RenderTexture>(new RenderTexture(this, 1, false, D3DFMT_X8R8G8B8, true, D3DFMT_D24S8));
	D3DVIEWPORT9 viewport;
	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = 1024;
	viewport.Height = 768;
	viewport.MinZ = 0.0f;
	viewport.MaxZ = 1.0f;
	//render_texture->SetViewport(&viewport);

	return S_OK;
}

void Graphics::EndRTT()
{
	//render_texture->EndScene();
}

IDirect3DDevice9* Graphics::GetDevice()
{
	return device;
}

void Graphics::GetSize(unsigned int* width, unsigned int* height)
{
	*width = d3dpp.BackBufferWidth;
	*height = d3dpp.BackBufferHeight;
}

IDirect3DTexture9* Graphics::GetTexture(unsigned int id)
//IDirect3DTexture9* Graphics::GetTexture(std::string name)
{
	return textures->GetTexture(id);
}

unsigned int Graphics::GetTextWidth(char* text)
{
	return font->GetWidth(text);
}

bool Graphics::IsDeviceLost()
{
	HRESULT hr = device->TestCooperativeLevel();
	bool result = true;

	if (hr == D3DERR_DEVICELOST)
	{
		Sleep(20);
		result = true;
	}
	else
	{
		if (hr == D3DERR_DRIVERINTERNALERROR)
		{
			MessageBox(NULL, L"D3DERR_DRIVERINTERNALERROR", L"Error", MB_ICONERROR);
			PostQuitMessage(0);
			result = true;
		}
		else
		{
			if (hr == D3DERR_DEVICENOTRESET)
			{
				Reset();
				result = false;
			}
			else
			{
				result = false;
			}
		}
	}

	return result;
}

void Graphics::LoadTexture(unsigned int id, wchar_t* filename)
{
	textures->Load(id, filename);
}

void Graphics::Print(float t)
{
	font->Print(t);
}

void Graphics::Reset()
{
	on_lost_device();
	device->Reset(&d3dpp);
	on_reset_device();
}

void Graphics::Resize(unsigned int width, unsigned int height)
{
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;

	Reset();
}

void Graphics::Write(const char* text_, float x, float y, float z)
{
	font->Write(text_, x, y, z);
}

void Graphics::Render(float dt)
{	
	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(115, 115, 115), 1.0f, 0);

	device->BeginScene();
	render(this, dt);
	Print(dt);
	device->EndScene();
	
	device->Present(0, 0, 0, 0);
}

bool Graphics::SetMultisampling(const D3DMULTISAMPLE_TYPE multisample_type, const D3DFORMAT buffer_format, const D3DFORMAT depth_format, DWORD& quality)
{
	bool result = false;

	quality = 0;

	// check if back buffer format is compatible with multisampling
	if (d3d->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, buffer_format, true, multisample_type, &quality) >= 0)
	{
		// check for depth format
		if (d3d->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, depth_format, true, multisample_type, &quality) >= 0)
		{
			result = true;
		}
	}

	return result;
}
