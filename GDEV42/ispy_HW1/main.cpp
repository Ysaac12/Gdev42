#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <fstream>

using namespace std;

//----------- CONSTANTS ----------
const int windowWidth = 800;
const int windowHeight = 600;
//----------- STRUCTURES ----------
struct Cursor {
    Vector2 position;
    Color color;
    float radius, speed;
    bool find, select, center;
};

struct Object {
    string name;
    Vector2 position;
    Color color;
    float radius;
    bool found;
};

//----------- FUNCTION ----------
bool CheckCircleCircleCollision(Cursor c, Object o) {
    Vector2 normal = Vector2Subtract(c.position, o.position);
    float distance = Vector2Length(normal);
    float radSum = c.radius + o.radius;

     if (distance <= radSum) {
        cout << "Colission!" << endl;
        return true;
     } else {
        cout << "FOUND NOTHING" << endl;
        return false;
     }
}

void drawObjectText(Object o, float x, float y, float size) {
    if (!o.found) {
        DrawText(TextFormat("%s not found", o.name.c_str()), x,y,size,RED);
    } else {
        DrawText(TextFormat("%s found", o.name.c_str()), x,y,size,GREEN);
    }
}

void handleObjectCollission(Cursor c, Object *o) {
    if(c.select && CheckCircleCircleCollision(c,*o)) {
        o->found = true;
    } else {
        c.select = false;
    }
}
//----------- VARIABLE ----------
string backgroundName;
int boxWidth, boxHeight, minCameraEdgeX, maxCameraEdgeX, minCameraEdgeY, maxCameraEdgeY;

//----------- MAIN ----------
int main() {
    SetTargetFPS(60);
    InitWindow(windowWidth, windowHeight, "Cool math games");

Object horse; 
    horse.color = BLACK;
    horse.radius = 110;
    horse.found = false;
Object pterodactyl;
    pterodactyl.color = BLACK;
    pterodactyl.radius = 200;
    pterodactyl.found = false;
Object globe;
    globe.radius = 120;
    globe.color = BLACK;
    globe.found = false;
Object car;
    car.radius = 145;
    car.color = BLACK;
    car.found = false;
Object pawn;
    pawn.radius = 80;
    pawn.color = BLACK;
    pawn.found = false;

ifstream infile("setting.txt");
infile >> backgroundName >> 
horse.name >> horse.position.x >> horse.position.y >> 
pterodactyl.name >> pterodactyl.position.x >> pterodactyl.position.y >>
globe.name >> globe.position.x >> globe.position.y >>
car.name >> car.position.x >> car.position.y >>
pawn.name >> pawn.position.x >> pawn.position.y >>
minCameraEdgeX>> maxCameraEdgeX >> minCameraEdgeY >>
maxCameraEdgeY >> boxWidth >> boxHeight;

Texture2D background = LoadTexture(backgroundName.c_str());

float bgWidth = background.width;
float bgHeight = background.height;

Cursor c;
    c.position = {bgWidth/2, bgHeight/2};  
    c.color = YELLOW;
    c.radius = 5.0f;
    c.speed = 300;
    c.find = false;
    c.select = false;


    Camera2D camera = {0};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    camera.offset = (Vector2){ windowWidth / 2, windowHeight / 2 };
    camera.target = c.position; 

    while (!WindowShouldClose()) {
        float delta_time = GetFrameTime();

        // Cursor Selection
        if(IsKeyReleased(KEY_ENTER)){
            c.find = true;
            camera.target = c.position;
        } 

        if (IsKeyPressed(KEY_TAB)) {
            c.find = false;
        }

        if(c.find) {
            camera.zoom = 2.0f;
            if(IsKeyPressed(KEY_ENTER)) {
                c.select = true;                            // kinda useless
                handleObjectCollission(c, &horse);
                handleObjectCollission(c, &pterodactyl);
                handleObjectCollission(c, &globe);
                handleObjectCollission(c, &car);
                handleObjectCollission(c, &pawn);
        
            }
        } else {
            camera.zoom = 1.0f;
            c.select = false;
        }

         // Cursor Movement
        if(IsKeyDown(KEY_W)){
            c.position.y -= c.speed * delta_time;
        }
        if(IsKeyDown(KEY_S)){
            c.position.y += c.speed * delta_time;
        }
        if(IsKeyDown(KEY_A)){
            c.position.x -= c.speed * delta_time;
        }
        if(IsKeyDown(KEY_D)){
            c.position.x += c.speed * delta_time;
        }        


        // Camera-Window Boundaries
        float leftEdge = camera.target.x - boxWidth / 2;
        float rightEdge = camera.target.x + boxWidth / 2;
        float topEdge = camera.target.y - boxHeight / 2;
        float bottomEdge = camera.target.y + boxHeight / 2;


        // Camera Target Movement
        if (!c.find) {
           if (c.position.x < leftEdge) {
                camera.target.x += c.position.x - leftEdge;
            }
            if (c.position.x > rightEdge) {
                camera.target.x += c.position.x - rightEdge;
            }
            if (c.position.y < topEdge) {
                camera.target.y += c.position.y - topEdge;
            }
            if (c.position.y > bottomEdge) {
                camera.target.y += c.position.y - bottomEdge;
            } 
        }

        // Edge Snapping
        camera.target.x = Clamp(camera.target.x, minCameraEdgeX, maxCameraEdgeX);
        camera.target.y = Clamp(camera.target.y, minCameraEdgeY, maxCameraEdgeY);

        BeginDrawing();

            ClearBackground(RAYWHITE);

        BeginMode2D(camera);

            DrawTexture(background, 0, 0, RAYWHITE);
            DrawCircleV(c.position, c.radius, c.color);
            DrawCircleLinesV(pterodactyl.position,pterodactyl.radius,pterodactyl.color);
            DrawCircleLinesV(horse.position, horse.radius, horse.color);
            DrawCircleLinesV(globe.position, globe.radius, globe.color);
            DrawCircleLinesV(car.position, car.radius, car.color);

        EndMode2D();

            DrawRectangleLines(195, 145, boxWidth+10, boxHeight+10, RED);
            DrawText(TextFormat("%.2f, %.2f",c.position.x,c.position.y), 45,550,15,BLACK);
            DrawText("Use WASD for movement", 25,25,15,BLACK);
            drawObjectText(horse, 45,45,15);
            drawObjectText(pterodactyl, 45,60,15);
            drawObjectText(globe, 45,75,15);
            drawObjectText(car, 45,90,15);
            drawObjectText(pawn, 45,105,15);

        EndDrawing();
    }

    UnloadTexture(background);
    CloseWindow();

    return 0;
}
