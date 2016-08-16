#pragma once


#include "resource.h"
#include <windows.h>
#include <windowsx.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <ctime>

#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>

#include <XAudio2.h>

IXAudio2* pXA2;

#include "soundStart.h"

// include the DirectInput Library files
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
// include the Direct3D Library file
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")


// define the screen resolution and keyboard macros
#define SCREEN_WIDTH  768
#define SCREEN_HEIGHT 512
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)


// function prototypes
void initDInput(HINSTANCE hInstance, HWND hWnd);    // sets up and initializes DirectInput
void detect_keys(HWND);    // gets the current keys being pressed
void detect_mousepos(void);    // gets the mouse movement and updates the static variables
void cleanDInput(void);    // closes DirectInput and releases memory
void initD3D(HWND hWnd); // sets up and initializes Direct3D
void render_frame(void); // renders a single frame
void cleanD3D(void); // closes Direct3D and releases memory
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);  // the WindowProc function prototype

// Global Variables:
LPDIRECTINPUT8 din;    // the pointer to our DirectInput interface
LPDIRECTINPUTDEVICE8 dinkeyboard;    // the pointer to the keyboard device
LPDIRECTINPUTDEVICE8 dinmouse;    // the pointer to the mouse device
LPD3DXFONT dxfont;    // the pointer to the font object
LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class
LPD3DXSPRITE d3dspt;    // the pointer to our Direct3D Sprite interface






//	INCLUDE CLASSES
/////////////////////

#include "clsParent.h"
#include "clsPlayer.h"
#include "clsBlock.h"
#include "clsKillBlock.h"
#include "clsStrtBlock.h"
#include "clsEndBlock.h"
#include "clsCheckBlock.h"
#include "clsWaterBlock.h"
#include "clsPlaySound.h"

//	SET SOUNDS
//////////////////////////
clsPlaySound* PlayTrack;
clsPlaySound* EditTrack; 

//MY GLOBAL VARIABLES
//////////////////////////

bool GameModeOn = false; 
bool canJump = true;
bool tabDown = true; 
int blockColor = 0, keyPause = 0, blockType = 0; 
int keyDelay = 7;
clsParent* pHead;
clsBlock* plrBlock; 
clsPlayer* plr;

float screenX, screenY;


//INCLUDE OTHER FUNCTIONS
//////////////////////////////

void MakePlayer(float x, float y, float w, float h);
void MakeBlockBase();
void MakeBlock(float x, float y, float w, float h, int count);
void MakeKillBlock(float x, float y, float w, float h, bool v);
void MakeStrtBlock(float x, float y, float w, float h);
void MakeEndBlock(float x, float y, float w, float h);
void MakeCheckBlock(float x, float y, float w, float h);
void MakeWaterBlock(float x, float y, float w, float h);
void MakePlayerBlock();
void ChangeBlockColor(bool);
void ChangeBlockType(bool);
void AddNewBlock();
void AddNewDefBlock();
void AddNewKillBlock();
void AddNewStrtBlock();
void AddNewEndBlock();
void AddNewCheckBlock();
void AddNewWaterBlock();
void DeleteBlock();
bool CheckStrtBlock();
bool CheckEndBlock();
void Reset();

void GamePlayKeys(static BYTE keystate[256]);
void GameEditKeys(static BYTE keystate[256]); 

void PlayerLogic();
void BlockLogic();


//#include "PlayerFunctions.h"
