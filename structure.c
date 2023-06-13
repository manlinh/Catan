#include "structure.h"

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#include "bot.h"
#include "panda.h"
#include "vectorInt.h"
const int ORDER[19] = {16, 17, 18, 15, 11, 6, 2, 1, 0, 3,
                       7,  12, 13, 14, 10, 5, 4, 8, 9};
const int NUMBER[18] = {5, 2, 6,  3, 8, 10, 9, 12, 11,
                        4, 8, 10, 9, 4, 5,  6, 3,  11};
const int TEAMCOLOR[5] = {255, 93, 75, 82, 196};
const int PIECECOLOR[6] = {11, 28, 202, 145, 94, 237};
const int PORTCOLOR[6] = {241, 136, 255, 9, 226, 246};
const char PORTTEXT[6] = {'?', 'l', 'w', 'b', 'h', 'm'};
const char NODETYPE[3] = {' ', 's', 'c'};
const int NUMSHOW[10][7] = {
    {1, 1, 1, 0, 1, 1, 1}, {0, 0, 1, 0, 0, 1, 0}, {1, 0, 1, 1, 1, 0, 1},
    {1, 0, 1, 1, 0, 1, 1}, {0, 1, 1, 1, 0, 1, 0}, {1, 1, 0, 1, 1, 1, 1},
    {1, 1, 0, 1, 1, 1, 1}, {1, 0, 1, 0, 0, 1, 0}, {1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 1, 1},
};
const char resourceStr[6][10] = {"None", "wood",  "bricks",
                                 "wool", "wheat", "metal"};
const char cardtoString[10][16] = {
    "None",   "Knight",    "Monopoly", "Year Of Plenty", "Road Building",
    "Chapel", "Greathall", "Market",   "Library",        "University"};
int dicePiece[13][2] = {0};
int robberLoc = -1;
node corner[54];
side edge[72];
port tradePort[9];
player gamePlayer[6];
piece land[19];
int playerNumber = 0;
int developCard[25];
int nextdevelopCard = 0;
void initGame(piece *p, node *n, side *s) {
    // corner bind
    for (int i = 0; i < 19; ++i) {
        for (int j = 0; j < 3; ++j) {
            p[i].linkedNode[j] = &(n[(i << 1) + (i >= 3) + (i >= 7) +
                                     ((i >= 12) << 1) + (i >= 16) * 3 + j]);
            p[18 - i].linkedNode[j + 3] = &(n[51 -
                                              (i * 2 + (i >= 3) + (i >= 7) +
                                               (i >= 12) * 2 + (i >= 16) * 3) +
                                              j]);
        }
    }
    // side bind
    for (int i = 0; i < 19; ++i) {
        for (int j = 0; j < 2; ++j) {
            p[i].linkedSide[j] = &(s[i * 2 + (i >= 3) * 4 + (i >= 7) * 5 +
                                     (i >= 12) * 7 + (i >= 16) * 7 + j]);
            p[18 - i].linkedSide[j + 4] =
                &(s[70 -
                    (i * 2 + (i >= 3) * 4 + (i >= 7) * 5 + (i >= 12) * 7 +
                     (i >= 16) * 7) +
                    j]);
            p[i].linkedSide[j + 2] =
                &(s[i + j + 6 + (i >= 3) * 9 + (i >= 7) * 11 + (i >= 12) * 11 +
                    (i >= 16) * 9]);
        }
    }
    // bind node and node | side and side | node and piece
    for (int i = 0; i < 19; ++i) {
        p[i].linkedNode[0]->linkedNode[1] = p[i].linkedNode[1];
        p[i].linkedNode[0]->linkedNode[2] = p[i].linkedNode[3];
        p[i].linkedNode[1]->linkedNode[1] = p[i].linkedNode[0];
        p[i].linkedNode[1]->linkedNode[2] = p[i].linkedNode[2];
        p[i].linkedNode[2]->linkedNode[0] = p[i].linkedNode[1];
        p[i].linkedNode[2]->linkedNode[2] = p[i].linkedNode[5];
        p[i].linkedNode[3]->linkedNode[0] = p[i].linkedNode[0];
        p[i].linkedNode[3]->linkedNode[2] = p[i].linkedNode[4];
        p[i].linkedNode[4]->linkedNode[0] = p[i].linkedNode[3];
        p[i].linkedNode[4]->linkedNode[1] = p[i].linkedNode[5];
        p[i].linkedNode[5]->linkedNode[0] = p[i].linkedNode[2];
        p[i].linkedNode[5]->linkedNode[1] = p[i].linkedNode[4];
        p[i].linkedNode[0]->linkedSide[1] = p[i].linkedSide[0];
        p[i].linkedNode[0]->linkedSide[2] = p[i].linkedSide[2];
        p[i].linkedNode[1]->linkedSide[1] = p[i].linkedSide[0];
        p[i].linkedNode[1]->linkedSide[2] = p[i].linkedSide[1];
        p[i].linkedNode[2]->linkedSide[0] = p[i].linkedSide[1];
        p[i].linkedNode[2]->linkedSide[2] = p[i].linkedSide[3];
        p[i].linkedNode[3]->linkedSide[0] = p[i].linkedSide[2];
        p[i].linkedNode[3]->linkedSide[2] = p[i].linkedSide[4];
        p[i].linkedNode[4]->linkedSide[0] = p[i].linkedSide[4];
        p[i].linkedNode[4]->linkedSide[1] = p[i].linkedSide[5];
        p[i].linkedNode[5]->linkedSide[0] = p[i].linkedSide[3];
        p[i].linkedNode[5]->linkedSide[1] = p[i].linkedSide[5];
        p[i].linkedSide[0]->linkedNode[0] = p[i].linkedNode[1];
        p[i].linkedSide[0]->linkedNode[1] = p[i].linkedNode[0];
        p[i].linkedSide[1]->linkedNode[0] = p[i].linkedNode[1];
        p[i].linkedSide[1]->linkedNode[1] = p[i].linkedNode[2];
        p[i].linkedSide[2]->linkedNode[0] = p[i].linkedNode[0];
        p[i].linkedSide[2]->linkedNode[1] = p[i].linkedNode[3];
        p[i].linkedSide[3]->linkedNode[0] = p[i].linkedNode[2];
        p[i].linkedSide[3]->linkedNode[1] = p[i].linkedNode[5];
        p[i].linkedSide[4]->linkedNode[0] = p[i].linkedNode[3];
        p[i].linkedSide[4]->linkedNode[1] = p[i].linkedNode[4];
        p[i].linkedSide[5]->linkedNode[0] = p[i].linkedNode[5];
        p[i].linkedSide[5]->linkedNode[1] = p[i].linkedNode[4];

        for (int j = 0; j < 6; ++j) {
            p[i].linkedNode[j]->linkedPiece[2 - (int)(j / 2)] = &(p[i]);
        }
    }
}

