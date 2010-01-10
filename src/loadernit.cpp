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

void Loadernit::Load(wchar_t* filename, std::vector<D3DVERTEX>& vertices, std::vector<WORD>& indices)
{
	float* s(new float[65536]);
	float* t(new float[65536]);
	float* nx = new float[65536];
	float* ny = new float[65536];
	float* nz = new float[65536];
	unsigned int num_tex = 0, num_nor = 0;

	vertices.clear();
	indices.clear();

	std::ifstream ifs(filename);

	while (!ifs.eof())
	{
		const unsigned int MAX_BUFFER = 4096;
		char buffer[MAX_BUFFER];
		std::stringstream ss;

		ifs.getline(buffer, MAX_BUFFER);
		ss.str(buffer);

		std::string token;
		ss >> token;

		if (token == "v")
		{
			float x, y, z;
			ss >> x >> y >> z;

			D3DVERTEX v;
			v.pos = D3DXVECTOR3(x, y, z);
			v.tex0 = D3DXVECTOR2(0, 0);
			v.normal = D3DXVECTOR3(0, 0, 0);

			vertices.push_back(v);
		}
		if (token == "vt")
		{
			ss >> s[num_tex] >> t[num_tex];
			t[num_tex] = 1.0f - t[num_tex];
			num_tex++;
		}
		if (token == "vn")
		{
			ss >> nx[num_nor] >> ny[num_nor] >> nz[num_nor];
			num_nor++;
		}
		if (token == "f")
		{
			unsigned int ver[3], tex[3], nor[3];
			char delim = 0;

			ss >> ver[0] >> delim >> tex[0] >> delim >> nor[0] >>
				ver[1] >> delim >> tex[1] >> delim >> nor[1] >>
				ver[2] >> delim >> tex[2] >> delim >> nor[2];
			--ver[0];
			--ver[1];
			--ver[2];
			--tex[0];
			--tex[1];
			--tex[2];
			--nor[0];
			--nor[1];
			--nor[2];

			indices.push_back(ver[0]);
			indices.push_back(ver[1]);
			indices.push_back(ver[2]);
			vertices[ver[0]].tex0.x = s[tex[0]];
			vertices[ver[0]].tex0.y = t[tex[0]];
			vertices[ver[0]].normal = D3DXVECTOR3(nx[nor[0]], ny[nor[0]], nz[nor[0]]);
			vertices[ver[1]].tex0.x = s[tex[1]];
			vertices[ver[1]].tex0.y = t[tex[1]];
			vertices[ver[1]].normal = D3DXVECTOR3(nx[nor[1]], ny[nor[1]], nz[nor[1]]);
			vertices[ver[2]].tex0.x = s[tex[2]];
			vertices[ver[2]].tex0.y = t[tex[2]];
			vertices[ver[2]].normal = D3DXVECTOR3(nx[nor[2]], ny[nor[2]], nz[nor[2]]);
		}
	}

	delete[] s;
	delete[] t;
	delete[] nx;
	delete[] ny;
	delete[] nz;
}
