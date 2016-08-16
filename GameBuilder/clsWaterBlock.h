#pragma once
#include "clsblock.h"
class clsWaterBlock :
	public clsBlock
{
public:
	clsWaterBlock(void);
	~clsWaterBlock(void);

	void draw(LPD3DXSPRITE spt, float x, float y);
	void SetPic(LPDIRECT3DDEVICE9 ldspt); 

};

