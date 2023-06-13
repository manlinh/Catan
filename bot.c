#include "bot.h"

#include <math.h>

#include "structure.h"
extern node corner[54];
extern side edge[72];
extern int nextdevelopCard;
//just to simply run
int botOption(int state, player *Players, int index, piece *lands, int haveK) {
    if (state == 0 && haveK) {
        printf("step 2\n");
        return 2;
    }
    else if(state == 0){
        printf("step 1\n");
        return 1;
    }
    else if(state == 1){
        printf("step 2\n");
        return 2;
    }
    else if(state == 2){
        printf("step 1\n");
        return 1;
    }
    
}

int botRobber(piece *land, int playerID) {
    int bestID = -1;
    double bestWeights = -1;
    for (int i = 0; i < 19; ++i) {
        double nowWeights = 0;
        int peoplenearhere = 0;
        for (int j = 0; j < 6; ++j) {
            //如果這塊land至少一個點是public，那peoplenearhere = 0，所以peoplenearhere max =1
            if (land[i].linkedNode[j]->belong != playerID &&
                land[i].linkedNode[j]->belong != PUBLIC) {
                peoplenearhere++;
            } else if (land[i].linkedNode[j]->belong != playerID) {
                peoplenearhere = 0;
                break;
            }
        }
        //那這裡又如何sqrt(peoplenearhere==1)，想大概問這個97行
        nowWeights = (6 - abs(land[i].number - 7) *
                              log(10 * (land[i].type == DESERT ? 0.1 : 1)) *
                              sqrt(peoplenearhere));
        if (nowWeights > bestWeights) {
            bestID = i;
            bestWeights = nowWeights;
        }
    }
    return bestID;
}