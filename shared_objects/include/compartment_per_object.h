#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Car_priv
{
    int maxSpeed;
    void (*crash)();
};

struct Car
{
    int speed;
    void (*honk)();
    char name[];
};

void init();

struct Car *new_car();
