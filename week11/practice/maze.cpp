#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

using namespace std;

FILE *fp;
int WIDTH, HEIGHT;
int **maze;
int *verLines, *horLines;
int ID;

void FprintLine();
void DetermineVerLines(int i);
void FprintVerLines(int i);
void DetermineHorLines(int i);
void FprintHorLines(int i);
void FprintHorLines(int i);

int main() {
    int i, min;
    ID = 0;
    srand(time(NULL));

    fp = fopen("Maze.maz", "wt");
    
    cin >> WIDTH;
    cin >> HEIGHT;

    maze = new int*[HEIGHT];
    for(i = 0; i<HEIGHT; i++)
        maze[i] = new int[WIDTH]();
    verLines = new int[WIDTH-1];
    horLines = new int[WIDTH];
    
    FprintLine();
    for(i = 0; i<WIDTH; i++) 
        maze[0][i] = ++ID;

    for(i = 0; i<HEIGHT - 1; i++) {
        DetermineVerLines(i);
        FprintVerLines(i);
        DetermineHorLines(i);
        FprintHorLines(i);   
    }
    int lValue, k;
    for(i = 0; i<WIDTH - 1; i++) {
        lValue = maze[HEIGHT-1][i+1];
        if(maze[HEIGHT-1][i] != maze[HEIGHT-1][i+1]) {
            k = 1;
            
            min = maze[HEIGHT-1][i+1];
            min = (maze[HEIGHT-1][i] < maze[HEIGHT-1][i+1]) ? maze[HEIGHT-1][i] : min;
            maze[HEIGHT-1][i+1] = min;
            maze[HEIGHT-1][i] = min;
            while(i + k + 1< WIDTH && lValue == maze[HEIGHT-1][i+k+1]){
                maze[HEIGHT-1][i+k+1] = min;
                k++;
            }
            verLines[i] = 0;
        }
        else verLines[i] = 1;
        
    }
    //DetermineVerLines(HEIGHT-1);
    FprintVerLines(HEIGHT-1);
    FprintLine();

    return 0;
}

void FprintLine() {
    int i;
    for(i = 0; i<WIDTH; i++) {
        fprintf(fp, "+-");
    }
    fprintf(fp, "+\n");
}

void DetermineVerLines(int i) {
    int det, j, lValue, k, min;
    for(j = 0; j<WIDTH - 1; j++) {
        det = rand()%2;
        if(maze[i][j] != maze[i][j+1] && lValue != maze[i][j+1]){
            lValue = maze[i][j+1];
            if(det == 1){ 
                min = maze[i][j+1];
                min = (maze[i][j] < maze[i][j+1]) ? maze[i][j] : min;
                maze[i][j+1] = min;
                maze[i][j] = min;
                verLines[j] = 0;
                k = 1;
                while(j + k + 1< WIDTH && lValue == maze[i][j+k+1]){
                    maze[i][j+k+1] = maze[i][j+k];
                    k++;
                }
                k = 1;
                while(j - k >= 0 && verLines[j-k] == 0){
                    maze[i][j-k] = maze[i][j-k+1];
                    k++;
                }
            }
            else 
                verLines[j] = 1;
        }
        else{
            lValue = maze[i][j+1];
            verLines[j] = 1;
        }
    }
    ID++;
}

void FprintVerLines(int i) {
    int j;
    fprintf(fp, "|");
    for(j = 0; j<WIDTH - 1; j++) {
        if(verLines[j] == 0)
            fprintf(fp, "%d ", maze[i][j]);
        else fprintf(fp, "%d|", maze[i][j]);
    }
    fprintf(fp, "%d|\n", maze[i][j]);
}

void DetermineHorLines(int i) {
    int j, flag = 0, curId = maze[i][0]; //flag = 1(해당 ID에 아래칸이 뚫림)
    for(j = 0; j<WIDTH; j++) {
        verLines[j] = rand() % 2;
        if(verLines[j] == 0){
            flag = 1;
            maze[i+1][j] = maze[i][j];
        }
        if(curId != maze[i][j]){
            //if(flag == 0){
                verLines[j-1] = 0;
                maze[i+1][j-1] = maze[i][j-1];
            //}
            curId = maze[i][j];
            flag = 0;
        }
        if(j == WIDTH - 1) {
            verLines[j] = 0;
            maze[i+1][j] = maze[i][j];
        }
    }
    
    //다음줄 ID
    for(j = 0; j<WIDTH; j++) 
        if(maze[i+1][j] == 0) 
            maze[i+1][j] = ++ID;
}

void FprintHorLines(int i) {
    int j;
    for(j = 0; j<WIDTH; j++) {
        if(verLines[j] == 0) 
            fprintf(fp, "+ ");
        else
            fprintf(fp, "+-");
    }
    fprintf(fp, "+\n");
}