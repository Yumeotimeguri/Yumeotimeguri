#include "Block.h"
#include "DxLib.h"
#include "Keyboard.h"
#include "Field.h"
#include <time.h>
#include "SceneMgr.h"

static int tetrmino_x = 0;//現在のテトリミノのｘ座標
static int tetrmino_y = 0;//現在のテトリミノのｙ座標
static int old_tetrmino_x = 0;//移動前のテトリミノのｘ座標
static int old_tetrmino_y = 0;//移動前のテトリミノのｙ座標
static int (*p_tetrmino)[4];//テトリミノ配列を受け取るポインタ
static int tetrmino[4][4] = {0};//テトリミノ配列
static int turn_tetrmino[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};//回転時に使用するテトリミノ配列
static int next_tetrmino[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};//次に出現するテトリミノ配列
static int hold_tetrmino[4][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};//一時保存できるテトリミノ配列
static bool hold_tetrmino_flag = TRUE;//連続でホールドさせないためのフラグ
static bool first_hold_flag = FALSE;//ホールドテトリミノが空か否か
static int bom_x = 0;//ボムのx座標
static int bom_y = 0;//ボムのy座標
static int bom2_x = 0;//ボム2のx座標
static int bom2_y = 0;//ボム2のy座標
static int bom3_x = 0;//ボム3のx座標
static int bom3_y = 0;//ボム3のy座標
static bool game_flag = TRUE;//ゲームオーバーか否か
static int z_action_count = 5;//ｚボタンアクションの使用回数（ラインを消せる数）


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


