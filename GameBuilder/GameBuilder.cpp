// jesusBaseball.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GameBuilder.h"



// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    HWND hWnd;
    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC)WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"WindowClass";
    RegisterClassEx(&wc);

    hWnd = CreateWindowEx(NULL,
                          L"WindowClass",
                          L"Block Builder! ",
                          WS_OVERLAPPEDWINDOW, 
                          0, 45,
                          SCREEN_WIDTH, SCREEN_HEIGHT,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hWnd, nCmdShow);

	// set up and initialize Direct3D
    initD3D(hWnd);

	// initialise tranceperancies
	d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	d3ddev->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	d3ddev->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

    // set up and initialize DirectInput
    initDInput(hInstance, hWnd);


	//		Setup sound
	initSound(); 

    MSG msg;


	//////////////////////////////////////////////////////////////////
	// initialise game object here////////////////////////////////////
	//////////////////////////////////////////////////////////////////

	pHead = NULL;
	plr = NULL; 
	plrBlock = NULL; 
	screenX = 0;
	screenY = 0; 

	MakeBlockBase();
	MakePlayerBlock();
	//MakeKillBlock(32,256,32,32);

	//	INIT SOUNDS

	PlayTrack = new clsPlaySound;
	PlayTrack->createSound(pXA2,L"Sounds/PlayTrack.wav");


	EditTrack =new clsPlaySound;
	EditTrack->createSound(pXA2,L"Sounds/EditTrack.wav");

    // enter the main loop:
    while(TRUE)
    {
        DWORD starting_point = GetTickCount();

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

		
		//////////////////////////////////////////////////////
		//place main game logic here /////////////////////////
		//////////////////////////////////////////////////////
	
		if(GameModeOn)
		{
			PlayerLogic(); 
			BlockLogic();
			EditTrack->StopSound(pXA2);
			PlayTrack->playSound(pXA2,L"Sounds/PlayTrack.wav");
		}
		else
		{
			PlayTrack->StopSound(pXA2);
			EditTrack->playSound(pXA2,L"Sounds/EditTrack.wav");
		}


		render_frame();
		detect_keys(hWnd);

		while((GetTickCount() - starting_point) < 25); 
	}

	//DELETE LISTS
	/////////////////////

	clsParent* pTemp;
	clsParent* pDel;

	pTemp = pHead;

	while(pTemp != NULL)
	{
		pDel = pTemp;
		pTemp = pTemp->pNext;
		delete(pDel);
	}

	//	Delete Player
	if(plr)
		delete(plr);
	if(plrBlock)
		delete(plrBlock);

    // Clean up DirectInput
    cleanDInput();
    // clean up DirectX and COM
    cleanD3D();

    return msg.wParam;
}

// this is the function that detects keystrokes and displays them in a message box
void detect_keys(HWND hWnd)
{
    static BYTE keystate[256];    // create a static storage for the key-states
    dinkeyboard->Acquire();    // get access if we don't have it already
    dinkeyboard->GetDeviceState(256, (LPVOID)keystate);    // fill keystate with values
	
	if(GameModeOn)
		GamePlayKeys(keystate);
	else
		GameEditKeys(keystate);

    if(keystate[DIK_ESCAPE] & 0x80)   // if the 'escape' key was pressed...
		PostMessage(hWnd, WM_DESTROY, 0, 0);   //delete the main window

    return;
}

// this is the function used to render a single frame
void render_frame(void)
{
    // clear the window
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,100,255), 1.0f, 0);
    d3ddev->BeginScene();    // begins the 3D scene
    d3dspt->Begin(NULL);    // begin sprite drawing


	////////////////////////////////////////////////////////
	//draw all game object here/////////////////////////////
	////////////////////////////////////////////////////////

	clsParent* pTemp;

	pTemp = pHead;

	while(pTemp != NULL)
	{
		pTemp->draw(d3dspt,screenX,screenY);
		pTemp = pTemp->pNext;
	}

	if(plr != NULL)
		plr->draw(d3dspt,screenX,screenY);

	if(plrBlock)
		plrBlock->draw(d3dspt,screenX,screenY);
	

	///////////////////////////////////////////////////////
		///////////////		DRAW HUD		/////////////////
	/////////////////////////////////////////////////////


    d3dspt->End();    // end sprite drawing
    d3ddev->EndScene();    // ends the 3D scene
    d3ddev->Present(NULL, NULL, NULL, NULL);

    return;
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            } break;
    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}

// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
    d3ddev->Release();
    d3d->Release();
    return;
}

// this is the function that initializes DirectInput
void initDInput(HINSTANCE hInstance, HWND hWnd)
{
    // create the DirectInput interface
    DirectInput8Create(hInstance,    // the handle to the application
                       DIRECTINPUT_VERSION,    // the compatible version
                       IID_IDirectInput8,    // the DirectInput interface version
                       (void**)&din,    // the pointer to the interface
                       NULL);    // COM stuff, so we'll set it to NULL

    // create the keyboard device
    din->CreateDevice(GUID_SysKeyboard,    // the default keyboard ID being used
                      &dinkeyboard,    // the pointer to the device interface
                      NULL);    // COM stuff, so we'll set it to NULL    

    dinkeyboard->SetDataFormat(&c_dfDIKeyboard); // set the data format to keyboard format

    // set the control you will have over the keyboard
    dinkeyboard->SetCooperativeLevel(hWnd,
                                     DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

    return;    // return to WinMain()
}



// this function initializes and prepares Direct3D for use
void initD3D(HWND hWnd)
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION);
    D3DPRESENT_PARAMETERS d3dpp;

    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = true;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hWnd;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferWidth = SCREEN_WIDTH;
    d3dpp.BackBufferHeight = SCREEN_HEIGHT;

    // create a device class using this information and the info from the d3dpp stuct
    d3d->CreateDevice(D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      hWnd,
                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                      &d3dpp,
                      &d3ddev);

    D3DXCreateSprite(d3ddev, &d3dspt);    // create the Direct3D Sprite drawing object

	//initialise font object
    D3DXCreateFont(d3ddev,    // the D3D Device
                   12,    // font height of 30
                   0,    // default font width
                   FW_NORMAL,    // font weight
                   1,    // not using MipLevels
                   false,    // italic font
                   DEFAULT_CHARSET,    // default character set
                   OUT_DEFAULT_PRECIS,    // default OutputPrecision,
                   DEFAULT_QUALITY,    // default Quality
                   DEFAULT_PITCH | FF_DONTCARE,    // default pitch and family
                   L"Ariel",    // use Facename Arial
                   &dxfont);    // the font object

    return;
}

// this is the function that closes DirectInput
void cleanDInput(void)
{
    dinkeyboard->Unacquire();    // make sure the keyboard is unacquired
    din->Release();    // close DirectInput before exiting

    return;
}

//			SOUND FUNCTION

void initSound()
{
	CoInitializeEx( NULL, COINIT_MULTITHREADED );
	pXA2 = NULL;
	XAudio2Create( &pXA2, 0, XAUDIO2_DEFAULT_PROCESSOR );
	IXAudio2MasteringVoice* pMasterVoice = NULL;

	pXA2->CreateMasteringVoice( &pMasterVoice, XAUDIO2_DEFAULT_CHANNELS,
                            XAUDIO2_DEFAULT_SAMPLERATE, 0, 0, NULL );
}


//PLAYER FUNCTIONS
///////////////////////

void MakePlayer(float x, float y, float w, float h)
{

	clsParent* pT = pHead;
	clsStrtBlock* pS = NULL; 
	while(pT)
	{
		clsStrtBlock* pStrt = dynamic_cast<clsStrtBlock*>(pT);
		if(pStrt)
		{
			pS = pStrt;
		}
		pT = pT->pNext;
	}

	if(!pS)
		return;

	clsPlayer* pTemp;
	pTemp = new clsPlayer;
	pTemp->SetLocal(pS->GetX() + 10,pS->GetY(),w,h);
	pTemp->SetStrtX(pS->GetX());
	pTemp->SetStrtY(pS->GetY());
	pTemp->SetPic(d3ddev);

	plr = pTemp;
}

