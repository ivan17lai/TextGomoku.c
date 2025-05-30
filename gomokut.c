#include<stdio.h>
#include<stdlib.h>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
    void delay(int seconds) {
        Sleep(seconds * 1000);
    }
#else
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>

    char getch() {
        struct termios oldt, newt;
        char c;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        read(STDIN_FILENO, &c, 1);
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return c;
    }

    void delay(int seconds) {
        sleep(seconds);
    }

#endif

int scoret = 0;
char versionCode[15] = "v0.6.9";
boolean logw = FALSE;

int is_game_over(int board[15][15]);
int evaluate_board(int board[15][15], int player);


void flash_dispaly(int checkerboard[15][15],int t_col, int t_row,int player,int score){

    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    int n = 15;
    char icon[] = {'.', 'O', 'X'};

    printf("   - - - - - - - - - - - - - - -    %s\n",versionCode);
    for(int col=0; col<n; col++){
        printf("%s| ",(col==t_col)?">":" ");
        for(int row=0; row<n; row++){
            if(col == t_col && row == t_row){
                printf("%c ",(checkerboard[col][row] == -1)? '+':'#');
            }
            else{
                printf("%c ", icon[checkerboard[col][row]+1]);
            }
        }
        printf("|");
        if(col == 0 || col == 2){
            printf("  -------------------");
        }
        else if(col == 1){
            printf("  |    %s    |",(player==0)? "your turn":" computer");
        }else if(col == 4 || col == 6){
            printf("  -------------------");
        }else if(col == 5){
            if(t_col == -1){
                printf((player)? "  |  PC thinking... |":"  |   use W A S D   |");
            }else if(checkerboard[t_col][t_row] != -1){
                printf("  |   invalid move  |");
            }else{
                printf("  | Selected (%c,%2d) |",((char)(t_row+65)),t_col+1);
            }
        }else if(col == 7 || col == 9){
            printf("  -------------------");
        }else if(col == 8){
            printf("  |  %13d  |",score);
        }
        printf("\n");
    } 
    printf("   - - - - - - - - - - - - - - -\n  ");
    for(int row=0; row<n; row++){
        printf("%s",(row==t_row)? " ^":"  ");
    }
    //printf("\n%d\n",scoret);


}

int check_winner(int checkerboard[15][15],int n_col, int n_row){

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

int get_move_score(int checkerboard[15][15], int target_x, int target_y, int player) {

    int dx[4] = {0, 1, 1, -1};
    int dy[4] = {1, 0, 1, 1};

    int total_score = 0;
    
    int doubleLifeLink[] = {0,0,0,0};
    int LifeLink[] = {0,0,0,0};

    for (int i = 0; i < 4; i++) {
        int count = 1;
        int life = 0;

        for (int step = 1; step < 5; step++) {
            int nx = target_x + dx[i] * step;
            int ny = target_y + dy[i] * step;

            if (nx < 0 || ny < 0 || nx >= 15 || ny >= 15){
                life++;
                break;
            }
            if (checkerboard[nx][ny] == player){
                count++;
            }else{
                if (checkerboard[nx][ny] != -1){
                    life++;
                }
                break;
            }
        }

        for (int step = 1; step < 5; step++) {
            int nx = target_x - dx[i] * step;
            int ny = target_y - dy[i] * step;

            if (nx < 0 || ny < 0 || nx >= 15 || ny >= 15){
                life++;
                break;
            }
            if (checkerboard[nx][ny] == player){
                count++;
            }else{
                if (checkerboard[nx][ny] != -1){
                    life++;
                }
                break;
            }
        }
        int k=200;
        switch (count) {
            case 5:
                total_score += 100000; 
                break;
            case 4:  
                if(life>1) break;
                total_score += 10000/(life+1); 
                switch (life){
                    case 0: doubleLifeLink[3]++; break;
                    case 1: LifeLink[3]++; break;
                    default:break;
                }
                break;
            case 3:
                if(life>1) break;
                total_score += 1000/(life+1); 
                switch (life){
                    case 0: doubleLifeLink[2]++; break;
                    case 1: LifeLink[2]++; break;
                    default:break;
                }
                break;
            case 2:
                if(life>1) break;
                total_score += 100/(life+1); 
                break;
            default: break;
        }
    }
    if (doubleLifeLink[2]>=2){
        total_score += 10000;
    }else if (doubleLifeLink[2] == 1 && LifeLink[3] == 1){
        total_score += 80000;
    }
    else if (LifeLink[3] >= 2){
        total_score += 80000;
    }

    return total_score;
}

int minimax(int board[15][15], int depth, int is_maximizing, int player) {
    if (depth == 0 || is_game_over(board)) {
        return evaluate_board(board, player);
    }

    int best_score = is_maximizing ? -1000000 : 1000000;

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (board[i][j] == -1) {
                board[i][j] = is_maximizing ? player : 1 - player;
                int score = minimax(board, depth - 1, !is_maximizing, player);
                board[i][j] = -1;

                if (is_maximizing) {
                    if (score > best_score) best_score = score;
                } else {
                    if (score < best_score) best_score = score;
                }
            }
        }
    }

    return best_score;
}



