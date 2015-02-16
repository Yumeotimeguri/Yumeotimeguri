#include "DxLib.h"
#include "Block.h"
#include "Tetrmino.h"
#include "Game.h"
#include "SceneMgr.h"
#include <math.h>

static int FIELD_X = 10;
static int FIELD_Y = 20;
static int mImageHandle;
static int field[10][20]={//�t�B�[���h�z��dao
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	};
static int Block[100] = {0};//�u���b�N�̓������z��B�v�f�ԍ��ŐF�Ȃǂ��w�肷��
static int next_field_tetrmino[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};//���̃e�g���~�m
static int hold_field_tetrmino[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};//�z�[���h�e�g���~�m
static int Level = 1;//���݂̃��x��
static int delete_line_count = 0;//�폜�������C���̐�


void Field_Draw();
void Field_Check();
void Delete_Line(int line);
void Slide_Line(int line);
void Next_Tetrmino_Draw();
void Hold_Tetrmino_Draw();
void Info_Draw();
void Game_Over_Draw();
void For_A_Moment_Draw();
void Add_Dlete_Line_Count();
void Add_Level();
void Bom_Update();
void Delete_Old_Tetrmino();
void GameClear();

//������
void Field_Initialize(){
	LoadDivGraph("�摜/Tetris.bmp",11,11,1,20,20,Block);
	mImageHandle = LoadGraph("�摜/haikei.png");
	Block[11] = Block[1];
	Block[12] = Block[2];
	Block[13] = Block[3];
	Block[14] = Block[4];
	Block[15] = Block[5];
	Block[16] = Block[6];
	Block[17] = Block[7];
	Block[99] = Block[4];
	Level = 1;
	delete_line_count = 0;
}

//���݂̃e�g���~�m�A�S�[�X�g�e�g���~�m���t�B�[���h�ɔ��f�iflag=1�͌Œ�,flag=0�͔��f�j
void Field_Update(int flag){

	int (*p_field_tetrmino)[4];
	int field_tetrmino[4][4];
	//���݂̃e�g���~�m�z��擾
	p_field_tetrmino = current_Tetrmino_Get();
	for(int x=0;x<4;x++){
		for(int y=0;y<4;y++){
			field_tetrmino[x][y] = p_field_tetrmino[x][y];
		}
	}
	//���̃e�g���~�m�z��擾
	p_field_tetrmino = next_Tetrmino_Get();
	for(int x=0;x<4;x++){
		for(int y=0;y<4;y++){
			next_field_tetrmino[x][y] = p_field_tetrmino[x][y];
		}
	}
	//�z�[���h�e�g���~�m�z��擾
	p_field_tetrmino = hold_Tetrmino_Get();
	for(int x=0;x<4;x++){
		for(int y=0;y<4;y++){
			hold_field_tetrmino[x][y] = p_field_tetrmino[x][y];
		}
	}
	





	if(flag == 1){//flag���P�̎����݂̃e�g���~�m���Œ肷��i�{�P�O���Ċi�[�j
		for(int x = 0; x<4; x++){
			for(int y = 0; y<4; y++){
				if(current_Tetrmino_Get_X()+x < 10 && current_Tetrmino_Get_Y()+y <20 && field_tetrmino[x][y] != 0){
					field[current_Tetrmino_Get_X()+x][current_Tetrmino_Get_Y()+y] = field_tetrmino[x][y]+10;
				}
			}
		}
	}
	
	Delete_Old_Tetrmino();//�ړ��O�̃e�g���~�m���폜
	
	if(flag != 1){//�Œ莞�͎��s���Ȃ�
		for(int x =0;x<10;x++){//�ړ��O�̃S�[�X�g�e�g���~�m�ƃ{��������
			for(int y = 0;y<20;y++){
				if(field[x][y] == 9 || field[x][y] == 8){
					field[x][y] = 0;
				}
			}
		}
		for(int x = 0; x<4; x++){//�S�[�X�g�e�g���~�m���X�V
			for(int y = 0; y<4; y++){
				if(current_Tetrmino_Get_X()+x >=0){
					if(current_Tetrmino_Get_X()+x < 10 && current_Tetrmino_Get_Y()+y <20 && field[current_Tetrmino_Get_X()+x][checkShiftMove(2)+y]<11 && field_tetrmino[x][y] !=0){
						field[current_Tetrmino_Get_X()+x][checkShiftMove(2)+y] = 9;
					}
				}
			}
		}
		if(getGameOver()){
			Bom_Update();//�{���̍X�V
		}
	}
	
	


	for(int x = 0; x<4; x++){//�ړ���̃e�g���~�m���X�V
		for(int y = 0; y<4; y++){
			if(current_Tetrmino_Get_X()+x >=0){
				if(current_Tetrmino_Get_X()+x < 10 && current_Tetrmino_Get_Y()+y <20 && field[current_Tetrmino_Get_X()+x][current_Tetrmino_Get_Y()+y]<10 && field_tetrmino[x][y] !=0){
					field[current_Tetrmino_Get_X()+x][current_Tetrmino_Get_Y()+y] = field_tetrmino[x][y];
				}
			}
		}
	}
}

