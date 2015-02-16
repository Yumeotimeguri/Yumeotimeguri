#include "Block.h"
#include "DxLib.h"
#include "Keyboard.h"
#include "Field.h"
#include <time.h>
#include "SceneMgr.h"

static int tetrmino_x = 0;//���݂̃e�g���~�m�̂����W
static int tetrmino_y = 0;//���݂̃e�g���~�m�̂����W
static int old_tetrmino_x = 0;//�ړ��O�̃e�g���~�m�̂����W
static int old_tetrmino_y = 0;//�ړ��O�̃e�g���~�m�̂����W
static int (*p_tetrmino)[4];//�e�g���~�m�z����󂯎��|�C���^
static int tetrmino[4][4] = {0};//�e�g���~�m�z��
static int turn_tetrmino[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};//��]���Ɏg�p����e�g���~�m�z��
static int next_tetrmino[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};//���ɏo������e�g���~�m�z��
static int hold_tetrmino[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};//�ꎞ�ۑ��ł���e�g���~�m�z��
static bool hold_tetrmino_flag = TRUE;//�A���Ńz�[���h�����Ȃ����߂̃t���O
static bool first_hold_flag = FALSE;//�z�[���h�e�g���~�m���󂩔ۂ�
static int bom_x = 0;//�{����x���W
static int bom_y = 0;//�{����y���W
static int bom2_x = 0;//�{��2��x���W
static int bom2_y = 0;//�{��2��y���W
static int bom3_x = 0;//�{��3��x���W
static int bom3_y = 0;//�{��3��y���W
static bool game_flag = TRUE;//�Q�[���I�[�o�[���ۂ�
static int z_action_count = 5;//���{�^���A�N�V�����̎g�p�񐔁i���C���������鐔�j


BOOL checkMove(int move,int move_x,int move_y);
int checkShiftMove(int move);
BOOL checkGameOver();
void holdTetrmino();
int getRight();
int getLeft();
int getDown();
void makeBom(int level);
BOOL checkBom(int move,int move_x,int move_y);
BOOL checkShiftBom(int move);
void GameOver();
void Turn_Tetrmino();
void Down_Tetrmino();
void Right_Tetrmino();
void Left_Tetrmino();
void Shift_Down_Tetrmino();
void Shift_Right_Tetrmino();
void Shift_Left_Tetrmino();


//�V�����e�g���~�m���쐬(�Q�[���N����flag=0,�Q��ڂ���flag=1)
void Tetrmino_Initialize(int flag){
	if(game_flag){//�Q�[���I�[�o�[���Ă���ꍇ�V�����̂����Ȃ�
		tetrmino_x = 3;
		tetrmino_y = 0;
		old_tetrmino_x = 0;
		old_tetrmino_y = 0;
		hold_tetrmino_flag = TRUE;
	
		makeBom(return_Level());//�{���쐬
	
		if(flag==0){
			srand((unsigned)time(NULL));
			p_tetrmino = createTetrmino(rand()%7+1);
			for(int x=0;x<4;x++){
				for(int y=0;y<4;y++){
					tetrmino[x][y] = p_tetrmino[x][y];
				}
			}
			srand((unsigned)time(NULL));
			p_tetrmino = createTetrmino(rand()%7+1);

			for(int x=0;x<4;x++){
				for(int y=0;y<4;y++){
					next_tetrmino[x][y] = p_tetrmino[x][y];
				}
			}

		}else if(flag==1){
			for(int x=0;x<4;x++){
				for(int y=0;y<4;y++){
					tetrmino[x][y] = next_tetrmino[x][y];
				}
			}
			srand((unsigned)time(NULL));
			p_tetrmino = createTetrmino(rand()%7+1);

			for(int x=0;x<4;x++){
				for(int y=0;y<4;y++){
					next_tetrmino[x][y] = p_tetrmino[x][y];
				}
			}
		}

		if(return_Level() == 5){
			int turn = rand()*3+1;
			for(int i=0;i<turn;i++){
				Turn_Tetrmino();
			}
		}	

		if(checkGameOver() == FALSE){
			game_flag = FALSE;
			GameOver();
		}
	}
}




