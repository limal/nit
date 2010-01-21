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

#pragma once

#define nitAPI __declspec(dllexport)

#include "FastDelegate.h"

struct nitGraphics
{
	virtual void AddOnLostDevice(void* delegate) = 0;
	virtual void AddOnRender(void* delegate, unsigned int priority) = 0;
	virtual void AddOnResetDevice(void* delegate) = 0;
	virtual IDirect3DDevice9* GetDevice() = 0;
	virtual  unsigned int GetHeight() = 0;
	virtual unsigned int GetTextWidth(char* text) = 0;
	virtual  unsigned int GetWidth() = 0;
	virtual IDirect3DTexture9* GetTexture(unsigned int id) = 0;
	virtual void LoadTexture(unsigned int id, wchar_t* filename) = 0;
	virtual void SetClearColor(D3DCOLOR color) = 0;
	virtual void SetnitVERTEXDeclaration() = 0;
	virtual void SetnitVERTEXCOLORDeclaration() = 0;
	virtual void Write(const char* text_, float x, float y, float z = 0) = 0;
};

struct nitUPDATE
{
	bool vKeys[256];
	bool prevKeys[256];
	unsigned int key_pressed;
	unsigned int key_params;
	unsigned int mouse_params;
	HWND hWnd;
	int mx, my;
	int wheel_x, wheel_y;
	bool mouse_up, mouse_down;
	bool mouse_middle_up, mouse_middle_down;
	bool mouse_right_up, mouse_right_down;
	unsigned short width, height;
	unsigned int command;
	void* gui;
};

struct nitVERTEXCOLOR
{
	D3DXVECTOR3 pos;
	D3DCOLOR col;

	static IDirect3DVertexDeclaration9* decl;
};

struct nitVERTEXSCREEN
{
	nitVERTEXSCREEN()
	{
	}
	nitVERTEXSCREEN(const float x, const float y, const D3DCOLOR color);
	nitVERTEXSCREEN(const float x, const float y, const float z, const float u, const float v, const D3DCOLOR color)
	{
		post.x = x; post.y = y; post.z = z; post.w = 1.0f;
		tex0.x = u; tex0.y = v;
		col = color;
	}

	D3DXVECTOR4 post;
	D3DXVECTOR2 tex0;
	D3DCOLOR col;

	static IDirect3DVertexDeclaration9* decl;
};

struct nitVERTEX
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 tex0;

	static IDirect3DVertexDeclaration9* decl;
};

struct nitGUIBITMAP
{
	float u1, v1, u2, v2;
	float width, height;
	D3DCOLOR col;
};

struct Application
{
	virtual void AddOnCreated(void* delegate) = 0;
	virtual void AddRenderFunction(void* delegate, unsigned int priority) = 0;
	virtual void AddUpdatingFunction(void* delegate) = 0;
	virtual nitUPDATE* Create(wchar_t* title, int nShowCmd) = 0;
	virtual nitGraphics* GetGraphics() = 0;
	virtual nitUPDATE* GetUPDATE() = 0;
	virtual void Update() = 0;
};

struct Effect
{
	virtual void Begin(unsigned int* num_passes) = 0;
	virtual void BeginPass(unsigned int pass) = 0;
	virtual void CommitChanges() = 0;
	virtual void End() = 0;
	virtual void EndPass() = 0;
	virtual void GetParameterByName(char* name) = 0;
	virtual void GetTechniqueByName(char* name) = 0;
	virtual void SetFloat(char* name, const float f) = 0;
	virtual void SetMatrix(char* name, const D3DXMATRIX* matrix) = 0;
	virtual void SetTechnique(char* name) = 0;
	virtual void SetTexture(char* name, IDirect3DTexture9* texture) = 0;
	virtual void SetVector(char* name, const D3DXVECTOR4* vector) = 0;
};

extern "C" nitAPI Effect* APIENTRY nitCreateEffect(nitGraphics* gfx, wchar_t* filename);

class nitModel
{
	public:
		virtual void AddLine(float x1, float y1, float z1, float x2, float y2, float z2, D3DCOLOR color1, D3DCOLOR color2) = 0;
		virtual void AddTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) = 0;		
		virtual void AddVertex(float x, float y, float z, D3DCOLOR color) = 0;
		virtual void DrawVertex(IDirect3DDevice9* device) = 0;
		virtual void DrawVertexColor(IDirect3DDevice9* device) = 0;
		virtual void DrawLines(IDirect3DDevice9* device) = 0;
		virtual nitVERTEX* GetVertices() = 0;
		virtual int* GetIndices() = 0;
		virtual unsigned int GetNumIndices() = 0;
		virtual unsigned int GetNumVertices() = 0;
		virtual void LoadVertex(char* filename) = 0;
		virtual void LoadVertexColor(char* filename) = 0;
};

extern "C" nitAPI nitModel* APIENTRY nitCreateModel();

class Gui
{
	public:
		virtual void AddBitmap(unsigned int id, nitGUIBITMAP bitmap) = 0;
		virtual void Draw(const nitVERTEXSCREEN* vsrc, const unsigned int num_vsrc, const WORD* isrc, const unsigned int num_isrc) = 0;
		virtual unsigned int GetActive() = 0;
		virtual unsigned int GetHot() = 0;
		virtual unsigned int GetTextureSize() = 0;
		virtual void SetActive(unsigned int id) = 0;
		virtual void SetHot(unsigned int id) = 0;
		virtual void RenderBitmap(unsigned int key, const float x, const float y, const float z) = 0;
};

extern "C" nitAPI Application* APIENTRY nitCreateApplication(unsigned int width, unsigned int height, bool antialiasing, bool windowed, bool show_fps);