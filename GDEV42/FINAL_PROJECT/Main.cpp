#include <raylib.h>
#include <raymath.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>

#include "PlayerStateMachine.cpp"
#include "BeeStateMachine.cpp"
#include "slimeStateMachine.cpp"
#include "GhostStateMachine.cpp"
#include "projectile.cpp"
#include "TileMap.cpp"

#define GAME_SCENE_MUSIC "Assets/Audio/Music/symphony.ogg"

const int WINDOW_WIDTH(1280);
const int WINDOW_HEIGHT(720);
const float FPS(60.0f);

Rectangle camera_window = {(WINDOW_WIDTH / 2) - 150, (WINDOW_HEIGHT / 2) - 150, 300.0f, 300.0f};
float cam_drift = 2.0f;

std::vector<BaseEnemy*> enemies;
int current_wave = 1;
int base_wave_points = 5;
float wave_timer = 0.0f;
float wave_delay = 2.0f;
bool wave_cleared = false;

void MoveCamera(Camera2D* cam, Player* player, float delta_time) {
    float cam_push_x = 0.0f;
    float cam_push_y = 0.0f;
    float drift_x = Clamp(player->position.x - (camera_window.x + camera_window.x + camera_window.width) / 2, -cam_drift, cam_drift);
    float drift_y = Clamp(player->position.y - (camera_window.y + camera_window.y + camera_window.height) / 2, -cam_drift, cam_drift);

    if (player->position.x > camera_window.x + camera_window.width) {
        cam_push_x = player->position.x - (camera_window.x + camera_window.width);
        cam->target.x += cam_push_x;
        camera_window.x += cam_push_x;
    } else if (player->position.x < camera_window.x) {
        cam_push_x = player->position.x - camera_window.x;
        cam->target.x += cam_push_x;
        camera_window.x += cam_push_x;
    } else {
        cam->target.x += drift_x;
        camera_window.x += drift_x;
    }

    if (player->position.y > camera_window.y + camera_window.height) {
        cam_push_y = player->position.y - (camera_window.y + camera_window.height);
        cam->target.y += cam_push_y;
        camera_window.y += cam_push_y;
    } else if (player->position.y < camera_window.y) {
        cam_push_y = player->position.y - camera_window.y;
        cam->target.y += cam_push_y;
        camera_window.y += cam_push_y;
    } else {
        cam->target.y += drift_y;
        camera_window.y += drift_y;
    }
}

void SpawnWave(std::vector<BaseEnemy*>& enemies, TileMap& map, int wave_num) {
    for (auto* e : enemies) delete e;
    enemies.clear();

    int points = base_wave_points + wave_num * 3;

    while (points > 0) {
        int type = GetRandomValue(0, 2); // 0 = Slime(2pts), 1 = Ghost(1pt), 2 = Bee(3pts)
        Vector2 spawn = {
            static_cast<float>(GetRandomValue(300, 1000)),
            static_cast<float>(GetRandomValue(300, 700))
        };

        if (type == 0 && points >= 2) {
            Slime* slime = new Slime(spawn, 50, 15, 100, 250, 15, 2);
            slime->setTileMap(&map);
            enemies.push_back(slime);
            points -= 2;
        } else if (type == 1 && points >= 1) {
            Ghost* ghost = new Ghost(spawn, 50, 15, 100, 250, 15, 2);
            ghost->setTileMap(&map);
            enemies.push_back(ghost);
            points -= 1;
        } else if (type == 2 && points >= 3) {
            Bee* bee = new Bee(spawn, 100, 15, 100, 250, 50, 2);
            bee->setTileMap(&map);
            enemies.push_back(bee);
            points -= 3;
        }
    }

    std::cout << "Wave " << wave_num << " spawned with " << enemies.size() << " enemies.\n";
}

int main() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Enemy Wave Spawn Example");
    InitAudioDevice();
    Music gameSceneMusic = LoadMusicStream(GAME_SCENE_MUSIC);
    SetTargetFPS(FPS);

    TileMap Map;
    Map.LoadTilemapData("TileInfo.txt");

    Player player(Map.playerPos, 15.0f, 150.0f, 100);
    player.setTileMap(&Map);

    SpawnWave(enemies, Map, current_wave);

    bool game_ongoing = true;
    camera_window = {player.position.x - 150, player.position.y - 150, 300.0f, 300.0f};

    Camera2D camera_view = {0};
    camera_view.target = player.position;
    camera_view.offset = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
    camera_view.zoom = 2.0f;

    while (!WindowShouldClose()) {
        float delta_time = GetFrameTime();

        if (IsMusicReady(gameSceneMusic)) {
            UpdateMusicStream(gameSceneMusic);
    
            if (!IsMusicStreamPlaying(gameSceneMusic)) {
                PlayMusicStream(gameSceneMusic);
            }
        }
    

        if (game_ongoing) {
            wave_cleared = true;

            player.Update(delta_time);

            for (auto* enemy : enemies) {
                if (enemy->active) {
                    enemy->Update(delta_time);
                    enemy->HandleCollision(&player);
                    player.HandleCollision(enemy);
                    wave_cleared = false;
                }
            }

            if (wave_cleared) {
                wave_timer += delta_time;
                if (wave_timer >= wave_delay) {
                    current_wave++;
                    SpawnWave(enemies, Map, current_wave);
                    wave_timer = 0.0f;
                }
            }

            if (player.health <= 0) {
                game_ongoing = false;
            }
        }

        MoveCamera(&camera_view, &player, delta_time);

        BeginDrawing();
        BeginMode2D(camera_view);
        ClearBackground(BLACK);

        Map.DrawTilemap();
        player.Draw();

        for (auto* enemy : enemies)
            if (enemy->active) enemy->Draw();

        EndMode2D();

        if (game_ongoing) {
            DrawText(TextFormat("Health: %d", player.health), 10, 10, 30, WHITE);
            DrawText(TextFormat("Wave: %d", current_wave), 10, 50, 30, YELLOW);
        } else {
            ClearBackground(BLACK);
            DrawText("GAME OVER", WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2 - 25, 100, RED);
        }

        EndDrawing();
    }

    for (auto* e : enemies)
        delete e;

    UnloadMusicStream(gameSceneMusic);
    UnloadSound(player.projectileSFX);
    UnloadSound(player.damageSFX);

    CloseWindow();
    return 0;
}