void MakeBlockBase()
{
	for(int i = 0; i < 768; i+=32)
		MakeBlock(i,0,32,32,0);
	for(int i = 32; i < 512; i+=32)
		MakeBlock(736,i,32,32,0);
	for(int i = 0; i < 736; i += 32)
		MakeBlock(i,480,32,32,0);
	for(int i = 32; i < 480; i+=32)
		MakeBlock(0,i,32,32,0);
}

void MakeBlock(float x, float y, float w, float h, int count)
{

	clsParent* pTemp;
	pTemp = new clsBlock;
	pTemp->SetLocal(x,y,w,h);
	pTemp->SetPic(d3ddev);
	pTemp->SetColor(255,255,255,255);
	pTemp->pNext = pHead;
	pHead = pTemp; 

}

void MakeKillBlock(float x, float y, float w, float h, bool v)
{

	clsBlock* pTemp = new clsKillBlock;
	
	if(!plrBlock)
		pTemp->SetLocal(x,y,32,32);
	else
		pTemp->SetLocal(plrBlock->GetX(),plrBlock->GetY(),32,32);

	pTemp->SetPic(d3ddev);
	pTemp->SetVert(v);
	pTemp->SetColor(125,255,255,255);
	plrBlock = pTemp;

}

void MakeStrtBlock(float x, float y, float w, float h)
{
	clsBlock* pTemp = new clsStrtBlock;
	if(!plrBlock)
		pTemp->SetLocal(x,y,32,32);
	else
		pTemp->SetLocal(plrBlock->GetX(),plrBlock->GetY(),32,32);

	pTemp->SetPic(d3ddev);
	pTemp->SetColor(125,255,255,255);
	plrBlock = pTemp;
}

void MakeEndBlock(float x, float y, float w, float h)
{
	clsBlock* pTemp = new clsEndBlock;
	if(!plrBlock)
		pTemp->SetLocal(x,y,32,32);
	else
		pTemp->SetLocal(plrBlock->GetX(),plrBlock->GetY(),32,32);

	pTemp->SetPic(d3ddev);
	pTemp->SetColor(125,255,255,255);
	plrBlock = pTemp;
}

void MakeCheckBlock(float x, float y, float w, float h)
{
	clsBlock* pTemp = new clsCheckBlock;
	if(!plrBlock)
		pTemp->SetLocal(x,y,32,32);
	else
		pTemp->SetLocal(plrBlock->GetX(),plrBlock->GetY(),32,32);

	pTemp->SetPic(d3ddev);
	pTemp->SetColor(125,255,255,255);
	plrBlock = pTemp;
}

void MakeWaterBlock(float x, float y, float w, float h)
{
	clsBlock* pTemp = new clsWaterBlock;
	if(!plrBlock)
		pTemp->SetLocal(x,y,32,32);
	else
		pTemp->SetLocal(plrBlock->GetX(),plrBlock->GetY(),32,32);

	pTemp->SetPic(d3ddev);
	pTemp->SetColor(125,255,255,255);
	plrBlock = pTemp;
}

void AddNewBlock()
{
	DeleteBlock(); 

	switch(blockType)
	{
	case 0:
		AddNewDefBlock();
		break;
	case 1:
		AddNewKillBlock();
		break;
	case 2:
		AddNewKillBlock();
		break;
	case 3:
		if(!CheckStrtBlock())
			AddNewStrtBlock();
		break;
	case 4:
		if(!CheckEndBlock())
			AddNewEndBlock();
		break;
	case 5:
		AddNewCheckBlock();
		break;
	case 6:
		AddNewWaterBlock();
		break;
	default:
		break;
	}
}

void AddNewDefBlock()
{
	clsParent* pTemp = new clsBlock;
	pTemp->SetLocal(plrBlock->GetX(),plrBlock->GetY(),32,32);
	pTemp->SetPic(d3ddev);
	pTemp->SetColor(255,plrBlock->GetRed(),plrBlock->GetGreen(),plrBlock->GetBlue());
	pTemp->pNext = pHead;
	pHead = pTemp;
}

