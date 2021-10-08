#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <graphics.h>
#include <math.h>
#include <windows.h> 

#define GRID_SIDE 19 //�]�w������� 
#define SCREEN_HEIGHT 500 //�]�w�C���������� 
#define SCREEN_WIDTH 500 //�]�w�C�������e��
#define LEFT_MARGINE 40 //�]�w����� 
#define TOP_MARGINE 50 //�]�w�W��� 


//�ŧi�C�����X�{����C�|��� 
enum Object {
	WALL, //�ѽL
	BLACKS, //�´� 
	WHITES //�մ� 
};
//�ŧi�Ѥl�����V 
struct Node {
	int row; //�`�I��b�ĴX�� 
	int col; //�`�I��b�ĴX�C  
}; 
//�ŧi�ؼ��I ��AI�� 
struct link
{
	int row;
	int col;
	struct link* next;
};
//�w�q���V�`�I���c�������ܼ� 
typedef struct Node *NodePointer;

int AI_mode=1;
int degree=10;

int playGame(int field[][GRID_SIDE], NodePointer snake); //�C���i���޿� 
void drawSquare(int row, int col, int color);//ø�s�C���ϰ�
void drawGameField(int field[][GRID_SIDE]);//ø�s��� 
int  controlpieceDirection(int field[][GRID_SIDE], NodePointer piece);//����Ѥl�޿� 
void closeGame(NodePointer piece); //�B�z�C�������޿� 
void drawBlack(int row, int col, int color);//�e�¦�Ѥl 
void showInfo(int countRound);//��ܹC���T�� 
int win(int field[GRID_SIDE][GRID_SIDE],int row,int col,int who);//�P�_�ӭt 
int dfs(int field[GRID_SIDE][GRID_SIDE],int row,int col,int dir[2],int dir2[2],int target);//�j�M�ѽL 

