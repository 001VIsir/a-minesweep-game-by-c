
//
// Created by visir on 2023/5/21.
//


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <string.h>

//雷格结构体定义
typedef struct mine {
    char isMine;//是否为雷（'*'为雷， ‘.’反之）
    int mark;//被标记状态，1-没翻开，2-标雷，3-标问，4-翻开
    int numMine;//九个内雷数（不包括自己）
    int edge;//边缘标记
    int figuremark;//周围感叹号数量
} Mine;

//局势——步数和剩余雷数
typedef struct information {
    int step;//步数
    int total;//总雷数
    int restMine;//剩余雷数
} Infor;

//所用到的函数
void nanduChoose(int *len, int *wid, Infor *infor);

void set_edge(Mine mine[][35], int len, int wid);

void emptyClick(Mine mine[][35], int len, int wid, int x, int y, int *boom);

void Singleclick(Mine mine[][35], int len, int wid, int x, int y, int *boom);

void markMine(Mine mine[][35], int len, int wid, int x, int y);

void markQuestion(Mine mine[][35], int len, int wid, int x, int y);

void cancel(Mine mine[][35], int len, int wid, int x, int y);

void Nineclick(Mine mine[][35], int len, int wid, int x, int y, int *boom);

void print_process(Mine mine[][35], int len, int wid, int step, int restMine);

void print_end(Mine mine[][35], int len, int wid, int step, int restMine);

void setting(Mine mine[][35], int len, int wid);

void figure(Mine mine[][35], int len, int wid);

int figureTheRest(Mine mine[][35], int len, int wid);

void figureMark(Mine mine[][35], int len, int wid);

int win_lose(Mine mine[][35], int len, int wid, Infor infor);

//难度选择（长和宽的指针）
void nanduChoose(int *len, int *wid, Infor *infor) {
    int nandu;
    scanf("%d", &nandu);
    getchar();
    switch (nandu) {
        //初级
        case 1:
            *len = 9;
            *wid = 9;
            infor->restMine = 10;
            infor->total = 10;
            break;

            //中级
        case 2:
            *len = 16;
            *wid = 16;
            infor->restMine = 40;
            infor->total = 40;
            break;

            //高级
        case 3:
            *len = 16;
            *wid = 30;
            infor->restMine = 99;
            infor->total = 99;
            break;

            //自定义
        case 4:
            scanf("%d %d %d", len, wid, &infor->restMine);
            infor->total = infor->restMine;
            getchar();//习惯性清除缓存区
            break;
    }
}

//边缘标记
void set_edge(Mine mine[][35], int len, int wid) {
    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < wid; ++j) {
            mine[i][j].edge = 1;
        }
    }
}

//剩余雷数计算
int figureTheRest(Mine mine[][35], int len, int wid) {
    int a = 0;
    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < wid; ++j) {
            if (mine[i][j].isMine == '*')
                a++;
        }
    }
    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < wid; ++j) {
            if (mine[i][j].mark == 2)
                a--;
        }
    }
    return a;
}

//空翻
void emptyClick(Mine mine[][35], int len, int wid, int x, int y, int *boom) {
    if (mine[x][y].mark == 2 || (mine[x][y].mark == 4 && mine[x][y].numMine != 0) || mine[x][y].edge == 0)
        return;
    else {
        Singleclick(mine, len, wid, x - 1, y - 1, boom);
        Singleclick(mine, len, wid, x - 1, y, boom);
        Singleclick(mine, len, wid, x - 1, y + 1, boom);
        Singleclick(mine, len, wid, x, y - 1, boom);
        Singleclick(mine, len, wid, x, y + 1, boom);
        Singleclick(mine, len, wid, x + 1, y - 1, boom);
        Singleclick(mine, len, wid, x + 1, y, boom);
        Singleclick(mine, len, wid, x + 1, y + 1, boom);
    }
}

//单击操作
void Singleclick(Mine mine[][35], int len, int wid, int x, int y, int *boom) {
    if (x >= 0 && y >= 0 && mine[x][y].edge && (mine[x][y].mark == 1 || mine[x][y].mark == 3)) {//如果合法
        mine[x][y].mark = 4;//变成翻开状态
        if (mine[x][y].isMine == '*') {
            *boom = 0;
            return;
        } else if (mine[x][y].numMine == 0)//如果为零触发空翻
            emptyClick(mine, len, wid, x, y, boom);
    } else if (mine[x][y].mark == 2 || mine[x][y].mark == 4 || mine[x][y].edge == 0)//如果为其他状态则非法操作
    {
        return;
    }
}