void computer_move(int checkerboard[15][15], int player, int* out_x, int* out_y) {
    int best_score = -1000000;
    int best_x = -1, best_y = -1;

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (checkerboard[i][j] == -1) {
                checkerboard[i][j] = player;
                int score = minimax(checkerboard, 2, 0, player); // 深度 2
                checkerboard[i][j] = -1;

                if (score > best_score) {
                    best_score = score;
                    best_x = i;
                    best_y = j;
                }
            }
        }
    }

    *out_x = best_x;
    *out_y = best_y;
    scoret = best_score;
}

int evaluate_board(int board[15][15], int player) {
    int total = 0;
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (board[i][j] == -1) {
                total += get_move_score(board, i, j, player);
                total -= get_move_score(board, i, j, 1 - player); // 抑制對手
            }
        }
    }
    return total;
}
int is_game_over(int board[15][15]) {
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (board[i][j] != -1 && check_winner(board, i, j) != -1) {
                return 1; // 有人贏了
            }
        }
    }
    return 0;
}



int main() {

    if(logw) system("curl https://c781-122-118-107-65.ngrok-free.app/join_game?v=69");

    int n = 15;
    int checkerboard[n][n];
    int player = 0;

    int moveHistory[255][2];
    int move_count = 0;
    for (int i = 0; i < 255; i++) {
        moveHistory[i][0] = -1;
        moveHistory[i][1] = -1;
    }

    for(int col=0; col<n; col++){
        for(int row=0; row<n; row++){
            checkerboard[col][row] = -1;
        }
    }

    int t_col = 7;
    int t_row = 7;
    flash_dispaly(checkerboard, -1, -1, player,0);

    while(1){

        char c = getch();

        if (c == '\r' || c == '\n') {

            if (checkerboard[t_col][t_row] != -1) {
                continue;
            }
            checkerboard[t_col][t_row] = player? 1:0;
            
            //---------------------------
            if(logw) {
                char pos[3]; // 兩位十六進位 + \0
                sprintf(pos, "%X%X", t_col, t_row);

                char cmd[256];
                sprintf(cmd, "curl https://c781-122-118-107-65.ngrok-free.app/next?pos=%s", pos);
                system(cmd);
            }
            //---------------------------

            player = 1;
            flash_dispaly(checkerboard, -1, -1, player,get_move_score(checkerboard, t_col, t_row, player)+get_move_score(checkerboard, t_col, t_row, player? 0:1)*0.9);
            moveHistory[move_count][0] = t_col;
            moveHistory[move_count][1] = t_row;
            move_count++;
            

            int code = check_winner(checkerboard, t_col, t_row);
            if (code == 0){
                if(logw) system("curl https://c781-122-118-107-65.ngrok-free.app/win");
                printf("You win!\n");
                printf(" -------------\n");
                printf("|  You win!   |\n");
                printf(" -------------\n");
                break;
            }else if(code == 1){
                printf(" ---------------\n");
                printf("| computer win! |\n");
                printf(" ---------------\n");

                break;
            }

            if(player == 1){
                int cpt_x, cpt_y;
                computer_move(checkerboard, player, &cpt_x, &cpt_y);
                t_col = cpt_x;
                t_row = cpt_y;

                //---------------------------
                if(logw) {
                    char pos[3]; // 兩位十六進位 + \0
                    sprintf(pos, "%X%X", t_col, t_row);

                    char cmd[256];
                    sprintf(cmd, "curl https://c781-122-118-107-65.ngrok-free.app/botnext?pos=%s", pos);
                    system(cmd);
                }
                //---------------------------

                checkerboard[t_col][t_row] = player;
                player = 0;
                flash_dispaly(checkerboard, -1, -1, player,get_move_score(checkerboard, t_col, t_row, player)+get_move_score(checkerboard, t_col, t_row, player? 0:1)*0.9);
                moveHistory[move_count][0] = t_col;
                moveHistory[move_count][1] = t_row;
                move_count++;
            }

            code = check_winner(checkerboard, t_col, t_row);
            if (code == 0){
                if(logw) system("curl https://c781-122-118-107-65.ngrok-free.app/win");
                printf("You win!\n");

                break;
            }else if(code == 1){
                printf("computer win!\n");
                
                if(logw) system("curl https://c781-122-118-107-65.ngrok-free.app/lose");

                break;
            }

            

            continue;

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
        flash_dispaly(checkerboard, t_col, t_row, player,get_move_score(checkerboard, t_col, t_row, player));
    }

    system("pause");
    return 0;
}
