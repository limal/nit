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
#include "gui.h"
#include "effect.h"
#include "graphics.h"
#include "mesh.h"

using namespace nit;

IGui::IGui(nitUPDATE& update, Graphics* gfx) : update(update), gfx(gfx), pos(0, 0)
{
	Initialize();
}

IGui::~IGui()
{
	delete[] vertices;
	delete[] indices;
}

void IGui::AddBitmap(unsigned int id, nitGUIBITMAP bitmap)
{
	bitmaps[id] = bitmap;
}

void IGui::Command(unsigned int command_id)
{
	update.command = command_id;
}

void IGui::Draw(const nitVERTEXSCREEN* vsrc, const unsigned int num_vsrc, const WORD* isrc, const unsigned int num_isrc)
{
	memcpy(v, vsrc, num_vsrc * sizeof(nitVERTEXSCREEN));
	//memcpy(k, isrc, num_isrc * sizeof(WORD));
	for (unsigned int i = 0; i < num_isrc; i++)
	{
		*k = *isrc + num_ver;
		k++;
		isrc++;
	}
	v += num_vsrc;
	//k += num_isrc;
	num_ver += num_vsrc;
	num_ind += num_isrc;
}

unsigned int IGui::GetHeight()
{
	return gfx->GetHeight();
}

int IGui::GetMouseX()
{
	return update.mx;
}

int IGui::GetMouseY()
{
	return update.my;
}

unsigned int IGui::GetTextWidth(char* text)
{
	return gfx->GetTextWidth(text);
}

unsigned int IGui::GetWidth()
{
	return gfx->GetWidth();
}

void IGui::Initialize()
{
	active = hot = 0;
	mouse_down = mouse_up = false;

	gfx->LoadTexture(2, L"data/gui-default.dds");
	tex_size = 256;
	effect = shared_ptr<IEffect>(new IEffect(gfx, L"shaders/gui.fx"));
	effect->GetParameterByName("gTex");

	unsigned int max_ver = IGui::MAX_VERTICES;
	unsigned int max_ind = max_ver * 6 /4;

	vertices = new nitVERTEXSCREEN[max_ver];
	indices = new WORD[max_ind];
	v = vertices;
	k = indices;
	num_ver = num_ind = 0;

	LoadBitmaps();

	gfx->AddOnRender(&MakeDelegate(this, &IGui::Render), 50);
}

void IGui::LoadBitmaps()
{
	nitGUIBITMAP bm;

	// load from file

	bm.u1 =  1.0f / 256.0f;
	bm.v1 = 23.0f / 256.0f;
	bm.u2 = 15.0f / 256.0f;	
	bm.v2 = 37.0f / 256.0f;
	bm.width = 14.0f;
	bm.height = 14.0f;
	bm.col = 0xffffffff;
	//bitmaps[BITMAP_NODE_1] = bm;
}

void IGui::Render(Graphics* gfx, float t)
{


	effect->SetTexture("gTex", gfx->GetTexture(2));

	unsigned int num_passes = 0;
	effect->Begin(&num_passes);
	for (unsigned int i = 0; i < num_passes; i++)
	{
		effect->BeginPass(i);
		gfx->GetDevice()->SetVertexDeclaration(nitVERTEXSCREEN::decl);
		gfx->GetDevice()->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, num_ver - 0, (num_ind - 0) / 3, indices, D3DFMT_INDEX16, vertices, sizeof(nitVERTEXSCREEN));
		
		effect->EndPass();
	}
	effect->End();

	if ((mouse_down || mouse_up) && hot == 0)
	{
		//active = 0;
	}
	hot = 0;	
	mouse_down = mouse_up = false;

	v = vertices;
	k = indices;
	num_ver = 0;
	num_ind = 0;
}

void IGui::RenderBitmap(unsigned int key, const float x, const float y, float z)
{
	nitVERTEXSCREEN ver[4];
	WORD ind[6];

	nitVERTEXSCREEN* v = ver;
	WORD* k = ind;

	nitGUIBITMAP& bm = bitmaps[key];
	D3DCOLOR col = bm.col;

	*v++ = nitVERTEXSCREEN(x - 0.5f, y - 0.5f, z, bm.u1, bm.v1, col);
	*v++ = nitVERTEXSCREEN(x + bm.width - 0.5f, y - 0.5f, z, bm.u2, bm.v1, col);
	*v++ = nitVERTEXSCREEN(x + bm.width - 0.5f, y + bm.height - 0.5f, z, bm.u2, bm.v2, col);
	*v++ = nitVERTEXSCREEN(x - 0.5f, y + bm.height - 0.5f, z, bm.u1, bm.v2, col);

	*k++ = 0; *k++ = 1; *k++ = 2;
	*k++ = 2; *k++ = 3; *k++ = 0;

	Draw(ver, 4, ind, 6);
}

void IGui::SetPosition(const float x, const float y)
{
	pos.x = x;
	pos.y = y;
}

void IGui::Text(const char* text, float x, float y)
{
	gfx->Write(text, x, y);
}
