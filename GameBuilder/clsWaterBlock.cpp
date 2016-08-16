#include "stdafx.h"
#include "clsWaterBlock.h"


clsWaterBlock::clsWaterBlock(void)
{
}


clsWaterBlock::~clsWaterBlock(void)
{
}

void clsWaterBlock::draw(LPD3DXSPRITE spt, float x, float y)
{
		RECT frame;

		SetRect(&frame, 32*animPos, 0, 32 + (32*animPos), 32);


	D3DXVECTOR3 position(xPos,yPos,0);
	spt->Draw(sprite,&frame,NULL,&position,D3DCOLOR_ARGB(trans,redBlock,greenBlock,blueBlock));

	frameRate++;

	if(frameRate > 8)
	{
		animPos++;
		if(animPos > 1)
			animPos = 0;

		frameRate = 0;
	}
}

void clsWaterBlock::SetPic(LPDIRECT3DDEVICE9 ldspt)
{
		D3DXCreateTextureFromFileEx(ldspt,
		L"Images/WaterBlock.png",
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