void initPlayer(player *p) {
    p->card = create_vector_vectorInt();
    for (int i = 1; i <= 5; ++i) p->resource[i] = 0;
    p->knight = p->road = p->Score = 0;
    p->haveNode = create_vector_vectorInt();
    p->haveSide = create_vector_vectorInt();
    p->havePort = create_vector_vectorInt();
    p->type = 0;
}
void freePlayer(player *p) {
    p->haveNode->free(p->haveNode);
    p->haveSide->free(p->haveSide);
    p->havePort->free(p->havePort);
    p->card->free(p->card);
    free(p);
}
void shufflePlayer(player *p, int n, int times) {
    srand(time(NULL));
    for (int i = 0; i < times; ++i) {
        int a = rand() % n;
        int b = rand() % n;
        player tmp = p[a];
        p[a] = p[b];
        p[b] = tmp;
    }
}
void initNode(node *n) {
    for (int i = 0; i < 3; ++i) {
        n->linkedNode[i] = NULL;
        n->linkedPiece[i] = NULL;
        n->linkedPiece[i] = NULL;
    }
    n->nearPort = NULL;
    n->belong = 0;
    n->type = 0;
}
void initSide(side *n) {
    n->linkedNode[0] = NULL;
    n->linkedNode[1] = NULL;
    n->belong = 0;
    n->type = 0;
}
void initPiece(piece *n) {
    for (int i = 0; i < 6; ++i) {
        n->linkedNode[i] = NULL;
        n->linkedSide[i] = NULL;
    }
    n->robber = 0;
    n->type = 0;
    n->number = 7;
}
int rollDice() {
    srand(time(NULL));
    int a, b;
    a = rand() % 6 + 1;
    b = rand() % 6 + 1;
    printf("roll %d and %d => %d\n", a, b, a + b);
    return a + b;
}

