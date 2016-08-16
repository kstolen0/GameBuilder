#pragma once
#include "clsblock.h"
class clsEndBlock :
	public clsBlock
{
public:
	clsEndBlock(void);
	~clsEndBlock(void);

	void SetPic(LPDIRECT3DDEVICE9 ldspt);

};

