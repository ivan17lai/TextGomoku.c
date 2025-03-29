#include<stdio.h>
#include<stdlib.h>

void flash_dispaly(int checkerboard[15][15],int t_col, int t_row,int player){

    system("cls");

    int n = 15;
    char icon[] = {' ', 'o', 'x'};

    printf("   - - - - - - - - - - - - - - - v0.1.1\n");
    for(int col=0; col<n; col++){
        printf("%s[ ",(col==t_col)?">":" ");
        for(int row=0; row<n; row++){
            if(col == t_col && row == t_row){
                printf("%c ",(checkerboard[col][row] == -1)? '+':'#');
            }
            else{
                printf("%c ", icon[checkerboard[col][row]+1]);
            }
        }
        printf("]");
        if(col == 0 || col == 2){
            printf("  -------------------");
        }
        else if(col == 1){
            printf("  |    %s    |",(player==0)? "your turn":" computer ");
        }
        printf("\n");
    }
    printf("   - - - - - - - - - - - - - - -\n  ");
    for(int row=0; row<n; row++){
        printf("%s",(row==t_row)? " ^":"  ");
    }
    printf("\n\n");

}

int cheak_winner(int checkerboard[15][15],int n_col, int n_row){

    int n = 15;
    int player = checkerboard[n_col][n_row];
    
    int count_c = 0, count_r = 0;
    for(int i=0; i<n; i++){
        if(checkerboard[n_col][i] == player){
            count_c++;
            if(count_c == 5){
                return player;
            }
        }
        else{
            count_c = 0;
        }
        if(checkerboard[i][n_row] == player){
            count_r++;
            if(count_r == 5){
                return player;
            }
        }
        else{
            count_r = 0;
        }
    }

    int c_col=n_col,c_row=n_row;
    int lu_count = 0,rd_count = 0;
    for(int i=1;i<=15;i++){
        if(c_col-i<0 || c_row-i<0){
            break;
        }
        if(checkerboard[c_col-i][c_row-i] == player){
            lu_count++;
            if(lu_count == 5){
                return player;
            }
        }else{
            break;
        }
    }
    for(int i=1;i<=15;i++){
        if(c_col+i>14 || c_row+i>14){
            break;
        }
        if(checkerboard[c_col+i][c_row+i] == player){
            rd_count++;
            if(rd_count == 5){
                return player;
            }
        }else{
            break;
        }
    }
    if(lu_count + rd_count >= 4){
        return player;
    }

    int ru_count = 0,ld_count = 0;
    for(int i=1;i<=15;i++){
        if(c_col-i<0 || c_row+i>14){
            break;
        }
        if(checkerboard[c_col-i][c_row+i] == player){
            ru_count++;
            if(ru_count == 5){
                return player;
            }
        }else{
            break;
        }
    }
    for(int i=1;i<=15;i++){
        if(c_col+i>14 || c_row-i<0){
            break;
        }
        if(checkerboard[c_col+i][c_row-i] == player){
            ld_count++;
            if(ld_count == 5){
                return player;
            }
        }else{
            break;
        }
    }
    if(ru_count + ld_count >= 4){
        return player;
    }
    
    return -1;
}

int main() {

    int n = 15;
    int checkerboard[n][n];
    int player = 0;

    for(int col=0; col<n; col++){
        for(int row=0; row<n; row++){
            checkerboard[col][row] = -1;
        }
    }

    int t_col = 7;
    int t_row = 7;

    while(1){

        flash_dispaly(checkerboard, t_col, t_row, player);
        char c = _getch();

        if (c == '\r' || c == '\n') {
            checkerboard[t_col][t_row] = player? 1:0;
            flash_dispaly(checkerboard, -1, -1, player);
            int code = cheak_winner(checkerboard, t_col, t_row);
            if (player==0) {
                player = 1;
            }else{
                player = 0;
            }
            if (code == 0){
                printf("You win!\n");
                break;
            }else if(code == 1){
                printf("Computer win!\n");
                break;
            }
        }else if(c == 'w' && t_col > 0){
            t_col--;
        }
        else if(c == 's' && t_col < n-1){
            t_col++;
        }
        else if(c == 'a' && t_row > 0){
            t_row--;
        }
        else if(c == 'd' && t_row < n-1){
            t_row++;
        }

    }

    system("pause");
    return 0;
}