//
void print_arr(int field[][GRID_SIDE]);
void AI(int field[][GRID_SIDE],NodePointer piece);
int cal_points(int field[GRID_SIDE][GRID_SIDE],int row,int col,int who);//�p��M�I�� 
int dfs_count_color(int field[GRID_SIDE][GRID_SIDE],int row,int col,int dir[2],int dir2[2],int target);//�j�M�ѽL 
int  AIcontrolpieceDirection(int field[][GRID_SIDE], NodePointer piece);//AI������O 
//
int main( )
{
   initwindow(SCREEN_WIDTH + LEFT_MARGINE * 3, SCREEN_HEIGHT + TOP_MARGINE * 3, "Gomoku");
   while(true){
   		showInfo(0);
   		Node head = {0, 0}; //�]�w�Ѥl��l��m�M��V 
		NodePointer piece = &head;
		int field[GRID_SIDE][GRID_SIDE]={{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
										 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
 	 	char key;
 	 	key= playGame(field, piece); //�i��C��
		//if (key == 'a' || key == 'A'){
		//		closeGame(piece); //��a���s�C��
		//	}
		
   }
}

int playGame(int field[][GRID_SIDE], NodePointer piece){
	drawGameField(field);
		char key;
		while(true){
			if(AI_mode)AIcontrolpieceDirection(field, piece); 
			else controlpieceDirection(field, piece);
			showInfo(0);
			if(win(field,piece->row,piece->col,WHITES)||win(field,piece->row,piece->col,BLACKS))
				return false;
			
			if(kbhit()) {
	   		char key;
			key = getch();	
			
			if (key == 'a' || key == 'A')
				return key; 	
				
			}			  
		}
	
} 
void closeGame(NodePointer piece){
	clearviewport();
	cleardevice();
	free(piece);
	exit(0);
}
void drawGameField(int field[][GRID_SIDE]){
   	int row = 0, col = 0;
   	cleardevice(); //�M�z�ù��e�� 
	for(row = 0; row < GRID_SIDE; row ++){
   		for(col = 0; col <  GRID_SIDE; col++){
						switch(field[row][col])	{
					case WALL: //�ѽL�b�x�}�����ȬO0 
						drawSquare(row, col, YELLOW);
						break;
					case BLACKS: //�´Ѧb�x�}�����ȬO1
						drawBlack(row, col, BLACK);
						break;
					case WHITES: //�մѦb�x�}�����ȬO2
						drawSquare(row, col, WHITE);
						break;			
				}	
						
				}	
		   }
	}
void drawSquare(int row, int col, int color){
	
	int squareHeight = SCREEN_HEIGHT / GRID_SIDE;
   	int SquareWidth = SCREEN_WIDTH / GRID_SIDE;
	int left = 0, right = 0, bottom = 0, top = 0;
	left = LEFT_MARGINE + col * SquareWidth + col;
   	top =  TOP_MARGINE + row * squareHeight + row;
	right = left + SquareWidth;
   	bottom = top + squareHeight;
   	 
	setfillstyle(SOLID_FILL,color); //�]�wø�s���󪺬���ߩM�C�� 
   	bar(left , top, right, bottom);	//�]�wø�s�������m 
}
void drawBlack(int row, int col, int color){
		int squareHeight = SCREEN_HEIGHT / GRID_SIDE;
   	int SquareWidth = SCREEN_WIDTH / GRID_SIDE;
	int left = 0, right = 0, bottom = 0, top = 0;
	left = LEFT_MARGINE + col * SquareWidth + col;
   	top =  TOP_MARGINE + row * squareHeight + row;
	right = left + SquareWidth;
   	bottom = top + squareHeight;
   	
	setcolor(color);
	setfillstyle(SOLID_FILL,color);
   	//circle(left+25,top+25,30);
   	fillellipse(left+15,top+15,15,15);
	//bar(left , top, right, bottom);	//�]�wø�s�������m 
}

int controlpieceDirection(int field[][GRID_SIDE], NodePointer piece){
	int countRound=0;
		
	//get key code by pressing keybord
	int key;
	//decide piece's moving direction
	while(1){
		Sleep(50);
		key = getch();
		if (key==13){ //��ENTER 
				if(countRound==0){//�ª��^�X
					if(field[piece->row][piece->col]==2||field[piece->row][piece->col]==1){//����ª��^�X�U��մѪ��Ŧ�
						drawGameField(field);
						piece->col++;
						drawSquare(piece->row, piece->col, RED);
						showInfo(countRound);
						
					} 
					else{
						field[piece->row][piece->col]=1;
						drawGameField(field);
						countRound++;
						if(win(field,piece->row,piece->col,BLACKS))
						{
							
							char RoundMsg[45] = " BLACK WIN!";
							int i=0;
							char closeMsg[45]="press a restart      press b exit";
							setcolor(RED); //�]�w��r�C�� 
   							settextstyle(TRIPLEX_FONT, HORIZ_DIR ,3); //�]�w�r���A�����Ϋ����M�r���j�p 
   							outtextxy(80,570, closeMsg);
							for(;;i++){
								setcolor(i%14); //�]�w��r�C�� 
   								settextstyle(TRIPLEX_FONT, HORIZ_DIR , 5); //�]�w�r���A�����Ϋ����M�r���j�p 
   								outtextxy(100, SCREEN_HEIGHT / 2, RoundMsg); //�¤�^�X��X��r	
	   							delay(200);
	   							if(kbhit()){
	   								char key2;
	   								key2=getch();
	   								if(key2=='a'||key2=='A')
										return key2; 
								   	if(key2=='b'||key2=='B')
								   	 	closeGame(piece);
									}
							}
							
						}
						else showInfo(countRound);	
					} 
					
				}
				else if(countRound==1)
				{//�ժ��^�X 
				
					
						if(field[piece->row][piece->col]==1||field[piece->row][piece->col]==2)
						{//����ժ��^�X�U��´Ѫ��Ŧ� 
							drawGameField(field);
							piece->col++;
							drawSquare(piece->row, piece->col, RED);
							showInfo(countRound);
						}
						else
						{
							field[piece->row][piece->col]=2;
							drawGameField(field);
							countRound++;
							if(win(field,piece->row,piece->col,WHITES))
							{	
								char RoundMsg[45] = " WHITE WIN!";
								char closeMsg[45]="press a restart     press b exit";
								setcolor(RED); //�]�w��r�C�� 
   								settextstyle(TRIPLEX_FONT, HORIZ_DIR , 3); //�]�w�r���A�����Ϋ����M�r���j�p 
   								outtextxy(80,570, closeMsg);
								int i=0;
								for(;;i++)
								{
									setcolor(i%14); //�]�w��r�C�� 
   									settextstyle(TRIPLEX_FONT, HORIZ_DIR , 5); //�]�w�r���A�����Ϋ����M�r���j�p 
   									outtextxy(100, SCREEN_HEIGHT / 2, RoundMsg); //�դ�^�X��X��r	
		   							delay(200);
		   							if(kbhit())
									{
	   									char key3;
	   									key3=getch();
	   									if(key3=='a'||key3=='A')
	   										return key3;
	   									if(key3=='b'||key3=='B')
									   	 	closeGame(piece);	
									}
								}
							}
							else showInfo(countRound);	
						}
					
				}
				if(countRound==2){
					countRound=countRound%2;
				}
				
		}
		if(piece->col<0){
			drawSquare(piece->row, piece->col, BLACK);
			piece->col=0;
		
		}
		else if(piece->col>18){
			drawSquare(piece->row, piece->col, BLACK);
			piece->col=18;
		
		}
		else if(piece->row<0){
			drawSquare(piece->row, piece->col, BLACK);
			piece->row=0;
		}
		else if(piece->row>18){
			drawSquare(piece->row, piece->col, BLACK);
			piece->row=18;
		}
			switch(key){
			
			case 72://�W 
				drawGameField(field);
				piece->row--;
				showInfo(countRound);
				break;
			case 80://�U 
				drawGameField(field);
				piece->row++;
				showInfo(countRound);
				break;
			case 75://�� 
				drawGameField(field);	
				piece->col--;
				showInfo(countRound);
				break;
			case 77://�k 
				drawGameField(field);
				piece->col++;
				showInfo(countRound);
				break;	
			}
			drawSquare(piece->row, piece->col, RED);
	
	
	}	                                     
}

void showInfo(int countRound){
	char RoundMsg1[45] = " Round: Black";
	char RoundMsg2[45] = " Round: White";
	setcolor(WHITE); //�]�w��r�C�� 
   	settextstyle(COMPLEX_FONT, HORIZ_DIR , 5); //�]�w�r���A�����Ϋ����M�r���j�p 
   	if(countRound==0){
   		outtextxy(110, 0, RoundMsg1); //�¤�^�X��X��r	
	   }
	else if(countRound==1){
		outtextxy(110, 0, RoundMsg2); //�դ�^�X��X��r	
	}   
} 

int dfs(int field[GRID_SIDE][GRID_SIDE],int row,int col,int dir[2],int dir2[2],int target)
{
	int i=0,j=0,result=0,count=0;
	
	if(field[row][col]!=target)return 0;
	int row1=row,row2=row,col1=col,col2=col;
	for(i=0;i<5;i++)
	{
		if(0<=row1&&row1<GRID_SIDE&&0<=col1&&col1<GRID_SIDE)
		{
			if(field[row1][col1]==target)count++;
			else break;
		}
		row1+=dir[0];
		col1+=dir[1];
	}
	for(i=0;i<5;i++)
	{
		if(0<=row2&&row2<GRID_SIDE&&0<=col2&&col2<GRID_SIDE)
		{
			if(field[row2][col2]==target)count++;
			else break;
		}
		row2+=dir2[0];
		col2+=dir2[1];
	}
	if(count>=6)return 1;
	else return 0;
	
	
}
int win(int field[GRID_SIDE][GRID_SIDE],int row,int col,int who)
{
	int i=0;
	int dir[8][2]=
	{
		{-1,-1},//left up
		{-1,0},//up
		{-1,1},//right up
		{0,1},//right
		{1,1},//right down
		{1,0},//down
		{1,-1},//left down
		{0,-1}//left
	};
	int test=0;
	for(i=0;i<4;i++)
	{
		int tem=dfs(field,row,col,dir[i],dir[i+4],who);
		test|=tem;
		//printf("%d",tem);
		//puts("----");
	}
	return test;
}
void print_arr(int field[][GRID_SIDE])
{
	system("cls");
	int i=0,j=0;
	for(i=0;i<GRID_SIDE;i++)
	{
		for(j=0;j<GRID_SIDE;j++)
		{
			printf("%3d ",field[i][j]);
		}
		puts("");
	}
}
void AI(int field[][GRID_SIDE],NodePointer piece)
{
	int row=-1,col=-1,points=-1;
	int dis_map[GRID_SIDE][GRID_SIDE]={{0,0,0,0,0,0,0,0,0,0},
									 {0,0,0,0,0,0,0,0,0,0},
									 {0,0,0,0,0,0,0,0,0,0},
									 {0,0,0,0,0,0,0,0,0,0},
									 {0,0,0,0,0,0,0,0,0,0},
									 {0,0,0,0,0,0,0,0,0,0},
									 {0,0,0,0,0,0,0,0,0,0},
									 {0,0,0,0,0,0,0,0,0,0},
									 {0,0,0,0,0,0,0,0,0,0},
									 {0,0,0,0,0,0,0,0,0,0}};
	struct link *head=(struct link*)malloc(sizeof(struct link));
	struct link *now=head;
	
	int i=0,j=0,count=0;
	for(i=0;i<GRID_SIDE;i++)
	{
		for(j=0;j<GRID_SIDE;j++)
		{
			if(field[i][j]==0)
			{
				dis_map[i][j]=cal_points(field,i,j,WHITES);
				if(dis_map[i][j]>points)//���󰪤� 
				{
					points = dis_map[i][j];
					struct link *tem = (struct link*)malloc(sizeof(struct link));
					tem->row = i;
					tem->col = j;
					tem->next = NULL;
					head->next=tem;
					now=tem;
					count=1;
					
				}
				else if(dis_map[i][j]==points)
				{
					
					struct link *tem = (struct link*)malloc(sizeof(struct link));
					tem->row = i;
					tem->col = j;
					tem->next = NULL;
					now->next = tem;
					now=tem;
					count++;
				}
			}
		}
	}
	srand(time(NULL));
	int rand_index=rand()%count+1;
	printf("rand %d count %d",rand_index,count);
	for(i = 0;i < rand_index;i++)
	{
		head=head->next;
	}
	col=head->col;
	row=head->row;
	printf("This is dangerous table:\n");
	print_arr(dis_map);
	field[row][col]=WHITES;
	piece->row=row;
	piece->col=col;
	//printf("target is %d %d",row,col);
}
int cal_points(int field[GRID_SIDE][GRID_SIDE],int row,int col,int who)
{
	int i=0,points=-1,sum_points=0;
	int dir[8][2]=
	{
		{-1,-1},//left up
		{-1,0},//up
		{-1,1},//right up
		{0,1},//right
		{1,1},//right down
		{1,0},//down
		{1,-1},//left down
		{0,-1}//left
	};
	for(i=0;i<4;i++)
	{
		int tem=dfs_count_color(field,row,col,dir[i],dir[i+4],BLACKS);
		if(tem>0)
		sum_points+=pow(degree,tem-1);
		tem=dfs_count_color(field,row,col,dir[i],dir[i+4],WHITES);
		if(tem>0)
			sum_points+=pow(degree,tem-1)+1;
	}
	return sum_points;
}
int dfs_count_color(int field[GRID_SIDE][GRID_SIDE],int row,int col,int dir[2],int dir2[2],int target)
{
	
	int i=0,j=0,result=0,count=0;
	int row1=row,row2=row,col1=col,col2=col;
	for(i=0;i<4;i++)
	{
		row1+=dir[0];
		col1+=dir[1];
		if(0<=row1&&row1<GRID_SIDE&&0<=col1&&col1<GRID_SIDE)
		{
			if(field[row1][col1]==target)count++;
			else break;
		}
		
	}
	for(i=0;i<4;i++)
	{
		row2+=dir2[0];
		col2+=dir2[1];
		if(0<=row2&&row2<GRID_SIDE&&0<=col2&&col2<GRID_SIDE)
		{
			if(field[row2][col2]==target)count++;
			else break;
		}
		
	}
	
	if(target==WHITES)//�P�_���S�������Ŷ��U 
	{
		int space_counter=0;
		row1=row,row2=row,col1=col,col2=col;
		while(1)
		{
			row1+=dir[0];
			col1+=dir[1];
			if(0<=row1&&row1<GRID_SIDE&&0<=col1&&col1<GRID_SIDE)
			{
				if(field[row1][col1]!=BLACKS)space_counter++;
				else break;
				if(space_counter>=4)break;
			}
			else break;
		}
		while(1)
		{
			row2+=dir2[0];
			col2+=dir2[1];
			if(0<=row2&&row2<GRID_SIDE&&0<=col2&&col2<GRID_SIDE)
			{
				if(field[row2][col2]!=BLACKS)space_counter++;
				else break;
				if(space_counter>=4)break;
			}
			else break;
		}
		if(space_counter>=4)return count;
		else return 0;
	}
	return count;
	
	
//
	/*int i=0,j=0,result=0;
	for(i=0;i<5;i++)
	{
		row+=dir[0];
		col+=dir[1];
		if(0<=row&&row<GRID_SIDE&&0<=col&&col<GRID_SIDE)
		{
			if(field[row][col]!=target)
			{
				break;
			}
			else
			{
				if(result==0)result++;
				else
				{
					if(target==BLACKS)result*=degree;
					else result*=int(degree/2);
				} 
			}
		}
		else
		{
			return result;
		}
	}
	return result;*/
	
}
int AIcontrolpieceDirection(int field[][GRID_SIDE], NodePointer piece){
	int countRound=0;
		
	//get key code by pressing keybord
	int key;
	//decide piece's moving direction
	while(1){
		Sleep(50);
		key = getch();
		if (key==13)
		{ //��ENTER 
				countRound=0;
				if(countRound==0)
				{//�ª��^�X
					if(field[piece->row][piece->col]==2||field[piece->row][piece->col]==1)
					{//����ª��^�X�U��մѪ��Ŧ�
						drawGameField(field);
						piece->col++;
						drawSquare(piece->row, piece->col, RED);
						showInfo(countRound);
						
					} 
					else
					{
						field[piece->row][piece->col]=1;
						drawGameField(field);
						if(win(field,piece->row,piece->col,BLACKS))
						{
							
							char RoundMsg[45] = " BLACK WIN!";
							int i=0;
							char closeMsg[45]="press a restart      press b exit";
							setcolor(RED); //�]�w��r�C�� 
   							settextstyle(TRIPLEX_FONT, HORIZ_DIR ,3); //�]�w�r���A�����Ϋ����M�r���j�p 
   							outtextxy(80,570, closeMsg);
							for(;;i++)
							{
								setcolor(i%14); //�]�w��r�C�� 
   								settextstyle(TRIPLEX_FONT, HORIZ_DIR , 5); //�]�w�r���A�����Ϋ����M�r���j�p 
   								outtextxy(100, SCREEN_HEIGHT / 2, RoundMsg); //�¤�^�X��X��r	
	   							delay(200);
	   							if(kbhit())
								{
	   								char key2;
	   								key2=getch();
	   								if(key2=='a'||key2=='A')
										return key2; 
								   	if(key2=='b'||key2=='B')
								   	 	closeGame(piece);
								}
							}
							
						}
						else
						{
							showInfo(countRound);
							AI(field,piece);
							//drawGameField(field);
							if(win(field,piece->row,piece->col,WHITES))
							{	
								char RoundMsg[45] = " WHITE WIN!";
								char closeMsg[45]="press a restart     press b exit";
								setcolor(RED); //�]�w��r�C�� 
   								settextstyle(TRIPLEX_FONT, HORIZ_DIR , 3); //�]�w�r���A�����Ϋ����M�r���j�p 
   								outtextxy(80,570, closeMsg);
								int i=0;
								for(;;i++)
								{
									setcolor(i%14); //�]�w��r�C�� 
   									settextstyle(TRIPLEX_FONT, HORIZ_DIR , 5); //�]�w�r���A�����Ϋ����M�r���j�p 
   									outtextxy(100, SCREEN_HEIGHT / 2, RoundMsg); //�դ�^�X��X��r	
		   							delay(200);
		   							if(kbhit())
									{
	   									char key3;
	   									key3=getch();
	   									if(key3=='a'||key3=='A')
	   										return key3;
	   									if(key3=='b'||key3=='B')
									   	 	closeGame(piece);	
									}
								}
							}
							else showInfo(countRound);
						}	
					} 
					
				}
				
				
		}
		if(piece->col<0){
			drawSquare(piece->row, piece->col, BLACK);
			piece->col=0;
		
		}
		else if(piece->col>18){
			drawSquare(piece->row, piece->col, BLACK);
			piece->col=18;
		
		}
		else if(piece->row<0){
			drawSquare(piece->row, piece->col, BLACK);
			piece->row=0;
		}
		else if(piece->row>18){
			drawSquare(piece->row, piece->col, BLACK);
			piece->row=18;
		}
			switch(key){
			
			case 72://�W 
				drawGameField(field);
				piece->row--;
				showInfo(countRound);
				break;
			case 80://�U 
				drawGameField(field);
				piece->row++;
				showInfo(countRound);
				break;
			case 75://�� 
				drawGameField(field);	
				piece->col--;
				showInfo(countRound);
				break;
			case 77://�k 
				drawGameField(field);
				piece->col++;
				showInfo(countRound);
				break;	
			}
			drawSquare(piece->row, piece->col, RED);
	
	
	}	                                     
}




