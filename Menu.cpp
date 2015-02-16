#include "Menu.h"
#include "SceneMgr.h"
#include "DxLib.h"
#include "Keyboard.h"

const static int GAME_Y   = 240;    //「ゲーム」文字のy位置
const static int EXIT_Y = 280;    //「設定」文字のy位置

typedef enum{
    eMenu_Game,        //ゲーム
    eMenu_Exit,    //設定

    eMenu_Num,        //本項目の数
} eMenu ;

static int NowSelect = eMenu_Game;    //現在の選択状態(初期はゲーム選択中)
static int mImageHandle;
static int mImageTitleHandle;
static int menuEnd = 0;

void Menu_Initialize(){
	mImageHandle = LoadGraph("画像/haikei.png");
	mImageTitleHandle = LoadGraph("画像/title.png");

}

void Menu_Finalize(){
	DeleteGraph(mImageHandle);
	DeleteGraph(mImageTitleHandle);
}

int Menu_End(){
	return menuEnd;
}

//更新
void Menu_Update(){
    if(Keyboard_Get(KEY_INPUT_DOWN)==1){//下キーが押されていたら
        NowSelect = (NowSelect+1)%eMenu_Num;//選択状態を一つ下げる
    }
    if(Keyboard_Get(KEY_INPUT_UP)==1){//上キーが押されていたら
        NowSelect = (NowSelect+(eMenu_Num-1))%eMenu_Num;//選択状態を一つ上げる
    }
    if(Keyboard_Get(KEY_INPUT_RETURN)==1){//エンターキーが押されたら
        switch(NowSelect){//現在選択中の状態によって処理を分岐
        case eMenu_Game://ゲーム選択中なら
            SceneMgr_ChangeScene(eScene_Game);//シーンをゲーム画面に変更
            break;
        case eMenu_Exit://Exit選択中なら
			menuEnd = 1;
			Menu_End();
			break;
        }
    }
}

//描画
void Menu_Draw(){
	for(int x = 0;x<6;x++){
		for(int y=0;y<4;y++){
			DrawGraph(x*127,y*127,mImageHandle,TRUE);
		}
	}
	DrawGraph(0,0,mImageTitleHandle,TRUE);
    DrawString(480, GAME_Y,  "Game Start",GetColor(255,255,255));
    DrawString(480, EXIT_Y,"Exit",GetColor(255,255,255));
    int y=0;
    switch(NowSelect){//現在の選択状態に従って処理を分岐
    case eMenu_Game://ゲーム選択中なら
        y=GAME_Y;    //ゲームの座標を格納
        break;
    case eMenu_Exit://設定選択中なら
        y=EXIT_Y;    //設定の座標を格納
        break;
    }
    DrawString(450, y, "→", GetColor(255,255,255));
}
