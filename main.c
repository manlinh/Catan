#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// #include "panda.h"
#include "bot.h"
#include "structure.h"
#include "vectorInt.h"
#define SEASIZE 2
#define MAPSIZE 4
extern const int ORDER[19];
extern const int NUMBER[18];
// const int TEAMCOLOR[5] = {255, 9, 75, 82, 196};
extern int TEAMCOLOR[5];
extern char resourceStr[6][10];

extern int dicePiece[13][2];
extern int robberLoc;
extern node corner[54];
extern side edge[72];
extern port tradePort[9];
extern player gamePlayer[6];
extern piece land[19];
extern int playerNumber;
extern int developCard[25];
extern int nextdevelopCard;
void setUpGame() {
    while (1) {
        printf("how mamy player?");
        scanf("%d", &playerNumber);
        if (playerNumber <= 4 && playerNumber >= 2) {
            break;
        }
        printf("it shoud in [2,4]\n");
    }
    for (int i = 0; i < 14; ++i) developCard[i] = KNIGHT;
    for (int i = 14; i < 20; i += 2)
        developCard[i] = developCard[i + 1] = (i >> 1) - 6;
    for (int i = 20; i < 25; ++i) developCard[i] = i - 16;
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
            robberLoc = ORDER[i];
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
            printf(
                "(land,corner)\n"
                "0/  1 \\2\n"
                " |land| \n"
                "3\\  4 /5\n");
            scanf("%d%d", &i, &j);
            if (land[i].linkedNode[j]->belong == PUBLIC) {
                int check = 1;
                for (int l = 0; l < 3; ++l) {
                    if (land[i].linkedNode[j]->linkedNode[l] != NULL &&
                        land[i].linkedNode[j]->linkedNode[l]->belong !=
                            PUBLIC) {
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
            printf(
                "(land,edge)\n"
                "0/    \\1\n"
                "2|land|3\n"
                "4\\    /5\n");
            scanf("%d%d", &i, &j);
            if (land[i].linkedSide[j]->belong == PUBLIC) {
                int check = 0;
                for (int l = 0; l < 2; ++l) {
                    // printf("%d %d\n",
                    //        land[i].linkedSide[j]->linkedNode[l]->index,
                    //        land[i].linkedSide[j]->linkedNode[l]->belong);
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
                    printf(
                        "there had no your swttlement near here, input "
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
            printf(
                "(land,corner)\n"
                "0/  1 \\2\n"
                " |land| \n"
                "3\\  4 /5\n");
            scanf("%d%d", &i, &j);
            if (land[i].linkedNode[j]->belong == PUBLIC) {
                int check = 1;
                for (int l = 0; l < 3; ++l) {
                    if (land[i].linkedNode[j]->linkedNode[l] != NULL &&
                        land[i].linkedNode[j]->linkedNode[l]->belong !=
                            PUBLIC) {
                        check = 0;
                        break;
                    }
                }
                if (check) {
                    land[i].linkedNode[j]->belong = gamePlayer[k].type;
                    land[i].linkedNode[j]->type = SWTTLEMENT;
                    printf("you got:");
                    for (int l = 0; l < 3; ++l) {
                        enum pieceType tmp =
                            land[i].linkedNode[j]->linkedPiece[l]->type;
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
            printf(
                "(land,edge)\n"
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
                    printf(
                        "there had no your swttlement near here, input "
                        "again\n");
                }
            } else {
                printf("there had people here, input again\n");
            }
        }
    }
    // printMap(land, 19, tradePort);
}
int main() {
    setUpGame();
    gamePlayer[0].bot = 1;
    int knight_owner = 0;
    
    while(score_calculate()){
        for (int i = 0; i < playerNumber; ++i) {
            int state = 0;
            int step = 0;
            int number;
            int haveK = 0;
            gamePlayer[i].card->insert(gamePlayer[i].card, gamePlayer[i].card->size, KNIGHT);
            for (int j = 0; j < gamePlayer[i].card->size; ++j) {
                if (gamePlayer[i].card->get(gamePlayer[i].card, j) == KNIGHT) {
                    ++haveK;
                } 
            }
            //display_details(i);
            while (1) {
                state = 0;
                printMap(land, 19, tradePort, MAPSIZE, SEASIZE);
                printf("\e[38;5;%dmplayer %d \e[0m\n", TEAMCOLOR[gamePlayer[i].type], i+1);
                while (state == 0) { // doro
                    printf("1.roll dice\n");
                    if (haveK) {
                        printf("2. use Knight Card\n");
                    }
                    printf("your step:");
                    if (gamePlayer[i].bot)
                        step = botOption(state, gamePlayer, i, land, haveK);
                    else
                        scanf("%d", &step);

                    if (step == 1) {
                        number = rollDice();
                        if (number == 7) {
                            robber(i,  playerNumber);
                        } else {
                            giveResource(number );
                        }
                        state = 1;
                    } else if (step == 2 && haveK) {
                        haveK = 0;
                        for (int j = 0; j < gamePlayer[i].card->size; ++j) {
                            if (gamePlayer[i].card->get(gamePlayer[i].card, j) == KNIGHT) {
                                gamePlayer[i].card->remove(gamePlayer[i].card, j);
                                break;
                            } 
                        }
                        if(gamePlayer[i].bot){
                            printf("in\n");
                                bot_robberK(i, playerNumber);          
                        }
                        else{
                            printf("in\n");
                            robberK(i, playerNumber);
                        }
                        //knight_king(gamePlayer, i, playerNumber, &knight_owner);
                    }
                }
                while(state == 1) {    
                    gamePlayer[i].resource[0] = 10;
                    printf("1. trade\n");
                    printf("2. continue\n");
                    printf("your step:");
                    if(gamePlayer[i].bot)
                        step = botOption(state, gamePlayer, i, land, haveK);
                    else
                        scanf(" %d", &step);
                    if (step == 1)
                        trade(i);
                    if (step == 2)
                        state = 2;
                }
                if (state == 2) {
                    printf("1. continue next player\n");
                    if(gamePlayer[i].bot)
                        step = botOption(state, gamePlayer, i, land, haveK);
                    else
                        scanf("%d", &step);
                    if (step == 1)
                        break;
                }
            }
        }
    }


}