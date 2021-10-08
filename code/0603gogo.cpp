#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <graphics.h>
#include <math.h>
#include <windows.h> 

#define GRID_SIDE 19 //設定視窗格數 
#define SCREEN_HEIGHT 500 //設定遊戲視窗高度 
#define SCREEN_WIDTH 500 //設定遊戲視窗寬度
#define LEFT_MARGINE 40 //設定左邊界 
#define TOP_MARGINE 50 //設定上邊界 


//宣告遊戲場出現物體列舉函數 
enum Object {
	WALL, //棋盤
	BLACKS, //黑棋 
	WHITES //白棋 
};
//宣告棋子控制方向 
struct Node {
	int row; //節點位在第幾行 
	int col; //節點位在第幾列  
}; 
//宣告目標點 給AI用 
struct link
{
	int row;
	int col;
	struct link* next;
};
//定義指向節點結構的指標變數 
typedef struct Node *NodePointer;

int AI_mode=1;
int degree=10;

int playGame(int field[][GRID_SIDE], NodePointer snake); //遊戲進行邏輯 
void drawSquare(int row, int col, int color);//繪製遊戲區域
void drawGameField(int field[][GRID_SIDE]);//繪製方塊 
int  controlpieceDirection(int field[][GRID_SIDE], NodePointer piece);//控制棋子邏輯 
void closeGame(NodePointer piece); //處理遊戲結束邏輯 
void drawBlack(int row, int col, int color);//畫黑色棋子 
void showInfo(int countRound);//顯示遊戲訊息 
int win(int field[GRID_SIDE][GRID_SIDE],int row,int col,int who);//判斷勝負 
int dfs(int field[GRID_SIDE][GRID_SIDE],int row,int col,int dir[2],int dir2[2],int target);//搜尋棋盤 

