#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_SIZE 100
#define MAZE_SIZE 6

// 0: NOT VISITED
// 1: WALL
// 2: VISITED
// 3: BACKTRACKED

typedef struct StackObjectRec { 
	short r;
	short c;
} StackObject;

StackObject stack[MAX_STACK_SIZE]; 
int top = -1; 
StackObject here={1,0}, entry={1,0};

char maze[MAZE_SIZE][MAZE_SIZE] = {
	{'1', '1', '1', '1', '1', '1'},
	{'e', '0', '1', '0', '0', '1'},
	{'1', '0', '0', '0', '1', '1'},
	{'1', '0', '1', '0', '1', '1'},
	{'1', '0', '1', '0', '0', 'x'},
	{'1', '1', '1', '1', '1', '1'},
};

void initialize()
{
	top = -1;
}

int isEmpty()
{
	return (top == -1);
}

int isFull()
{
	return (top == (MAX_STACK_SIZE-1));
}

void push(StackObject item)
{ 
  	  if( isFull() ) { 
		  printf("stack is full\n");
	  } 
	  else stack[++top] = item; 
}

StackObject pop() 
{ 
   	if( isEmpty() ) {
		printf("stack is empty\n");
	}
	else return stack[top--]; 
} 

void printStack()
{
	int i;
	for(i=5;i>top;i--)
		printf("|     |\n");
	for(i=top;i>=0;i--)
		printf("|(%01d,%01d)|\n", stack[i].r, stack[i].c);
	printf("-------\n");
}

void pushLoc(int r, int c)
{
	if( r < 0 || c < 0 ) return;
	if( maze[r][c] != '1' && maze[r][c] != '.' ){
		StackObject tmp;
		tmp.r = r;
		tmp.c = c;
		push(tmp);
	}
}

void printMaze(char m[MAZE_SIZE][MAZE_SIZE])
{
	int r,c;
	printf("\n\n");
	for(r=0;r<MAZE_SIZE;r++){
		for(c=0;c<MAZE_SIZE;c++){
			if( c == here.c && r == here.r )
				printf("m ");
			else {
				if( m[r][c] == 0 ) printf("0 ");
				else printf("%c ",m[r][c]);
			}
		}
		printf("\n");
	}
	printf("\n\n");
}