//�X�V


//�L�[�����͂ɂ���Ă����ꍇ�ړ��A��]����
void Tetrmino_Update(){
	int level = return_Level();
	//Up�i��]�j
	if(Keyboard_Get(KEY_INPUT_UP) == 1){
		if(level != 4 && level != 5){
			Turn_Tetrmino();
		}else if(level == 4){//���]
			Down_Tetrmino();
		}
	}

	//Down�i���Ɉړ��j
	if(Keyboard_Get(KEY_INPUT_DOWN) == 1 || Keyboard_Get(KEY_INPUT_DOWN)%15 == 1){
		if(level != 4){
			Down_Tetrmino();
		}else if(level == 4){//���]
			Turn_Tetrmino();
		}
	}

	//Right�i�E�Ɉړ��j
	if(Keyboard_Get(KEY_INPUT_RIGHT) == 1){
		if(level != 4){
			Right_Tetrmino();
		}else if(level == 4){//���]
			Left_Tetrmino();
		}
	}

	//Left�i���Ɉړ��j
	if(Keyboard_Get(KEY_INPUT_LEFT) == 1){
		if(level != 4){
			Left_Tetrmino();
		}else if(level == 4){//���]
			Right_Tetrmino();
		}
	}

	//Shift+Up(�ʏ���ɂȂ�)
	if(Keyboard_Get(KEY_INPUT_LSHIFT) > 0 && Keyboard_Get(KEY_INPUT_UP) == 1){
		if(level == 4){//���]
			Shift_Down_Tetrmino();
		}
	}

	//Shift+Down�i�ŉ��Ɉړ��j
	if(Keyboard_Get(KEY_INPUT_LSHIFT) > 0 && Keyboard_Get(KEY_INPUT_DOWN) == 1){
		if(level != 4){
			Shift_Down_Tetrmino();
		}
	}

	//Shift+Right�i�ŉE�Ɉړ��j
	if(Keyboard_Get(KEY_INPUT_LSHIFT) > 0 && Keyboard_Get(KEY_INPUT_RIGHT) == 1){
		if(level != 4){
			Shift_Right_Tetrmino();
		}else if(level == 4){//���]
			Shift_Left_Tetrmino();
		}
	}

	//Shift+Left�i�ō��Ɉړ��j
	if(Keyboard_Get(KEY_INPUT_LSHIFT) > 0 && Keyboard_Get(KEY_INPUT_LEFT) == 1){
		if(level != 4){
			Shift_Left_Tetrmino();
		}else if(level == 4){//���]
			Shift_Right_Tetrmino();
		}
	}
	//0�������ƃQ�[�����f1�ōĊJ
	if(Keyboard_Get(KEY_INPUT_0) == 1){
		while(1){
			ProcessMessage();
			if(CheckHitKey(KEY_INPUT_1) == 1){
				break;
			}
		}
	}
	//X�������ƃz�[���h
	if(Keyboard_Get(KEY_INPUT_X) == 1){
		holdTetrmino();//�z�[���h
		Fresh_Field();//�t�B�[���h�̃e�g���~�m�폜
	}

	if(Keyboard_Get(KEY_INPUT_Z) == 1){//���C����������
		if(z_action_count>0){
			Delete_Line(19);
		}
		z_action_count--;
	}
}

//�ړ��O�̃e�g���~�m�̂S�~�S�z��̍���̍��W�����݂̍��W�ɍX�V
void Tetrmino_Old_Update(){
	old_tetrmino_x = tetrmino_x;
	old_tetrmino_y = tetrmino_y;
}








//�ړ��`�F�b�N�֌W


