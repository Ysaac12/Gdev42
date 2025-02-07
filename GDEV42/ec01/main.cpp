#include <raylib.h>
#include <raymath.h>
#include <iostream>
using namespace std;

//----------- CONSTANTS ----------

const int windowWidth = 800;
const int windowHeight = 600;
const int worldBorderWidth = 1800;
const int worldBorderHeight = 1200;
const int cellSize = 100;
int numCellsX = worldBorderWidth / cellSize;
int numCellsY = worldBorderHeight / cellSize;


//----------- STRUCTURES ----------

struct Player {
    Vector2 position, direction, targetPosition;
    Color playerColor;
    float radius, speed;
};

struct Wall {
    float width, height;
};

//----------- FUNCTIONS ----------
void DrawGrid() {
    for (int i = 0; i < numCellsX; ++i) {
        for (int j = 0; j < numCellsY; ++j) {
            int cellX = i * cellSize;
            int cellY = j * cellSize;
            DrawRectangleLines(cellX, cellY, cellSize, cellSize, RED);

        }
    }
}
//----------- MAIN  ----------

int main() {

    Player p;
        p.playerColor = BLUE;
        p.position = {worldBorderWidth/2 , worldBorderHeight/2};
        p.radius = 5.0;
        p.speed = 500.0;

    SetTargetFPS(60);
    InitWindow(windowWidth, windowHeight, "Cool Camera Game");

    Camera2D camera = {0};
    camera.offset = (Vector2){ windowWidth/2.0f, windowHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;


    while(!WindowShouldClose()) {
        float delta_time = GetFrameTime();

        if(IsKeyDown(KEY_W)){
            p.position.y -= p.speed * delta_time;
        }
        if(IsKeyDown(KEY_S)){
            p.position.y += p.speed * delta_time;
        }
        if(IsKeyDown(KEY_A)){
            p.position.x -= p.speed * delta_time;
        }
        if(IsKeyDown(KEY_D)){
            p.position.x += p.speed * delta_time;
        }        

// Edge-snapping logic

if (p.position.x <= 400 && p.position.y <= 300) {
    camera.target = (Vector2){400, 300};
} else if (p.position.x <= 400 && p.position.y >= 900) {
    camera.target = (Vector2){400, 900};
} else if (p.position.x >= 1400 && p.position.y <= 300) {
    camera.target = (Vector2){1400, 300};
} else if (p.position.x >= 1400 && p.position.y >= 900) {
    camera.target = (Vector2){1400, 900};
} else if (p.position.x <= 400) {
    camera.target = (Vector2){400, p.position.y};
} else if (p.position.x >= 1400) {
    camera.target = (Vector2){1400, p.position.y};
} else if (p.position.y <= 300) {
    camera.target = (Vector2){p.position.x, 300};
} else if (p.position.y >= 900) {
    camera.target = (Vector2){p.position.x, 900};
} else {
    camera.target = (Vector2){p.position.x, p.position.y};
}
        BeginDrawing();

        BeginMode2D(camera);
        
        ClearBackground(BLACK);
        DrawRectangle(0,0,worldBorderWidth,worldBorderHeight,YELLOW);
        DrawGrid();
        DrawCircleV(p.position, p.radius, p.playerColor);

        EndMode2D();

        EndDrawing();
    }



    CloseWindow();
    return 0;
}