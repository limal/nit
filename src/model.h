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

struct nitVERTEX;

namespace nit
{
	class IModel : public nitModel
	{
		public:
			IModel();
			~IModel();

			
			void AddLine(float x1, float y1, float z1, float x2, float y2, float z2, D3DCOLOR color1, D3DCOLOR color2);
			void AddTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);			
			void AddVertex(float x, float y, float z, D3DCOLOR color);
			void DrawVertex(IDirect3DDevice9* device);
			void DrawVertexColor(IDirect3DDevice9* device);
			void DrawLines(IDirect3DDevice9* device);
			nitVERTEX* GetVertices() { return &vertices.front(); }			
			int* GetIndices() { return &indices.front(); }
			unsigned int GetNumIndices() { return indices.size(); }
			unsigned int GetNumVertices() { return vertices.size(); }
			void Line(float x1, float y1, float z1, float x2, float y2, float z2, float width);
			void LoadVertex(char* filename);
			void LoadVertexColor(char* filename);

		protected:
			std::vector<nitVERTEXCOLOR> vertices_color;
			std::vector<nitVERTEX> vertices;
			std::vector<int> indices;
	};
}
