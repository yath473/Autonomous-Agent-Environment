#include "raylib.h"
#include "raymath.h"
#include <random>
#include <iostream>
#include <cmath>
#include <vector> 

struct StopSign {
    Vector2 pos;
    bool isActive;
};
struct Car {
    Vector2 car;
    Vector2 currentV;
    Vector2 desiredV;
    Vector2 tPoint;
    Vector2 steering;
    float size;
    float currentRot;
    Color carColor;
    int count;
    float maxV;
};
void Movement(Texture2D carTexture, Texture2D targTexture, float radius, float wTimer, float wInterval);


