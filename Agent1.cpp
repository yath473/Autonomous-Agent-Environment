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
void Movement(Texture2D carTexture, Texture2D targTexture, float radius, float wTimer, float wInterval) {
    SetTargetFPS(30);
    
    int sizex = (float)carTexture.width;
    int sizey = (float)carTexture.height;

    float elapsed = 0.0f;
    float maxInterval = 20.0f;

    float rotSpeed = 90.0f;
    float angle = 0.0f;

    Color textCol = WHITE;

    std::vector<Car> cars;
    int carcount = 5;
    for (int i = 0; i < carcount; i++) {
        int playerx = rand() % (GetScreenWidth() - carTexture.width);
        int playery = rand() % (GetScreenHeight() - carTexture.height);

        Color randomColor = {
            (unsigned char)GetRandomValue(0, 255),
            (unsigned char)GetRandomValue(0, 255),
            (unsigned char)GetRandomValue(0, 255),
            255 
        };

        cars.push_back({ { (float)playerx, (float)playery }, { 100.0f, 100.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, 50.0f, 0.0f, randomColor, 0, 50.0f});
    }
    std::vector<StopSign> stopSigns;
    
    auto spawnStopSigns = [&]() {
        stopSigns.clear();
        int signs = 50;
        for (int i = 0; i < signs; i++) {
            int targx = rand() % (GetScreenWidth() - targTexture.width);
            int targy = rand() % (GetScreenHeight() - targTexture.height);

            stopSigns.push_back({ { (float)targx, (float)targy }, true });
        }
    };
    spawnStopSigns();

    for (Car& c : cars) {
        c.tPoint = { c.car.x, c.car.y };
    }

    bool atTarget = false;
    bool reached = false;
    bool isActive = true;

    bool stopsignspwaned = false;

    float countdownTimer = 0.0f;

    float steeringForce = 200.0f;

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        elapsed += deltaTime;
        wTimer += deltaTime;
        for (Car& c : cars) {  
            float edgeMargin = 30.0f;

            c.car.x = Clamp(c.car.x, edgeMargin, GetScreenWidth() - edgeMargin);
            c.car.y = Clamp(c.car.y, edgeMargin, GetScreenHeight() - edgeMargin);

            if (wTimer > wInterval) {

                float rando = GetRandomValue(0, 360) * DEG2RAD;
                c.tPoint.x = c.car.x + cos(rando) * radius;
                c.tPoint.y = c.car.y + sin(rando) * radius;

                if (Vector2Distance(c.car, c.tPoint) < 50.0f) {
                    c.tPoint.x += 50.0f;
                    c.tPoint.y += 50.0f;
                }

                c.tPoint.x = Clamp(c.tPoint.x, 50.0f, 1450.0f);
                c.tPoint.y = Clamp(c.tPoint.y, 50.0f, 800.0f);

                wTimer = 0.0f;
            }

            Vector2 sub = Vector2Subtract(c.tPoint, c.car);
            float dist = Vector2Length(sub);

            if (!reached && dist > 5.0f) {

                c.desiredV = Vector2Scale(Vector2Normalize(sub), c.maxV);

                c.steering = Vector2Subtract(c.desiredV, c.currentV);

                c.steering = Vector2Scale(c.steering, 0.1f);
                c.steering = Vector2Scale(Vector2Normalize(c.steering), steeringForce);

                c.currentV = Vector2Add(c.currentV, c.steering);

                if (Vector2Length(c.currentV) > c.maxV) {
                    c.currentV = Vector2Scale(Vector2Normalize(c.currentV), c.maxV);
                }

                angle = atan2(c.desiredV.y, c.desiredV.x) * RAD2DEG;

                float angleDiff = angle - c.currentRot;

                if (angleDiff > 180.0f) {
                    angleDiff -= 360.0f;
                }
                else if (angleDiff < -180.0f) {
                    angleDiff += 360.0f;
                }

                float rotFact = 0.0f;

                if (angleDiff > 0) {
                    rotFact = 1.0f;
                }
                else {
                    rotFact = -1.0f;
                }

                if (fabs(angleDiff) > 2.0f) {
                    c.currentRot += rotSpeed * deltaTime * rotFact;
                }

            }

            Vector2 direction = { cos(c.currentRot * DEG2RAD), sin(c.currentRot * DEG2RAD) };

            for (StopSign& stopSign : stopSigns) {
                float distance = Vector2Distance(c.car, stopSign.pos);

                if (radius >= distance) {
                    c.count++;

                    Vector2 directionToCar = Vector2Normalize(Vector2Subtract(c.car, stopSign.pos));

                    float stopSpeed = 100.0f;
                    stopSign.pos = Vector2Add(stopSign.pos, Vector2Scale(directionToCar, stopSpeed * deltaTime));
                    

                    if (distance <= 5.0f) {
                        stopSign.isActive = false;
                        isActive = false;
                        c.size += 0.02f;
                        radius += 0.01f;  
                        c.maxV -= 0.01f;
                    }
                }
            }
            c.car = Vector2Add(c.car, Vector2Scale(direction, Vector2Length(c.currentV) * deltaTime));
        }

            BeginDrawing();
            ClearBackground(WHITE);

            for (Car& c : cars) {
                Vector2 origin = { (float)carTexture.width / 2.0f, (float)carTexture.height / 2.0f };
                Rectangle source = { 0.0f, 0.0f, (float)carTexture.width, (float)carTexture.height };
                Rectangle dest = { c.car.x, c.car.y, c.size + 25.0f, c.size };

                DrawTexturePro(carTexture, source, dest, origin, c.currentRot, c.carColor);
            }

                for (StopSign& stopSign : stopSigns) {
                    if (elapsed >= maxInterval) {
                        for (Car& c : cars) {
                            c.size = 50.0f;  
                            c.maxV = 50.0f;
                        }
                        radius = 100.0f;
                        spawnStopSigns();
                        stopSign.pos.x = rand() % (GetScreenWidth() - targTexture.width);
                        stopSign.pos.y = rand() % (GetScreenHeight() - targTexture.height);

                        stopSign.pos = { stopSign.pos.x,  stopSign.pos.y };

                        stopsignspwaned = true;
                        reached = false;
                        elapsed = 0.0f;                  

                        if (!cars.empty()) {
                            auto minC = std::min_element(cars.begin(), cars.end(), [](const Car& c1, const Car& c2) {
                                return c1.count < c2.count;
                             });
                            if (minC != cars.end()) {
                                cars.erase(minC);
                            }
                            for (size_t i = 0; i < cars.size(); ++i) {
                                float incP = 0.0f;

                                switch (i) {
                                case 0: incP = 1.0f;   
                                    break;
                                case 1: incP = 0.8f;   
                                    break;
                                case 2: incP = 0.6f;  
                                    break;
                                case 3: incP = 0.4f;   
                                    break;
                                default:
                                    if (i == cars.size() - 1) {
                                        cars.erase(cars.begin() + i);
                                        return; 
                                    }
                                    break;
                                }  
                                cars[i].maxV *= (1.0f + incP);
                                
                            }
                        }
                    }
                }

            for (StopSign& stopSign : stopSigns) {
                if (stopSign.isActive) {
                    Vector2 origin2 = { (float)targTexture.width / 2.0f, (float)targTexture.height / 2.0f };
                    Rectangle source2 = { 0.0f, 0.0f, (float)targTexture.width, (float)targTexture.height };
                    Rectangle dest2 = { stopSign.pos.x, stopSign.pos.y, (float)targTexture.width, (float)targTexture.height };

                    DrawTexturePro(targTexture, source2, dest2, origin2, 0.0f, WHITE);
                }
            }
            EndDrawing();
        }
}
