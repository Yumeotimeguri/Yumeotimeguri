//������
void Field_Initialize();

//field�z��ɏ]����ʂ�`�ʂ���
void Field_Draw();

//flag���O�̏ꍇ���݂̃e�g���~�m��field�z��ɍX�V����Bflag���P�̏ꍇ�A���݂̃e�g���~�m��field�z��ɌŒ�i�{�P�O�j����
void Field_Update(int flag);

//�񑵂��Ă��邩�`�F�b�N����
void Field_Check();

//���C��������
void Delete_Line(int line);

//������field�z��̒l��Ԃ�
int return_Field(int x,int y);

//���̃e�g���~�m�̕`��
void Next_Tetrmino_Draw();

//0.3�b�`�ʂ���
void For_A_Moment_Draw();

//�t�B�[���h�z�񂩂�1�`8�̃u���b�N������
void Fresh_Field();

void Field_Restart();

//�Q�[���I�[�o�[����
void Game_Over_Draw();

//���x�����P������
void Add_Level();

//���x����Ԃ�
int return_Level();