int main(void)
{
	int r,c;
	here = entry;
	FILE *f1 = fopen("maze.txt","w");
	if(f1 == NULL){
		printf("File open error!\n");
		return -1;
	}
	//printMaze(maze);
	fprintf(f1, "Your starting position is [1, 0].\n");


	while ( maze[here.r][here.c]!='x' ){
		// Deadlock check.
		// Check if it can go to somewhere else.
		// Check whether there is a way to go more than one way.
		if( (maze[here.r-1][here.c] == '1' || maze[here.r-1][here.c] == '2' || maze[here.r-1][here.c] == '3') 
			&& (maze[here.r][here.c+1] == '1' || maze[here.r][here.c+1] == '2' || maze[here.r][here.c+1] == '3') 
			&& (maze[here.r+1][here.c] == '1' || maze[here.r+1][here.c] == '2' || maze[here.r+1][here.c] == '3') 
			&& (maze[here.r][here.c-1] == '1' || maze[here.r][here.c-1] == '2' || maze[here.r][here.c-1] == '3') ){ 
			// If there is nowhere to go, then mark the current direction as backtracked('3'), pop the stack and backtrack.
			fprintf(f1, "Deadlock at [%d, %d]!\n", here.r, here.c);
			printf("Deadlock!\n");
			printf("Select the direction to move: \n");
			printf("1: N, 2: E, 3: S, 4: W\n");
			int input;
			scanf("%d", &input);
			fprintf(f1, "Input: %d\n", input);

			switch(input){
			case 1: // Check if it can move to North then move to North
				// Move to maze[r-1][c]
				printf("You chose North!\n");
				fprintf(f1, "You chose North!\n");
				if(maze[here.r-1][here.c] == '1'){
					fprintf(f1, "Wall detected!\n");
				}
				else if( (r-1) < 0 ){
					fprintf(f1, "Boundary error!\n");
				}
				else{
					// Mark the current location as backtracked('3')
					maze[here.r][here.c] = '3';
					// Then move
					here.r--;
					fprintf(f1, "Moved to [%d, %d].\n", here.r, here.c);
				}
				break;
			case 2: // Check if it can move to East then move to East
				// Move to maze[r][c+1]
				printf("You chose East!\n");
				fprintf(f1, "You chose East!\n");
				if(maze[here.r][here.c+1] == '1'){
					fprintf(f1, "Wall detected!\n");

				}
				else if(maze[here.r][here.c+1] == '3'){
					fprintf(f1, "Already backtracked!\n");
				}
				else if( (c+1) >= MAZE_SIZE ){
					fprintf(f1, "Boundary error!\n");
				}
				else{
					// Mark the current location as backtracked('3')
					maze[here.r][here.c] = '3';
					// Then move
					here.c++;
					fprintf(f1, "Moved to [%d, %d].\n", here.r, here.c);
				}
				break;
			case 3:	// Check if it can move to South then move to South
			// Move to maze[r+1][c]
				printf("You chose South!\n");
				fprintf(f1, "You chose South!\n");
				if(maze[here.r+1][here.c] == '1'){
					fprintf(f1, "Wall detected!\n");

				}
				else if(maze[here.r+1][here.c] == '3'){
					fprintf(f1, "Already backtracked!\n");
				}
				else if( (r+1) >= MAZE_SIZE ){
					fprintf(f1, "Boundary error!\n");
				}
				else{
					// Mark the current location as backtracked('3')
					maze[here.r][here.c] = '3';
					// Then move
					here.r++;
					fprintf(f1, "Moved to [%d, %d].\n", here.r, here.c);
				}
				break;
			case 4:	// Check if it can move to West then move to West
				// Move to maze[r][c-1]
				printf("You chose West!\n");
				fprintf(f1, "You chose West!\n");
				// Move to maze[r][c-1]
				if(maze[here.r][here.c-1] == '1'){
					fprintf(f1, "Wall detected!\n");

				}
				else if(maze[here.r][here.c-1] == '3'){
					fprintf(f1, "Already backtracked!\n");
				}
				else if( (c-1) < 0 ){
					fprintf(f1, "Boundary error!\n");
				}
				else{
					// Mark the current location as backtracked('3')
					maze[here.r][here.c] = '3';
					// Then move
					here.c--;
					fprintf(f1, "Moved to [%d, %d].\n", here.r, here.c);
				}
				break;
			}

			StackObject tmp;
			maze[here.r][here.c] = '3';
			tmp = pop();
			here.r = tmp.r;
			here.c = tmp.c;
			fprintf(f1, "pop the stack and move to [%d, %d].\n", here.r, here.c);
			fprintf(stdout, "pop the stack and move to [%d, %d]\n", here.r, here.c);
			}

		r = here.r;
		c = here.c;
		fprintf(f1, "Your current position is [%d, %d].\n", r, c);
		maze[r][c] = '2'; // Mark the current position as VISITED.

		// Select the direction to move.
		int input;
		printf("Select the direction to move: \n");
		printf("1: N, 2: E, 3: S, 4: W\n");
		do{
			scanf("%d", &input);
			fprintf(f1, "Input: %d\n", input);
		}while(input != 1 && input != 2 && input != 3 && input != 4);
		// printf("Loop out\n");
		switch(input){
			case 1: // Check if it can move to North then move to North
				// Move to maze[r-1][c]
				printf("You chose North!\n");
				fprintf(f1, "You chose North!\n");
				if(maze[here.r-1][here.c] == '1'){
					fprintf(f1, "Wall detected!\n");
				}
				else if(maze[here.r-1][here.c] == '2'){
					fprintf(f1, "Already Visited!\n");
				}
				else if(maze[here.r-1][here.c] == '3'){
					fprintf(f1, "Already backtracked!\n");
				}
				else if( (r-1) < 0 ){
					fprintf(f1, "Boundary error!\n");
				}
				else{
					// Push current location
					fprintf(f1, "Pushed [%d, %d] into the stack.\n", here.r, here.c);
					pushLoc(here.r, here.c);
					// Mark the current location as visited('2')
					maze[here.r][here.c] = '2';
					// Then move
					here.r--;
					fprintf(f1, "Moved to [%d, %d].\n", here.r, here.c);
				}
				break;
			case 2: // Check if it can move to East then move to East
				// Move to maze[r][c+1]
				printf("You chose East!\n");
				fprintf(f1, "You chose East!\n");
				if(maze[here.r][here.c+1] == '1'){
					fprintf(f1, "Wall detected!\n");

				}
				else if(maze[here.r][here.c+1] == '2'){
					fprintf(f1, "Already Visited!\n");
				}
				else if(maze[here.r][here.c+1] == '3'){
					fprintf(f1, "Already backtracked!\n");
				}
				else if( (c+1) >= MAZE_SIZE ){
					fprintf(f1, "Boundary error!\n");
				}
				else{
					// Push current location
					fprintf(f1, "Pushed [%d, %d] into the stack.\n", here.r, here.c);
					pushLoc(here.r, here.c);
					// Mark the current location as visited('2')
					maze[here.r][here.c] = '2';
					// Then move
					here.c++;
					fprintf(f1, "Moved to [%d, %d].\n", here.r, here.c);
				}
				break;
			case 3:	// Check if it can move to South then move to South
			// Move to maze[r+1][c]
				printf("You chose South!\n");
				fprintf(f1, "You chose South!\n");
				if(maze[here.r+1][here.c] == '1'){
					fprintf(f1, "Wall detected!\n");

				}
				else if(maze[here.r+1][here.c] == '2'){
					fprintf(f1, "Already Visited!\n");
				}
				else if(maze[here.r+1][here.c] == '3'){
					fprintf(f1, "Already backtracked!\n");
				}
				else if( (r+1) >= MAZE_SIZE ){
					fprintf(f1, "Boundary error!\n");
				}
				else{
					// Push current location
					fprintf(f1, "Pushed [%d, %d] into the stack.\n", here.r, here.c);
					pushLoc(here.r, here.c);
					// Mark the current location as visited('2')
					maze[here.r][here.c] = '2';
					// Then move
					here.r++;
					fprintf(f1, "Moved to [%d, %d].\n", here.r, here.c);
				}
				break;
			case 4:	// Check if it can move to West then move to West
				// Move to maze[r][c-1]
				printf("You chose West!\n");
				fprintf(f1, "You chose West!\n");
				// Move to maze[r][c-1]
				if(maze[here.r][here.c-1] == '1'){
					fprintf(f1, "Wall detected!\n");

				}
				else if(maze[here.r][here.c-1] == '2'){
					fprintf(f1, "Already Visited!\n");
				}
				else if(maze[here.r][here.c-1] == '3'){
					fprintf(f1, "Already backtracked!\n");
				}
				else if( (c-1) < 0 ){
					fprintf(f1, "Boundary error!\n");
				}
				else{
					// Push current location
					fprintf(f1, "Pushed [%d, %d] into the stack.\n", here.r, here.c);
					pushLoc(here.r, here.c);
					// Mark the current location as visited('2')
					maze[here.r][here.c] = '2';
					// Then move
					here.c--;
					fprintf(f1, "Moved to [%d, %d].\n", here.r, here.c);
				}
				break;

		}
		
		if(maze[here.r][here.c] == 'x'){
			fprintf(f1, "Maze exit!\n");
			break;
		}
	}
	fprintf(f1,"finished.\n");
	fclose(f1);
	return 0;
}
