#pragma once
#include "clsparent.h"
class clsPlayer :
	public clsParent
{
public:
	clsPlayer(void);
	~clsPlayer(void);

	//void draw(LPD3DXSPRITE spt);
	void draw(LPD3DXSPRITE spt,float x, float y);

	void SetPic(LPDIRECT3DDEVICE9 ldspt);

	void MoveLeft();
	void MoveRight();
	void IsMoving(bool);
	void GravityCheck();
	void Jump();
	void IsOnFloor(clsParent* f);
	void IsOnPlatform(clsParent* p, bool isFinish);
	void WaterBlockCol(clsParent* col);
	void KillBlockCol(clsParent* col);
	void SetCheckLocal(float x, float y);

protected:

	bool moving, grounded, jumping, back, forward;

	bool isDead, finish;

	int faceLeft;
	int moveSpeed, gravSpeed, jumpForce, jumpCount;

	float checkX, checkY;

};

