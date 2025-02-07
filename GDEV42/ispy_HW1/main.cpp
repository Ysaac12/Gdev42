#include <raylib.h>
#include <raymath.h>

//----------- CONSTANTS ----------
const int windowWidth = 800;
const int windowHeight = 600;
const int boxWidth = 400;  
const int boxHeight = 300; 
//----------- STRUCTURES ----------
struct Cursor {
    Vector2 position;
    Color color;
    float radius, speed;
    bool find, select, center;
};

struct Object {
    Vector2 position;
    Color color;
    float radius;
};

//----------- MAIN ----------
int main() {
    SetTargetFPS(60);
    InitWindow(windowWidth, windowHeight, "Cool math games");

    Texture2D background = LoadTexture("ispy.png");

    float bgWidth = background.width;
    float bgHeight = background.height;

    Cursor c;
    c.position = {bgWidth/2, bgHeight/2};  
    c.color = YELLOW;
    c.radius = 5.0f;
    c.speed = 300;
    c.find = false;
    c.select = false;
    c.select = false;


    Camera2D camera = {0};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    camera.offset = (Vector2){ windowWidth / 2, windowHeight / 2 };
    camera.target = c.position; 

    while (!WindowShouldClose()) {
        float delta_time = GetFrameTime();

        // Cursor Movement
        if(IsKeyPressed(KEY_ENTER)){
            c.find = true;
            camera.target = c.position;
        } 
        if (IsKeyPressed(KEY_TAB)) {
            c.find = false;
        }

        if(c.find) {
            camera.zoom = 2.0f;
        } else {
            camera.zoom = 1.0f;
        }

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

        // Edge Snapping
        camera.target.x = Clamp(camera.target.x, windowWidth / 2, bgWidth - windowWidth / 2);
        camera.target.y = Clamp(camera.target.y, windowHeight / 2, bgHeight - windowHeight / 2);

        BeginDrawing();

            ClearBackground(RAYWHITE);

        BeginMode2D(camera);

            DrawTexture(background, 0, 0, RAYWHITE);
            DrawCircleV(c.position, c.radius, c.color);

        EndMode2D();

            DrawRectangleLines(195, 145, boxWidth+10, boxHeight+10, RED);
            DrawText("Use WASD for movement", 25,25,15,BLACK);

        EndDrawing();
    }

    UnloadTexture(background);
    CloseWindow();

    return 0;
}
