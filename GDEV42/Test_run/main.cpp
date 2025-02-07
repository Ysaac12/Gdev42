#include <raylib.h>
#include <raymath.h>

//----------- CONSTANTS ----------
const int windowWidth = 800;
const int windowHeight = 600;
const float FPS = 60;

//----------- FUNCTION ----------
Vector2 GetClosestPointToAABB(Vector2 P, Vector2 aabb_pos, Vector2 aabb_size) {
    Vector2 ret;
    ret.x = Clamp(P.x, aabb_pos.x, aabb_pos.x + aabb_size.x);
    ret.y = Clamp(P.y, aabb_pos.y, aabb_pos.y + aabb_size.y);
    return ret;
}

//----------- STRUCTURES ----------
struct Cursor {
    Vector2 position;
    Color color;
    float radius, speed;
};

struct CameraEdge {
    Vector2 position;
    float width, height;
};

//----------- MAIN ----------
int main() {
    SetTargetFPS(FPS);
    InitWindow(windowWidth, windowHeight, "coolmathgames");

    Texture2D background = LoadTexture("ispy.png");

    Cursor c = {{1400, 900}, YELLOW, 5.0f, 300.0f};

    CameraEdge cam = {{100, 100}, 600, 400};

    Camera2D camera = {0};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    camera.offset = (Vector2){windowWidth / 2, windowHeight / 2}; // Keep camera centered

    while (!WindowShouldClose()) {
        float delta_time = GetFrameTime();

        // Cursor movement
        if (IsKeyDown(KEY_W)) c.position.y -= c.speed * delta_time;
        if (IsKeyDown(KEY_S)) c.position.y += c.speed * delta_time;
        if (IsKeyDown(KEY_A)) c.position.x -= c.speed * delta_time;
        if (IsKeyDown(KEY_D)) c.position.x += c.speed * delta_time;

        // Camera Window (Position Snapping)
        Vector2 q = GetClosestPointToAABB(c.position, cam.position, (Vector2){cam.width, cam.height});
        float distance = Vector2Distance(c.position, q);

        if (distance >= c.radius) { 
            camera.target = c.position; // Move camera when cursor leaves Camera Window
        } 
        // Edge Snapping (Restrict camera movement within background)
        float bgWidth = background.width;
        float bgHeight = background.height;
        camera.target.x = Clamp(camera.target.x, windowWidth / 2, bgWidth - windowWidth / 2);
        camera.target.y = Clamp(camera.target.y, windowHeight / 2, bgHeight - windowHeight / 2);

        //--- Rendering ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode2D(camera);
        DrawTexture(background, 0, 0, RAYWHITE); // Draw background
        DrawCircleV(c.position, c.radius, c.color); // Draw cursor
        EndMode2D();

        DrawRectangleLines(cam.position.x, cam.position.y, cam.width, cam.height, BROWN); // Camera Window
        DrawText("Use WASD to move", 10, 10, 20, DARKGRAY);

        EndDrawing();
    }

    UnloadTexture(background);
    CloseWindow();
    return 0;
}