//�ړ��ł��邩�A��]�ł��邩�`�F�b�N����(0=�E,1=��,2=��,3=��])
BOOL checkMove(int move,int move_x,int move_y){
	int right,left,down;
	switch (move)
	{
	case 0://�E
		right = getRight();
		if(move_x + right >= 9){
			return FALSE;
		}
		for(int y=0;y<4;y++){
			for(int x=0;x<4;x++){
				if(tetrmino[x][y]>0 && return_Field(move_x+x+1,move_y+y)>10){
					return FALSE;
				}
			}
		}
		return TRUE;

	case 1://��
		left = getLeft();
		if(move_x+ left <= 0){
			return FALSE;
		}
		for(int y=0;y<4;y++){
			for(int x=0;x<4;x++){
				if(move_x+x-1 >= 0){
					if(tetrmino[x][y]>0 && return_Field(move_x+x-1,move_y+y)>10){
						return FALSE;
					}
				}
			}
		}
		return TRUE;
	
	case 2://��
		down = getDown();
		if(move_y+ down >=19){
			return FALSE;
		}
		for(int y=0;y<4;y++){
			for(int x=0;x<4;x++){
				if(move_x+x >= 0){
					if(tetrmino[x][y]>0 && return_Field(move_x+x,move_y+y+1)>10){
						return FALSE;
					}
				}
			}
		}
		return TRUE;

	case 3://��]
		for(int y=0; y<4; y++){
			for(int x=0; x<4; x++){
				turn_tetrmino[3-y][x] = tetrmino[x][y];
			}
		}
		for(int y=0;y<4;y++){
			for(int x=0;x<4;x++){
				if(turn_tetrmino[x][y]>0){
					if(return_Field(move_x+x,move_y+y+1)>10 || move_x+x<0 || move_x+x >9){
					return FALSE;
					}
				}
			}
		}
		return TRUE;
	}
	return FALSE;
}

//Shift�ړ��̃`�F�b�N(0=�E,1=��,2=��)
int checkShiftMove(int move){
	int shift_tetrmino_x = tetrmino_x;
	int shift_tetrmino_y = tetrmino_y;
	switch(move){
	case 0:
		for(int x = shift_tetrmino_x; x < shift_tetrmino_x+9; x++){
			if(checkMove(0,x,shift_tetrmino_y)){
				shift_tetrmino_x = x + 1;
			}
			if(!checkMove(0,x,shift_tetrmino_y)){
				return shift_tetrmino_x;
			}
		}
		return shift_tetrmino_x;

	case 1:
		for(int x = shift_tetrmino_x; x > shift_tetrmino_x-9; x--){
			if(checkMove(1,x,shift_tetrmino_y)){
				shift_tetrmino_x = x - 1;
			}
			if(!checkMove(1,x,shift_tetrmino_y)){
				return shift_tetrmino_x;
			}
		}
		return shift_tetrmino_x;

	case 2:
		for(int y = shift_tetrmino_y; y < shift_tetrmino_y+20; y++){
			if(checkMove(2,shift_tetrmino_x,y)){
				shift_tetrmino_y = y + 1;
			}
			if(!checkMove(2,shift_tetrmino_x,y)){
				return shift_tetrmino_y;
			}
		}
		return shift_tetrmino_y;
	}
	return 0;
}

//�Q�[���I�[�o�[���`�F�b�N����
BOOL checkGameOver(){
	for(int x=0;x<4;x++){
		for(int y=0;y<4;y++){
			if(tetrmino[x][y] != 0 && return_Field(x+3,y) > 0 ){
				return FALSE;
			}
		}
	}
	return TRUE;
}






//�z�[���h�e�g���~�m�ƌ��݂̃e�g���~�m�����ւ���B�����z�[���h�e�g���~�m������̏ꍇ�͊i�[����
void holdTetrmino(){
	if(!first_hold_flag){
		for(int x=0;x<4;x++){
			for(int y=0;y<4;y++){
				hold_tetrmino[x][y] = tetrmino[x][y];
			}
		}
		Fresh_Field();
		Tetrmino_Initialize(1);
	}
	if(first_hold_flag && hold_tetrmino_flag){
		int _hold_tetrmino[4][4];
		for(int x=0;x<4;x++){
			for(int y=0;y<4;y++){
				_hold_tetrmino[x][y] = tetrmino[x][y];
			}
		}
		for(int x=0;x<4;x++){
			for(int y=0;y<4;y++){
				tetrmino[x][y] = hold_tetrmino[x][y];
			}
		}
		for(int x=0;x<4;x++){
			for(int y=0;y<4;y++){
				hold_tetrmino[x][y] = _hold_tetrmino[x][y];
			}
		}
		tetrmino_x = 3;
		tetrmino_y = 0;
		hold_tetrmino_flag = FALSE;
		makeBom(return_Level());
	}
	first_hold_flag = TRUE;
}




