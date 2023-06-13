#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// #include "panda.h"
#include "structure.h"
#include "vectorInt.h"
#define SEASIZE 2
#define MAPSIZE 4
const int ORDER[19] = {16, 17, 18, 15, 11, 6, 2, 1, 0, 3,
            7,  12, 13, 14, 10, 5, 4, 8, 9};
const int NUMBER[18] = {5, 2, 6,  3, 8, 10, 9, 12, 11,
            4, 8, 10, 9, 4, 5,  6, 3,  11};
// const int TEAMCOLOR[5] = {255, 9, 75, 82, 196};
extern int TEAMCOLOR[5];
extern char resourceStr[6][10];

static void initPlayer(player *p) {
    p->card = create_vector_vectorInt();
    for (int i = 1; i <= 5; ++i)
        p->resource[i] = 0;
    p->knight = p->road = p->Score = 0;
    p->haveNode = create_vector_vectorInt();
    p->haveSide = create_vector_vectorInt();
    p->havePort = create_vector_vectorInt();
    p->type = 0;
    }
static void freePlayer(player *p) {
    p->haveNode->free(p->haveNode);
    p->haveSide->free(p->haveSide);
    p->havePort->free(p->havePort);
    p->card->free(p->card);
    free(p);
}
static void shufflePlayer(player *p, int n, int times) {
    srand(time(NULL));
    for (int i = 0; i < times; ++i) {
        int a = rand() % n;
        int b = rand() % n;
        player tmp = p[a];
        p[a] = p[b];
        p[b] = tmp;
    }
}
static int dicePiece[13][2] = {0};
static int desertLoc = -1;
static node corner[54];
static side edge[72];
static port tradePort[9];
static player gamePlayer[6];
static piece land[19];
static int playerNumber = 0;
static int developCard[25];
void setUpGame() {
    while (1) {
        printf("how mamy player?");
        scanf("%d", &playerNumber);
        if (playerNumber <= 4 && playerNumber >= 2) {
            break;
        } else {
            printf("it shoud in [2,4]\n");
        }
    }
    for (int i = 0; i < 14; ++i)
        developCard[i] = KNIGHT;
    for (int i = 14; i < 20; i += 2)
        developCard[i] = developCard[i + 1] = (i >> 1) - 6;
    for (int i = 20; i < 25; ++i)
        developCard[i] = i - 16;
    shuffleInt(developCard, 25, 1000);
    for (int i = 2; i <= 12; ++i) {
        dicePiece[i][0] = -1;
        dicePiece[i][1] = -1;
    }
    for (int i = 0; i < 19; ++i) {
        initPiece(&(land[i]));
        if (i < 4)
            land[i].type = FOREST;
        else if (i < 8)
            land[i].type = ARABLE;
        else if (i < 12)
            land[i].type = FARM;
        else if (i < 15)
            land[i].type = HILL;
        else if (i < 18) {
            land[i].type = MINING;
        } else {
            land[i].type = DESERT;
        }
    }
    shuffle(land, 19, 1000);
    int cnt = 0;
    for (int i = 0; i < 19; ++i) {
        if (land[ORDER[i]].type == DESERT) {
            desertLoc = ORDER[i];
            land[ORDER[i]].number = 7;
            land[ORDER[i]].robber = 1;
        } else {
            if (dicePiece[NUMBER[cnt]][0] == -1)
            dicePiece[NUMBER[cnt]][0] = ORDER[i];
            else
            dicePiece[NUMBER[cnt]][1] = ORDER[i];
            land[ORDER[i]].number = NUMBER[cnt];
            ++cnt;
        }
    }
    // printf("test");
    for (int i = 0; i < 54; ++i) {
    // printf("%d ", i);
        initNode(&(corner[i]));
        corner[i].index = i;
    }
    for (int i = 0; i < 72; ++i) {
        initSide(&(edge[i]));
        edge[i].index = i;
    }
    for (int i = 0; i < 4; ++i) {
        tradePort[i].type = 0;
        tradePort[i].request = 3;
    }
    for (int i = 1; i <= 5; ++i) {
        tradePort[i + 3].type = i;
        tradePort[i + 4].request = 2;
    }
    shufflePort(tradePort, 9, 1000);
    corner[0].nearPort = &(tradePort[0]);
    corner[1].nearPort = &(tradePort[0]);
    corner[3].nearPort = &(tradePort[1]);
    corner[4].nearPort = &(tradePort[1]);
    corner[14].nearPort = &(tradePort[2]);
    corner[15].nearPort = &(tradePort[2]);
    corner[7].nearPort = &(tradePort[3]);
    corner[17].nearPort = &(tradePort[3]);
    corner[26].nearPort = &(tradePort[5]);
    corner[37].nearPort = &(tradePort[5]);
    corner[28].nearPort = &(tradePort[4]);
    corner[38].nearPort = &(tradePort[4]);
    corner[45].nearPort = &(tradePort[7]);
    corner[46].nearPort = &(tradePort[7]);
    corner[47].nearPort = &(tradePort[6]);
    corner[48].nearPort = &(tradePort[6]);
    corner[50].nearPort = &(tradePort[8]);
    corner[51].nearPort = &(tradePort[8]);
    initGame(land, corner, edge);
    for (int i = 0; i < playerNumber; ++i) {
        initPlayer(&(gamePlayer[i]));
        gamePlayer[i].type = i + 1;
    }
    shufflePlayer(gamePlayer, playerNumber, 100);
    int i, j;
    for (int k = 0; k < playerNumber; ++k) {
    while (1) {
        printMap(land, 19, tradePort, MAPSIZE, SEASIZE);
        printf("\e[38;5;%dmplayer %d\e[0m choose your first swttlement:",
            TEAMCOLOR[gamePlayer[k].type], gamePlayer[k].type);
        printf("(land,corner)\n"
            "0/  1 \\2\n"
            " |land| \n"
            "3\\  4 /5\n");
        scanf("%d%d", &i, &j);
        if (land[i].linkedNode[j]->belong == PUBLIC) {
            int check = 1;
            for (int l = 0; l < 3; ++l) {
                if (land[i].linkedNode[j]->linkedNode[l] != NULL &&
                    land[i].linkedNode[j]->linkedNode[l]->belong != PUBLIC) {
                    check = 0;
                    break;
                }
            }
            if (check) {
                land[i].linkedNode[j]->belong = gamePlayer[k].type;
                land[i].linkedNode[j]->type = SWTTLEMENT;

                break;
            } else {
                printf("there had swttlement near here, input again\n");
            }
            } else {
                printf("there had people here, input again\n");
            }
    }
    while (1) {
        printMap(land, 19, tradePort, MAPSIZE, SEASIZE);
        printf("\e[38;5;%dmplayer %d\e[0m choose your first road:",
            TEAMCOLOR[gamePlayer[k].type], gamePlayer[k].type);
        printf("(land,edge)\n"
            "0/    \\1\n"
            "2|land|3\n"
            "4\\    /5\n");
        scanf("%d%d", &i, &j);
        if (land[i].linkedSide[j]->belong == PUBLIC) {
            int check = 0;
        for (int l = 0; l < 2; ++l) {
            printf("%d %d\n", land[i].linkedSide[j]->linkedNode[l]->index,
                    land[i].linkedSide[j]->linkedNode[l]->belong);
            if (land[i].linkedSide[j]->linkedNode[l] != NULL &&
                land[i].linkedSide[j]->linkedNode[l]->belong ==
                    gamePlayer[k].type) {
                check = 1;
                break;
            }
        }
        if (check) {
            land[i].linkedSide[j]->belong = gamePlayer[k].type;
            land[i].linkedSide[j]->type = ROAD;

            break;
        } else {
            printf("there had no your swttlement near here, input "
                    "again\n");
        }
        } else {
            printf("there had people here, input again\n");
        }
    }
}
for (int k = playerNumber - 1; k >= 0; --k) {
    while (1) {
        printMap(land, 19, tradePort, MAPSIZE, SEASIZE);
        printf("\e[38;5;%dmplayer %d\e[0m choose your second swttlement:",
            TEAMCOLOR[gamePlayer[k].type], gamePlayer[k].type);
        printf("(land,corner)\n"
            "0/  1 \\2\n"
            " |land| \n"
            "3\\  4 /5\n");
        scanf("%d%d", &i, &j);
        if (land[i].linkedNode[j]->belong == PUBLIC) {
            int check = 1;
            for (int l = 0; l < 3; ++l) {
                if (land[i].linkedNode[j]->linkedNode[l] != NULL &&
                    land[i].linkedNode[j]->linkedNode[l]->belong != PUBLIC) {
                    check = 0;
                    break;
                }
            }
            if (check) {
                land[i].linkedNode[j]->belong = gamePlayer[k].type;
                land[i].linkedNode[j]->type = SWTTLEMENT;
                printf("you got:");
                for (int l = 0; l < 3; ++l) {
                    enum pieceType tmp = land[i].linkedNode[j]->linkedPiece[l]->type;
                    if (tmp != DESERT) {
                        printf(" %s", resourceStr[tmp]);
                        gamePlayer[k].resource[tmp]++;
                    }
                }
                printf("\n");
                break;
            } else {
                printf("there had swttlement near here, input again\n");
            }
            } else {
                printf("there had people here, input again\n");
            }
    }
    while (1) {
        printMap(land, 19, tradePort, MAPSIZE, SEASIZE);
        printf("\e[38;5;%dmplayer %d\e[0m choose your second road:",
            TEAMCOLOR[gamePlayer[k].type], gamePlayer[k].type);
        printf("(land,edge)\n"
            "0/    \\1\n"
            "2|land|3\n"
            "4\\    /5\n");
        scanf("%d%d", &i, &j);
        if (land[i].linkedSide[j]->belong == PUBLIC) {
            int check = 0;
            for (int l = 0; l < 2; ++l) {
                if (land[i].linkedSide[j]->linkedNode[l] != NULL &&
                    land[i].linkedSide[j]->linkedNode[l]->belong ==
                        gamePlayer[k].type) {
                    check = 1;
                    break;
                }
            }
            if (check) {
                land[i].linkedSide[j]->belong = gamePlayer[k].type;
                land[i].linkedSide[j]->type = ROAD;
                break;
            } else {
                printf("there had no your swttlement near here, input "
                        "again\n");
            }
            } else {
                printf("there had people here, input again\n");
            }
        }
    }
// printMap(land, 19, tradePort);
}
/*
void display_details(int gamer_idx) {
int s_count = 0;//SWTTLEMENT
printf("\e[38;5;%dmplayer %d \e[0m\n", TEAMCOLOR[gamePlayer[gamer_idx].type], gamer_idx+1);
printf("resources:");
for (int i = 1; i < 6; i++) {
printf("%d %s ", gamePlayer[gamer_idx].resource[i], resourceStr[i]);
}
printf("\n");
return;
}*/
int score_calculate(){
    return 1;
}
int main() {
    setUpGame();
    while(score_calculate()){
        for (int i = 0; i < playerNumber; ++i) {
            printf("\e[38;5;%dmplayer %d \e[0m\n", TEAMCOLOR[gamePlayer[gamer_idx].type], gamer_idx+1);
            int state = 0;
            int step = 0;
            int number;
            int haveK = 0;
            for (int j = 0; j < gamePlayer[i].card->size; ++i) {
                if (gamePlayer[i].card->get(gamePlayer[i].card, j) == KNIGHT) {
                    ++haveK;
                }
            }
            //display_details(i);
            while (1) {
                state = 0;
                printMap(land, 19, tradePort, MAPSIZE, SEASIZE);
                if (state == 0) { // doro
                    printf("1.roll dice\n");
                if (haveK) {
                    printf("2. use Knight Card\n");
                }
                    printf("your step:");
                scanf("%d", &step);

                if (step == 1) {
                    number = rollDice();
                    // DONT GIVE RESOURCE
                    if (number == 7) {
                        robber(i, gamePlayer, playerNumber, land);
                    } else {
                        giveResource(number, land, gamePlayer);
                    }
                    state = 1;
                } else if (step == 2 && haveK) {
                    robberK(i, gamePlayer, playerNumber, land);
                }
                }
                if (state == 1) {
                    gamePlayer[i].resource[0] = 10;
                    printf("1. trade\n");
                    printf("2. continue\n");
                    printf("your step:");
                    scanf("%d", &step);
                    if (step == 1)
                        trade(i, gamePlayer, tradePort);
                    if (step == 2)
                        state = 3;
                }
                if (state == 3) {
                    printf("1. continue next player\n");
                    scanf("%d", &step);
                    if (step == 1)
                        break;
                }
            }
        }
    }

}