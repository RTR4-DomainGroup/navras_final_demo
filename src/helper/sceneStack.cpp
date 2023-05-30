/**
 * @file SinglyLinkedList.c
 * @author Prashant (adlinge.pa@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-03-05
 *
 * @copyright Copyright (c) 2022
 *
 */

 // Header files
#include <stdio.h>   // for File IO functions
#include <stdlib.h>  // for exit()
#include "../../inc/helper/sceneStack.h"
#include "../../inc/helper/common.h"


static scene_types_t sceneStack[MAX_SCENES];
static int head = 0;
static int curr = 0;

void scenePush(scene_types_t scene)
{
    if(head < (int)MAX_SCENES) {    
        sceneStack[head] = scene;

        head++;
        LOG("scene stack Head: %d !!!\n", head);
        curr = head;
    }
    else {
        LOG("scene stack overflow: %d !!!\n", head);
    }
}

scene_types_t scenePop ()
{
    if((head-1) >= 0) {
        head--;
        LOG("scene stack Head: %d !!!\n", head);
        curr = head;
        sceneStack[head+1] = SCENE_INVALID;
    }
    else {
        LOG("scene stack underflow: %d !!!\n", head);
    }
    return sceneStack[head];
}


scene_types_t sceneNext() 
{
    return sceneStack[((curr - 1) >= 0) ? --curr : curr];
}

scene_types_t scenePrev() 
{
    return sceneStack[((curr + 1) <= head) ? ++curr : curr];
}