void shuffle(piece *p, int n, int times) {
    srand(time(NULL));
    for (int i = 0; i < times; ++i) {
        int a = rand() % n;
        int b = rand() % n;
        piece tmp = p[a];
        p[a] = p[b];
        p[b] = tmp;
    }
}
void shufflePort(port *p, int n, int times) {
    srand(time(NULL));
    for (int i = 0; i < times; ++i) {
        int a = rand() % n;
        int b = rand() % n;
        port tmp = p[a];
        p[a] = p[b];
        p[b] = tmp;
    }
}
void shuffleInt(int *p, int n, int times) {
    srand(time(NULL));
    for (int i = 0; i < times; ++i) {
        int a = rand() % n;
        int b = rand() % n;
        int tmp = p[a];
        p[a] = p[b];
        p[b] = tmp;
    }
}
static char numberofPiece(int x, int y, int num) {
    // printf("%d %d:%d\n", x, y, num);
    if (x == 0) {
        if (y == 0) {
            return ' ';
        } else if (y == 1) {
            return NUMSHOW[num][0] ? '_' : ' ';
        } else {
            return ' ';
        }
    } else if (x == 1) {
        if (y == 0) {
            return NUMSHOW[num][1] ? '|' : ' ';

        } else if (y == 1) {
            return NUMSHOW[num][3] ? '_' : ' ';
        } else {
            return NUMSHOW[num][2] ? '|' : ' ';
        }
    } else {
        if (y == 0) {
            return NUMSHOW[num][4] ? '|' : ' ';
        } else if (y == 1) {
            return NUMSHOW[num][6] ? '_' : ' ';
        } else {
            return NUMSHOW[num][5] ? '|' : ' ';
        }
    }
}
static void printpart(const piece *p, int l, int r, int t, int tl, int size,
                      int space) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < space; ++j) printf("\e[48;5;17m \e[0m");
        for (int k = l; k <= r; ++k) {
            if (k == 0)
                for (int j = 0; j < (size)*3 * 2; ++j)
                    printf("\e[48;5;17m \e[0m");
            if (k == 3 || k == 16)
                for (int j = 0; j < (size)*3; ++j) printf("\e[48;5;17m \e[0m");
            if (k == 16 || k == 12) {
                if (i == 0) {
                    printf("\e[38;5;%dm%c\e[0m",
                           TEAMCOLOR[p[k].linkedNode[0]
                                         ->linkedPiece[0]
                                         ->linkedNode[3]
                                         ->belong],
                           NODETYPE[p[k].linkedNode[0]
                                        ->linkedPiece[0]
                                        ->linkedNode[3]
                                        ->type]);
                }
                for (int j = 0; j < 1 + i * 3 - (i == 0); ++j)
                    printf("\e[48;5;17m \e[0m");
                printf("\e[38;5;%dm＼\e[0m", TEAMCOLOR[p[k].linkedNode[0]
                                                           ->linkedPiece[0]
                                                           ->linkedSide[4]
                                                           ->belong]);
                for (int j = 0; j < (size - i - 1) * 3; ++j)
                    printf(
                        "\e[48;5;%dm \e[0m",
                        PIECECOLOR[p[k].linkedNode[0]->linkedPiece[0]->type]);
            }

            if (size - 1 == i)
                printf("\e[38;5;%dm%c\e[0m",
                       TEAMCOLOR[p[k].linkedNode[0]->belong],
                       NODETYPE[p[k].linkedNode[0]->type]);
            for (int j = 0; j < 1 + (size - i - 1) * 3 - (size - 1 == i); ++j) {
                if (p[k].linkedNode[1]->linkedPiece[0] != NULL) {
                    printf(
                        "\e[48;5;%dm \e[0m",
                        PIECECOLOR[p[k].linkedNode[0]->linkedPiece[0]->type]);
                } else
                    printf("\e[48;5;17m \e[0m");
            }
            printf("\e[38;5;%dm／\e[0m", TEAMCOLOR[p[k].linkedSide[0]->belong]);

            for (int j = 0; j < 1 + 2 * i * 3; ++j)
                if (i)
                    printf("\e[48;5;%dm \e[0m", PIECECOLOR[p[k].type]);
                else {
                    printf("\e[38;5;%dm%c\e[0m",
                           TEAMCOLOR[p[k].linkedNode[1]->belong],
                           NODETYPE[p[k].linkedNode[1]->type]);
                }
            printf("\e[38;5;%dm＼\e[0m", TEAMCOLOR[p[k].linkedSide[1]->belong]);
            if (size - 1 == i && k == r)
                printf("\e[38;5;%dm%c\e[0m",
                       TEAMCOLOR[p[k].linkedNode[2]->belong],
                       NODETYPE[p[k].linkedNode[2]->type]);
            for (int j = 0; j < (size - i - 1) * 3 - (size - 1 == i) + (k == r);
                 ++j)
                if (p[k].linkedNode[1]->linkedPiece[1] != NULL) {
                    printf(
                        "\e[48;5;%dm \e[0m",
                        PIECECOLOR[p[k].linkedNode[1]->linkedPiece[1]->type]);
                } else
                    printf("\e[48;5;17m \e[0m");
            if (k == 18 || k == 15) {
                for (int j = 0; j < (size - i - 1) * 3; ++j)
                    printf(
                        "\e[48;5;%dm \e[0m",
                        PIECECOLOR[p[k].linkedNode[2]->linkedPiece[0]->type]);
                printf("\e[38;5;%dm／\e[0m", TEAMCOLOR[p[k].linkedNode[2]
                                                           ->linkedPiece[0]
                                                           ->linkedSide[5]
                                                           ->belong]);
                if (i == 0) {
                    printf("\e[38;5;%dm%c\e[0m",
                           TEAMCOLOR[p[k].linkedNode[2]
                                         ->linkedPiece[0]
                                         ->linkedNode[5]
                                         ->belong],
                           NODETYPE[p[k].linkedNode[2]
                                        ->linkedPiece[0]
                                        ->linkedNode[5]
                                        ->type]);
                }
                for (int j = 0; j < 1 + i * 3 - (i == 0); ++j)
                    printf("\e[48;5;17m \e[0m");
            }
            if (k == r) {
                if (r == 2)
                    for (int j = 0; j < (size)*6; ++j)
                        printf("\e[48;5;17m \e[0m");
                if (r == 6 || r == 18)
                    for (int j = 0; j < (size)*3; ++j)
                        printf("\e[48;5;17m \e[0m");
                ;
            }
        }
        for (int j = 0; j < space; ++j) printf("\e[48;5;17m \e[0m");

        printf("\n");
    }
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < space; ++j) printf("\e[48;5;17m \e[0m");
        for (int k = l; k <= r; ++k) {
            if (k == 0 || k == 16)
                for (int j = 0; j < (size)*3 * 2; ++j)
                    printf("\e[48;5;17m \e[0m");
            if (k == 3 || k == 12)
                for (int j = 0; j < (size)*3; ++j) printf("\e[48;5;17m \e[0m");
            printf("\e[38;5;%dm|\e[0m", TEAMCOLOR[p[k].linkedSide[2]->belong]);

            for (int j = 0; j < 2 * (size)*3 - 1; ++j) {
                if (j > (size)*3 - 1 && j < (size)*3 + 3 && i < 3) {
                    printf("\e[48;5;%dm%c\e[0m", PIECECOLOR[p[k].type],
                           numberofPiece(i, j - size * 3, p[k].number / 10));
                } else if (j > (size)*3 + 3 && j < (size)*3 + 7 && i < 3) {
                    printf(
                        "\e[48;5;%dm%c\e[0m", PIECECOLOR[p[k].type],
                        numberofPiece(i, j - size * 3 - 4, p[k].number % 10));
                } else {
                    if (j < (size)*3 - 3 && j >= (size)*3 - 5 && i >= 1 &&
                        i <= 2 && p[k].robber) {
                        printf("\e[48;5;240m \e[0m");
                    } else {
                        printf("\e[48;5;%dm \e[0m", PIECECOLOR[p[k].type]);
                    }
                }
            }
            if (k == r) {
                printf("\e[38;5;%dm|\e[0m",
                       TEAMCOLOR[p[k].linkedSide[3]->belong]);
                if (r == 2 || r == 18)
                    for (int j = 0; j < (size)*6; ++j)
                        printf("\e[48;5;17m \e[0m");
                if (r == 6 || r == 15)
                    for (int j = 0; j < (size)*3; ++j)
                        printf("\e[48;5;17m \e[0m");
            }
        }
        for (int j = 0; j < space; ++j) printf("\e[48;5;17m \e[0m");

        printf("\n");
    }
    if (l == 16) {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < space; ++j) printf("\e[48;5;17m \e[0m");
            for (int j = 0; j < 6 * size; ++j) printf("\e[48;5;17m \e[0m");
            for (int k = l; k <= r; ++k) {
                if (i)
                    for (int j = 0; j < 3 * i + 1; ++j)
                        printf("\e[48;5;17m \e[0m");
                else {
                    printf("\e[38;5;%dm%c\e[0m",
                           TEAMCOLOR[p[k].linkedNode[3]->belong],
                           NODETYPE[p[k].linkedNode[3]->type]);
                }
                printf("\e[38;5;%dm＼\e[0m",
                       TEAMCOLOR[p[k].linkedSide[4]->belong]);
                if (i != size - 1)
                    for (int j = 0; j < (size - i - 1) * 6 + 1; ++j)
                        printf("\e[48;5;%dm \e[0m",
                               PIECECOLOR
                                   [p[k].linkedNode[4]->linkedPiece[0]->type]);
                else {
                    printf("\e[38;5;%dm%c\e[0m",
                           TEAMCOLOR[p[k].linkedNode[4]->belong],
                           NODETYPE[p[k].linkedNode[4]->type]);
                }
                printf("\e[38;5;%dm／\e[0m",
                       TEAMCOLOR[p[k].linkedSide[5]->belong]);
                // if (i)
                for (int j = 0; j < 3 * i; ++j) printf("\e[48;5;17m \e[0m");
            }
            if (i == 0)
                printf("\e[38;5;%dm%c\e[0m",
                       TEAMCOLOR[p[r].linkedNode[5]->belong],
                       NODETYPE[p[r].linkedNode[5]->type]);
            else
                printf("\e[48;5;17m \e[0m");
            if (r == 18)
                for (int j = 0; j < (size)*6; ++j) printf("\e[48;5;17m \e[0m");
            for (int j = 0; j < space; ++j) printf("\e[48;5;17m \e[0m");
            printf("\n");
        }
    }
}
void printMap(const piece *p, int n, const port *t, int size, int space) {
    printpart(p, 0, 2, 5, t[3].type, size, space);
    printpart(p, 3, 6, 3, t[2].type, size, space);
    printpart(p, 7, 11, 1, t[5].type, size, space);
    printpart(p, 12, 15, 3, t[4].type, size, space);
    printpart(p, 16, 18, 5, t[7].type, size, space);
}






