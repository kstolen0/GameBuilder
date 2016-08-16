#pragma once
#include "clsparent.h"
class clsBlock :
	public clsParent
{
public:
	clsBlock(void);
	~clsBlock(void);

	void draw(LPD3DXSPRITE spt, float x, float y);
	void SetPic(LPDIRECT3DDEVICE9 ldspt);
	
protected:
};

