#include "stdafx.h"
#include "clsStrtBlock.h"


clsStrtBlock::clsStrtBlock(void)
{
}


clsStrtBlock::~clsStrtBlock(void)
{
}

void clsStrtBlock::SetPic(LPDIRECT3DDEVICE9 ldspt)
{
		D3DXCreateTextureFromFileEx(ldspt,
		L"Images/StartBlock.png",
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
