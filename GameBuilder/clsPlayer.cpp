#include "stdafx.h"
#include "clsPlayer.h"


clsPlayer::clsPlayer(void)
{
	 moving = false;
	 grounded = false;
	 jumping = false;
	 back = false;
	 forward = false; 
	 isDead = false;
	 finish = false;
	 faceLeft = 0;
	 numFrames = 6;
	 moveSpeed = 2;
	 gravSpeed = 5;
	 jumpForce = 16;
	 jumpCount = 0; 
	 checkX = 0;
	 checkY = 0;
}


clsPlayer::~clsPlayer(void)
{
}

//void clsPlayer::draw(LPD3DXSPRITE spt)
//{
//}

void clsPlayer::draw(LPD3DXSPRITE spt,float x, float y)
{
	RECT frame;

	if(!isDead)
	{
		if(finish)						// If I reach the end
		{
			SetRect(&frame,			
				16*animPos,
				48,
				16 + (16*animPos),
				64);
			
			D3DXVECTOR3 position(xPos,yPos,0);
			spt->Draw(sprite,&frame,NULL,&position,D3DCOLOR_ARGB(255,255,255,255));

			grounded = false;

			frameRate++;

			if(frameRate > 4)
			{
				animPos++;
				if(animPos > 6)
					animPos = 6;

				frameRate = 0;
			}
		}
		else
		{
			SetRect(&frame,
			16*animPos,
			16*faceLeft,
			16 + (16*animPos),
			16 + (16*faceLeft));


		D3DXVECTOR3 position(xPos,yPos,0);
		spt->Draw(sprite,&frame,NULL,&position,D3DCOLOR_ARGB(255,255,255,255));
	
		grounded = false; 

		if(moving)
		{
			frameRate++;
			if(frameRate >= 4)
			{
				frameRate = 0;
				animPos++;
				if(animPos >= 6)
					animPos = 0;
			}

			moving = false;
		}
		else
		{
			animPos = 0;
		}
		}

	}
	else					//	If IM DEAD
	{
		SetRect(&frame, 
			16 * animPos,
			32,
			16 + (16 * animPos),
			48);

		D3DXVECTOR3 position(xPos,yPos,0);
		spt->Draw(sprite,&frame,NULL,&position,D3DCOLOR_ARGB(255,255,255,255));

		grounded = false;

		frameRate++;
		if(frameRate >= 4)
		{
			frameRate = 0;
			animPos++;
			if(animPos > 7)
			{
				animPos = 0;
				isDead = false;
				if(checkX > 0 && checkY > 0)
				{
					xPos = checkX + 10;
					yPos = checkY - 16;
				}
				else
				{
					xPos = strtX;
					yPos = strtY;
				}
			}
		}
	}
}

void clsPlayer::SetPic(LPDIRECT3DDEVICE9 ldspt)
{
	D3DXCreateTextureFromFileEx(ldspt,
		L"Images/MrBlock.png",
		D3DX_DEFAULT,    
		D3DX_DEFAULT,    
		D3DX_DEFAULT,    
		NULL,    
		D3DFMT_A8R8G8B8,    
		D3DPOOL_MANAGED,    
		D3DX_DEFAULT,    
		D3DX_DEFAULT,    
		D3DCOLOR_XRGB(255, 0, 255),    
		NULL,    
		NULL,
        &sprite);
}

void clsPlayer::MoveLeft()
{
	if(!isDead && !finish)
	{
		xPos -= moveSpeed;
		faceLeft = 1;
		moving = true;
	
		if(xPos <= 0)
		{
			back = true;
			xPos += moveSpeed;
		}
	}
}

void clsPlayer::MoveRight()
{
	if(!isDead && !finish)
	{
		xPos += moveSpeed;
		faceLeft = 0;
		moving = true;

		if(xPos >= 376)
		{
			forward = true;
			//xPos -= moveSpeed; 
		}
	}
}

void clsPlayer::IsMoving(bool m)
{
	moving = m;
}

void clsPlayer::GravityCheck()
{
	if(!grounded)
		yPos += gravSpeed;


	if(jumping)
	{
		yPos -= jumpForce;
		jumpForce--;
		if(jumpForce <= 0)
			jumpForce = 0;
	}
}

