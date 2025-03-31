#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#include "PlayerStateMachine.cpp"
#include "EnemyStateMachine.cpp"


const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const float FPS = 60;


int TILE_COUNT;
struct Tile {
    Rectangle tileLocation;
    bool hasCollision;
};

Texture2D tileset;
vector<Tile> tileList;
int tilemap[100][100]; 
int mapWidth, mapHeight;
Vector2 playerPos, enemyPos;


bool CheckTileCollision(Vector2 playerPos, float radius) {
    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            int tileIndex = tilemap[y][x];

            if (tileIndex < 0 || tileIndex >= TILE_COUNT) continue;

            if (tileList[tileIndex].hasCollision) {
                Rectangle tileRect = { x * 16.0f, y * 16.0f, 16, 16 };

                if (CheckCollisionCircleRec(playerPos, radius, tileRect)) {
                    cout << "wall collision" << endl;
                    return true; // Collision detected
                }
            }
        }
    }
    return false;
}


void LoadTilemapData(const char* filename) {
    ifstream file(filename);

    string tilesetFile;
    file >> tilesetFile;
    tileset = LoadTexture(tilesetFile.c_str());

    file >> TILE_COUNT;
    tileList.resize(TILE_COUNT);

    cout << "Tile count: " << TILE_COUNT << endl;

    // Read tile positions
    for (int i = 0; i < TILE_COUNT; i++) {
        file >> tileList[i].tileLocation.x >> tileList[i].tileLocation.y
             >> tileList[i].tileLocation.width >> tileList[i].tileLocation.height
             >> tileList[i].hasCollision;
    }

    file >> mapWidth >> mapHeight;
    cout << "Map size: " << mapWidth << "x" << mapHeight << endl;

    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            file >> tilemap[y][x];
        }
        cout << endl;
    }

    file >> playerPos.x >> playerPos.y;
    cout << "Player position: " << playerPos.x << " " << playerPos.y << endl;

    file >> enemyPos.x >> enemyPos.y;
    cout << "Enemy position: " << enemyPos.x << " " << enemyPos.y << endl;

    file.close();
}

void DrawTilemap() {
    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            int tileIndex = tilemap[y][x];
            if (tileIndex < 0 || tileIndex >= TILE_COUNT) continue;

            Rectangle src = tileList[tileIndex].tileLocation;
            Rectangle dest = { x * 16.0f, y * 16.0f, 16, 16 };

            DrawTexturePro(tileset, src, dest, (Vector2){ 0, 0 }, 0.0f, WHITE);
        }
    }
}

int main() {


    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "MesaReyesRuiz_Homework4");
    SetTargetFPS(FPS);

    LoadTilemapData("TileInfo.txt");

    Player* player = new Player(playerPos, 10, 200);
    Enemy* enemy = new Enemy(enemyPos, 40, 40, 100, 200, 360, 120);

    Camera2D camera = {0};
    camera.rotation = 0.0f;
    camera.zoom = 2.0f;
    camera.offset = {WINDOW_WIDTH/2, WINDOW_HEIGHT/2};
    camera.target = player->position;

    while (!WindowShouldClose()) {
        CheckTileCollision(player->position, player->radius);
        float deltaTime = GetFrameTime();
        

        camera.target = player->position;

        player->Update(deltaTime, *enemy);
        enemy->Update(deltaTime, *player);
        

        BeginDrawing();

        BeginMode2D(camera);
        ClearBackground(RAYWHITE);
        DrawTilemap();
        player->Draw();
        enemy->Draw();


        EndMode2D();
        DrawText(TextFormat("Player Healthpoint: %i", player->healthPoints), 20, 20, 20, WHITE);
        EndDrawing();
    }

    UnloadTexture(tileset);
    CloseWindow();
    delete player;
    delete enemy;

    return 0;
}
