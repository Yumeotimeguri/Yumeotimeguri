//新しいテトリミノを作成(ゲーム起動時flag=0,２回目からflag=1)
void Tetrmino_Initialize(int flag);

//キーの入力にされていた場合移動、回転する
void Tetrmino_Update();

//現在のテトリミノの４×４配列の左上のX座標を返す
int current_Tetrmino_Get_X();

//現在のテトリミノの４×４配列の左上のY座標を返す
int current_Tetrmino_Get_Y();

//移動前のテトリミノの４×４配列の左上のX座標を返す
int Old_Tetrmino_Get_X();

//移動前のテトリミノの４×４配列の左上のY座標を返す
int Old_Tetrmino_Get_Y();

//現在ののテトリミノの４×４配列を返す
int (*current_Tetrmino_Get())[4];

//移動前のテトリミノの４×４配列の左上の座標を現在の座標に更新
void Tetrmino_Old_Update();

//現在のテトリミノの４×４配列の一番右側にあるブロックの座標を返す（０～３）
int getRight();

//現在のテトリミノの４×４配列の一番左側にあるブロックの座標を返す（０～３）
int getLeft();

//現在のテトリミノの４×４配列の一番下側にあるブロックの座標を返す（０～３）
int getDown();

//現在のテトリミノの４×４配列の左上のY座標を＋１する
void addDown();

//移動できるか、回転できるかチェックする(０＝右、１＝左、２＝下、３＝回転）
BOOL checkMove(int move,int move_x,int move_y);

//Shift移動のチェック
int checkShiftMove(int move);

//次のテトリミノの４×４配列を返す
int (*next_Tetrmino_Get())[4];

//ホールドテトリミノの配列を返す
int (*hold_Tetrmino_Get())[4];

//ボムのｘ座標を返す
int bom_Get_X();

//ボムのｙ座標を返す
int bom_Get_Y();

//ボム2のｘ座標を返す
int bom2_Get_X();

//ボム2のｙ座標を返す
int bom2_Get_Y();

//ボム3のｘ座標を返す
int bom3_Get_X();

//ボム3のｙ座標を返す
int bom3_Get_Y();

void GameStart();

//ゲームオーバーか確認する
BOOL getGameOver();

//ボムのｙ座標を１さげる
void Down_Bom_Y();

//zアクションの回数を返す
int getZ_Action_Count();