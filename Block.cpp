#include "DxLib.h"

int (*createTetrmino(int i))[4]{

	int p[4][4];

	int p1[4][4] = {
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0},
			{0,1,0,0}
		};

	int p2[4][4] = {
			{0,0,0,0},
			{0,2,2,0},
			{0,2,2,0},
			{0,0,0,0}
		};

	int p3[4][4] = {
			{0,3,3,0},
			{0,0,3,0},
			{0,0,3,0},
			{0,0,0,0}
		};

	int p4[4][4] = {
			{0,4,0,0},
			{0,4,4,0},
			{0,0,4,0},
			{0,0,0,0}
		};

	int p5[4][4] = {
			{0,0,5,0},
			{0,0,5,0},
			{0,5,5,0},
			{0,0,0,0}
		};

	int p6[4][4] = {
			{0,0,6,0},
			{0,6,6,0},
			{0,6,0,0},
			{0,0,0,0},
		};

	int p7[4][4] = {
			{0,0,7,0},
			{0,7,7,0},
			{0,0,7,0},
			{0,0,0,0}
		};


	switch (i)
	{
	case 1:
		for(int x = 0; x<4; x++){
			for(int y= 0; y<4; y++){
				p[x][y] = p1[x][y];
			}
		}
		break;
		
	case 2:
		for(int x = 0; x<4; x++){
			for(int y= 0; y<4; y++){
				p[x][y] = p2[x][y];
			}
		}
		break;

	case 3:
		for(int x = 0; x<4; x++){
			for(int y= 0; y<4; y++){
				p[x][y] = p3[x][y];
			}
		}
		break;

	case 4:
		for(int x = 0; x<4; x++){
			for(int y= 0; y<4; y++){
				p[x][y] = p4[x][y];
			}
		}
		break;

	case 5:
		for(int x = 0; x<4; x++){
			for(int y=0; y<4; y++){
				p[x][y] = p5[x][y];
			}
		}
		break;

	case 6:
		for(int x = 0;x<4; x++){
			for(int y= 0; y<4; y++){
				p[x][y] = p6[x][y];
			}
		}
		break;

	case 7:
		for(int x = 0; x<4; x++){
			for(int y= 0; y<4; y++){
				p[x][y] = p7[x][y];
			}
		}
		break;
		
	default:
		break;
	}
	
	return p;
}
