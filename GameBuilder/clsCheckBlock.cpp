#include "stdafx.h"
#include "clsCheckBlock.h"


clsCheckBlock::clsCheckBlock(void)
{
	checked = 0;
}


clsCheckBlock::~clsCheckBlock(void)
{
}

void clsCheckBlock::draw(LPD3DXSPRITE spt, float x, float y)
{
	RECT frame;

		SetRect(&frame, 32*checked, 0, 32 + (32*checked), 32);


	D3DXVECTOR3 position(xPos,yPos,0);
	spt->Draw(sprite,&frame,NULL,&position,D3DCOLOR_ARGB(trans,redBlock,greenBlock,blueBlock));
}

void clsCheckBlock::SetPic(LPDIRECT3DDEVICE9 ldspt)
{
		D3DXCreateTextureFromFileEx(ldspt,
		L"Images/CheckBlock.png",
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

bool clsCheckBlock::PlrCol(clsParent* col)
{
	float oX = col->GetX();
	float oY = col->GetY();
	float oW = col->GetW();
	float oH = col->GetH();

	if(oY + oH >= yPos)
		if(oY + oH < yPos + height)
			if((oX + oW > xPos && oX + oW < xPos + width) ||
				(oX < xPos + width && oX + oW > xPos + width) ||
				(oX > xPos && oX + oW < xPos + width))
			{
				checked = 1;
				return true;
			}

	return false;
}

void clsCheckBlock::ResetCheck()
{
	checked = 0;
}