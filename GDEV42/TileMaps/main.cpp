#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;


const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 700;
const float FPS = 60;


int TILE_COUNT;
struct Tile {
    Rectangle tileLocation;
};

Texture2D tileset;
vector<Tile> tileList;
int tilemap[50][50]; 
int mapWidth, mapHeight;


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
             >> tileList[i].tileLocation.width >> tileList[i].tileLocation.height;
    }

    file >> mapWidth >> mapHeight;
    cout << "Map size: " << mapWidth << "x" << mapHeight << endl;

    for (int y = 0; y < mapHeight; y++) {
        for (int x = 0; x < mapWidth; x++) {
            file >> tilemap[y][x];
        }
        cout << endl;
    }

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
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tilemap Example");
    SetTargetFPS(FPS);

    LoadTilemapData("TileInfo.txt");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTilemap();

        EndDrawing();
    }

    UnloadTexture(tileset);
    CloseWindow();
    return 0;
}