//新しいテトリミノを作成(ゲーム起動時flag=0,２回目からflag=1)
void Tetrmino_Initialize(int flag){
	if(game_flag){//ゲームオーバーしている場合新しいのを作らない
		tetrmino_x = 3;
		tetrmino_y = 0;
		old_tetrmino_x = 0;
		old_tetrmino_y = 0;
		hold_tetrmino_flag = TRUE;
	
		makeBom(return_Level());//ボム作成
	
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




//更新


//キーが入力にされていた場合移動、回転する
void Tetrmino_Update(){
	int level = return_Level();
	//Up（回転）
	if(Keyboard_Get(KEY_INPUT_UP) == 1){
		if(level != 4 && level != 5){
			Turn_Tetrmino();
		}else if(level == 4){//反転
			Down_Tetrmino();
		}
	}

	//Down（下に移動）
	if(Keyboard_Get(KEY_INPUT_DOWN) == 1 || Keyboard_Get(KEY_INPUT_DOWN)%15 == 1){
		if(level != 4){
			Down_Tetrmino();
		}else if(level == 4){//反転
			Turn_Tetrmino();
		}
	}

	//Right（右に移動）
	if(Keyboard_Get(KEY_INPUT_RIGHT) == 1){
		if(level != 4){
			Right_Tetrmino();
		}else if(level == 4){//反転
			Left_Tetrmino();
		}
	}

	//Left（左に移動）
	if(Keyboard_Get(KEY_INPUT_LEFT) == 1){
		if(level != 4){
			Left_Tetrmino();
		}else if(level == 4){//反転
			Right_Tetrmino();
		}
	}

	//Shift+Up(通常特になし)
	if(Keyboard_Get(KEY_INPUT_LSHIFT) > 0 && Keyboard_Get(KEY_INPUT_UP) == 1){
		if(level == 4){//反転
			Shift_Down_Tetrmino();
		}
	}

	//Shift+Down（最下に移動）
	if(Keyboard_Get(KEY_INPUT_LSHIFT) > 0 && Keyboard_Get(KEY_INPUT_DOWN) == 1){
		if(level != 4){
			Shift_Down_Tetrmino();
		}
	}

	//Shift+Right（最右に移動）
	if(Keyboard_Get(KEY_INPUT_LSHIFT) > 0 && Keyboard_Get(KEY_INPUT_RIGHT) == 1){
		if(level != 4){
			Shift_Right_Tetrmino();
		}else if(level == 4){//反転
			Shift_Left_Tetrmino();
		}
	}

	//Shift+Left（最左に移動）
	if(Keyboard_Get(KEY_INPUT_LSHIFT) > 0 && Keyboard_Get(KEY_INPUT_LEFT) == 1){
		if(level != 4){
			Shift_Left_Tetrmino();
		}else if(level == 4){//反転
			Shift_Right_Tetrmino();
		}
	}
	//0を押すとゲーム中断1で再開
	if(Keyboard_Get(KEY_INPUT_0) == 1){
		while(1){
			ProcessMessage();
			if(CheckHitKey(KEY_INPUT_1) == 1){
				break;
			}
		}
	}
	//Xを押すとホールド
	if(Keyboard_Get(KEY_INPUT_X) == 1){
		holdTetrmino();//ホールド
		Fresh_Field();//フィールドのテトリミノ削除
	}

	if(Keyboard_Get(KEY_INPUT_Z) == 1){//ラインを消せる
		if(z_action_count>0){
			Delete_Line(19);
		}
		z_action_count--;
	}
}

//移動前のテトリミノの４×４配列の左上の座標を現在の座標に更新
void Tetrmino_Old_Update(){
	old_tetrmino_x = tetrmino_x;
	old_tetrmino_y = tetrmino_y;
}








//移動チェック関係


//移動できるか、回転できるかチェックする(0=右,1=左,2=下,3=回転)
BOOL checkMove(int move,int move_x,int move_y){
	int right,left,down;
	switch (move)
	{
	case 0://右
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

	case 1://左
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
	
	case 2://下
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

	case 3://回転
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

//Shift移動のチェック(0=右,1=左,2=下)
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

//ゲームオーバーかチェックする
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






//ホールドテトリミノと現在のテトリミノを入れ替える。もしホールドテトリミノがからの場合は格納する
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




//現在のテトリミノの４×４配列の左上のY座標を＋１する
void addDown(){
	if(game_flag){
		if(!checkBom(2,tetrmino_x,tetrmino_y)){//ボムがあるかチェック
			GameOver();
		}else{
			if(checkMove(2,tetrmino_x,tetrmino_y)){//下に移動できるかチェック
				tetrmino_y++;
			}else{//移動できない場合
				Field_Update(1);//固定
				Field_Check();//ラインが揃っているかチェック
				For_A_Moment_Draw();//一瞬描写
				Tetrmino_Initialize(1);//新しいテトリミノ作成
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










//ボム関係


//ボムを作成
void makeBom(int level){
	if(level == 1 || level == 4 || level == 5){//ボム一つ
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
	if(level == 2){//ボム二つ
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
	if(level == 3){//連続ボム３つ
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

//ボムがあるかチェック
BOOL checkBom(int move,int move_x,int move_y){
	int right,left,down;
	switch (move)
	{
	case 0://右
		right = getRight();
		for(int y=0;y<4;y++){
			for(int x=0;x<4;x++){
				if(tetrmino[x][y]>0 && (return_Field(move_x+x+1,move_y+y)==8 || return_Field(move_x+x+1,move_y+y)==10)){
					return FALSE;
				}
			}
		}
		return TRUE;

	case 1://左
		left = getLeft();
		for(int y=0;y<4;y++){
			for(int x=0;x<4;x++){
				if(tetrmino[x][y]>0 && (return_Field(move_x+x-1,move_y+y)==8 || return_Field(move_x+x-1,move_y+y)==10)){
					return FALSE;
				}
			}
		}
		return TRUE;
	
	case 2://下
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

//ボムがあるかチェック（Shift移動ver)
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

//ボムのｙ座標を１さげる
void Down_Bom_Y(){
	if(bom_y != 19){
		bom_y++;
	}
	if(bom2_y != 19){
		bom2_y++;
	}
}




//移動関数まとめ


//チェックして回転
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
//チェックして下に移動
void Down_Tetrmino(){
	if(!checkBom(2,tetrmino_x,tetrmino_y)){//ボムがあるかチェック
		tetrmino_y++;
		GameOver();
	}else{
		if(checkMove(2,tetrmino_x,tetrmino_y)){//下に移動できるかチェック
			tetrmino_y++;
		}else{//移動できない場合
			Field_Update(1);//固定
			Field_Check();//ラインが揃っているかチェック
			For_A_Moment_Draw();//一瞬描写
			Tetrmino_Initialize(1);//新しいテトリミノ作成
		}
	}
}
//チェックして右に移動
void Right_Tetrmino(){
	if(!checkBom(0,tetrmino_x,tetrmino_y)){//ボムがあるかチェック
		GameOver();
	}else{
		if(checkMove(0,tetrmino_x,tetrmino_y)){//右に移動できるかチェック
			tetrmino_x++;
		}
	}
}
//チェックして左に移動
void Left_Tetrmino(){
	if(!checkBom(1,tetrmino_x,tetrmino_y)){//ボムがあるかチェック
		GameOver();
	}else{
		if(checkMove(1,tetrmino_x,tetrmino_y)){//左に移動できるかチェック
			tetrmino_x--;
		}
	}
}
//チェックしてシフト下に移動
void Shift_Down_Tetrmino(){
	if(!checkShiftBom(2)){//下にシフト移動時のボムがあるかチェック
		tetrmino_y = checkShiftMove(2);
		GameOver();
	}else{
		tetrmino_y = checkShiftMove(2);//シフト移動のｙ座標を返す
		Field_Update(1);//固定
		Field_Check();//ラインが揃っているかチェック
		For_A_Moment_Draw();//一瞬描写
		Tetrmino_Initialize(1);//新しいテトリミノ作成
	}
}
//チェックしてシフト右に移動
void Shift_Right_Tetrmino(){
	if(!checkShiftBom(0)){//右にシフト移動時のボムがあるかチェック
		GameOver();
	}else{
		tetrmino_x = checkShiftMove(0);//シフト移動のｘ座標を返す
	}
}
//チェックしてシフト左に移動
void Shift_Left_Tetrmino(){
	if(!checkShiftBom(1)){//左にシフト移動時のボムがあるかチェック
		GameOver();
	}else{
		tetrmino_x = checkShiftMove(1);//シフト移動のｘ座標を返す
	}
}





//情報を返す関数//



//現在のテトリミノの４×４配列の一番右側にあるブロックの座標を返す（０～３）
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

//現在のテトリミノの４×４配列の一番左側にあるブロックの座標を返す（０～３）
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

//現在のテトリミノの４×４配列の一番下側にあるブロックの座標を返す（０～３）
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

//ボムのｘ座標を返す
int bom_Get_X(){
	return bom_x;
}

//ボムのｙ座標を返す
int bom_Get_Y(){
	return bom_y;
}

//ボム2のｘ座標を返す
int bom2_Get_X(){
	return bom2_x;
}

//ボム2のｙ座標を返す
int bom2_Get_Y(){
	return bom2_y;
}

//ボム3のｘ座標を返す
int bom3_Get_X(){
	return bom3_x;
}

//ボム3のｙ座標を返す
int bom3_Get_Y(){
	return bom3_y;
}

//移動前のテトリミノの４×４配列の左上のX座標を返す
int Old_Tetrmino_Get_X(){
	return old_tetrmino_x;
}

//移動前のテトリミノの４×４配列の左上のY座標を返す
int Old_Tetrmino_Get_Y(){
	return old_tetrmino_y;
}

//現在のテトリミノの４×４配列の左上のX座標を返す
int current_Tetrmino_Get_X(){
	return tetrmino_x;
}

//現在のテトリミノの４×４配列の左上のY座標を返す
int current_Tetrmino_Get_Y(){
	return tetrmino_y;
}

//ゲームオーバーか確認する
BOOL getGameOver(){
	return game_flag;
}

//zアクションの回数を返す
int getZ_Action_Count(){
	return z_action_count;
}

//現在ののテトリミノの４×４配列を返す
int (*current_Tetrmino_Get())[4]{
	return tetrmino;
}

//次のテトリミノの４×４配列を返す
int (*next_Tetrmino_Get())[4]{
	return next_tetrmino;
}

//ホールドテトリミノの配列を返す
int (*hold_Tetrmino_Get())[4]{
	return hold_tetrmino;
}