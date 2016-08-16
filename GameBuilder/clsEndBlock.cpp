#include "stdafx.h"
#include "clsEndBlock.h"


clsEndBlock::clsEndBlock(void)
{
}


clsEndBlock::~clsEndBlock(void)
{
}

void clsEndBlock::SetPic(LPDIRECT3DDEVICE9 ldspt)
{
		D3DXCreateTextureFromFileEx(ldspt,
		L"Images/EndBlock.png",
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		NULL,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(255,0,255),
		NULL,
		NULL,
		&sprite);
}
