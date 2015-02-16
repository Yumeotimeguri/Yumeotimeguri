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
		
	//������//
	SetMainWindowText("������");
	ChangeWindowMode(true);
    DxLib_Init();   
	SetDrawScreen(DX_SCREEN_BACK);
	SceneMgr_Initialize();
	//������//
	

	while(ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && Menu_End() == 0){
		Keyboard_Update();//�L�[�̓��͏�Ԃ��X�V
		SceneMgr_Update();
		SceneMgr_Draw();
	}

    DxLib_End();    // DX���C�u�����I������
    return 0;
}

