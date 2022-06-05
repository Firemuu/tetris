#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>


typedef struct{ //structure for return a 4*4 matrix
  int shape[4][4];
}pieces;


typedef struct{ //structure for return the board matrix
  int matri[13][12];
}matrix;

int height(pieces piece){ //return the height of a tetromino
  int upside=0;
  for(int i=0; i<3; i++){
    if((piece.shape[3-i][0]+piece.shape[3-i][1]+piece.shape[3-i][2]+piece.shape[3-i][3])==0 && upside==i){
      upside++;
    }
  }
  return upside;
}

int width1(pieces piece){ //return the empty colums on the left of a tetromino
  int side1=0;
  for(int i=0;i<3;i++){
    if((piece.shape[0][i]+piece.shape[1][i]+piece.shape[2][i]+piece.shape[3][i])==0 && side1==i){
      side1++;
    }
  }
  return (side1);
}

int width2(pieces piece){ //return the empty colums on the rhight of a tetromino
  int side2=0;
  for(int i=0;i<3;i++){
    if((piece.shape[0][3-i]+piece.shape[1][3-i]+piece.shape[2][3-i]+piece.shape[3][3-i])==0 && side2==i){
      side2++;
    }
  }
  return (side2);
}

int min(int a, int b, int c, int d){ //return the smallest number of the 4
  int min;
  if (c <=d){
    min=c;}
  else{
    min=d;}
  if (b <=min){
    min=b;}
  if (a <=min){
    min=a;}
  return min;
}

int max(int num1, int num2) // return the maximum of 2 numbers
{
    return (num1 > num2 ) ? num1 : num2;
}

int maxheight (matrix board, int column){ // look for the highest square in the desired column
  for(int i = 0 ; i < 13 ;  i++){
      if(board.matri[i][column] == 1){
        return i;}
  }
  return -1;
}

pieces list_to_struct(int matrix[4][4]){ // transform a int piece into pieces piece 
  pieces transform;
  for(int i = 0 ; i < 4 ; i++){ // line
    for(int j = 0 ; j < 4 ; j++){ // column
      transform.shape[i][j] = matrix[i][j]; // rotation
    }
  }
  return transform;
}

matrix matrix_to_struct(int matrice[13][12]){  // transform an int matrix into a matrix type matrix
  matrix transform ;
  for(int i = 0 ; i < 13 ; i++){ // line
    for(int j = 0 ; j < 12 ; j++){ // column
      transform.matri[i][j] = matrice[i][j]; // rotation
    }
  }
  return transform;
}

bool verif_end(matrix matrix){ // verify if it's the end of the game
  for(int i = 0 ; i < 3 ; i++){ // line
    for(int j = 1 ; j < 12 ; j++){ // column
      if (matrix.matri[i][j]==1){
        return true;
      }
    }
  }
  return false;
}

void printmatrix(matrix board){ // print a matrix
  printf("    0  1  2  3  4  5  6  7  8  9\n");
  for(int i = 3 ; i < 13 ; i++){ // line
    for(int j = 0 ; j < 12 ; j++){ // column
      if(board.matri[i][j]==0){
        printf(" . ");} //put a . where there is nothing
      if(board.matri[i][j]==1){
        printf(" @ ");} // put an @ where there is a piece
      if(board.matri[i][j]==2){
        printf(" | ");} // put a | where is the edge of the board
    }
    printf("\n");
  }
}

void printpiece(pieces piece){
  for(int i = 0 ; i < 4 ; i++){
    for(int j = 0 ; j < 4 ; j++){
      if(piece.shape[i][j]==0){
        printf(" ");} // put nothing where there is nothing
      if(piece.shape[i][j]==1){
        printf("@");} // put an @ where there is a piece of tetromino
    }
    printf("\n");
  }
}

/*int isthere (matrix board , int x, int y){ // check if the indicated coordinates are full or empty
  if(board.matri[x][y] == 0 ){
    return 0 ;  // return true if there is nothing
  }
  else return 1;
}*/

int random_pieces(){ // choose a random tetromino
  int random = rand()%7;
  return random;
}

pieces rotation(pieces piece){ // flip a tetromino by 45° 
  pieces temp;
  int i, j;
  for(int i = 0 ; i < 4 ; i++){ // line
    for(int j = 0 ; j < 4 ; j++){ // column
      temp.shape[3-j][i] = piece.shape[i][j]; // rotation
    }
  }
  return temp;
}