//���݂̃e�g���~�m�̂S�~�S�z��̍����Y���W���{�P����
void addDown(){
	if(game_flag){
		if(!checkBom(2,tetrmino_x,tetrmino_y)){//�{�������邩�`�F�b�N
			GameOver();
		}else{
			if(checkMove(2,tetrmino_x,tetrmino_y)){//���Ɉړ��ł��邩�`�F�b�N
				tetrmino_y++;
			}else{//�ړ��ł��Ȃ��ꍇ
				Field_Update(1);//�Œ�
				Field_Check();//���C���������Ă��邩�`�F�b�N
				For_A_Moment_Draw();//��u�`��
				Tetrmino_Initialize(1);//�V�����e�g���~�m�쐬
			}
		}
	}
}




void GameOver(){
	game_flag = FALSE;
	Game_Over_Draw();

}

void GameStart(){
	game_flag = TRUE;
	z_action_count = 5;
	Field_Restart();		
	Tetrmino_Initialize(0);
}










//�{���֌W


//�{�����쐬
void makeBom(int level){
	if(level == 1 || level == 4 || level == 5){//�{�����
		srand((unsigned)time(NULL));
		bom_x = rand()%9+1;
		bom_y = 19;
		for(int y = 0; y<20; y++){
			if(return_Field(bom_x,y) > 9){
				bom_y = y-1;
				break;
			}
		}
	}
	if(level == 2){//�{�����
		srand((unsigned)time(NULL));
		bom_x = rand()%9+1;
		bom_y = 19;
		bom2_y = 19;
		for(int y = 0; y<20; y++){
			if(return_Field(bom_x,y) > 9){
				bom_y = y-1;
				break;
			}
		}
		while(1){
			bom2_x = rand()%9+1;
			if(bom2_x != bom_x){
				break;
			}
		}
		for(int y = 0; y<20; y++){
			if(return_Field(bom2_x,y) > 9){
				bom2_y = y-1;
				break;
			}
		}
	}
	if(level == 3){//�A���{���R��
		srand((unsigned)time(NULL));
		bom_x = rand()%7 + 1;
		bom2_x = bom_x + 1;
		bom3_x = bom_x + 2;
		bom_y = 19;
		bom2_y = 19;
		bom3_y = 19;
		for(int y = 0; y<20; y++){
			if(return_Field(bom_x,y) > 9){
				bom_y = y-1;
				break;
			}
		}
		for(int y = 0; y<20; y++){
			if(return_Field(bom2_x,y) > 9){
				bom2_y = y-1;
				break;
			}
		}
		for(int y = 0; y<20; y++){
			if(return_Field(bom3_x,y) > 9){
				bom3_y = y-1;
				break;
			}
		}
	}
}

//�{�������邩�`�F�b�N
BOOL checkBom(int move,int move_x,int move_y){
	int right,left,down;
	switch (move)
	{
	case 0://�E
		right = getRight();
		for(int y=0;y<4;y++){
			for(int x=0;x<4;x++){
				if(tetrmino[x][y]>0 && (return_Field(move_x+x+1,move_y+y)==8 || return_Field(move_x+x+1,move_y+y)==10)){
					return FALSE;
				}
			}
		}
		return TRUE;

	case 1://��
		left = getLeft();
		for(int y=0;y<4;y++){
			for(int x=0;x<4;x++){
				if(tetrmino[x][y]>0 && (return_Field(move_x+x-1,move_y+y)==8 || return_Field(move_x+x-1,move_y+y)==10)){
					return FALSE;
				}
			}
		}
		return TRUE;
	
	case 2://��
		down = getDown();
		for(int y=0;y<4;y++){
			for(int x=0;x<4;x++){
				if(tetrmino[x][y]>0 && (return_Field(move_x+x,move_y+y+1)==8 || return_Field(move_x+x,move_y+y+1)==10)){
					return FALSE;
				}
			}
		}
		return TRUE;
	}
	return FALSE;
}