//�{���̈ʒu�����X�V
void Bom_Update(){
	
	if(Level == 1 || Level == 4 || Level == 5){//���x���P���{���P��
		if(field[bom_Get_X()][bom_Get_Y()] == 9){
			field[bom_Get_X()][bom_Get_Y()] = 10;
		}else{
			field[bom_Get_X()][bom_Get_Y()] = 8;
		}
	}

	if(Level == 2){//���x���Q���{���Q��
		if(field[bom_Get_X()][bom_Get_Y()] == 9){
			field[bom_Get_X()][bom_Get_Y()] = 10;
		}else{
			field[bom_Get_X()][bom_Get_Y()] = 8;
		}
		if(field[bom2_Get_X()][bom2_Get_Y()] == 9){
			field[bom2_Get_X()][bom2_Get_Y()] = 10;
		}else{
			field[bom2_Get_X()][bom2_Get_Y()] = 8;
		}
	}
	if(Level == 3){
		if(field[bom_Get_X()][bom_Get_Y()] == 9){
			field[bom_Get_X()][bom_Get_Y()] = 10;
		}else{
			field[bom_Get_X()][bom_Get_Y()] = 8;
		}
		if(field[bom2_Get_X()][bom2_Get_Y()] == 9){
			field[bom2_Get_X()][bom2_Get_Y()] = 10;
		}else{
			field[bom2_Get_X()][bom2_Get_Y()] = 8;
		}
		if(field[bom3_Get_X()][bom3_Get_Y()] == 9){
			field[bom3_Get_X()][bom3_Get_Y()] = 10;
		}else{
			field[bom3_Get_X()][bom3_Get_Y()] = 8;
		}
	}
}

//�ړ��O�̃e�g���~�m������
void Delete_Old_Tetrmino(){
	for(int x = 0; x<4; x++){
		for(int y = 0; y<4; y++){
			if(Old_Tetrmino_Get_X()+x < 10 && Old_Tetrmino_Get_Y()+y <20 && field[Old_Tetrmino_Get_X()+x][Old_Tetrmino_Get_Y()+y] <10){
				field[Old_Tetrmino_Get_X()+x][Old_Tetrmino_Get_Y()+y] = 0;
			}
		}
	}
}

//�t�B�[���h�Ƀ��C���������Ă邩�`�F�b�N�����A�����Ă����ꍇDelete_Line()�����s�����C��������
void Field_Check(){
	int field_check_counter = 0;
	bool field_check_flag;
	for(int y=19; y>=0; y--){
		field_check_counter = 0;
		field_check_flag = TRUE;
		
		for(int x=0; x<10; x++){
			field_check_counter += field[x][y];
			if(field[x][y] == 0 || field[x][y] == 8){
				field_check_flag = FALSE;
			}
		}
		if(field_check_counter == 0){
			break;
		}
		if(field_check_flag){
			Delete_Line(y);
			y++;
			Add_Dlete_Line_Count();
		}
	}
}

//���C��������
void Delete_Line(int line){
	//�����O�̃��C����`��
	For_A_Moment_Draw();

	//�����������C���ɂO������
	for(int x=0; x<10;x++){
		if(field[x][line] != 8 && field[x][line] != 9 && field[x][line]){
			field[x][line] = 0;
		}
	}

	//��̃��C����`��
	For_A_Moment_Draw();

	//�X���C�h������
	Slide_Line(line);

	
}

//�X���C�h������
void Slide_Line(int line){
	for(int y=line; y>0; y--){
		for(int x=0; x<10; x++){
			field[x][y] = field[x][y-1];
		}
	}
	Down_Bom_Y();
}


void Field_Restart(){
	for(int x =0;x<10;x++){
		for(int y = 0;y<20;y++){
			field[x][y] = 0;
		}
	}
}