void sswap(int *a, int *b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
    return;
}
void  random_Array(int *arr, int size){
    int *chosen_index = calloc(size, sizeof(int));
    srand(time(NULL));
    for(int i = 0; i < size; i++){
        int rnum = rand() % size;
        while(!chosen_index[rnum]){
            swap(arr[i], arr[rnum]);
            chosen_index[rnum] = 1;
        }
    }
    free(chosen_index);
    return;
}
void giveResource(int dice_number, piece *land, player *gamePlayer) {
  // number is two only one land
  if (dice_number == 2) {
    int the_land;
    for (int i = 0; i < 19; i++) {
      if (land[i].number == dice_number)
        the_land = i;
    }

    if (land[the_land].type == DESERT)
      return;
    for (int k = 0; k < 6; k++) {
      if (land[the_land].linkedNode[k]->belong == PUBLIC)
        continue;
      gamePlayer[land[the_land].linkedNode[k]->belong - 1]
          .resource[land[the_land].type] += 1;
    }
  } else { // number not two then 2 lands
    int the_land[2], idx = 0;
    for (int i = 0; i < 19; i++) {
      if (land[i].number == dice_number)
        the_land[idx++] = i;
    }
    for (int i = 0; i < 2; i++) {
      if (land[the_land[i]].type == DESERT)
        continue;
      for (int k = 0; k < 6; k++) {
        if (land[the_land[i]].linkedNode[k]->belong == PUBLIC)
          continue;
        gamePlayer[land[the_land[i]].linkedNode[k]->belong - 1]
            .resource[land[the_land[i]].type] += 1;
      }
    }
  }
  return;
}
void bot_discards_resources(int bot_player, player *gamePlayer, int playerNumber, piece *land){
    int *bot_random_resources;// an array with randomized resources of bot player e.g. 1 3 4 5 2 1 -> 2 wood .....
    int brr_idx = 0; // ^'s index
    int give_amount = 0, total_input = 0;
    int array_give[6]={0};
    for (int k = 1; k < 6; k++) {
      give_amount += gamePlayer[bot_player].resource[k];
    }
    if (give_amount <= 7) {
      return;
    }
    bot_random_resources = malloc(give_amount * sizeof(int));
  
    for (int k = 1; k < 6; k++) {
        for(int j = 0; j < gamePlayer[bot_player].resource[k]; j++){
            bot_random_resources[brr_idx++] = k;
        }
    }
    give_amount = (give_amount + (give_amount % 2)) / 2;
    random_Array(bot_random_resources, brr_idx);
    for(int i = 0; i < give_amount; i ++){
        int that_resource = bot_random_resources[i];
        array_give[that_resource] ++;
        gamePlayer[bot_player].resource[that_resource] -- ;
    }
    free(bot_random_resources);
    printf("\e[38;5;%dmplayer %d \e[0m please would %d resources to giveup, as follows\n", TEAMCOLOR[gamePlayer[bot_player].type], bot_player + 1, give_amount);
    printf("wood:%d bricks:%d wool:%d wheat:%d metal:%d\n",
           array_give[WOOD], array_give[BRICKS],
          array_give[WOOL], array_give[WHEAT],
           array_give[METAL]);

    return;
}
// bot is moving robber
void bot_choose_robber(int bot_player,piece *land, int robber_land,int desert_land, int *input_land){
    int is_self_here[20] = {0};
    int opponents_num_here[20]={0};
    is_self_here[19] = 0;
    opponents_num_here[19] = 0;
    for (int i = 0; i < 19; ++i) {
        if(i == desert_land || i == robber_land)
            continue;
        for (int j = 0; j < 6; ++j) {
            if (land[i].linkedNode[j]->belong != bot_player+1 &&
                land[i].linkedNode[j]->belong != PUBLIC) {
                opponents_num_here[i]++;
            } else if (land[i].linkedNode[j]->belong == bot_player) {
                is_self_here[i] = 1;
            }
        }
    }
    int best;
    int priority = 4;
    for(int i = 0; i < 19; i++){
        if(i == desert_land || i == robber_land)
            continue;
        if(is_self_here[i] == 0 && opponents_num_here[i] > 0){
            best = i;
            priority = 1;
            break;
        }
        if(is_self_here[i] == 0 && opponents_num_here[i] >0 && priority > 2){
            best = i;
            priority = 2;
        }
        if(is_self_here[i] == 0 && opponents_num_here[i] == 0 && priority > 3){
            best = i;
            priority =3;
        }
    }
    if(priority == 4){
        srand(time(NULL));
        while((*input_land = (rand() % 19))!= desert_land && *input_land != robber_land);
        return;
    }
    else
        *input_land = best;
    return;
}
void robber(int current_player, player *gamePlayer, int playerNumber, piece *land) {
  // give up resouces more than 7
  for (int i = 0; i < playerNumber; i++) {
    if(gamePlayer[i].bot){
        bot_discards_resources(i, gamePlayer, playerNumber, land);
        continue;
    }
    int give_amount = 0, total_input = 0;
    int array_give[6];
    for (int k = 1; k < 6; k++) {
      give_amount += gamePlayer[i].resource[k];
    }
    if (give_amount <= 7) {
      continue;
    }
    give_amount = (give_amount + (give_amount % 2)) / 2;
    printf("\e[38;5;%dmplayer %d \e[0m please choose %d resources to giveup\n", TEAMCOLOR[gamePlayer[i].type], i + 1, give_amount);
    printf("wood:%d bricks:%d wool:%d wheat:%d metal:%d\n",
           gamePlayer[i].resource[WOOD], gamePlayer[i].resource[BRICKS],
           gamePlayer[i].resource[WOOL], gamePlayer[i].resource[WHEAT],
           gamePlayer[i].resource[METAL]);
    int red_flag = 0;
    while (give_amount != total_input || red_flag) {
      red_flag = 0;
      printf("%d amount of resource should be given, how would you give?\n input:wood, bricks, wool, wheat, metal\n",
             give_amount);
      scanf(" %d %d %d %d %d", &array_give[WOOD], &array_give[BRICKS],
            &array_give[WOOL], &array_give[WHEAT], &array_give[METAL]);
      total_input = 0;
      for (int k = 1; k < 6; k++) {
        total_input += array_give[k];
        if (gamePlayer[i].resource[k] < array_give[k] || array_give[k] < 0) {
          red_flag = 1;
          printf("\e[4;31m wrong input! please input legally\e[0m\n");
          break;
        }
      }
      if (red_flag == 0) {
        if (total_input != give_amount)
          printf("\e[4;31mwrong input! giveup amount should be %d \e[0m\n", give_amount);
        else {
          for (int k = 1; k < 6; k++)
            gamePlayer[i].resource[k] -= array_give[k];
        }
      }
    }
  }
  // move robber
  int robber_land, desert_land, input_land;
  for (int i = 0; i < 19; i++) {
    if (land[i].robber == 1)
      robber_land = i;
    if (land[i].type == DESERT)
      desert_land = i;
  }
  int bool_success = 0;
    if(gamePlayer[current_player].bot){
        bot_choose_robber(current_player, land, robber_land, desert_land, &input_land);
        bool_success = 1;
    }
  
  while (!bool_success) {
    printf("which land would you move the robber to ? :");
    scanf(" %d", &input_land);
    if (input_land > 18 || input_land < 0) {
      continue;
    }
    if (input_land == robber_land || input_land == desert_land)
      printf("\e[4;31mwrong input! Desert is %d, original robber at %d\e[0m\n", desert_land,
             robber_land);
    else
      bool_success = 1;
  }
  land[input_land].robber = 1;
  land[robber_land].robber = 0;
  robber_land = input_land;
  //
  // steal resource
  int array_bool_player[4] = {0};
  int array_player_total[4] = {0};
  int bool_steal_able = 0;
  int steal_from; // 0 - 3
  int bot_steal_from, most_stealable = 0;
  for (int i = 0; i < 6; i++) {
    if (land[robber_land].linkedNode[i]->belong != PUBLIC)
      array_bool_player[land[robber_land].linkedNode[i]->belong - 1] = 1;
  }
  array_bool_player[current_player] = 0;
  for (int i = 0; i < 4; i++) {
    if (array_bool_player[i] == 1) {// if this player i index is near the new robber land
      for (int k = 1; k < 6; k++) {
        array_player_total[i] += gamePlayer[i].resource[k];
      }
      if (array_player_total[i]) {
        printf("\e[38;5;%dmplayer %d \e[0mhas %d resources", TEAMCOLOR[gamePlayer[i].type], i + 1, array_player_total[i]);
        //find most resources among steable player for bot to steal
        if(most_stealable < array_player_total[i]){
            most_stealable = array_player_total[i];
            bot_steal_from = i;
            }
        bool_steal_able = 1;// 稍後若機器人，則選這個i偷
      } else
        array_bool_player[i] = 0; // 原本是候選人，但無資源所以移除
    }
  }
  
  int bool_select = 0;
  if(gamePlayer[current_player].bot && bool_steal_able){
    bool_select = 1;
    steal_from = bot_steal_from;
  }
  while (!bool_select && bool_steal_able) {
    printf(", choose one player to select their resources:");
    scanf(" %d", &steal_from); // input 1 - 4
    steal_from--;              // 0 - 3
    if (steal_from >= playerNumber || steal_from < 0) {
      printf("\e[4;31m wrong input! Input illegal: out of player range\e[0m\n");
      continue;
    }
    if (steal_from == current_player) {
      printf("\e[4;31mwrong input! Input illegal: you chose Player %d yourself\e[0m\n",
             current_player);
      continue;
    }
    if (array_bool_player[steal_from] == 0) {
      printf("\e[4;31mwrong input! Input illegal: player not available\e[0m\n");
    }
    bool_select = 1;
  }
  // get resource
  if (bool_select) {
    srand(time(NULL));
    int r = rand() % array_player_total[steal_from] + 1;
    int tmp = 0;
    while (r > 0) {
      tmp++;
      r -= gamePlayer[steal_from].resource[tmp];
    }
    gamePlayer[steal_from].resource[tmp] -= 1;
    gamePlayer[current_player].resource[tmp] += 1;
    printf("\e[38;5;%dmplayer %d \e[0m steal %s from \e[38;5;%dmplayer %d \e[0m \n", TEAMCOLOR[gamePlayer[current_player].type], current_player + 1,
           resourceStr[tmp], TEAMCOLOR[gamePlayer[steal_from].type], steal_from + 1);
  } else {
    printf("no stealing\n");
  }
}
void trade(int current_palyer, player *gamePlayer, port *tradePort) {
  int give, get, give_amount;
  int best_trade_int[6] = {0, 4, 4, 4, 4, 4};

  // print resource
  printf("wood:%d bricks:%d wool:%d wheat:%d metal:%d\n",
         gamePlayer[current_palyer].resource[WOOD], gamePlayer[current_palyer].resource[BRICKS],
         gamePlayer[current_palyer].resource[WOOL], gamePlayer[current_palyer].resource[WHEAT],
         gamePlayer[current_palyer].resource[METAL]);
  // find the best trade among bank and port
  for (int port_idx = 0; port_idx < gamePlayer[current_palyer].havePort->size; port_idx++) {
    for (int resource_idx = 1; resource_idx < 6; resource_idx++) {
      int port_code =
          gamePlayer[current_palyer].havePort->get(gamePlayer[current_palyer].havePort, port_idx);
      if (tradePort[port_code].type == resource_idx ||
          tradePort[port_code].type == 0) {
        if (best_trade_int[resource_idx] > tradePort[port_code].request)
          best_trade_int[resource_idx] = tradePort[port_code].request;
      }
    }
  }

  // print best options
  printf("\e[38;5;%dmplayer %d \e[0m", TEAMCOLOR[gamePlayer[current_palyer].type], current_palyer+1);
  printf("can give %d %s or %d %s or %d %s or %d %s or %d %s or for one "
         "resource (1. wood 2. bricks 3. wool 4. wheat 5. metal):\n give, get",
         best_trade_int[1], resourceStr[1], best_trade_int[2], resourceStr[2],
         best_trade_int[3], resourceStr[3], best_trade_int[4], resourceStr[4],
         best_trade_int[5], resourceStr[5]);
  // choose resouce to trade
  scanf("%d %d", &give, &get);
  if (give > 5 || give < 1 || get > 5 || get < 1) {
    printf("\e[4;31m wrong input, try again\e[0m\n");
    return;
  }

  // check player resource amount
  if (gamePlayer[current_palyer].resource[give] < best_trade_int[give]) {
    printf("\e[4;31m not enough resources, try again\e[0m\n");
    return;
  }
  gamePlayer[current_palyer].resource[get] += 1;
  gamePlayer[current_palyer].resource[give] -= best_trade_int[give];
  return;
}