matrix delete_line(matrix board,int *score){ //delete a line and lower the upper lines
  int sum = 0;
  for(int i = 0; i < 13; i++){
    for(int j = 1 ; j < 11 ; j++){
      if(board.matri[i][j] == 1){
          sum +=1;}
    }
      if(sum == 10){
        *score = *score + 100;
        int line = i;
        for(int k = 1; k < 11; k++){
          board.matri[i][k] = 0;} // erase the line
        for(int m = 0 ; m < line ; m++){ // line descent loop
          for(int p = 1 ; p < 11; p++){
            int num = board.matri[line-m-1][p];
            board.matri[line-m][p] = num;
          }
        }
      }
    sum = 0 ;
  }
return board;
}

matrix placepiece (matrix board, pieces piece,int choice){ // place the piece on the board
  choice=choice+1-width1(piece); //ajusts the colum the payer wants with the one of the matrix
  int b=0;
  int a1=0,a2=0,a3=0,a4=0;
  int count=0;
  int reduce=0;
  int start1=0;
  int a1_max=0,a2_max=0,a3_max=0,a4_max=0;
  int line,g=0,h=0;
  int side_1,side_2;
  while(piece.shape[b][0]==0 && piece.shape[b][1]==0 && piece.shape[b][2]==0 && piece.shape[b][3]==0 ){ // while that counts the amount of empty lines on the bottom
    count++;
    b++;
  } 
  for(int i = count ; i < 4 ; i++){ 
    for(int j = 0 ; j < 4 ; j++){
      if (j==0 && piece.shape[i][j]==1){
        a1=max(a1,i);
      }
      if (j==1 && piece.shape[i][j]==1){
        a2=max(a2,i);
      }
      if (j==2 && piece.shape[i][j]==1){
        a3=max(a3,i);
      }
      if (j==3 && piece.shape[i][j]==1){
        a4=max(a4,i);
      }
    }
  }
  if(a1==0){
    reduce++;
  }
  else{
    if(start1==0){
      start1=1;
    }
  }
  if(a2==0){
    reduce++; 
  }
  else{
    if(start1==0){
      start1=2;
    }
  }
  if(a3==0){ 
    reduce++;
  }
  else{ 
    if(start1==0){
      start1=3;
    }
  }
  if(a4==0){ 
    reduce++; 
  }
  for(int i = 0 ; i < 13 ; i++){ // looks at the minimum position a piece can go
    if(a1!=0){
      if(board.matri[i][choice]!=1 && a1!=-1){
        a1_max++;
      }
      else{
        a1=-1;
      }
    }
    else{
      a1_max=69;
    }
    if(a2!=0){
      if(board.matri[i][choice+1]!=1 && a2!=-1){
        a2_max++;
      }
      else{
        a2=-1;
      }
    }
    else{
      a2_max=69;
    }
    if(a3!=0){
      if(board.matri[i][choice+2]!=1 && a3!=-1){
        a3_max++;
      }
      else{
        a3=-1;
      }
    }
    else{
      a3_max=69;
    }
    if(a4!=0){
      if(board.matri[i][choice+3]!=1 && a4!=-1){
        a4_max++;
      }
      else{
        a4=-1;
      }
    }
    else{a4_max=69;}
  }
  line=min(a1_max,a2_max,a3_max,a4_max)-1; //takes the lowest position it can go to and ajusts it to the matrix scale
  side_1=width1(piece); // left empty spaces of the piece
  side_2=width2(piece); // right empty spaces of the piece
  pieces piecetest = piece;
  for(int i=line; i>(line-4+height(piecetest)) ;i--){ // puts the information gathered before to place the piece
    for(int j=choice+side_1; j<choice+4-side_2;j++){
      board.matri[i][j] = piecetest.shape[3-height(piecetest)-g][side_1+h];
      h++; // a normal count to be used as the colums of the piece
    }
    h=0; // reset of the count
    g++;// a normal count to be used as the lines of the piece
  }
  return board;
}


