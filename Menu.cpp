#include "Menu.h"
#include "SceneMgr.h"
#include "DxLib.h"
#include "Keyboard.h"

const static int GAME_Y   = 240;    //�u�Q�[���v������y�ʒu
const static int EXIT_Y = 280;    //�u�ݒ�v������y�ʒu

typedef enum{
    eMenu_Game,        //�Q�[��
    eMenu_Exit,    //�ݒ�

    eMenu_Num,        //�{���ڂ̐�
} eMenu ;

static int NowSelect = eMenu_Game;    //���݂̑I�����(�����̓Q�[���I��)
static int mImageHandle;
static int mImageTitleHandle;
static int menuEnd = 0;

void Menu_Initialize(){
	mImageHandle = LoadGraph("�摜/haikei.png");
	mImageTitleHandle = LoadGraph("�摜/title.png");

}

void Menu_Finalize(){
	DeleteGraph(mImageHandle);
	DeleteGraph(mImageTitleHandle);
}

int Menu_End(){
	return menuEnd;
}

//�X�V
void Menu_Update(){
    if(Keyboard_Get(KEY_INPUT_DOWN)==1){//���L�[��������Ă�����
        NowSelect = (NowSelect+1)%eMenu_Num;//�I����Ԃ��������
    }
    if(Keyboard_Get(KEY_INPUT_UP)==1){//��L�[��������Ă�����
        NowSelect = (NowSelect+(eMenu_Num-1))%eMenu_Num;//�I����Ԃ���グ��
    }
    if(Keyboard_Get(KEY_INPUT_RETURN)==1){//�G���^�[�L�[�������ꂽ��
        switch(NowSelect){//���ݑI�𒆂̏�Ԃɂ���ď����𕪊�
        case eMenu_Game://�Q�[���I�𒆂Ȃ�
            SceneMgr_ChangeScene(eScene_Game);//�V�[�����Q�[����ʂɕύX
            break;
        case eMenu_Exit://Exit�I�𒆂Ȃ�
			menuEnd = 1;
			Menu_End();
			break;
        }
    }
}

//�`��
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
    switch(NowSelect){//���݂̑I����Ԃɏ]���ď����𕪊�
    case eMenu_Game://�Q�[���I�𒆂Ȃ�
        y=GAME_Y;    //�Q�[���̍��W���i�[
        break;
    case eMenu_Exit://�ݒ�I�𒆂Ȃ�
        y=EXIT_Y;    //�ݒ�̍��W���i�[
        break;
    }
    DrawString(450, y, "��", GetColor(255,255,255));
}
