//�V�����e�g���~�m���쐬(�Q�[���N����flag=0,�Q��ڂ���flag=1)
void Tetrmino_Initialize(int flag);

//�L�[�̓��͂ɂ���Ă����ꍇ�ړ��A��]����
void Tetrmino_Update();

//���݂̃e�g���~�m�̂S�~�S�z��̍����X���W��Ԃ�
int current_Tetrmino_Get_X();

//���݂̃e�g���~�m�̂S�~�S�z��̍����Y���W��Ԃ�
int current_Tetrmino_Get_Y();

//�ړ��O�̃e�g���~�m�̂S�~�S�z��̍����X���W��Ԃ�
int Old_Tetrmino_Get_X();

//�ړ��O�̃e�g���~�m�̂S�~�S�z��̍����Y���W��Ԃ�
int Old_Tetrmino_Get_Y();

//���݂̂̃e�g���~�m�̂S�~�S�z���Ԃ�
int (*current_Tetrmino_Get())[4];

//�ړ��O�̃e�g���~�m�̂S�~�S�z��̍���̍��W�����݂̍��W�ɍX�V
void Tetrmino_Old_Update();

//���݂̃e�g���~�m�̂S�~�S�z��̈�ԉE���ɂ���u���b�N�̍��W��Ԃ��i�O�`�R�j
int getRight();

//���݂̃e�g���~�m�̂S�~�S�z��̈�ԍ����ɂ���u���b�N�̍��W��Ԃ��i�O�`�R�j
int getLeft();

//���݂̃e�g���~�m�̂S�~�S�z��̈�ԉ����ɂ���u���b�N�̍��W��Ԃ��i�O�`�R�j
int getDown();

//���݂̃e�g���~�m�̂S�~�S�z��̍����Y���W���{�P����
void addDown();

//�ړ��ł��邩�A��]�ł��邩�`�F�b�N����(�O���E�A�P�����A�Q�����A�R����]�j
BOOL checkMove(int move,int move_x,int move_y);

//Shift�ړ��̃`�F�b�N
int checkShiftMove(int move);

//���̃e�g���~�m�̂S�~�S�z���Ԃ�
int (*next_Tetrmino_Get())[4];

//�z�[���h�e�g���~�m�̔z���Ԃ�
int (*hold_Tetrmino_Get())[4];

//�{���̂����W��Ԃ�
int bom_Get_X();

//�{���̂����W��Ԃ�
int bom_Get_Y();

//�{��2�̂����W��Ԃ�
int bom2_Get_X();

//�{��2�̂����W��Ԃ�
int bom2_Get_Y();

//�{��3�̂����W��Ԃ�
int bom3_Get_X();

//�{��3�̂����W��Ԃ�
int bom3_Get_Y();

void GameStart();

//�Q�[���I�[�o�[���m�F����
BOOL getGameOver();

//�{���̂����W���P������
void Down_Bom_Y();

//z�A�N�V�����̉񐔂�Ԃ�
int getZ_Action_Count();