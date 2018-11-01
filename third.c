#include<stdio.h>
#include<stdlib.h>

int grid[9][9];

int solved(){ //1 if full, 0 if not full
	for (int i=0; i<9; i++){
		for (int j=0; j<9; j++){
			if (grid[i][j] == 0) return 0;
		}
	}
	return 1;
}

int* searchrow(int row){
	static int rowpop[9]; //1, 2, 3, 4, 5, 6, 7, 8, 9
	for (int m=0; m<9; m++){ //setting all values back to zero
		rowpop[m]=0;
	}
	for (int i=0; i<9; i++){
		if (grid[row][i] == 0) continue;
		else{
			rowpop[grid[row][i]-1] = 1;
		}
	}
	return rowpop;
}

int* searchcol(int col){
	static int colpop[9]; //1, 2, 3, 4, 5, 6, 7, 8, 9
	for (int m=0; m<9; m++){ //setting all values back to zero
		colpop[m]=0;
	}
	for (int i=0; i<9; i++){
		if (grid[i][col] == 0) continue;
		else{
			colpop[grid[i][col]-1] = 1;
		}
	}
	return colpop;
}

int* searchbox(int row, int col){
	int i = row - row%3; //finds where to start and end search for the box
	int j = col - col%3;
	static int boxpop[9];
	for (int m=0; m<9; m++){ //setting all values back to zero
		boxpop[m]=0;
	}
	for (int r = i; r < i+3; r++){
		for (int c = j; c < j+3; c++){
			if(grid[r][c] == 0) continue;
			else{ 
				boxpop[grid[r][c]-1] = 1;
			}
		}
	}
	return boxpop;
}

int* common(int* rows, int* cols, int* box){
	static int comm[9];
	for (int m=0; m<9; m++){ //setting all values back to zero
		comm[m]=0;
	}
	for (int i=0; i<9; i++){
		if (rows[i]==1 || cols[i]==1 || box[i]==1){
			comm[i] = 1;
		}
	}
	return comm;
}

int* possiblevalues(int row, int col){ 
	int* rows; int* cols; int* box; int* comm;
	rows = searchrow(row);
	cols = searchcol(col);
	box = searchbox(row, col);
	comm = common(rows, cols, box);
	return comm;
}

int solve(){ //1 if solved, 0 if unsolvable
	//check 1
	int* rows; int* cols; int* box; int* comm;
	for (int i=0; i<9; i++){ //traverses rows
		for (int j=0; j<9; j++){ //traverses cols
			if (grid[i][j]!=0) continue;
			rows = searchrow(i);
			cols = searchcol(j);
			box = searchbox(i, j);
			comm = common(rows, cols, box);
			
			int possnum = 0;	
			for (int a=0; a<9; a++){
				if (comm[a] == 0 && possnum == 0){
					possnum = a+1;
				}else if (comm[a] == 0 && possnum!=0){ //more than one poss num
					possnum=0;
					break;
				}
			}
			if (possnum!=0){
				grid[i][j] = possnum;
				i=-1; j=-1; //start loop over again
				break;
			}
		}
	}
	//check if solved
	return solved();
}


int solverecur(){ //recursively solves and backtracks, 1 for worked, 0 didn't
	int i, j, x;
	int* possible;
	if (solved()==1) return 1;
	for (i=0; i<9; i++){
		for (j=0; j<9; j++){
			if (grid[i][j]==0){ //find first zero 
				possible = possiblevalues(i, j); //find all poss values for this cell
				/*for (int m=0; m<9; m++){
					printf("%d\t", possible[m]);
				}
				printf("\n");*/
				for (x=0; x<9; x++){ //try each value
					if (possible[x]==0){
						grid[i][j] = x+1;
						if (solverecur()==1) return 1;
						else{ 
							grid[i][j]=0;
							possible = possiblevalues(i, j);
						}
					}
					
				}
				return 0; //went through all poss values and nothing works
			}
		}
	}
	return 1;
}

int main(int argc, char** argv){
	if (argc < 2){
		printf("not enough arguments\n");
		return 0;
	}
	FILE *puzzle;
	puzzle = fopen(argv[1], "r"); 
	if (puzzle==NULL) return 0;

	int val; 
	
	//reads matrix from file
	for (int i=0; i<9; i++){
		for (int j=0; j<9; j++){
			if (fscanf(puzzle, "%d\t", &val)==1){
				grid[i][j] = val;
			}else{
				grid[i][j] = 0;

			}
		}
	} 
	if (solve()==1){ //solve regularly first, filling everything with 100% certainty
		//printf("solved!!\n");
		for (int i=0; i<9; i++){
			for (int j=0; j<9; j++){
				printf("%d\t", grid[i][j]);
			}
			printf("\n");
		}
	}else if (solverecur()==1){ //use backtracking method if not solved simply
		//printf("solved!!\n");
		for (int i=0; i<9; i++){
			for (int j=0; j<9; j++){
				printf("%d\t", grid[i][j]);
			}
			printf("\n");
		}	
		
	}else printf("no-solution\n");

	return 0;
}




