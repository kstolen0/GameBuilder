#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <stdlib.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib") 
class clsParent
{
public:
	clsParent(void);
	~clsParent(void);


	//BASE FUNCTIONS 
	/////////////////////////////////

	virtual void draw(LPD3DXSPRITE spt);
	virtual void draw(LPD3DXSPRITE spt, float x, float y);

	virtual void SetPic(LPDIRECT3DDEVICE9 ldspt);
	virtual void SetPic(LPDIRECT3DDEVICE9 ldpst, LPCWSTR pic);
	void SetLocal(float x, float y, float w, float h);
	
	float GetX();
	float GetY();
	float GetW();
	float GetH();

	float GetStrtX();
	float GetStrtY();
	float GetCx();

	void SetStrtX(float x);
	void SetStrtY(float y);

	// Pointer for LinkedList
	clsParent* pNext;

	//Block Functions
	virtual void MoveBlock(float x, float y); 
	virtual void Move();
	virtual void BlockCol(clsParent* col);
	virtual bool PlrCol(clsParent* col);
	virtual void SetColor(int t, int r, int g, int b);
	virtual int GetRed();
	virtual int GetGreen();
	virtual int GetBlue();

	//KILL BLOCK FUNCTIONS
	virtual void SetVert(bool);
	virtual bool GetVert();

	// CHECK BLOCK FUNCTIONS
	virtual void ResetCheck();


protected:

	//BASE VARIABLES
	////////////////////////////////

	LPDIRECT3DTEXTURE9 sprite;
	float xPos,yPos,width,height;
	float strtX,cX, strtY; 
	int animPos,frameRate,numFrames;

	//BLOCK VARIABLES
	/////////////////////////

	int trans,redBlock,greenBlock,blueBlock;

};

