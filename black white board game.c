#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <math.h>
#include <mem.h>
#include <malloc.h>

/*
Explanation of functions and code structure:
	Represent the board as a n by n array. Numerically, the board can have 3 states, 0 for empty, 1 for white, and 2 for black. The
	board is then rendered based on the current state of the array (kaw na bahala dito vhien). 
		Move checker: checks if a move encloses opposing pieces. Uses a current var as marker of the current coordinate being checked.
		Checks for every direction (8), and goes through the ff. condition each time a direction is checked:
			Check if current var is out of bounds !(>= 0 || <= n - 1) - break
			Check if current var is 0 - break
			Check if current var is opposing color - append current var to an memory stack
			Check if current var is current color:
				if 1st iteration - break - means current var is the same color but is just adjacent to the piece placed.
				else - change the states of the coordinates in the memory array to the state of the piece placed, break.
			After break - clear memory stack
	Bakit stacks? Can adapt to changing board sizes.
	
*/

//node
typedef struct node{
	int coordX;
	int coordY;	//store coords
	struct node *next;
}node;

// globally declare the stack for convenience
node *TOP = NULL, *pushNode = NULL, *temp = NULL, *popNode = NULL;


//other globals
int countWhite = 0, countBlack = 0;

//stack functions

void createAndPush(int x, int y){
	//create node
	pushNode = (node*)malloc(sizeof(node));
	pushNode->coordX = x; pushNode->coordY = y;
	
	//push
	if (TOP == NULL){
		TOP = pushNode;
		pushNode->next = NULL;
	}else{
		pushNode->next = TOP;
		TOP = pushNode;
	}
	
	pushNode = NULL;
}

void popCoord(){
	
	TOP = TOP->next;
	popNode->next = NULL;
	//reassign the values of popNode to temp to return
	free(temp);
	temp = (node*)malloc(sizeof(node));
	temp->coordX = popNode->coordX; temp->coordY = popNode->coordY;
	
	//free popnode
	free(popNode);
	popNode = TOP;
	
	//after calling this function, temp node pointer will now hold the coordinates.
}


// GotoXY function used by draw_board
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// draw_board implementation
void draw_board(int board[][20], int size) {
    int i, r, f, square_start_y = 5;

    system("cls");

    // Displays file labels
    gotoxy(26, 4);
    for (i = 0; i < size; i++) {
        printf(" %d  ", i);
    }

    for (r = 0; r < size; r++) {
        int square_start_x = 25;

        // Displays rank labels
        gotoxy(square_start_x - 2, square_start_y + 1);
        printf("%d", r);

        for (f = 0; f < size; f++) {
            gotoxy(square_start_x, square_start_y);
            printf("+---+");

            gotoxy(square_start_x, square_start_y + 1);
            char disp;
            if (board[f][r] == 1)
                disp = 'O';
            else if (board[f][r] == 2)
                disp = '0';
            else
                disp = ' ';
            printf("| %c |", disp);

            gotoxy(square_start_x, square_start_y + 2);
            printf("+---+");

            square_start_x += 4;
        }

        square_start_y += 2;
    }
}


int main(){
	int size;
	char name1[20], name2[20];
	node *popNode = TOP;	 //by default, popNode is equal to TOP
	int player = 1; //default white goes first
	int i;
	
	printf("Enter size of the board => ");
	scanf("%d", &size);
	getchar();
	
//	int coordBoard[size][size];
	
	printf("Enter name of Player 1 (White) => ");
	gets(name1);
	printf("Enter name of Player 2 (Black) => ");
	gets(name2);
	
	int coordBoard[20][20] = { 0 }; // 20 by 20 max
	system("cls");
	draw_board(coordBoard, size);
	
	while(1){
		// initialize player variables
		char curName[20];
		int oppPlayer;
		int ix, iy; //coordinates for the piece placed
		int curX, curY; //moving variable coordinates
		int moveX[8] = {1, 1, 0, -1, -1, -1, 0, 1};
		int moveY[8] = {0, 1, 1, 1, 0, -1, -1, -1};
		
		if (player == 1){
			oppPlayer = 2;
		}else if (player == 2){
			oppPlayer = 1;
		}		
		
		if (player == 1){
			strcpy(curName, name1);
		}else if (player == 2){
			strcpy(curName, name2);
		}
		
		// Move picking
		printf("\n%s's turn. Pick a coordinate: ", curName);
		scanf("%d %d", &ix, &iy);
		
		//check if coordinate picked is occupied
		if (coordBoard[ix][iy] != 0){
			printf("\nThat coordinate is already occupied. Try again.");
			continue;
		}else{
			
			coordBoard[ix][iy] = player;
			draw_board(coordBoard, size);
			
			//first add to count
			switch (player){
				case 1:
					countWhite++;
					break;
				case 2:
					countBlack++;
					break;
			}
			
			//check surrounding cells
			
			//outer for loop for changing directions
			for (i = 0; i < 8; i++){
				//initialize cur var positions
				curX = ix + moveX[i];
				curY = iy + moveY[i];
				
				while ((curX >= 0 && curX <= size - 1) && (curY >= 0 && curY <= size - 1)){
					//inner loop for checking cells in current direction
					
					if (coordBoard[curX][curY] == 0){
						break;
					}else if (abs(curX - ix) <= 1) && (abs(curY - iy) <= 1)){
						break;
					}else if (coordBoard[curX][curY] == oppPlayer){
						createAndPush(curX, curY);
						//move to next cell
						curX += moveX[i];
						curY += moveY[i];
						
						//decrease piece count of opposing player
						switch (player){
							case 1:
								countBlack--;
								break;
							case 2:
								countWhite--;
								break;
							}
					}else if (coordBoard[curX][curY] == player){
						while(TOP != NULL){
							popCoord();
							coordBoard[temp->coordX][temp->coordY] = player;
							//add each turned piece to count
							switch (player){
								case 1:
									countWhite++;
									break;
								case 2:
									countBlack++;
									break;
							}

							
							//decrease the counter for the opposing player
							switch (oppPlayer){
								case 1:
									countWhite--;
									break;
								case 2:
									countBlack--;
									break;
							}
							//drawboard
              draw_board(coordBoard, size);
						}
						break;
					}	
				}
			}
			
			if ((countWhite + countBlack) == size * size){
				if (countWhite > countBlack){
					printf("White has more pieces on board. White wins!");
				} else if (countBlack > countWhite){
					printf("Black has more pieces on board. Black wins!");
				} else if (countBlack == countWhite){
					printf("White has the same amount of pieces as Black. It's a Tie!");
				}
				
				return 0;
			}
			
		}
		
		//switch to next player
		player = oppPlayer;
	}
	
	
}
