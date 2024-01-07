#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ogcsys.h>
#include <gccore.h>
#include <wiiuse/wpad.h>

typedef struct {
    u8 exists;
    float leftX, leftY, rightX, rightY;
    u8 square, cross, circle, triangle, l1, r1, l2, r2, select, start, up, down, left, right;
} ds4_state;

ds4_state get_ds4_state();

void start_ds4();
void update_ds4();