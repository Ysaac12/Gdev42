#include <raylib.h>
#include <raymath.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>


#include "PlayerStateMachine.cpp"
#include "EnemyStateMachine.cpp"
#include "slimeStateMachine.cpp"
#include "GhostStateMachine.cpp"
#include "projectile.cpp"
#include "TileMap.cpp"

const int WINDOW_WIDTH(1280);
const int WINDOW_HEIGHT(720);
const float FPS(60.0f);

using namespace std;


Rectangle camera_window = {(WINDOW_WIDTH / 2) - 150, (WINDOW_HEIGHT / 2) - 150, 300.0f, 300.0f};
float cam_drift = 2.0f;
float cam_zoom;
bool zoom_in;

void MoveCamera(Camera2D* cam, Player* player, float delta_time) {
    float cam_push_x = 0.0f;
    float cam_push_y = 0.0f;
    float drift_x = Clamp(player->position.x - (camera_window.x + camera_window.x + camera_window.width) / 2, -cam_drift, cam_drift);
    float drift_y = Clamp(player->position.y - (camera_window.y + camera_window.y + camera_window.height) / 2, -cam_drift, cam_drift);

    if (player->position.x > camera_window.x + camera_window.width) {
        cam_push_x = player->position.x - (camera_window.x + camera_window.width);
        cam->target.x += cam_push_x;
        camera_window.x += cam_push_x;
    }
    else if (player->position.x < camera_window.x) {
        cam_push_x = player->position.x - camera_window.x;
        cam->target.x += cam_push_x;
        camera_window.x += cam_push_x;
    }
    else {
        cam->target.x += drift_x;
        camera_window.x += drift_x;
    }

    if (player->position.y > camera_window.y + camera_window.height) {
        cam_push_y = player->position.y - (camera_window.y + camera_window.height);
        cam->target.y += cam_push_y;
        camera_window.y += cam_push_y;
    }
    else if (player->position.y < camera_window.y) {
        cam_push_y = player->position.y - camera_window.y;
        cam->target.y += cam_push_y;
        camera_window.y += cam_push_y;
    }
    else {
        cam->target.y += drift_y;
        camera_window.y += drift_y;
    }
}

int main() {

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "MesaReyesRuiz_Homework4");
    SetTargetFPS(FPS);

    TileMap Map;
    Map.LoadTilemapData("TileInfo.txt");
    Player player( Map.playerPos, 15.0f, 150.0f, 100);
    vector<Enemy> enemies;
    Enemy enemy01(Map.enemyPos, 100.0f, 15.0f, 100.0f, 250.0f, 50.0f, 2);
    Slime slime01({1000,300}, 50.0f, 15.0f, 100.0f, 250.0f, 15.0f, 2 );
    Ghost ghost01 ({400,500}, 50.0f, 15.0f, 100.0f, 250.0f, 15.0f, 2 );

    player.setTileMap(&Map);
    enemy01.setTileMap(&Map);
    slime01.setTileMap(&Map);
    ghost01.setTileMap(&Map);

    bool game_ongoing = true;
    bool enemy_lose = false;

    enemies.push_back(enemy01);

    camera_window = {player.position.x - 150, player.position.y - 150, 300.0f, 300.0f};

    Camera2D camera_view = {0};
    camera_view.target = player.position;
    camera_view.offset = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
    camera_view.zoom = 2.0f;

    while (!WindowShouldClose()) {
        float delta_time = GetFrameTime();

        if (game_ongoing) {
            enemy_lose = true;
            player.Update(delta_time);
            player.HandleCollision(&slime01);
            player.HandleCollision(&ghost01);

            slime01.Update(delta_time);
            slime01.HandleCollision(&player);

            ghost01.Update(delta_time);
            ghost01.HandleCollision(&player);

            for (size_t i = 0; i < enemies.size(); i++) {
                if(enemies[i].active) {
                    enemy_lose = false;
                    enemies[i].Update(delta_time);
                    enemies[i].HandleCollision(&player);
                    player.HandleCollision(&enemies[i]);
                }
            }
            
            if(enemy_lose) {
                game_ongoing = false;
            }

            if(player.health <= 0) {
                game_ongoing = false;
            }

            

            // std::cout << player.health << std::endl;
        }
        
        MoveCamera(&camera_view, &player, delta_time);
        

        BeginDrawing();
        BeginMode2D(camera_view);
        ClearBackground(BLACK);
        Map.DrawTilemap();
        if (game_ongoing) {
            player.Draw();
            if(slime01.active) {
                slime01.Draw();
            }
            if(ghost01.active) {
                ghost01.Draw();
            }
            for (size_t i = 0; i < enemies.size(); i++) {
                if (enemies[i].active) {
                    enemies[i].Draw();
                } 
            }
        }
        EndMode2D();
        if (game_ongoing) {
            DrawText(to_string(player.health).c_str(), 10, 10, 50, WHITE);
            DrawText(TextFormat("Pos: (%.0f, %.0f)", player.position.x, player.position.y), 120, 10, 20, WHITE);
        }
        else {
            if (enemy_lose) {
                ClearBackground(BLACK);
                DrawText("YOU WIN", WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2 - 25, 100, WHITE);
            }
            else if (!enemy_lose) {
                ClearBackground(BLACK);
                DrawText("YOU LOSE", WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2 - 25, 100, WHITE);
            }
            
        }
        EndDrawing();
    }

    CloseWindow();
    
    return 0;
}
