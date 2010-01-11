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

/*
   FontLoader class is stripped, modified version of AngelCode Tool Box Library copyrighted by Andreas Jonsson.

   AngelCode Tool Box Library
   Copyright (c) 2007-2008 Andreas Jonsson
  
   This software is provided 'as-is', without any express or implied 
   warranty. In no event will the authors be held liable for any 
   damages arising from the use of this software.

   Permission is granted to anyone to use this software for any 
   purpose, including commercial applications, and to alter it and 
   redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you 
      must not claim that you wrote the original software. If you use
      this software in a product, an acknowledgment in the product 
      documentation would be appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and 
      must not be misrepresented as being the original software.

   3. This notice may not be removed or altered from any source 
      distribution.
  
   Andreas Jonsson
   andreas@angelcode.com
*/

#include "StdAfx.h"
#include "font.h"
#include "effect.h"
#include "graphics.h"
#include "mesh.h"

using namespace nit;

namespace nit
{

	class FontLoader
	{
		public:
			FontLoader(FILE *f, Font *font);

			int Load();

			void LoadPage(int id, const char *pageFile);
			void SetCommonInfo(int fontHeight, int base, int scaleW, int scaleH, int pages, bool isPacked);
			void AddChar(int id, int x, int y, int w, int h, int xoffset, int yoffset, int xadvance, int page, int chnl);

			void ReadInfoBlock(int size);
			void ReadCommonBlock(int size);
			void ReadPagesBlock(int size);
			void ReadCharsBlock(int size);
			void ReadKerningPairsBlock(int size);

		protected:
			FILE* f;
			Font* font;
	};

}

SCharDescr::SCharDescr() :
	srcX(0),
	srcY(0),
	srcW(0),
	srcH(0),
	xOff(0),
	yOff(0),
	xAdv(0),
	page(0)
{
}

Font::Font(Graphics* gfx, char* filename, wchar_t* texture_filename) : gfx(gfx), update_time(0)
{
	// effect
	effect = shared_ptr<IEffect>(new IEffect(gfx, L"shaders/font.fx"));
	effect->GetTechniqueByName("Render");
	effect->GetParameterByName("wvp");
	effect->GetParameterByName("gTex");

	// mesh preparation
	num_ver = 4 * MAX_CHARS;
	num_ind = 6 * MAX_CHARS;

	v = new D3DVERTEX[num_ver];
	in = new WORD[num_ind];
	c_num_ver = 0;
	c_num_ind = 0;
	c_v = v;
	c_in = in;

	mesh = shared_ptr<Mesh>(new Mesh(gfx, Mesh::DYNAMIC, Mesh::POS_NOR_TEX, num_ver, num_ind));

	// font loading
	FILE* file = 0;
	fopen_s(&file, filename, "rb");
	
	FontLoader loader(file, this);
	loader.Load();

	gfx->LoadTexture(1, texture_filename);

	// colors
	colors = new D3DXVECTOR3[16];
	colors[0].x = 1.0f; colors[0].y = 1.0f; colors[0].z = 1.0f;
	colors[1].x = 0.0f; colors[1].y = 0.0f; colors[1].z = 0.0f;	
	colors[2].x = 0.8f; colors[2].y = 1.0f; colors[2].z = 0.0f;
	colors[3].x = 0.0f; colors[3].y = 0.8f; colors[3].z = 1.0f;
	colors[4].x = 1.0f; colors[4].y = 0.8f; colors[4].z = 0.0f;	
	colors[5].x = 1.0f; colors[5].y = 0.0f; colors[5].z = 0.0f;	
	colors[6].x = 0.0f; colors[6].y = 1.0f; colors[6].z = 0.0f;	
	colors[7].x = 0.0f; colors[7].y = 0.0f; colors[7].z = 1.0f;	
}

Font::~Font()
{
	delete[] colors;
	delete[] v;
	delete[] in;
}

unsigned int Font::GetWidth(const char* text)
{
	unsigned int width = 0;
	const char* t = text;
	unsigned int count = strlen(t);

	unsigned int n = 0;
	while (n < count)
	{		
		int charId = t[n];
		CHARDESC& ch = chars[charId];

		width += (unsigned int)ch.x_adv;
		n++;
	}

	return width;
}

