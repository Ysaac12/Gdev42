#include <raylib.h>
#include <raymath.h>

#include "PlayerStateMachine.cpp"
#include "EnemyStateMachien.cpp"

const float WINDOW_WIDTH = 1200;
const float WINDOW_HEIGHT = 720;
const int FPS = 60;



int main() {
    Player* player = new Player(Vector2 {100,100}, 10, 200);
    Enemy* enemy = new Enemy(Vector2 {WINDOW_WIDTH/2,WINDOW_HEIGHT/2}, 40, 40, 200);

    InitWindow(WINDOW_WIDTH,WINDOW_HEIGHT, "cool world");
    SetTargetFPS(FPS);

    while(!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        player->Update(deltaTime);
        enemy->Update(deltaTime);
        

        BeginDrawing();
        ClearBackground(BLACK);
        player->Draw();
        enemy->Draw();
        EndDrawing();

    };
    CloseWindow();
    delete player;
    delete enemy;
    return 0;
}