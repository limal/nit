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
#include "loadernit.h"
#include "graphics.h"
#include "mesh.h"

using namespace nit;

Loadernit::Loadernit()
{
	Initialize();
}

Loadernit::~Loadernit()
{
}

void Loadernit::Initialize()
{
}

void Loadernit::LoadVertex(char* filename, std::vector<nitVERTEX>& vertices, std::vector<int>& indices)
{
	FILE* file;
	vector<VECTOR3> normals;
	vector<VECTOR2> texcoords;
	vector<POLYLIST> polylist;
	int num_ver, num_nor, num_tex, num_poly;

	vertices.clear();
	indices.clear();

	fopen_s(&file, filename, "rb");	
	// positions
	fread(&num_ver, sizeof(int), 1, file);
	num_ver /= 3;
	for (int i = 0; i < num_ver; ++i)
	{
		VECTOR3 v;
		nitVERTEX nit_v;
		fread(&v, sizeof(double), 3, file);
		nit_v.pos.x = (float)v.x;
		nit_v.pos.y = (float)v.y;
		nit_v.pos.z = (float)v.z;
		vertices.push_back(nit_v);
	}
	// normals
	fread(&num_nor, sizeof(int), 1, file);
	num_nor /= 3;	
	for (int i = 0; i < num_nor; ++i)
	{
		VECTOR3 n;
		fread(&n, sizeof(double), 3, file);
		normals.push_back(n);
	}
	// uvs
	fread(&num_tex, sizeof(int), 1, file);
	num_tex /= 2;	
	for (int i = 0; i < num_tex; ++i)
	{
		VECTOR2 t;
		fread(&t, sizeof(double), 2, file);
		texcoords.push_back(t);
	}
	// polylist
	fread(&num_poly, sizeof(int), 1, file);
	for (int i = 0; i < num_poly; ++i)
	{
		POLYLIST pl;
		fread(&pl.v1, sizeof(int), 2, file);
		fread(&pl.v2, sizeof(int), 2, file);
		fread(&pl.v3, sizeof(int), 2, file);
		polylist.push_back(pl);
	}

	// assembling
	for (int i = 0; i < num_poly; ++i)
	{
		POLYLIST& pl = polylist[i];
		nitVERTEX& v1 = vertices[pl.v1.vn];
		nitVERTEX& v2 = vertices[pl.v2.vn];
		nitVERTEX& v3 = vertices[pl.v3.vn];

		v1.normal = D3DXVECTOR3((float)normals[pl.v1.vn].x, (float)normals[pl.v1.vn].y, (float)normals[pl.v1.vn].z);
		v1.tex0 = D3DXVECTOR2((float)texcoords[pl.v1.t].u, (float)texcoords[pl.v1.t].v);

		v2.normal = D3DXVECTOR3((float)normals[pl.v2.vn].x, (float)normals[pl.v2.vn].y, (float)normals[pl.v2.vn].z);
		v2.tex0 = D3DXVECTOR2((float)texcoords[pl.v2.t].u, (float)texcoords[pl.v2.t].v);

		v3.normal = D3DXVECTOR3((float)normals[pl.v3.vn].x, (float)normals[pl.v3.vn].y, (float)normals[pl.v3.vn].z);
		v3.tex0 = D3DXVECTOR2((float)texcoords[pl.v3.t].u, (float)texcoords[pl.v3.t].v);

		indices.push_back(pl.v1.vn);
		indices.push_back(pl.v2.vn);
		indices.push_back(pl.v3.vn);
	}

	fclose(file);
}

void Loadernit::LoadVertexColor(char* filename, std::vector<nitVERTEXCOLOR>& vertices_color, std::vector<int>& indices)
{
	FILE* file;
	vector<VECTOR3> normals;
	vector<VECTOR2> texcoords;
	vector<POLYLIST> polylist;
	int num_ver, num_nor, num_tex, num_poly;

	vertices_color.clear();
	indices.clear();

	fopen_s(&file, filename, "rb");	
	// positions
	fread(&num_ver, sizeof(int), 1, file);
	num_ver /= 3;
	for (int i = 0; i < num_ver; ++i)
	{
		VECTOR3 v;
		nitVERTEXCOLOR nit_v;
		fread(&v, sizeof(double), 3, file);
		nit_v.pos.x = (float)v.x;
		nit_v.pos.y = (float)v.y;
		nit_v.pos.z = (float)v.z;
		vertices_color.push_back(nit_v);
	}
	// normals
	fread(&num_nor, sizeof(int), 1, file);
	fseek(file, num_nor * sizeof(double), SEEK_CUR);
	// uvs
	fread(&num_tex, sizeof(int), 1, file);
	fseek(file, num_tex * sizeof(double), SEEK_CUR);
	// polylist
	fread(&num_poly, sizeof(int), 1, file);
	for (int i = 0; i < num_poly; ++i)
	{
		POLYLIST pl;
		fread(&pl.v1, sizeof(int), 2, file);
		fread(&pl.v2, sizeof(int), 2, file);
		fread(&pl.v3, sizeof(int), 2, file);
		polylist.push_back(pl);
	}

	// assembling
	for (int i = 0; i < num_poly; ++i)
	{
		POLYLIST& pl = polylist[i];
		nitVERTEXCOLOR& v1 = vertices_color[pl.v1.vn];
		nitVERTEXCOLOR& v2 = vertices_color[pl.v2.vn];
		nitVERTEXCOLOR& v3 = vertices_color[pl.v3.vn];

		v1.col = 0xffacbd6c;
		v2.col = v1.col;
		v3.col = v1.col;

		indices.push_back(pl.v1.vn);
		indices.push_back(pl.v2.vn);
		indices.push_back(pl.v3.vn);
	}

	fclose(file);
}