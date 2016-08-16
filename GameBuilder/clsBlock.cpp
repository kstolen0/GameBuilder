#include "stdafx.h"
#include "clsBlock.h"


clsBlock::clsBlock(void)
{
	trans = 255;
	redBlock = 255;
	greenBlock = 255;
	blueBlock = 255;

}


clsBlock::~clsBlock(void)
{
}

void clsBlock::draw(LPD3DXSPRITE spt, float x, float y)
{
		D3DXVECTOR3 position(xPos,yPos,0);
	spt->Draw(sprite,NULL,NULL,&position,D3DCOLOR_ARGB(trans,redBlock,greenBlock,blueBlock));

	cX = x;
}

void clsBlock::SetPic(LPDIRECT3DDEVICE9 ldspt)
{
	D3DXCreateTextureFromFileEx(ldspt,
		L"Images/WhiteBlock.png",
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

//void clsBlock::Move()
//{
//}
//
//void clsBlock::BlockCol(clsParent* col)
//{
//}