//九击
void Nineclick(Mine mine[][35], int len, int wid, int x, int y, int *boom) {
    if (x >= 0 && y >= 0 && mine[x][y].edge && (mine[x][y].figuremark == mine[x][y].numMine) &&
        mine[x][y].mark == 4)//合法
    {
        for (int i = x - 1; i < x + 2; ++i) {
            for (int j = y - 1; j < y + 2; ++j) {
                if (i >= 0 && j >= 0 && mine[i][j].edge && mine[i][j].mark != 2) {
                    if (mine[i][j].isMine == '*') {
                        *boom = 0;
                        return;
                    }
                }
            }
        }
        Singleclick(mine, len, wid, x - 1, y - 1, boom);
        Singleclick(mine, len, wid, x - 1, y, boom);
        Singleclick(mine, len, wid, x - 1, y + 1, boom);
        Singleclick(mine, len, wid, x, y - 1, boom);
        Singleclick(mine, len, wid, x, y + 1, boom);
        Singleclick(mine, len, wid, x + 1, y - 1, boom);
        Singleclick(mine, len, wid, x + 1, y, boom);
        Singleclick(mine, len, wid, x + 1, y + 1, boom);
    } else
        return;
}

//标雷
void markMine(Mine mine[][35], int len, int wid, int x, int y) {
    if (x >= 0 && y >= 0 && mine[x][y].edge && (mine[x][y].mark == 1 || mine[x][y].mark == 3))//合法
    {
        mine[x][y].mark = 2;
    } else
        return;
}

//标问
void markQuestion(Mine mine[][35], int len, int wid, int x, int y) {
    if (x >= 0 && y >= 0 && mine[x][y].edge && (mine[x][y].mark == 1 || mine[x][y].mark == 2))//合法
    {
        mine[x][y].mark = 3;
    } else
        return;
}


//取消
void cancel(Mine mine[][35], int len, int wid, int x, int y) {
    if (x >= 0 && y >= 0 && mine[x][y].edge && (mine[x][y].mark == 2 || mine[x][y].mark == 3))//合法
    {
        mine[x][y].mark = 1;
    } else
        return;
}

//打印雷盘（游戏未结束）
void print_process(Mine mine[][35], int len, int wid, int step, int restMine) {
    printf("Game in progress\n");
    printf("%d %d\n", step, restMine);
    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < wid; ++j) {
            if (mine[i][j].mark == 4)
                printf("%d", mine[i][j].numMine);
            else if (mine[i][j].mark == 1)
                printf(".");
            else if (mine[i][j].mark == 2)
                printf("!");
            else if (mine[i][j].mark == 3)
                printf("?");
        }
        printf("\n");
    }
}

//打印雷盘（游戏已结束）
void print_end(Mine mine[][35], int len, int wid, int step, int restMine) {
    printf("%d %d\n", step, restMine);
    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < wid; ++j) {
            if (mine[i][j].isMine == '*')//如果为雷直接输出*，必须先判断雷
                printf("*");
            else if (mine[i][j].mark == 4)//已经翻开的输出数字
                printf("%d", mine[i][j].numMine);
            else if (mine[i][j].mark == 1)//没被翻开的就保持原状
                printf(".");
            else if (mine[i][j].mark == 2)//没被翻开的就保持原状
                printf("!");
            else if (mine[i][j].mark == 3)//没被翻开的就保持原状
                printf("?");
        }
        if (i != len - 1)
            printf("\n");
    }
}

//手动放雷
void setting(Mine mine[][35], int len, int wid) {
    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < wid; ++j) {
            scanf("%c", &mine[i][j].isMine);
        }
        getchar();
    }
}

//计算周围雷数
void figure(Mine mine[][35], int len, int wid) {
    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < wid; ++j) {
            int nummine = 0;
            if (i - 1 >= 0 && j - 1 >= 0 && mine[i - 1][j - 1].isMine == '*' && mine[i - 1][j - 1].edge == 1)
                nummine++;
            if (i - 1 >= 0 && j >= 0 && mine[i - 1][j].isMine == '*' && mine[i - 1][j].edge == 1)
                nummine++;
            if (i - 1 >= 0 && j + 1 >= 0 && mine[i - 1][j + 1].isMine == '*' && mine[i - 1][j + 1].edge == 1)
                nummine++;
            if (i >= 0 && j - 1 >= 0 && mine[i][j - 1].isMine == '*' && mine[i][j - 1].edge == 1)
                nummine++;
            if (i >= 0 && j + 1 >= 0 && mine[i][j + 1].isMine == '*' && mine[i][j + 1].edge == 1)
                nummine++;
            if (i + 1 >= 0 && j - 1 >= 0 && mine[i + 1][j - 1].isMine == '*' && mine[i + 1][j - 1].edge == 1)
                nummine++;
            if (i + 1 >= 0 && j >= 0 && mine[i + 1][j].isMine == '*' && mine[i + 1][j].edge == 1)
                nummine++;
            if (i + 1 >= 0 && j + 1 >= 0 && mine[i + 1][j + 1].isMine == '*' && mine[i + 1][j + 1].edge == 1)
                nummine++;
            mine[i][j].numMine = nummine;
        }
    }
}