void AddNewKillBlock()
{
	clsParent* pTemp = new clsKillBlock;
	pTemp->SetLocal(plrBlock->GetX(),plrBlock->GetY(),32,32);
	pTemp->SetPic(d3ddev);
	pTemp->SetVert(plrBlock->GetVert());
	pTemp->SetColor(255,plrBlock->GetRed(),plrBlock->GetGreen(),plrBlock->GetBlue());
	pTemp->pNext = pHead;
	pHead = pTemp;
}

void AddNewStrtBlock()
{
	clsParent* pTemp = new clsStrtBlock;
	pTemp->SetLocal(plrBlock->GetX(),plrBlock->GetY(),32,32);
	pTemp->SetPic(d3ddev);
	pTemp->SetColor(255,plrBlock->GetRed(),plrBlock->GetGreen(),plrBlock->GetBlue());
	pTemp->pNext = pHead;
	pHead = pTemp;
}

void AddNewEndBlock()
{
	clsParent* pTemp = new clsEndBlock;
	pTemp->SetLocal(plrBlock->GetX(),plrBlock->GetY(),32,32);
	pTemp->SetPic(d3ddev);
	pTemp->SetColor(255,plrBlock->GetRed(),plrBlock->GetGreen(),plrBlock->GetBlue());
	pTemp->pNext = pHead;
	pHead = pTemp;
}

void AddNewCheckBlock()
{
	clsParent* pTemp = new clsCheckBlock;
	pTemp->SetLocal(plrBlock->GetX(),plrBlock->GetY(),32,32);
	pTemp->SetPic(d3ddev);
	pTemp->SetColor(255,plrBlock->GetRed(),plrBlock->GetGreen(),plrBlock->GetBlue());
	pTemp->pNext = pHead;
	pHead = pTemp;
}

void AddNewWaterBlock()
{
	clsParent* pTemp = new clsWaterBlock;
	pTemp->SetLocal(plrBlock->GetX(),plrBlock->GetY(),32,32);
	pTemp->SetPic(d3ddev);
	pTemp->SetColor(255,plrBlock->GetRed(),plrBlock->GetGreen(),plrBlock->GetBlue());
	pTemp->pNext = pHead;
	pHead = pTemp;
}

void DeleteBlock()
{
	int myX = plrBlock->GetX();
	int myY = plrBlock->GetY();
	int oX,oY, dX = 0, dY = 0;
	clsParent* pFound = NULL;

	oX = pHead->GetX();
	oY = pHead->GetY();
	dX = oX - myX; 
	dY = oY - myY; 

	if(((dX < 32 && dX > -1) && (dY < 32 && dY > -1)) || ((dX > -32 && dX < 1) && (dY > -32 && dY < 1)) )
	{
		pFound = pHead;
		pHead = pHead->pNext;
		delete(pFound);
	}
	else
	{
		clsParent* pPrev = pHead;
		clsParent* pCurr = pPrev->pNext;

		while(pCurr)
		{
			oX = pCurr->GetX();
			oY = pCurr->GetY();
			dX = oX - myX;
			dY = oY - myY; 

			if(((dX < 32 && dX > -1) && (dY < 32 && dY > -1)) || ((dX > -32 && dX < 1) && (dY > -32 && dY < 1)) )
			{
				pPrev->pNext = pCurr->pNext;
				delete(pCurr);
				return;
			}

			pPrev = pCurr;
			pCurr = pCurr->pNext;
		}
	}
}

void MakePlayerBlock()
{
	clsBlock* pTemp = new clsBlock;

	if(!plrBlock)
		pTemp->SetLocal(32*4,32*4,32,32);
	else
	{
		pTemp->SetLocal(plrBlock->GetX(),plrBlock->GetY(),32,32);
	}

	pTemp->SetPic(d3ddev);
	pTemp->SetColor(125,255,255,255);
	plrBlock = pTemp;
}