//�{�������邩�`�F�b�N�iShift�ړ�ver)
BOOL checkShiftBom(int move){
	int shift_tetrmino_x = tetrmino_x;
	int shift_tetrmino_y = tetrmino_y;
	int max_x = checkShiftMove(0);
	int min_x = checkShiftMove(1);
	int max_y = checkShiftMove(2);
	switch(move){
	case 0:
		for(int x = shift_tetrmino_x; x < max_x; x++){
			if(!checkBom(0,x,shift_tetrmino_y)){
				return FALSE;
			}
		}
		return TRUE;

	case 1:
		for(int x = shift_tetrmino_x; x > min_x; x--){
			if(!checkBom(1,x,shift_tetrmino_y)){
				return FALSE;
			}
		}
		return TRUE;

	case 2:
		for(int y = shift_tetrmino_y; y < max_y; y++){
			if(!checkBom(2,shift_tetrmino_x,y)){
				return FALSE;
			}
		}
		return TRUE;
	}
	return 0;
}

//�{���̂����W���P������
void Down_Bom_Y(){
	if(bom_y != 19){
		bom_y++;
	}
	if(bom2_y != 19){
		bom2_y++;
	}
}




//�ړ��֐��܂Ƃ�


//�`�F�b�N���ĉ�]
void Turn_Tetrmino(){
	if(checkMove(3,tetrmino_x,tetrmino_y)){
		for(int y=0; y<4; y++){
			for(int x=0; x<4; x++){
				turn_tetrmino[3-y][x] = tetrmino[x][y];
			}
		}

		for(int x=0;x<4;x++){
			for(int y=0;y<4;y++){
				tetrmino[x][y] = turn_tetrmino[x][y];
			}
		}
	}
}
//�`�F�b�N���ĉ��Ɉړ�
void Down_Tetrmino(){
	if(!checkBom(2,tetrmino_x,tetrmino_y)){//�{�������邩�`�F�b�N
		tetrmino_y++;
		GameOver();
	}else{
		if(checkMove(2,tetrmino_x,tetrmino_y)){//���Ɉړ��ł��邩�`�F�b�N
			tetrmino_y++;
		}else{//�ړ��ł��Ȃ��ꍇ
			Field_Update(1);//�Œ�
			Field_Check();//���C���������Ă��邩�`�F�b�N
			For_A_Moment_Draw();//��u�`��
			Tetrmino_Initialize(1);//�V�����e�g���~�m�쐬
		}
	}
}
//�`�F�b�N���ĉE�Ɉړ�
void Right_Tetrmino(){
	if(!checkBom(0,tetrmino_x,tetrmino_y)){//�{�������邩�`�F�b�N
		GameOver();
	}else{
		if(checkMove(0,tetrmino_x,tetrmino_y)){//�E�Ɉړ��ł��邩�`�F�b�N
			tetrmino_x++;
		}
	}
}
//�`�F�b�N���č��Ɉړ�
void Left_Tetrmino(){
	if(!checkBom(1,tetrmino_x,tetrmino_y)){//�{�������邩�`�F�b�N
		GameOver();
	}else{
		if(checkMove(1,tetrmino_x,tetrmino_y)){//���Ɉړ��ł��邩�`�F�b�N
			tetrmino_x--;
		}
	}
}
//�`�F�b�N���ăV�t�g���Ɉړ�
void Shift_Down_Tetrmino(){
	if(!checkShiftBom(2)){//���ɃV�t�g�ړ����̃{�������邩�`�F�b�N
		tetrmino_y = checkShiftMove(2);
		GameOver();
	}else{
		tetrmino_y = checkShiftMove(2);//�V�t�g�ړ��̂����W��Ԃ�
		Field_Update(1);//�Œ�
		Field_Check();//���C���������Ă��邩�`�F�b�N
		For_A_Moment_Draw();//��u�`��
		Tetrmino_Initialize(1);//�V�����e�g���~�m�쐬
	}
}
//�`�F�b�N���ăV�t�g�E�Ɉړ�
void Shift_Right_Tetrmino(){
	if(!checkShiftBom(0)){//�E�ɃV�t�g�ړ����̃{�������邩�`�F�b�N
		GameOver();
	}else{
		tetrmino_x = checkShiftMove(0);//�V�t�g�ړ��̂����W��Ԃ�
	}
}
//�`�F�b�N���ăV�t�g���Ɉړ�
void Shift_Left_Tetrmino(){
	if(!checkShiftBom(1)){//���ɃV�t�g�ړ����̃{�������邩�`�F�b�N
		GameOver();
	}else{
		tetrmino_x = checkShiftMove(1);//�V�t�g�ړ��̂����W��Ԃ�
	}
}





