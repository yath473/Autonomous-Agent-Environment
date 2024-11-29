#include "raylib.h"
#include "raymath.h"
#include <random>
#include <iostream>
#include <cmath>
#include "Agent1.h"

using namespace std;


int main(void)
{
    InitWindow(1500, 850, "raylib [core] example - basic window");

    Image carImage = LoadImage("car.png");
    Image targets = LoadImage("stopsign.png");

    Texture2D carTexture = LoadTextureFromImage(carImage);
    Texture2D targTexture = LoadTextureFromImage(targets);

    UnloadImage(carImage);
    UnloadImage(targets);

    std::vector<Texture2D> player;
    player.push_back(carTexture);

    std::vector<Texture2D> target;
    target.push_back(targTexture);


    Movement(carTexture, targTexture, 100.0f, 0.0f, 10.0f);

    UnloadTexture(carTexture);
    UnloadTexture(targTexture);

    CloseWindow();

    return 0;
}