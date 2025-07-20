#include <iostream>
#include <raylib.h>
#include <vector>
using namespace std;

void updateSnakeMovement(Vector2 &pos, Vector2 &d, float tileSize, float &timer, float &delay, vector <Vector2> &snakeBody, int sw, int sh){
    if (IsKeyPressed(KEY_UP)) d = {0, -tileSize};
    if (IsKeyPressed(KEY_DOWN)) d = {0, tileSize};
    if (IsKeyPressed(KEY_RIGHT)) d = {tileSize, 0};
    if (IsKeyPressed(KEY_LEFT)) d = {-tileSize, 0};

    timer += GetFrameTime();

    if (timer >= delay){
        timer = 0.0f;
        pos.x += d.x;
        pos.y += d.y;

        if (pos.x > sw) pos.x = 0;
        if (pos.x < 0) pos.x = sw;
        if (pos.y > sh) pos.y = 0;
        if (pos.y < 0) pos.y = sh; 

        Vector2 coord = {pos.x, pos.y};
        snakeBody.push_back(coord);
    }

    

    for (int i = 0; i < snakeBody.size(); i++){
        DrawRectangle((int) snakeBody[i].x, (int) snakeBody[i].y, tileSize, tileSize, BLUE);
    }
    
    

}

bool isOccupied(Vector2 pos, const vector <Vector2> &snakeBody){
    for (const Vector2 &segment : snakeBody){
        if (segment.x == pos.x && segment.y == pos.y){
            return true;
        }
    }

    return false;
}

void appleSpawner(int sw, int sh, int tileSize, vector <Vector2> &snakeBody, float &timer, float delay, Vector2 &applePos, vector <Vector2> &applePositions){

    timer += GetFrameTime();

    if (timer >= delay){

        do{
            applePos.x = (GetRandomValue(0, sw / tileSize - 1) * tileSize);
            applePos.y = (GetRandomValue(0, sh / tileSize - 1) * tileSize); 
        }while(isOccupied(applePos, snakeBody));


        timer = 0.0f;
        applePositions.push_back(applePos);

    }   

    for (int i = 0; i < applePositions.size();  i++){
        DrawRectangle((int) applePositions[i].x, (int) applePositions[i].y, tileSize, tileSize, RED);
    }
}

void nomnom(vector <Vector2> &snakeBody, vector <Vector2> &applePositions){

    for (int i = 0; i < snakeBody.size(); i++){
        for (int x = 0; x < applePositions.size(); x++){
            if (snakeBody[i].x == applePositions[x].x && snakeBody[i].y == applePositions[x].y){
                applePositions.erase(applePositions.begin() + x);
            }
        }
    }
}

void death(vector <Vector2> &snakeBody){
    
    if (snakeBody.size() < 2) return;

    Vector2 snakeHead = snakeBody.back();

    for (int i = 0; i < snakeBody.size() - 1; i++){
        if (snakeBody[i].x == snakeHead.x && snakeBody[i].y == snakeHead.y){
            CloseWindow();
        }
    }


}

int main() {
    const Color darkGreen = {20, 160, 133, 255};
    
    constexpr int screenWidth = 800;
    constexpr int screenHeight = 800;
    
    
    InitWindow(screenWidth, screenHeight, "SNAKE");
    SetTargetFPS(60);

    float tileSize = 25;
    Vector2 snakePos = {screenWidth/2.0f, screenHeight/2.0f};
    Vector2 dir = {tileSize, 0};
    vector <Vector2> snakeBody;
    
    Vector2 applePos = {-100, 0};
    vector <Vector2> applePositions;

    float moveTimer = 0.0f;
    float moveDelay = 0.1f;


    float aTimer = 0.0f;
    float aDelay = 3.0f;

    while (!WindowShouldClose())
    {
        
        updateSnakeMovement(snakePos, dir, tileSize, moveTimer, moveDelay, snakeBody, screenWidth, screenHeight);
        appleSpawner(screenWidth, screenHeight, tileSize, snakeBody, aTimer, aDelay, applePos, applePositions);
        nomnom(snakeBody, applePositions);
        death(snakeBody);

        BeginDrawing();
            ClearBackground(darkGreen);

            DrawRectangle((int) snakePos.x, (int) snakePos.y, (int) tileSize, (int) tileSize, BLUE);

        EndDrawing();
    }
    
    CloseWindow();
}