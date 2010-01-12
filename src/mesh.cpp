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
#include "mesh.h"
#include "graphics.h"

using namespace nit;

// TODO Factory for dynamic/static classes

nitVERTEXSCREEN::nitVERTEXSCREEN(const float x, const float y, const D3DCOLOR color) : post(x, y, 0, 1), tex0(0, 0), col(color)
{
}

Mesh::Mesh(Graphics* gfx, MeshType mesh_type, VertexType vertex_type, unsigned int num_ver, unsigned int num_ind)
	: gfx(gfx), mesh_type(mesh_type), vertex_size(0), vertex_decl(0), vertex_type(vertex_type), num_ver(num_ver), num_ind(num_ind)
{
	gfx->AddOnLostDevice(&MakeDelegate(this, &Mesh::OnLostDevice));
	gfx->AddOnResetDevice(&MakeDelegate(this, &Mesh::OnResetDevice));

	Initialize();
}

Mesh::~Mesh()
{
	ReleaseCOM(vb);
	ReleaseCOM(ib);
}

void Mesh::Initialize()
{
	IDirect3DDevice9* device = gfx->GetDevice();

	switch (vertex_type)
	{
		case POS_COL:
			vertex_size = sizeof(nitVERTEXCOLOR);
			vertex_decl = nitVERTEXCOLOR::decl;
			break;

		case POS_NOR_TEX:
			vertex_size = sizeof(nitVERTEX);
			vertex_decl = nitVERTEX::decl;
			break;

		case POS_SCREEN:
			vertex_size = sizeof(nitVERTEXSCREEN);
			vertex_decl = nitVERTEXSCREEN::decl;
			break;
	}

	switch (mesh_type)
	{
		case STATIC:
			device->CreateVertexBuffer(
				num_ver * vertex_size,
				D3DUSAGE_WRITEONLY,
				0,
				D3DPOOL_MANAGED,
				&vb,
				0);

			device->CreateIndexBuffer(
				num_ind * sizeof(WORD),
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&ib,
				0);
			break;

		case DYNAMIC:
			device->CreateVertexBuffer(
				num_ver * vertex_size,
				D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
				0,
				D3DPOOL_DEFAULT,
				&vb,
				0);

			device->CreateIndexBuffer(
				num_ind * sizeof(WORD),
				D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_DEFAULT,
				&ib,
				0);

			break;
	}
}

void Mesh::OnLostDevice()
{
	if (mesh_type == Mesh::DYNAMIC)
	{
		ReleaseCOM(vb);
		ReleaseCOM(ib);
	}
}

void Mesh::OnResetDevice()
{
	if (mesh_type == Mesh::DYNAMIC)
	{
		Initialize();
	}
}

void Mesh::IndexStaticDataFill(const void *src, unsigned int offset, unsigned int num_ind)
{
	WORD* i;

	ib->Lock(offset, num_ind * sizeof(WORD), (void**)&i, 0);
	memcpy(i, src, num_ind * sizeof(WORD));
	ib->Unlock();
}

void Mesh::VertexStaticDataFill(const void *src, unsigned int offset, unsigned int num_ver)
{
	nitVERTEX* v;

	vb->Lock(offset, num_ver * vertex_size, (void**)&v, 0);
	memcpy(v, src, num_ver * vertex_size);
	vb->Unlock();
}

void Mesh::IndexDynamicDataFill(const void *src, unsigned int offset, unsigned int num_ind)
{
	WORD* i;

	// TODO
	//
	// Check if D3DLOCK_NOOVERWRITE flag would be beeter than D3DLOCK_DISCARD!
	// source: http://forum.gamedev.pl/index.php/topic,13496.msg165991.html#msg165991

	//ib->Lock(offset, num_ind * sizeof(WORD), (void**)&i, D3DLOCK_DISCARD);
	ib->Lock(offset, num_ind * sizeof(WORD), (void**)&i, D3DLOCK_NOOVERWRITE);
	memcpy(i, src, num_ind * sizeof(WORD));
	ib->Unlock();
}

void Mesh::VertexDynamicDataFill(const void *src, unsigned int offset, unsigned int num_ver)
{
	nitVERTEX* v;

	//vb->Lock(offset, num_ver * vertex_size, (void**)&v, D3DLOCK_DISCARD);
	vb->Lock(offset, num_ver * vertex_size, (void**)&v, D3DLOCK_NOOVERWRITE);
	memcpy(v, src, num_ver * vertex_size);
	vb->Unlock();
}

void Mesh::Render(Graphics* gfx, unsigned int num_ver, unsigned count)
{
	IDirect3DDevice9* device = gfx->GetDevice();

	device->SetStreamSource(0, vb, 0, vertex_size);
	device->SetIndices(ib);
	device->SetVertexDeclaration(vertex_decl);    

	device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, num_ver, 0, count);
}

void Mesh::RenderNotIndexed(Graphics* gfx, unsigned int count)
{
	IDirect3DDevice9* device = gfx->GetDevice();

	device->SetStreamSource(0, vb, 0, vertex_size);
	device->SetVertexDeclaration(vertex_decl);    

	device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, count);
}