void clsPlayer::Jump()
{
	 jumpCount++;

	if(jumpCount < 3 && !isDead && !finish)
	{
   		jumpForce = 14;
		jumping = true;
		yPos -= gravSpeed;

   		if(jumpCount == 2)
			yPos -= gravSpeed;
	}
}

void clsPlayer::IsOnFloor(clsParent* f)
{
}

void clsPlayer::IsOnPlatform(clsParent* p, bool isFinish)
{

	float x = p->GetX();
	float w = p->GetW();
	float y = p->GetY();
	float h = p->GetH();
	float sX = p->GetStrtX();

	if(!grounded)
	{
		if(yPos + height >= y)
			if(yPos <= y)
				if((xPos + width > x && xPos + width <= x + w) ||
					(xPos < x + w && xPos >= x) || 
					(xPos >= x && xPos + width <= x + w))
				{
					grounded = true;
					jumping = false;
					jumpCount = 0;
					jumpForce = 12;
					yPos = y - height; 
					finish = isFinish;
				}
	}


	if(!isDead)
	{
		if(xPos + width >= x)
			if(xPos <= x)
				if(yPos + (height / 2) > y && yPos <= y)
				{
					xPos -= moveSpeed;
				}
				else if(yPos + height >= y + h && yPos <= y + h)
				{
 					xPos -= moveSpeed;
				}
				else if(yPos >= y && yPos + height <= y + h)
				{
					xPos -= moveSpeed;
				}

		if(xPos <= x + w)	// Player hits right side of block
			if(xPos + width >= x + w)
				if((yPos + (height / 2) > y && yPos <= y) || 
					(yPos + height >= y + h && yPos <= y + h) ||
					(yPos >= y && yPos + height <= y + h))
				{
					xPos += moveSpeed; 
				}
	}
	if(yPos <= y + h)	//	Player hits base of block
		if(yPos + height >= y + h)
			if((xPos + width >= x && xPos <= x) ||
				(xPos <= x + w && xPos + width >= x) ||
				(xPos >= x && xPos + width <= x + w))
			{
				jumpForce = 3; 
			}
}

void clsPlayer::WaterBlockCol(clsParent* col)
{
		float x = col->GetX();
	float w = col->GetW();
	float y = col->GetY();
	float h = col->GetH();
	float sX = col->GetStrtX();


	if(!grounded)
	{
		if(yPos + height >= y + 8)
			if(yPos <= y + 8)
				if((xPos + width > x && xPos + width <= x + w) ||
					(xPos < x + w && xPos >= x) || 
					(xPos >= x && xPos + width <= x + w))
				{
					grounded = true;
					jumping = false;
					jumpCount = 0;
					jumpForce = 12;
					yPos = y - height + 8; 
					isDead = true;
				}
	}


	if(!isDead)
	{
		if(xPos + width >= x)
			if(xPos <= x)
				if(yPos + (height / 2) > y && yPos <= y)
				{
					xPos -= moveSpeed;
				}
				else if(yPos + height >= y + h && yPos <= y + h)
				{
 					xPos -= moveSpeed;
				}
				else if(yPos >= y && yPos + height <= y + h)
				{
					xPos -= moveSpeed;
				}

		if(xPos <= x + w)	// Player hits right side of block
			if(xPos + width >= x + w)
				if((yPos + (height / 2) > y && yPos <= y) || 
					(yPos + height >= y + h && yPos <= y + h) ||
					(yPos >= y && yPos + height <= y + h))
				{
					xPos += moveSpeed; 
				}
	}
	if(yPos <= y + h)	//	Player hits base of block
		if(yPos + height >= y + h)
			if((xPos + width >= x && xPos <= x) ||
				(xPos <= x + w && xPos + width >= x) ||
				(xPos >= x && xPos + width <= x + w))
			{
				jumpForce = 3; 
			}
}

void clsPlayer::KillBlockCol(clsParent* col)
{
	if(!isDead)
	{
		int oX = col->GetX();
		int oY = col->GetY();
		int oW = col->GetW();
		int oH = col->GetH();

			if(!((xPos > (oX + oW)) ||
			((xPos + width) < oX) ||
			(yPos > (oY + oH)) ||
			((yPos + height) < oY)))
			{
				isDead = true;
				animPos = 0;
				frameRate = 0;
			}
	}
}

void clsPlayer::SetCheckLocal(float x, float y)
{
	checkX = x;
	checkY = y;
}


