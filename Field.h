//初期化
void Field_Initialize();

//field配列に従い画面を描写する
void Field_Draw();

//flagが０の場合現在のテトリミノをfield配列に更新する。flagが１の場合、現在のテトリミノをfield配列に固定（＋１０）する
void Field_Update(int flag);

//列揃っているかチェックする
void Field_Check();

//ラインを消す
void Delete_Line(int line);

//引数のfield配列の値を返す
int return_Field(int x,int y);

//次のテトリミノの描写
void Next_Tetrmino_Draw();

//0.3秒描写する
void For_A_Moment_Draw();

//フィールド配列から1～8のブロックを消す
void Fresh_Field();

void Field_Restart();

//ゲームオーバー処理
void Game_Over_Draw();

//レベルを１あげる
void Add_Level();

//レベルを返す
int return_Level();