//
void print_arr(int field[][GRID_SIDE]);
void AI(int field[][GRID_SIDE],NodePointer piece);
int cal_points(int field[GRID_SIDE][GRID_SIDE],int row,int col,int who);//計算危險度 
int dfs_count_color(int field[GRID_SIDE][GRID_SIDE],int row,int col,int dir[2],int dir2[2],int target);//搜尋棋盤 
int  AIcontrolpieceDirection(int field[][GRID_SIDE], NodePointer piece);//AI的控制面板 
//
int main( )
{
   initwindow(SCREEN_WIDTH + LEFT_MARGINE * 3, SCREEN_HEIGHT + TOP_MARGINE * 3, "Gomoku");
   while(true){
   		showInfo(0);
   		Node head = {0, 0}; //設定棋子初始位置和方向 
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
 	 	key= playGame(field, piece); //進行遊戲
		//if (key == 'a' || key == 'A'){
		//		closeGame(piece); //按a重新遊戲
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
   	cleardevice(); //清理螢幕畫面 
	for(row = 0; row < GRID_SIDE; row ++){
   		for(col = 0; col <  GRID_SIDE; col++){
						switch(field[row][col])	{
					case WALL: //棋盤在矩陣中的值是0 
						drawSquare(row, col, YELLOW);
						break;
					case BLACKS: //黑棋在矩陣中的值是1
						drawBlack(row, col, BLACK);
						break;
					case WHITES: //白棋在矩陣中的值是2
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
   	 
	setfillstyle(SOLID_FILL,color); //設定繪製物件的為實心和顏色 
   	bar(left , top, right, bottom);	//設定繪製方塊的位置 
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
	//bar(left , top, right, bottom);	//設定繪製方塊的位置 
}

int controlpieceDirection(int field[][GRID_SIDE], NodePointer piece){
	int countRound=0;
		
	//get key code by pressing keybord
	int key;
	//decide piece's moving direction
	while(1){
		Sleep(50);
		key = getch();
		if (key==13){ //按ENTER 
				if(countRound==0){//黑的回合
					if(field[piece->row][piece->col]==2||field[piece->row][piece->col]==1){//防止黑的回合下到白棋的空位
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
							setcolor(RED); //設定文字顏色 
   							settextstyle(TRIPLEX_FONT, HORIZ_DIR ,3); //設定字型，水平或垂直和字型大小 
   							outtextxy(80,570, closeMsg);
							for(;;i++){
								setcolor(i%14); //設定文字顏色 
   								settextstyle(TRIPLEX_FONT, HORIZ_DIR , 5); //設定字型，水平或垂直和字型大小 
   								outtextxy(100, SCREEN_HEIGHT / 2, RoundMsg); //黑方回合輸出文字	
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
				{//白的回合 
				
					
						if(field[piece->row][piece->col]==1||field[piece->row][piece->col]==2)
						{//防止白的回合下到黑棋的空位 
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
								setcolor(RED); //設定文字顏色 
   								settextstyle(TRIPLEX_FONT, HORIZ_DIR , 3); //設定字型，水平或垂直和字型大小 
   								outtextxy(80,570, closeMsg);
								int i=0;
								for(;;i++)
								{
									setcolor(i%14); //設定文字顏色 
   									settextstyle(TRIPLEX_FONT, HORIZ_DIR , 5); //設定字型，水平或垂直和字型大小 
   									outtextxy(100, SCREEN_HEIGHT / 2, RoundMsg); //白方回合輸出文字	
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
			
			case 72://上 
				drawGameField(field);
				piece->row--;
				showInfo(countRound);
				break;
			case 80://下 
				drawGameField(field);
				piece->row++;
				showInfo(countRound);
				break;
			case 75://左 
				drawGameField(field);	
				piece->col--;
				showInfo(countRound);
				break;
			case 77://右 
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
	setcolor(WHITE); //設定文字顏色 
   	settextstyle(COMPLEX_FONT, HORIZ_DIR , 5); //設定字型，水平或垂直和字型大小 
   	if(countRound==0){
   		outtextxy(110, 0, RoundMsg1); //黑方回合輸出文字	
	   }
	else if(countRound==1){
		outtextxy(110, 0, RoundMsg2); //白方回合輸出文字	
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
				if(dis_map[i][j]>points)//找到更高分 
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
	
	if(target==WHITES)//判斷有沒有足夠空間下 
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
		{ //按ENTER 
				countRound=0;
				if(countRound==0)
				{//黑的回合
					if(field[piece->row][piece->col]==2||field[piece->row][piece->col]==1)
					{//防止黑的回合下到白棋的空位
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
							setcolor(RED); //設定文字顏色 
   							settextstyle(TRIPLEX_FONT, HORIZ_DIR ,3); //設定字型，水平或垂直和字型大小 
   							outtextxy(80,570, closeMsg);
							for(;;i++)
							{
								setcolor(i%14); //設定文字顏色 
   								settextstyle(TRIPLEX_FONT, HORIZ_DIR , 5); //設定字型，水平或垂直和字型大小 
   								outtextxy(100, SCREEN_HEIGHT / 2, RoundMsg); //黑方回合輸出文字	
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
								setcolor(RED); //設定文字顏色 
   								settextstyle(TRIPLEX_FONT, HORIZ_DIR , 3); //設定字型，水平或垂直和字型大小 
   								outtextxy(80,570, closeMsg);
								int i=0;
								for(;;i++)
								{
									setcolor(i%14); //設定文字顏色 
   									settextstyle(TRIPLEX_FONT, HORIZ_DIR , 5); //設定字型，水平或垂直和字型大小 
   									outtextxy(100, SCREEN_HEIGHT / 2, RoundMsg); //白方回合輸出文字	
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
			
			case 72://上 
				drawGameField(field);
				piece->row--;
				showInfo(countRound);
				break;
			case 80://下 
				drawGameField(field);
				piece->row++;
				showInfo(countRound);
				break;
			case 75://左 
				drawGameField(field);	
				piece->col--;
				showInfo(countRound);
				break;
			case 77://右 
				drawGameField(field);
				piece->col++;
				showInfo(countRound);
				break;	
			}
			drawSquare(piece->row, piece->col, RED);
	
	
	}	                                     
}