void Font::Print(float t)
{
	D3DVIEWPORT9 vp;
	gfx->GetDevice()->GetViewport(&vp);

	D3DXMATRIX wvp;
	// Setup orthogonal view Origin is in lower-left corner
	D3DXMatrixIdentity(&wvp);
	D3DXMatrixOrthoOffCenterLH(&wvp, 0, (float)vp.Width, 0, (float)vp.Height, vp.MinZ, vp.MaxZ);

	effect->SetTechnique("Render");
	effect->SetMatrix("wvp", &wvp);
	effect->SetTexture("gTex", gfx->GetTexture(1));

	unsigned int num_passes = 0;
	effect->Begin(&num_passes);
	for (unsigned int i = 0; i < num_passes; i++)
	{
		effect->BeginPass(i);
		gfx->GetDevice()->SetVertexDeclaration(D3DVERTEX::decl);
		gfx->GetDevice()->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, c_num_ver, c_num_ind / 3, in, D3DFMT_INDEX16, v, sizeof(D3DVERTEX));
		effect->EndPass();
	}
	effect->End();

	c_num_ver = 0;
	c_num_ind = 0;
	c_v = v;
	c_in = in;
}

void Font::Write(const char* text_, float x, float y, float z)
{
	if (update_time != 0)
		return;

	// TODO insert 0.5 for more crispy fonts
	x -= 0.5f;
	y -= 0.5f;

	float sx = x;
	const float scale = 1.0f;
	const char* text = text_;
	int count = strlen(text_);

	y += scale * float(base);

	D3DVERTEX* pv = c_v;
	WORD* pin = c_in;

	D3DXVECTOR3* color = &colors[0];

	float width = (float)gfx->GetWidth();
	float height = (float)gfx->GetHeight();

	unsigned int cur = c_num_ver;
	int n = -1;
	while (n < count)
	{
		n++;

		unsigned int cur = c_num_ver;
		int charId = text[n];

		if (charId == '~')
		{
			color = &colors[text[n + 1] - 65];
			n += 1;
			continue;
		}

		if (charId == '\n')
		{
			x = sx;
			y -= scale * float(base) + 2.0f;
			continue;
		}

		CHARDESC& ch = chars[charId];

		float w = ch.width;
		float h = ch.height;
		float u = ch.x;
		float v = ch.y;
		float u2 = u + w * scaleW;
		float v2 = v + h * scaleH;
		float a = ch.x_adv;
		float ox = ch.x_off;
		float oy = ch.y_off;

		if (x > width || y > height)
		{
			continue;
		}

		pv->tex0.x = u;
		pv->tex0.y = v;
		pv->pos.x = x + ox;
		pv->pos.y = y - oy;
		pv->pos.z = z;
		pv->normal = *color;
		pv++;

		pv->tex0.x = u2;
		pv->tex0.y = v;
		pv->pos.x = x + w + ox;
		pv->pos.y = y - oy;
		pv->pos.z = z;
		pv->normal = *color;
		pv++;

		pv->tex0.x = u2;
		pv->tex0.y = v2;
		pv->pos.x = x + w + ox;
		pv->pos.y = y - h - oy;
		pv->pos.z = z;
		pv->normal = *color;
		pv++;

		pv->tex0.x = u;
		pv->tex0.y = v2;
		pv->pos.x = x + ox;
		pv->pos.y = y - h - oy;
		pv->pos.z = z;		
		pv->normal = *color;
		pv++;		

		*pin++ = cur;
		*pin++ = cur + 1;
		*pin++ = cur + 2;

		*pin++ = cur;
		*pin++ = cur + 2;
		*pin++ = cur + 3;

		x += a;
		c_num_ver += 4;
		c_num_ind += 6;
	}

	c_v = pv;
	c_in = pin;
}


// FONT LOADER STUFF

FontLoader::FontLoader(FILE* file, Font* font)
{
	this->f = file;
	this->font = font;
}

int FontLoader::Load()
{
	// Read and validate the tag. It should be 66, 77, 70, 2, 
	// or 'BMF' and 2 where the number is the file version.
	char magicString[4];
	fread(magicString, 4, 1, f);
	if( strncmp(magicString, "BMF\003", 4) != 0 )
	{
		fclose(f);
		return -1;
	}

	// Read each block
	char blockType;
	int blockSize;
	while (fread(&blockType, 1, 1, f))
	{
		// Read the blockSize
		fread(&blockSize, 4, 1, f);

		switch( blockType )
		{
		case 1: // info
			ReadInfoBlock(blockSize);
			break;
		case 2: // common
			ReadCommonBlock(blockSize);
			break;
		case 3: // pages
			ReadPagesBlock(blockSize);
			break;
		case 4: // chars
			ReadCharsBlock(blockSize);
			break;
		case 5: // kerning pairs
			ReadKerningPairsBlock(blockSize);
			break;
		default:
			fclose(f);
			return -1;
		}
	}

	fclose(f);

	// Success
	return 0;
}