void ChangeBlockColor(bool up)
{

	if(!plrBlock)
		return;

	if(up)
		blockColor++;
	else
		blockColor--;

	switch(blockColor)
	{
	case 0:
		plrBlock->SetColor(125,255,255,255);
		break;
	case 1:
		plrBlock->SetColor(125,220,0,0);
		break;
	case 2:
		plrBlock->SetColor(125,0,200,0);
		break;
	case 3:
		plrBlock->SetColor(125,0,70,255);
		break;
	case 4:
		plrBlock->SetColor(125,0,180,255);
		break;
	case 5:
		plrBlock->SetColor(125,155,155,155);
		break;
	default:
		if(up)
		{
			blockColor = 0;
			plrBlock->SetColor(125,255,255,255);
		}
		else
		{
			blockColor = 5;
			plrBlock->SetColor(125,155,155,155);
		}
		break;
	}
}

void ChangeBlockType(bool right)
{

	if(right)
		blockType++;
	else
		blockType--;

	switch(blockType)
	{
	case 0:
		MakePlayerBlock();
		break;
	case 1:
		MakeKillBlock(32,32,32,32, false);
		break;
	case 2:
		MakeKillBlock(32,32,32,32,true);
		break;
	case 3:
		MakeStrtBlock(32,32,32,32);
		break;
	case 4:
		MakeEndBlock(32,32,32,32);
		break;
	case 5:
		MakeCheckBlock(32,32,32,32);
		break;
	case 6:
		MakeWaterBlock(32,32,32,32);
		break;
	default:
		if(right)
		{
			MakePlayerBlock();
			blockType = 0;
		}
		else
		{
			MakeWaterBlock(32,32,32,32);
			blockType = 6;
		}
	}
}

bool CheckStrtBlock()
{
	clsParent* pTemp = pHead;

	while(pTemp)
	{
		clsStrtBlock* pStrt = dynamic_cast<clsStrtBlock*>(pTemp);
		if(pStrt)
			return true;

		pTemp = pTemp->pNext;
	}

	return false;
}

bool CheckEndBlock()
{
	clsParent* pTemp = pHead;

	while(pTemp)
	{
		clsEndBlock* pEnd = dynamic_cast<clsEndBlock*>(pTemp);
		if(pEnd)
			return true;

		pTemp = pTemp->pNext;
	}
	return false;
}

void Reset()
{
	clsParent* pTemp = pHead;

	while(pTemp)
	{
		pTemp->ResetCheck();
		pTemp = pTemp->pNext;
	}
}

void PlayerLogic()
{

	clsParent* pTemp = pHead;

	while(pTemp)
	{

		clsWaterBlock* wBlk = dynamic_cast<clsWaterBlock*>(pTemp);
		if(wBlk)
		{
			plr->WaterBlockCol(pTemp);
		}
		else
		{
			clsKillBlock* kBlk = dynamic_cast<clsKillBlock*>(pTemp);
			if(kBlk)
				plr->KillBlockCol(pTemp);

			clsEndBlock* eBlk = dynamic_cast<clsEndBlock*>(pTemp);
			if(eBlk)
				plr->IsOnPlatform(pTemp,true);
			else
				plr->IsOnPlatform(pTemp,false);

		}

		pTemp = pTemp->pNext;
	}

	plr->GravityCheck();

}

void BlockLogic()
{
	clsParent* pTemp = pHead;

	while(pTemp)
	{
		pTemp->Move();

		clsKillBlock* pKill = dynamic_cast<clsKillBlock*>(pTemp); 
		if(pKill)
		{
			clsParent* pCol = pHead;
			while(pCol)
			{
				if(pTemp != pCol)
					pTemp->BlockCol(pCol);

				pCol = pCol->pNext;
			}
			//pTemp->BlockCol(plr);
		}
		else
		{
			clsCheckBlock* pChk = dynamic_cast<clsCheckBlock*>(pTemp);
			if(pChk)
				if(pTemp->PlrCol(plr))
					plr->SetCheckLocal(pTemp->GetX(),pTemp->GetY());
		}
				
		pTemp = pTemp->pNext;
	}
}

