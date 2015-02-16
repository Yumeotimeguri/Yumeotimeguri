#include "DxLib.h"
#include "Block.h"
#include "Tetrmino.h"
#include "Game.h"
#include "SceneMgr.h"
#include <math.h>

static int FIELD_X = 10;
static int FIELD_Y = 20;
static int mImageHandle;
static int field[10][20]={//フィールド配列dao
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
static int Block[100] = {0};//ブロックの入った配列。要素番号で色などを指定する
static int next_field_tetrmino[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};//次のテトリミノ
static int hold_field_tetrmino[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};//ホールドテトリミノ
static int Level = 1;//現在のレベル
static int delete_line_count = 0;//削除したラインの数


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

//初期化
void Field_Initialize(){
	LoadDivGraph("画像/Tetris.bmp",11,11,1,20,20,Block);
	mImageHandle = LoadGraph("画像/haikei.png");
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

//現在のテトリミノ、ゴーストテトリミノをフィールドに反映（flag=1は固定,flag=0は反映）
void Field_Update(int flag){

	int (*p_field_tetrmino)[4];
	int field_tetrmino[4][4];
	//現在のテトリミノ配列取得
	p_field_tetrmino = current_Tetrmino_Get();
	for(int x=0;x<4;x++){
		for(int y=0;y<4;y++){
			field_tetrmino[x][y] = p_field_tetrmino[x][y];
		}
	}
	//次のテトリミノ配列取得
	p_field_tetrmino = next_Tetrmino_Get();
	for(int x=0;x<4;x++){
		for(int y=0;y<4;y++){
			next_field_tetrmino[x][y] = p_field_tetrmino[x][y];
		}
	}
	//ホールドテトリミノ配列取得
	p_field_tetrmino = hold_Tetrmino_Get();
	for(int x=0;x<4;x++){
		for(int y=0;y<4;y++){
			hold_field_tetrmino[x][y] = p_field_tetrmino[x][y];
		}
	}
	





	if(flag == 1){//flagが１の時現在のテトリミノを固定する（＋１０して格納）
		for(int x = 0; x<4; x++){
			for(int y = 0; y<4; y++){
				if(current_Tetrmino_Get_X()+x < 10 && current_Tetrmino_Get_Y()+y <20 && field_tetrmino[x][y] != 0){
					field[current_Tetrmino_Get_X()+x][current_Tetrmino_Get_Y()+y] = field_tetrmino[x][y]+10;
				}
			}
		}
	}
	
	Delete_Old_Tetrmino();//移動前のテトリミノを削除
	
	if(flag != 1){//固定時は実行しない
		for(int x =0;x<10;x++){//移動前のゴーストテトリミノとボムを消す
			for(int y = 0;y<20;y++){
				if(field[x][y] == 9 || field[x][y] == 8){
					field[x][y] = 0;
				}
			}
		}
		for(int x = 0; x<4; x++){//ゴーストテトリミノを更新
			for(int y = 0; y<4; y++){
				if(current_Tetrmino_Get_X()+x >=0){
					if(current_Tetrmino_Get_X()+x < 10 && current_Tetrmino_Get_Y()+y <20 && field[current_Tetrmino_Get_X()+x][checkShiftMove(2)+y]<11 && field_tetrmino[x][y] !=0){
						field[current_Tetrmino_Get_X()+x][checkShiftMove(2)+y] = 9;
					}
				}
			}
		}
		if(getGameOver()){
			Bom_Update();//ボムの更新
		}
	}
	
	


	for(int x = 0; x<4; x++){//移動後のテトリミノを更新
		for(int y = 0; y<4; y++){
			if(current_Tetrmino_Get_X()+x >=0){
				if(current_Tetrmino_Get_X()+x < 10 && current_Tetrmino_Get_Y()+y <20 && field[current_Tetrmino_Get_X()+x][current_Tetrmino_Get_Y()+y]<10 && field_tetrmino[x][y] !=0){
					field[current_Tetrmino_Get_X()+x][current_Tetrmino_Get_Y()+y] = field_tetrmino[x][y];
				}
			}
		}
	}
}

//ボムの位置情報を更新
void Bom_Update(){
	
	if(Level == 1 || Level == 4 || Level == 5){//レベル１＝ボム１つ
		if(field[bom_Get_X()][bom_Get_Y()] == 9){
			field[bom_Get_X()][bom_Get_Y()] = 10;
		}else{
			field[bom_Get_X()][bom_Get_Y()] = 8;
		}
	}

	if(Level == 2){//レベル２＝ボム２つ
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

//移動前のテトリミノを消す
void Delete_Old_Tetrmino(){
	for(int x = 0; x<4; x++){
		for(int y = 0; y<4; y++){
			if(Old_Tetrmino_Get_X()+x < 10 && Old_Tetrmino_Get_Y()+y <20 && field[Old_Tetrmino_Get_X()+x][Old_Tetrmino_Get_Y()+y] <10){
				field[Old_Tetrmino_Get_X()+x][Old_Tetrmino_Get_Y()+y] = 0;
			}
		}
	}
}

//フィールドにラインが揃ってるかチェックをし、揃っていた場合Delete_Line()を実行しラインを消す
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

//ラインを消す
void Delete_Line(int line){
	//消す前のラインを描写
	For_A_Moment_Draw();

	//消したいラインに０を入れる
	for(int x=0; x<10;x++){
		if(field[x][line] != 8 && field[x][line] != 9 && field[x][line]){
			field[x][line] = 0;
		}
	}

	//空のラインを描写
	For_A_Moment_Draw();

	//スライドさせる
	Slide_Line(line);

	
}

//スライドさせる
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

//フィールド配列から1～8のブロックを消す
void Fresh_Field(){
	for(int x =0;x<10;x++){
		for(int y = 0;y<20;y++){
			if(field[x][y] >0 && field[x][y] <11){
				field[x][y] = 0;
			}
		}
	}
}




//レベルを１あげる
void Add_Level(){
	Level++;
	if(Level == 7){
		GameClear();
	}
}

//削除したラインの数を１増やす(一定数達した場合レベルをあげる
void Add_Dlete_Line_Count(){
	delete_line_count++;
	if(delete_line_count%5 == 0){
		Add_Level();
	}
}


void GameClear(){
	
	SceneMgr_ChangeScene(eScene_GameClear);
	
}


//描写関数//



//フィールド描写
void Field_Draw(){
	//背景
	for(int x = 0;x<6;x++){
		for(int y=0;y<4;y++){
			DrawGraph(x*127,y*127,mImageHandle,TRUE);
		}
	}

	//フィールド描写
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
			DrawFormatString( 340 + x*20, 80 + y*20, GetColor(0,255,0), "%d", field[x][y]); // 文字を描画する
		}
	}
	*/
	//DrawFormatString(500,50,GetColor(0,255,0),"[%d,%d,%d,%d,%d]",current_Tetrmino_Get_X(),current_Tetrmino_Get_Y(),Level,delete_line_count,getZ_Action_Count());
	
}

//次のテトリミノの描写
void Next_Tetrmino_Draw(){
	for(int x=0; x<4;x++){//次のテトリミノ
		for(int y=0;y<4;y++){
			DrawGraph(x*20+340,y*20+60,Block[next_field_tetrmino[x][y]],true);
			//DrawFormatString(x*20+340,y*20+40,GetColor(0,250,0),"%d",next_field_tetrmino[x][y]);
		}
	}
}

//ホールドテトリミノの描写
void Hold_Tetrmino_Draw(){
	for(int x=0; x<4;x++){//ホールドテトリミノ
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


//0.3秒描写する
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

//ゲームオーバー処理
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





//情報を返す関数//


//フィールド配列を返す
int return_Field(int x,int y){
	return field[x][y];
}

//レベルを返す
int return_Level(){
	return Level;
}
