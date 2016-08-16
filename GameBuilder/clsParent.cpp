#include "stdafx.h"
#include "clsParent.h"


clsParent::clsParent(void)
{
	//BASE VARIABLES
	////////////////////////
	xPos = 0;
	yPos = 0;
	width = 0;
	height = 0;
	sprite = NULL;
	animPos = 0;
	frameRate = 0; 
	numFrames = 0;
	strtX = 0;
	strtY = 0;
	cX = 0;
	
}


clsParent::~clsParent(void)
{
	if(sprite != NULL)
		sprite->Release(); 
}

//BASE FUNCTIONS
////////////////////////////////////

void clsParent::draw(LPD3DXSPRITE spt)
{
	D3DXVECTOR3 position(xPos,yPos,0);
	spt->Draw(sprite,NULL,NULL,&position,D3DCOLOR_ARGB(255,255,255,255));

}

void clsParent::draw(LPD3DXSPRITE spt, float x, float y)
{
	D3DXVECTOR3 position(xPos,yPos,0);
	spt->Draw(sprite,NULL,NULL,&position,D3DCOLOR_ARGB(255,255,255,255));

	cX = x;
}

void clsParent::SetPic(LPDIRECT3DDEVICE9 ldspt)
{
	//polymorphed
}

void clsParent::SetPic(LPDIRECT3DDEVICE9 ldspt, LPCWSTR pic)
{
	D3DXCreateTextureFromFileEx(ldspt,
		pic,
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

void clsParent::SetLocal(float x, float y, float w, float h)
{
	xPos = x;
	yPos = y;
	width = w; 
	height = h; 
}

float clsParent::GetX()
{
	return xPos;
}

float clsParent::GetY()
{
	return yPos;
}

float clsParent::GetW()
{
	return width;
}

float clsParent::GetH()
{
	return height;
}

float clsParent::GetCx()
{
	return cX;
}

float clsParent::GetStrtX()
{
	return strtX;
}

float clsParent::GetStrtY()
{
	return strtY;
}

void clsParent::SetStrtX(float x)
{
	strtX = x;
}

void clsParent::SetStrtY(float y)
{
	strtY = y;
}

//BLOCK FUNCTIONS
/////////////////////

void clsParent::MoveBlock(float x, float y)
{
	if((xPos > 0 && x < 0) || (xPos + width < 768 && x > 0))
		xPos += x;
	if((yPos > 0 && y < 0) ||  (yPos + height < 512 && y > 0))
		yPos += y;	
}

void clsParent::Move()
{
}

void clsParent::BlockCol(clsParent* col)
{
}

bool clsParent::PlrCol(clsParent* col)
{
	return false;
}

void clsParent::SetColor(int t, int r, int g, int b)
{
	trans = t;
	redBlock = r;
	greenBlock = g;
	blueBlock = b;
}

int clsParent::GetRed()
{
	return redBlock;
}

int clsParent::GetGreen()
{
	return greenBlock;
}

int clsParent::GetBlue()
{
	return blueBlock;
}


//KILLBLOCK FUNCTIONS 

void clsParent::SetVert(bool v)
{
}

bool clsParent::GetVert()
{
	return false;
}


// CHECK BLOCK FUNCTIONS

void clsParent::ResetCheck()
{
}