int score_calculate(){
    return 1;
}


void knight_king(player *gamePlayer, int current_player, int playerNumber, int *knight_owner){
  ++gamePlayer[current_player].knight;
  int more_knight_out = 0;

  for(int j = 0; j < playerNumber; ++j){
    if(gamePlayer[current_player].knight > gamePlayer[j].knight){
      ++more_knight_out;
    }
  }

  //printf("player%d's outknight is more than %d player(s)\n", i+1, more_knight_out);
  if(more_knight_out == playerNumber - 1 && gamePlayer[current_player].knight >= 3){
    *knight_owner = current_player+1;
    printf("Knight owner is player %d.\n",*knight_owner);
    return ;
  }
  return;
}
void bot_robberK(int bot_player, player *gamePlayer, int playerNumber, piece *land){
    int robber_land, desert_land, input_land;
    for (int i = 0; i < 19; i++) {
        if (land[i].robber == 1){
          robber_land = i;
        }
        if (land[i].type == DESERT){
          desert_land = i;
        }
    }
    bot_choose_robber(bot_player,land, robber_land, desert_land, &input_land);
    
    land[input_land].robber = 1;
    land[robber_land].robber = 0;
    robber_land = input_land;
    int array_bool_player[4] = {0};
    int array_player_total[4] = {0};
    int bool_steal_able = 0;
    int bot_steal_from, most_stealable = 0;
    for (int i = 0; i < 6; i++) {
        if (land[robber_land].linkedNode[i]->belong != PUBLIC)
            array_bool_player[land[robber_land].linkedNode[i]->belong - 1] = 1;
    }
    array_bool_player[bot_player] = 0;
  for (int i = 0; i < 4; i++) {
    if (array_bool_player[i] == 1) {// if this player i index is near the new robber land
      for (int k = 1; k < 6; k++) {
        array_player_total[i] += gamePlayer[i].resource[k];
      }
      if (array_player_total[i]) {
        printf("\e[38;5;%dmplayer %d \e[0mhas %d resources", TEAMCOLOR[gamePlayer[i].type], i + 1, array_player_total[i]);
        //find most resources among steable player for bot to steal
        if(most_stealable < array_player_total[i]){
            most_stealable = array_player_total[i];
            bot_steal_from = i;
            }
        bool_steal_able = 1;// 稍後若機器人，則選這個i偷
      } else
        array_bool_player[i] = 0; // 原本是候選人，但無資源所以移除
    }
  }
  
 
  // get resource
    if (bool_steal_able) {
        srand(time(NULL));
        int r = rand() % array_player_total[bot_steal_from] + 1;
        int tmp = 0;
        while (r > 0) {
            tmp++;
            r -= gamePlayer[bot_steal_from].resource[tmp];
        }
        gamePlayer[bot_steal_from].resource[tmp] -= 1;
        gamePlayer[bot_player].resource[tmp] += 1;
        printf("\e[38;5;%dmplayer %d \e[0m steal %s from \e[38;5;%dmplayer %d \e[0m \n", TEAMCOLOR[gamePlayer[bot_player].type], bot_player + 1,
            resourceStr[tmp], TEAMCOLOR[gamePlayer[bot_steal_from].type], bot_steal_from + 1);
  } else {
    printf("no stealing\n");
  }
    return ;
}
void robberK(int current_player, player** gamePlayer, int playerNumber,
             piece** land) {
   // move robber
  int robber_land, desert_land, input_land;
  for (int i = 0; i < 19; i++) {
    if (land[i]->robber == 1){
      robber_land = i;
    }
    if (land[i]->type == DESERT){
      desert_land = i;
    }
  }
  int bool_success = 0;
  while (bool_success == 0) {
    printf("which land would you move the robber to ? :");
    scanf(" %d", &input_land);
    if (input_land > 18 || input_land < 0) {
      continue;
    }
    if (input_land == robber_land || input_land == desert_land)
      printf("\e[4;31mwrong input! Desert is %d, original robber at %d\e[0m\n", desert_land,
             robber_land);
    else
      bool_success = 1;
  }
  land[input_land]->robber = 1;
  land[robber_land]->robber = 0;
  robber_land = input_land;


  // steal resource

  int array_bool_player[4] = {0};
  int array_player_total[4] = {0};
  int bool_steal_able = 0;
  int steal_from; // 0 - 3
  for (int i = 0; i < 6; i++) {
    if (land[robber_land]->linkedNode[i]->belong != PUBLIC)
      array_bool_player[land[robber_land]->linkedNode[i]->belong - 1] = 1;
  }
  array_bool_player[current_player] = 0;
  for (int i = 0; i < 4; i++) {
    if (array_bool_player[i] == 1) {
      for (int k = 1; k < 6; k++) {
        array_player_total[i] += gamePlayer[i]->resource[k];
      }
      if (array_player_total[i]) {
        printf("\e[38;5;%dmplayer %d \e[0mhas %d resources", TEAMCOLOR[gamePlayer[i]->type], i + 1, array_player_total[i]);
        bool_steal_able = 1;
      } else{
        array_bool_player[i] = 0; // 原本是候選人，但無資源所以移除
      }
    }
  }
  int bool_select = 0;
  while (!bool_select && bool_steal_able) {
    printf(", choose one player to select their resources:");
    scanf(" %d", &steal_from); // input 1 - 4
    steal_from--;              // 0 - 3
    if (steal_from >= playerNumber || steal_from < 0) {
      printf("\e[4;31m wrong input! Input illegal: out of player range\e[0m\n");
      continue;
    }
    if (steal_from == current_player) {
      printf("\e[4;31mwrong input! Input illegal: you chose Player %d yourself\e[0m\n",
             current_player);
      continue;
    }
    if (array_bool_player[steal_from] == 0) {
      printf("\e[4;31mwrong input! Input illegal: player not available\e[0m\n");
    }
    bool_select = 1;
  }
  // get resource
  if (bool_select) {
    srand(time(NULL));
    int r = rand() % array_player_total[steal_from] + 1;
    int tmp = 0;
    while (r > 0) {
      tmp++;
      r -= gamePlayer[steal_from]->resource[tmp];
    }
    gamePlayer[steal_from]->resource[tmp] -= 1;
    gamePlayer[current_player]->resource[tmp] += 1;
    printf("\e[38;5;%dmplayer %d \e[0m steal %s from \e[38;5;%dmplayer %d \e[0m \n", TEAMCOLOR[gamePlayer[current_player]->type], current_player + 1,
           resourceStr[tmp], TEAMCOLOR[gamePlayer[steal_from]->type], steal_from + 1);
  } else {
    printf("no stealing\n");
  }

}