//计算周围感叹号数量
void figureMark(Mine mine[][35], int len, int wid) {
    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < wid; ++j) {
            int nummine = 0;
            if (i - 1 >= 0 && j - 1 >= 0 && mine[i - 1][j - 1].mark == 2 && mine[i - 1][j - 1].edge)
                nummine++;
            if (i - 1 >= 0 && j >= 0 && mine[i - 1][j].mark == 2 && mine[i - 1][j].edge)
                nummine++;
            if (i - 1 >= 0 && j + 1 >= 0 && mine[i - 1][j + 1].mark == 2 && mine[i - 1][j + 1].edge)
                nummine++;
            if (i >= 0 && j - 1 >= 0 && mine[i][j - 1].mark == 2 && mine[i][j - 1].edge)
                nummine++;
            if (i >= 0 && j + 1 >= 0 && mine[i][j + 1].mark == 2 && mine[i][j + 1].edge)
                nummine++;
            if (i + 1 >= 0 && j - 1 >= 0 && mine[i + 1][j - 1].mark == 2 && mine[i + 1][j - 1].edge)
                nummine++;
            if (i + 1 >= 0 && j >= 0 && mine[i + 1][j].mark == 2 && mine[i + 1][j].edge)
                nummine++;
            if (i + 1 >= 0 && j + 1 >= 0 && mine[i + 1][j + 1].mark == 2 && mine[i + 1][j + 1].edge)
                nummine++;
            mine[i][j].figuremark = nummine;
        }
    }
}

//判断输赢，原理为客观总雷格数减去已经被打开格子的差等于总雷数
int win_lose(Mine mine[][35], int len, int wid, Infor infor) {
    int a = 0;
    for (int i = 0; i < len; ++i) {
        for (int j = 0; j < wid; ++j) {
            if (mine[i][j].mark == 4) {
                a++;
            }
        }
    }
    if (len * wid - a == infor.total || infor.total == 0) {
        return 1;
    } else
        return 0;
}



//主函数
int main() {
    //定义一些局部变量
    int len;//长
    int wid;//宽
    int boom = 1;
    int win = 0;
    int lose = 0;
    Mine mine[35][35];//雷盘
    for (int i = 0; i < 35; ++i) {
        for (int j = 0; j < 35; ++j) {
            mine[i][j].isMine = '.';
            mine[i][j].mark = 1;
            mine[i][j].edge = 0;
        }
    }
    Infor infor;
    infor.step = 0;

    //开始
    nanduChoose(&len, &wid, &infor);//选择难度
    set_edge(mine, len, wid);//设置边缘以便于处理边界
    setting(mine, len, wid);//手动布置雷盘
    figure(mine, len, wid);//计算每个格子周围雷数

    do {
        if ((win = win_lose(mine, len, wid, infor)))
            break;
        int did;//操作1——单击，2——标雷，3——标问，9——九击
        int x, y;//坐标
        scanf("%d%d%d", &did, &x, &y);

        getchar();
        x = x - 1;

        y = y - 1;
        switch (did) {
            case 1:
                Singleclick(mine, len, wid, x, y, &boom);
                figureMark(mine, len, wid);
                infor.step++;
                infor.restMine = figureTheRest(mine, len, wid);
                if (boom == 0)
                    break;
                if ((win = win_lose(mine, len, wid, infor)))
                    goto c;
                print_process(mine, len, wid, infor.step, infor.restMine);
                break;

            case 2:
                markMine(mine, len, wid, x, y);
                figureMark(mine, len, wid);
                infor.step++;
                infor.restMine = figureTheRest(mine, len, wid);
                if ((win = win_lose(mine, len, wid, infor)))
                    goto c;
                print_process(mine, len, wid, infor.step, infor.restMine);
                break;

            case 3:
                markQuestion(mine, len, wid, x, y);
                figureMark(mine, len, wid);
                infor.step++;
                infor.restMine = figureTheRest(mine, len, wid);
                if ((win = win_lose(mine, len, wid, infor)))
                    goto c;
                print_process(mine, len, wid, infor.step, infor.restMine);
                break;

            case 4:
                cancel(mine, len, wid, x, y);
                figureMark(mine, len, wid);
                infor.step++;
                infor.restMine = figureTheRest(mine, len, wid);
                if ((win = win_lose(mine, len, wid, infor)))
                    goto c;
                print_process(mine, len, wid, infor.step, infor.restMine);
                break;

            case 9:
                figureMark(mine, len, wid);
                Nineclick(mine, len, wid, x, y, &boom);
                infor.step++;
                infor.restMine = figureTheRest(mine, len, wid);
                if (boom == 0)
                    break;
                if ((win = win_lose(mine, len, wid, infor)))
                    goto c;
                print_process(mine, len, wid, infor.step, infor.restMine);
                break;
        }
    } while (boom);
    c:;
    if (boom == 0) {    //判断输赢
        printf("Hit mine, you lose\n");
        print_end(mine, len, wid, infor.step, infor.restMine);
    } else if (win == 1) {
        printf("Game over, you win\n");
        print_end(mine, len, wid, infor.step, infor.restMine);
    }

}
