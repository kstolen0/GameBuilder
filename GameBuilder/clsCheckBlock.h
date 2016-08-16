#pragma once
#include "clsblock.h"
class clsCheckBlock :
	public clsBlock
{
public:
	clsCheckBlock(void);
	~clsCheckBlock(void);

	void draw(LPD3DXSPRITE spt, float x, float y);
	void SetPic(LPDIRECT3DDEVICE9 ldspt);
	bool PlrCol(clsParent* col);
	void ResetCheck();

protected:
	int checked;
};

