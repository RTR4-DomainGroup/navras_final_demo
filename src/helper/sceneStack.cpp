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


static scene_t sceneStack[32];
static int head = 0;

void scenePush(scene_t scene)
{
    sceneStack[head] = scene;
    head++;
}

scene_t scenePop ()
{
    head--;
    return sceneStack[head];
}
