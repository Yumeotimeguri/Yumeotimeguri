#include "GameOver.h"
#include "SceneMgr.h"
#include "DxLib.h"

static int mImageHandle;
static int mImageClearHandle;

void GameClear_Initialize(){
	mImageHandle = LoadGraph("画像/haikei.png");
	mImageClearHandle = LoadGraph("画像/clear.png");
}

void GameClear_Finalize(){
	DeleteGraph(mImageHandle);
}

//更新
void GameClear_Update(){
    if(CheckHitKey(KEY_INPUT_RETURN)!=0){//Enterキーが押されていたら
        SceneMgr_ChangeScene(eScene_Menu);//シーンをメニューに変更
    }
}

//描画
void GameClear_Draw(){
	for(int x = 0;x<6;x++){
		for(int y=0;y<4;y++){
			DrawGraph(x*127,y*127,mImageHandle,TRUE);
		}
	}
	DrawGraph(80,220,mImageClearHandle,TRUE);
	DrawString(310, 360,  "Please Push Enter Key",GetColor(255,255,255));
}

