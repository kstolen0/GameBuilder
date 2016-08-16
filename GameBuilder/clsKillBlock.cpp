#include "stdafx.h"
#include "clsKillBlock.h"


clsKillBlock::clsKillBlock(void)
{
	vert = false;
	up = false;
	left = false;
}


clsKillBlock::~clsKillBlock(void)
{
}

void clsKillBlock::draw(LPD3DXSPRITE spt, float x, float y)
{
	RECT frame;

	if(!vert)
		SetRect(&frame, 32*animPos, 0, 32 + (32*animPos), 32);
	else
		SetRect(&frame,32*animPos,32,32+(32*animPos),64);


	D3DXVECTOR3 position(xPos,yPos,0);
	spt->Draw(sprite,&frame,NULL,&position,D3DCOLOR_ARGB(trans,redBlock,greenBlock,blueBlock));
	
	frameRate++;
	if(frameRate >= 3)
	{
		frameRate = 0;
		animPos++;
		if(animPos >= 6)
			animPos = 0;
	}

}

void clsKillBlock::SetPic(LPDIRECT3DDEVICE9 ldspt)
{
		D3DXCreateTextureFromFileEx(ldspt,
		L"Images/KillBlock.png",
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

void clsKillBlock::Move()
{
	if(vert)
	{
		if(up)
			yPos -= 2;
		else
			yPos += 2;
	}
	else
	{
		if(left)
			xPos -= 2;
		else
			xPos += 2;
	}
}

void clsKillBlock::BlockCol(clsParent* col)
{
	float oX = col->GetX();
	float oY = col->GetY();
	float oW = col->GetW();
	float oH = col->GetH();

	if(!((xPos + 4 > (oX + oW)) ||
		((xPos + width) - 4 < oX) ||
		(yPos + 4 > (oY + oH)) ||
		((yPos + height) - 4 < oY)))
	{
		if(vert)
		{
			if(up)
				up = false;
			else
				up = true;
		}
		else
		{
			if(left)
				left = false;
			else
				left = true; 
		}
	}
}

void clsKillBlock::SetVert(bool v)
{
	vert = v; 
}

bool clsKillBlock::GetVert()
{
	return vert; 
}