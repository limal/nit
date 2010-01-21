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
#include "model.h"
#include "loadernit.h"

using namespace nit;

IModel::IModel()
{
}

IModel::~IModel()
{
}

void IModel::AddVertex(float x, float y, float z, D3DCOLOR color)
{
	nitVERTEXCOLOR v;

	v.pos.x = x;
	v.pos.y = y;
	v.pos.z = z;
	v.col = color;
	vertices_color.push_back(v);
}

void IModel::AddLine(float x1, float y1, float z1, float x2, float y2, float z2, D3DCOLOR color1, D3DCOLOR color2)
{
	AddVertex(x1, y1, z1, color1);
	AddVertex(x2, y2, z2, color2);
}

void IModel::AddTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
	nitVERTEX v;

	v.normal.x = v.normal.y = v.normal.z = 0;
	v.tex0.x = v.tex0.y = 0;

	WORD current = vertices.size();

	v.pos.x = x1;
	v.pos.y = y1;
	v.pos.z = z1;
	vertices.push_back(v);

	v.pos.x = x2;
	v.pos.y = y2;
	v.pos.z = z2;
	vertices.push_back(v);

	v.pos.x = x3;
	v.pos.y = y3;
	v.pos.z = z3;
	vertices.push_back(v);

	indices.push_back(current);
	indices.push_back(current + 1);
	indices.push_back(current + 2);
}

void IModel::DrawVertex(IDirect3DDevice9* device)
{
	//device->SetVertexDeclaration(nitVERTEX::decl);
	device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, vertices.size(), indices.size() / 3, &indices.front(), D3DFMT_INDEX32, &vertices.front(), sizeof(nitVERTEX));
}

void IModel::DrawVertexColor(IDirect3DDevice9* device)
{
	//device->SetVertexDeclaration(nitVERTEX::decl);
	device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, vertices_color.size(), indices.size() / 3, &indices.front(), D3DFMT_INDEX32, &vertices_color.front(), sizeof(nitVERTEXCOLOR));
}

void IModel::DrawLines(IDirect3DDevice9* device)
{
	//device->SetVertexDeclaration(nitVERTEX::decl);
	device->DrawPrimitiveUP(D3DPT_LINELIST, vertices_color.size() / 2, &vertices_color.front(), sizeof(nitVERTEXCOLOR));
}

void IModel::LoadVertex(char* filename)
{
	shared_ptr<Loadernit> nit(new Loadernit());

	nit->LoadVertex(filename, vertices, indices);
}

void IModel::LoadVertexColor(char* filename)
{
	shared_ptr<Loadernit> nit(new Loadernit());

	nit->LoadVertexColor(filename, vertices_color, indices);
}