//�t�B�[���h�z�񂩂�1�`8�̃u���b�N������
void Fresh_Field(){
	for(int x =0;x<10;x++){
		for(int y = 0;y<20;y++){
			if(field[x][y] >0 && field[x][y] <11){
				field[x][y] = 0;
			}
		}
	}
}




//���x�����P������
void Add_Level(){
	Level++;
	if(Level == 7){
		GameClear();
	}
}

//�폜�������C���̐����P���₷(��萔�B�����ꍇ���x����������
void Add_Dlete_Line_Count(){
	delete_line_count++;
	if(delete_line_count%5 == 0){
		Add_Level();
	}
}


void GameClear(){
	
	SceneMgr_ChangeScene(eScene_GameClear);
	
}


//�`�ʊ֐�//



//�t�B�[���h�`��
void Field_Draw(){
	//�w�i
	for(int x = 0;x<6;x++){
		for(int y=0;y<4;y++){
			DrawGraph(x*127,y*127,mImageHandle,TRUE);
		}
	}

	//�t�B�[���h�`��
	if(Level == 6){
		for(int x=0;x<10;x++){
			for(int y=19;y>=0;y--){
				DrawGraph(x*20+120,y*20+60,Block[field[abs(x-9)][abs(y-19)]],FALSE);
			}
		}
	}
	if(Level != 6){
		for(int x=0;x<10;x++){
			for(int y=19;y>=0;y--){
				DrawGraph(x*20+120,y*20+60,Block[field[x][y]],FALSE);
			}
		}
	}

	Next_Tetrmino_Draw();
	Hold_Tetrmino_Draw();
	Info_Draw();
	/*
	for(int x =0;x<10;x++){
		for(int y = 0;y<20;y++){
			DrawFormatString( 340 + x*20, 80 + y*20, GetColor(0,255,0), "%d", field[x][y]); // ������`�悷��
		}
	}
	*/
	//DrawFormatString(500,50,GetColor(0,255,0),"[%d,%d,%d,%d,%d]",current_Tetrmino_Get_X(),current_Tetrmino_Get_Y(),Level,delete_line_count,getZ_Action_Count());
	
}

//���̃e�g���~�m�̕`��
void Next_Tetrmino_Draw(){
	for(int x=0; x<4;x++){//���̃e�g���~�m
		for(int y=0;y<4;y++){
			DrawGraph(x*20+340,y*20+60,Block[next_field_tetrmino[x][y]],true);
			//DrawFormatString(x*20+340,y*20+40,GetColor(0,250,0),"%d",next_field_tetrmino[x][y]);
		}
	}
}

//�z�[���h�e�g���~�m�̕`��
void Hold_Tetrmino_Draw(){
	for(int x=0; x<4;x++){//�z�[���h�e�g���~�m
		for(int y=0;y<4;y++){
			DrawGraph(x*20+20,y*20+60,Block[hold_field_tetrmino[x][y]],true);
		}
	}
}

void Info_Draw(){
	DrawFormatString(460,40,GetColor(0,0,0),"Level           %d",Level);
	DrawFormatString(460,70,GetColor(0,0,0),"Delete Line     %d",delete_line_count);
	DrawFormatString(460,100,GetColor(0,0,0),"Z Action Point  %d",getZ_Action_Count());
	DrawFormatString(20,40,GetColor(0,0,0),"Hold");
	DrawFormatString(340,40,GetColor(0,0,0),"Next");
	
}


//0.3�b�`�ʂ���
void For_A_Moment_Draw(){
	ClearDrawScreen();
	Field_Draw();
	ScreenFlip();
	LONGLONG StartTime = GetNowHiPerformanceCount();
	while( GetNowHiPerformanceCount() - StartTime < 300000 )
	{
		if( ProcessMessage() == -1 )
		{
			break ;  
		}
	}
}

//�Q�[���I�[�o�[����
void Game_Over_Draw(){
	Delete_Old_Tetrmino();
	for(int x =0;x<10;x++){
		for(int y = 0;y<20;y++){
			if(field[x][y] != 0){
				field[x][y] = 99;
			}
		}
	}

	ClearDrawScreen();
	Field_Draw();
	ScreenFlip();
	LONGLONG StartTime = GetNowHiPerformanceCount();
	while( GetNowHiPerformanceCount() - StartTime < 800000 )
	{
		if( ProcessMessage() == -1 )
		{
			break ;  
		}
	}


	Game_Over();
}





//����Ԃ��֐�//


//�t�B�[���h�z���Ԃ�
int return_Field(int x,int y){
	return field[x][y];
}

//���x����Ԃ�
int return_Level(){
	return Level;
}
