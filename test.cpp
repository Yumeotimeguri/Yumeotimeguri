#include "DxLib.h"
#include "Block.h"
#include "Field.h"
#include "Keyboard.h"
#include "Tetrmino.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "SceneMgr.h"
#include "Menu.h"


int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
		
	//初期化//
	SetMainWindowText("夢落巡");
	ChangeWindowMode(true);
    DxLib_Init();   
	SetDrawScreen(DX_SCREEN_BACK);
	SceneMgr_Initialize();
	//初期化//
	

	while(ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && Menu_End() == 0){
		Keyboard_Update();//キーの入力状態を更新
		SceneMgr_Update();
		SceneMgr_Draw();
	}

    DxLib_End();    // DXライブラリ終了処理
    return 0;
}