void GamePlayKeys(static BYTE keystate[256])
{
	if(keystate[DIK_LEFT] & 0x80)
	{
		plr->MoveLeft();								//	MOVE ACTIONS
	}
	if(keystate[DIK_RIGHT] & 0x80)
	{
		plr->MoveRight();
	}
	if(keystate[DIK_SPACE] & 0x80)		//	JUMP ACTION
	{
		if(canJump)
		{
			plr->Jump();
			canJump = false;
		}
	}
	else
	{
		canJump = true; 
	}

	if(keystate[DIK_TAB] & 0x80)		//	CHANGE GAME MODE
	{
		if(!tabDown)
		{
			GameModeOn = false;
			tabDown = true;
			delete(plr);
			plr = NULL;
			Reset();
			MakePlayerBlock();
		}
	}
	else
	{
		tabDown = false; 
	}

}

void GameEditKeys(static BYTE keystate[256])
{
	if(keystate[DIK_TAB] & 0x80)		//CHANGE GAME MODE
	{
		if(!tabDown)
		{
			if(CheckStrtBlock() && CheckEndBlock())
			{
				GameModeOn = true; 
				MakePlayer(36,32,8,16);
				tabDown = true;
				delete(plrBlock);
				plrBlock = NULL;
				blockType = 0;
				blockColor = 0;
			}
		}

	}
	else
	{
		tabDown = false;
	}

	if(keystate[DIK_W] & 0x80)
	{
		if(!keyPause)
		{
			ChangeBlockColor(true);
			keyPause++;
		}
		else
		{
			keyPause++;
			if(keyPause > keyDelay)
				keyPause = 0;

		}
	}
	else if(keystate[DIK_S] & 0x80)
	{
		if(!keyPause)
		{
			ChangeBlockColor(false);
			keyPause++;
		}
		else
		{
			keyPause++;
			if(keyPause > keyDelay)
				keyPause = 0;
		}
	}
	else if(keystate[DIK_D] & 0x80)
	{
		if(!keyPause)
		{
			ChangeBlockType(true);
			keyPause++;
		}
		else
		{
			keyPause++;
			if(keyPause > keyDelay)
				keyPause = 0;
		}
	}
	else if(keystate[DIK_A] & 0x80)
	{
		if(!keyPause)
		{
			ChangeBlockType(false);
			keyPause++;
		}
		else
		{
			keyPause++;
			if(keyPause > keyDelay)
				keyPause = 0;
		}
	}
	else if(keystate[DIK_UP] & 0x80)
	{
		if(!keyPause)
		{
			plrBlock->MoveBlock(0,-32);
			keyPause++;
		}
		else
		{
			keyPause++;
			if(keyPause > keyDelay)
				keyPause = 0;
		}
	}
	else if(keystate[DIK_DOWN] & 0x80)
	{
		if(!keyPause)
		{
			plrBlock->MoveBlock(0,32);
			keyPause++;
		}
		else
		{
			keyPause++;
			if(keyPause > keyDelay)
				keyPause = 0;
		}	}
	else if(keystate[DIK_LEFT] & 0x80)
	{
		if(!keyPause)
		{
			plrBlock->MoveBlock(-32,0);
			keyPause++;
		}
		else
		{
			keyPause++;
			if(keyPause > keyDelay)
				keyPause = 0;
		}	}
	else if(keystate[DIK_RIGHT] & 0x80)
	{
		if(!keyPause)
		{
			plrBlock->MoveBlock(32,0);
			keyPause++;
		}
		else
		{
			keyPause++;
			if(keyPause > keyDelay)
				keyPause = 0;
		}	
	}
	else if(keystate[DIK_SPACE] & 0x80)
	{
		if(!keyPause)
		{
			AddNewBlock(); 
			keyPause++;
		}
		else
		{
			keyPause++;
			if(keyPause > keyDelay)
				keyPause = 0;
		}
	}
	else if(keystate[DIK_BACKSPACE] & 0x80)
	{
		if(!keyPause)
		{
			DeleteBlock();
			keyPause++;
		}
		else
		{
			keyPause++;
			if(keyPause > keyDelay)
				keyPause = 0;
		}
	}
	else
	{
		keyPause = 0;
	}

}