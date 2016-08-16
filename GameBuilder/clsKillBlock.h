#pragma once
#include "clsBlock.h"
class clsKillBlock :
	public clsBlock
{
public:
	clsKillBlock(void);
	~clsKillBlock(void);

	void draw(LPD3DXSPRITE spt, float x, float y);
	void SetPic(LPDIRECT3DDEVICE9 ldspt);
	void Move();
	void BlockCol(clsParent* col);
	void SetVert(bool);
	bool GetVert();

protected:
	bool vert, up, left; 
};

