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
#include "textures.h"
#include "graphics.h"

using namespace nit;

Textures::Textures(Graphics* gfx) :
	gfx(gfx)
{
}

Textures::~Textures()
{
}

IDirect3DTexture9* Textures::GetTexture(unsigned int id)
{
	return textures[id];
}

void Textures::Load(unsigned int id, wchar_t* filename)
{
	//D3DXCreateTextureFromFile(gfx->GetDevice(), filename.c_str(), &textures[name]);
	D3DXCreateTextureFromFileEx(
		gfx->GetDevice(),
		filename,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0, // D3DX_FROM_FILE to load mipmaps from file, 0 or D3DX_DEFAULT for autogen mipmaps
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT, // filter
		D3DX_DEFAULT, // mipmap filter
		0,
		NULL,
		NULL,
		&textures[id]
	);
}