void FontLoader::ReadInfoBlock(int size)
{
#pragma pack(push)
#pragma pack(1)
struct infoBlock
{
    WORD fontSize;
    BYTE reserved:4;
    BYTE bold    :1;
    BYTE italic  :1;
    BYTE unicode :1;
    BYTE smooth  :1;
    BYTE charSet;
    WORD stretchH;
    BYTE aa;
    BYTE paddingUp;
    BYTE paddingRight;
    BYTE paddingDown;
    BYTE paddingLeft;
    BYTE spacingHoriz;
    BYTE spacingVert;
    BYTE outline; // Added with version 2
    char fontName[1];
};
#pragma pack(pop)

	char *buffer = new char[size];
	fread(buffer, size, 1, f);

	// We're only interested in the outline thickness
	infoBlock *blk = (infoBlock*)buffer;

	delete[] buffer;
}

void FontLoader::ReadCommonBlock(int size)
{
#pragma pack(push)
#pragma pack(1)
struct commonBlock
{
    WORD lineHeight;
    WORD base;
    WORD scaleW;
    WORD scaleH;
    WORD pages;
    BYTE packed  :1;
    BYTE reserved:7;
	BYTE alphaChnl;
	BYTE redChnl;
	BYTE greenChnl;
	BYTE blueChnl;
}; 
#pragma pack(pop)

	char *buffer = new char[size];
	fread(buffer, size, 1, f);

	commonBlock *blk = (commonBlock*)buffer;

	SetCommonInfo(blk->lineHeight, blk->base, blk->scaleW, blk->scaleH, blk->pages, blk->packed ? true : false);

	delete[] buffer;
}

void FontLoader::ReadPagesBlock(int size)
{
#pragma pack(push)
#pragma pack(1)
struct pagesBlock
{
    char pageNames[1];
};
#pragma pack(pop)

	char *buffer = new char[size];
	fread(buffer, size, 1, f);

	pagesBlock *blk = (pagesBlock*)buffer;

	for( int id = 0, pos = 0; pos < size; id++ )
	{
		pos += 1 + (int)strlen(&blk->pageNames[pos]);
	}

	delete[] buffer;
}

void FontLoader::ReadCharsBlock(int size)
{
#pragma pack(push)
#pragma pack(1)
struct charsBlock
{
    struct charInfo
    {
        DWORD id;
        WORD  x;
        WORD  y;
        WORD  width;
        WORD  height;
        short xoffset;
        short yoffset;
        short xadvance;
        BYTE  page;
        BYTE  chnl;
    } chars[1];
};
#pragma pack(pop)

	char *buffer = new char[size];
	fread(buffer, size, 1, f);

	charsBlock *blk = (charsBlock*)buffer;

	for( int n = 0; int(n*sizeof(charsBlock::charInfo)) < size; n++ )
	{
		AddChar(blk->chars[n].id,
		        blk->chars[n].x,
				blk->chars[n].y,
				blk->chars[n].width,
				blk->chars[n].height,
				blk->chars[n].xoffset,
				blk->chars[n].yoffset,
				blk->chars[n].xadvance,
				blk->chars[n].page,
				blk->chars[n].chnl);
	}

	delete[] buffer;
}

void FontLoader::ReadKerningPairsBlock(int size)
{
#pragma pack(push)
#pragma pack(1)
struct kerningPairsBlock
{
    struct kerningPair
    {
        DWORD first;
        DWORD second;
        short amount;
    } kerningPairs[1];
};
#pragma pack(pop)

	char *buffer = new char[size];
	fread(buffer, size, 1, f);

	kerningPairsBlock *blk = (kerningPairsBlock*)buffer;

	for( int n = 0; int(n*sizeof(kerningPairsBlock::kerningPair)) < size; n++ )
	{
	}

	delete[] buffer;
}

void FontLoader::SetCommonInfo(int fontHeight, int base, int scaleW, int scaleH, int pages, bool isPacked)
{
	font->fontHeight = fontHeight;
	font->base = base;
	font->scaleW = 1.0f / scaleW;
	font->scaleH = 1.0f / scaleH;
}

void FontLoader::AddChar(int id, int x, int y, int w, int h, int xoffset, int yoffset, int xadvance, int page, int chnl)
{
	if( id >= 0 )
	{
		CHARDESC& ch = font->chars[id];
		float scaleW = font->scaleW;
		float scaleH = font->scaleH;

		ch.x = float(x) * scaleW;
		ch.y = float(y) * scaleH;
		ch.width = float(w);
		ch.height = float(h);
		ch.x_off = float(xoffset);
		ch.y_off = float(yoffset);
		ch.x_adv = float(xadvance);
	}

	if( id == -1 )
	{
		font->defChar.srcX = x;
		font->defChar.srcY = y;
		font->defChar.srcW = w;
		font->defChar.srcH = h;
		font->defChar.xOff = xoffset;
		font->defChar.yOff = yoffset;
		font->defChar.xAdv = xadvance;
		font->defChar.page = page;
		font->defChar.chnl = chnl;
	}
}
