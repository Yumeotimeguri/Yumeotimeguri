#include "GameClear.h"
#include "SceneMgr.h"
#include "DxLib.h"

static int mImageHandle;
static int mImageClearHandle;

void GameOver_Initialize(){
	mImageHandle = LoadGraph("�摜/haikei.png");
	mImageClearHandle = LoadGraph("�摜/gameover.png");
}

void GameOver_Finalize(){
	DeleteGraph(mImageHandle);
}

//�X�V
void GameOver_Update(){
    if(CheckHitKey(KEY_INPUT_RETURN)!=0){//Enter�L�[��������Ă�����
        SceneMgr_ChangeScene(eScene_Menu);//�V�[�������j���[�ɕύX
    }
}

//�`��
void GameOver_Draw(){
	for(int x = 0;x<6;x++){
		for(int y=0;y<4;y++){
			DrawGraph(x*127,y*127,mImageHandle,TRUE);
		}
	}
	DrawGraph(80,120,mImageClearHandle,TRUE);
	DrawString(310, 360,  "Please Push Enter Key",GetColor(255,255,255));
}

