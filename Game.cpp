#include "Game.h"
#include "SceneMgr.h"
#include "DxLib.h"
#include "Field.h"
#include "Tetrmino.h"
#include "Keyboard.h"

static BOOL game_flag;
class Fps{
	int mStartTime;         //測定開始時刻
	int mCount;             //カウンタ
	float mFps;             //fps
	static const int N = 60;//平均を取るサンプル数&移動スピードの制御
	static const int FPS = 60;	//設定したFPS

public:
	Fps(){
		mStartTime = 0;
		mCount = 0;
		mFps = 0;
	}

	bool Update(){
		if( mCount == 0 ){ //1フレーム目なら時刻を記憶
			mStartTime = GetNowCount();
		}
		if(Keyboard_Get(KEY_INPUT_DOWN) > 0){//下が押された場合計算を初期化
			mFps = 0;
			mCount = 0;
			mStartTime = GetNowCount();
		}
		if( mCount == N ){ //60フレーム目なら平均を計算する
			int t = GetNowCount();
			mFps = 1000.f/((t-mStartTime)/(float)N);
			mCount = 0;
			mStartTime = t;
			//下に移動
			addDown();
			
		}
		mCount++;
		return true;
	}

	void Draw(){
		DrawFormatString(550, 0, GetColor(255,255,255), "%.1f", mFps);
	}

	void Wait(){
		int tookTime = GetNowCount() - mStartTime;	//かかった時間
		int waitTime = mCount*1000/FPS - tookTime;	//待つべき時間
		if( waitTime > 0 ){
			Sleep(waitTime);	//待機
		}
	}
};

Fps fps;
void Game_Initialize(){
	game_flag = TRUE;
	Field_Initialize();
	GameStart();

	//Fps fps;
	
}

void Game_Restart(){
	game_flag = TRUE;
	GameStart();
}

void Game_Over(){
	game_flag = FALSE;
	SceneMgr_ChangeScene(eScene_GameOver);
}
void Game_Finalize(){
	
}//終了処理

//更新
void Game_Update(){
	if(game_flag){
		Tetrmino_Update();//キーが入力にされていた場合移動、回転する
		fps.Update();//fpsの更新（テトリミノが自動で下に移動）
		Field_Update(0);//フィールドの更新
		Tetrmino_Old_Update();//異動前のテトリミノの更新
	}
}

//描画
void Game_Draw(){
	if(!game_flag){
		Game_Over_Draw();
	}
	Field_Draw();//フィールドの描写
	Next_Tetrmino_Draw();//次のテトリミノの描写
	
	fps.Wait();//fps制御（６０ｆｐｓになるよう待機）
}
