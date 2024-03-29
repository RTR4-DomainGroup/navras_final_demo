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
static int head = -1;
static int curr = -1;

void scenePush(scene_types_t scene)
{
    if(head < (int)MAX_SCENES) {    
        sceneStack[++head] = scene;
        LOG("scene %d pushed, stack Head: %d !!!\n", scene, head);
        curr = head;
    }
    else {
        LOG("scene stack overflow: %d !!!\n", head);
    }
}

scene_types_t scenePop ()
{
    scene_types_t scene = SCENE_INVALID;
    if(head >= 0) {
        scene = sceneStack[head];
        LOG("scene %d poped, stack Head: %d !!!\n", scene, head);
        sceneStack[head] = SCENE_INVALID;
        curr = --head;
    }
    else {
        LOG("scene stack underflow: %d !!!\n", head);
    }
    return scene;
}


scene_types_t sceneNext() 
{
    return sceneStack[((curr - 1) >= 0) ? --curr : curr];
}

scene_types_t scenePrev() 
{
    return sceneStack[((curr + 1) <= head) ? ++curr : curr];
}