//����Ԃ��֐�//



//���݂̃e�g���~�m�̂S�~�S�z��̈�ԉE���ɂ���u���b�N�̍��W��Ԃ��i�O�`�R�j
int getRight()
{
	for(int x = 3; x>=0;x--){
			for(int y=0;y<4;y++){
				if(tetrmino[x][y]>0){
					return x;
				}
			}
	}
	return 0;
}

//���݂̃e�g���~�m�̂S�~�S�z��̈�ԍ����ɂ���u���b�N�̍��W��Ԃ��i�O�`�R�j
int getLeft(){
	for(int x = 0; x<4;x++){
		for(int y=0;y<4;y++){
			if(tetrmino[x][y]>0){
				return x;
			}
		}
	}
	return 0;
}

//���݂̃e�g���~�m�̂S�~�S�z��̈�ԉ����ɂ���u���b�N�̍��W��Ԃ��i�O�`�R�j
int getDown(){
	for(int y=3;y>=0;y--){
		for(int x = 0; x<4;x++){	
			if(tetrmino[x][y]>0){
				return y;
			}
		}
	}
	return 0;
}

//�{���̂����W��Ԃ�
int bom_Get_X(){
	return bom_x;
}

//�{���̂����W��Ԃ�
int bom_Get_Y(){
	return bom_y;
}

//�{��2�̂����W��Ԃ�
int bom2_Get_X(){
	return bom2_x;
}

//�{��2�̂����W��Ԃ�
int bom2_Get_Y(){
	return bom2_y;
}

//�{��3�̂����W��Ԃ�
int bom3_Get_X(){
	return bom3_x;
}

//�{��3�̂����W��Ԃ�
int bom3_Get_Y(){
	return bom3_y;
}

//�ړ��O�̃e�g���~�m�̂S�~�S�z��̍����X���W��Ԃ�
int Old_Tetrmino_Get_X(){
	return old_tetrmino_x;
}

//�ړ��O�̃e�g���~�m�̂S�~�S�z��̍����Y���W��Ԃ�
int Old_Tetrmino_Get_Y(){
	return old_tetrmino_y;
}

//���݂̃e�g���~�m�̂S�~�S�z��̍����X���W��Ԃ�
int current_Tetrmino_Get_X(){
	return tetrmino_x;
}

//���݂̃e�g���~�m�̂S�~�S�z��̍����Y���W��Ԃ�
int current_Tetrmino_Get_Y(){
	return tetrmino_y;
}

//�Q�[���I�[�o�[���m�F����
BOOL getGameOver(){
	return game_flag;
}

//z�A�N�V�����̉񐔂�Ԃ�
int getZ_Action_Count(){
	return z_action_count;
}

//���݂̂̃e�g���~�m�̂S�~�S�z���Ԃ�
int (*current_Tetrmino_Get())[4]{
	return tetrmino;
}

//���̃e�g���~�m�̂S�~�S�z���Ԃ�
int (*next_Tetrmino_Get())[4]{
	return next_tetrmino;
}

//�z�[���h�e�g���~�m�̔z���Ԃ�
int (*hold_Tetrmino_Get())[4]{
	return hold_tetrmino;
}