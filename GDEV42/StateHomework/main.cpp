#include <raylib.h>
#include <raymath.h>

#include "PlayerStateMachine.cpp"

const float WINDOW_WIDTH = 800;
const float WINDOW_HEIGHT = 600;
const int FPS = 60;



int main() {
    Player* player = new Player(Vector2 {WINDOW_WIDTH/2, WINDOW_HEIGHT/2}, 10, 200);
    InitWindow(WINDOW_WIDTH,WINDOW_HEIGHT, "cool world");
    SetTargetFPS(FPS);

    while(!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        player->Update(deltaTime);

        BeginDrawing();
        ClearBackground(BLACK);
        player->Draw();
        EndDrawing();

    };
    CloseWindow();
    delete player;
    return 0;
}