int main (){

///////////////////////////////////////////////////////////////////////////////////////////////
  
// different shapes of tetromino 

  int square[4][4] = { // square tetromino 0
    {0,0,0,0},
    {0,1,1,0},
    {0,1,1,0},
    {0,0,0,0}
 };
  int straight_line[4][4] = { // straight tetromino 1
    {0,0,1,0},
    {0,0,1,0},
    {0,0,1,0},
    {0,0,1,0}
  };
  int S[4][4] = { // S tetromino 2
    {0,0,0,0},
    {0,0,1,1},
    {0,1,1,0},
    {0,0,0,0}
  };
  int Z[4][4] = { // Z tetromino 3
    {0,0,0,0},
    {0,1,1,0},
    {0,0,1,1},
    {0,0,0,0}
  };
  int J[4][4] = { // J tetromino 4
    {0,0,1,0},
    {0,0,1,0},
    {0,1,1,0},
    {0,0,0,0}
  };
  int L[4][4] = { // L tetromino 5
    {0,1,0,0},
    {0,1,0,0},
    {0,1,1,0},
    {0,0,0,0}
  };
  int T[4][4] = { // T tetromino 6
    {0,0,0,0},    
    {0,1,1,1},
    {0,0,1,0},
    {0,0,0,0}
  };

///////////////////////////////////////////////////////////////////////////////////////////////

int tetris_boardtemp[13][12] = {   
    {0,0,0,0,0,0,0,0,0,0,0,0},  // initialisation of the matrix board
    {0,0,0,0,0,0,0,0,0,0,0,0},  
    {0,0,0,0,0,0,0,0,0,0,0,0}, 
    {2,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,2},
    {2,0,0,0,0,0,0,0,0,0,0,2}
  };
//////////////////////////////////////////////////////////////////////////////////////////////
  
int end = 0;
int score = 0 ;
int wrong=0;
int random;
int choice = 15;
int rotate = 8;
matrix newboard;
newboard = matrix_to_struct(tetris_boardtemp);  // the placement board of the pieces upthere
pieces tetromino;
pieces nextpiece ;
pieces currentpiece ;
pieces rotation1;
pieces rotation2;
pieces rotation3;

srand(time(NULL));
  while(end == 0){ // while loop that represents the game
    if(wrong==0){ // to make sure that if the user has the same piece if he/she did a mistake before
      random = random_pieces(); // random variable that will help in picking a random shape for the piece
      if(random == 0){
        nextpiece = list_to_struct(square);
      }
      if(random == 1){
        nextpiece = list_to_struct(straight_line);
      }
      if(random == 2){
        nextpiece = list_to_struct(S);
      }
      if(random == 3){
        nextpiece = list_to_struct(Z);
      }
      if(random == 4){
        nextpiece = list_to_struct(J);
      }
      if(random == 5){
        nextpiece = list_to_struct(L);
      }
      if(random == 6){
        nextpiece = list_to_struct(T);
      }
      currentpiece = nextpiece;
    }
    else{
        wrong=0; //resets the wrong variable so that it doesn't skip the random piece loop
    }
    printf("the next tetromino is : \n");
    printpiece(nextpiece); // shows the user the next piece
    printf("\n");
    printmatrix(newboard); // shows the game board
    printf("\n");
    printf("1 :");
    printf("\n");
    printpiece(currentpiece); //1  shows the diferent rotations of the piece
    printf("2 :");
    printf("\n");
    rotation1 = rotation(currentpiece); // 2
    rotation2 = rotation(rotation1); // 3
    rotation3 = rotation(rotation2); // 4
    printpiece(rotation1);
    printf("3 :");
    printf("\n");
    printpiece(rotation2);
    printf("4 :");
    printf("\n");
    printpiece(rotation3);
    printf("\n");
    printf("choose the rotation");
    printf("\n");
    scanf("%d",&rotate); // here the user choses the rotation he/she desiers
    if(rotate<=4 && rotate>=0){ // verifies that the number inputed is a usable one
      if(rotate==1){ // changes the rotation of the piece depending on what the user choses
      }
      if(rotate==2){
          currentpiece = rotation1;
      }
      if(rotate==3){
          currentpiece = rotation2;
      }
      if(rotate==4){
          currentpiece = rotation3;
      }
      printpiece(currentpiece); // shows the user the piece after the rotation
      int far=6+width1(currentpiece)+width2(currentpiece); // calculates the farthest colum the piece can go
      printf("Choose a colum between 0 and %d", far); // tells the user wich colum he/she can use
      printf("\n");   
      scanf("%d", &choice);
      if(choice>=0 && choice<=far){ // check that the choice is one that fits the game
        printf("\n");
        newboard = placepiece(newboard,currentpiece,choice); //makes the new board
        printmatrix(newboard); //prints the new board
        newboard=delete_line(newboard,&score); // checks if there are any lines that are complitly filled
      }
      else{ // tells the user that the input was not one that was an option
        printf("wrong input \n");
        wrong=1; 
      }
    }
      else{ // in case the user picked a wrong number to input
        wrong=1;
      }
    if (verif_end(newboard)==true){ // cheks if the the tetris board is overflowing to end the game
      end=1;
    }
    system("clear"); // clears the screan 
  }
  printf("u lOoSe, uR sCOrE Is %d ",score); // gives the player his/her score after 
}