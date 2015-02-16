#include "Game.h"
#include "SceneMgr.h"
#include "DxLib.h"
#include "Field.h"
#include "Tetrmino.h"
#include "Keyboard.h"

static BOOL game_flag;
class Fps{
	int mStartTime;         //����J�n����
	int mCount;             //�J�E���^
	float mFps;             //fps
	static const int N = 60;//���ς����T���v����&�ړ��X�s�[�h�̐���
	static const int FPS = 60;	//�ݒ肵��FPS

public:
	Fps(){
		mStartTime = 0;
		mCount = 0;
		mFps = 0;
	}

	bool Update(){
		if( mCount == 0 ){ //1�t���[���ڂȂ玞�����L��
			mStartTime = GetNowCount();
		}
		if(Keyboard_Get(KEY_INPUT_DOWN) > 0){//���������ꂽ�ꍇ�v�Z��������
			mFps = 0;
			mCount = 0;
			mStartTime = GetNowCount();
		}
		if( mCount == N ){ //60�t���[���ڂȂ畽�ς��v�Z����
			int t = GetNowCount();
			mFps = 1000.f/((t-mStartTime)/(float)N);
			mCount = 0;
			mStartTime = t;
			//���Ɉړ�
			addDown();
			
		}
		mCount++;
		return true;
	}

	void Draw(){
		DrawFormatString(550, 0, GetColor(255,255,255), "%.1f", mFps);
	}

	void Wait(){
		int tookTime = GetNowCount() - mStartTime;	//������������
		int waitTime = mCount*1000/FPS - tookTime;	//�҂ׂ�����
		if( waitTime > 0 ){
			Sleep(waitTime);	//�ҋ@
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
	
}//�I������

//�X�V
void Game_Update(){
	if(game_flag){
		Tetrmino_Update();//�L�[�����͂ɂ���Ă����ꍇ�ړ��A��]����
		fps.Update();//fps�̍X�V�i�e�g���~�m�������ŉ��Ɉړ��j
		Field_Update(0);//�t�B�[���h�̍X�V
		Tetrmino_Old_Update();//�ٓ��O�̃e�g���~�m�̍X�V
	}
}

//�`��
void Game_Draw(){
	if(!game_flag){
		Game_Over_Draw();
	}
	Field_Draw();//�t�B�[���h�̕`��
	Next_Tetrmino_Draw();//���̃e�g���~�m�̕`��
	
	fps.Wait();//fps����i�U�O�������ɂȂ�悤�ҋ@�j
}
