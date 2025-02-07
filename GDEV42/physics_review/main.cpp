#include <iostream>
#include <raylib.h>

// --- Variables --- 
int x = 10;
int y = 10;


int main() {
    InitWindow(800,600,"physics");
    
    while(!WindowShouldClose()) {

        if(IsKeyDown(KEY_A)) {
            x -= 1;
        } else if (IsKeyDown(KEY_D)){ 
            x += 1;
        } else if (IsKeyDown(KEY_W)) {
            y -= 1;
        } else if (IsKeyDown(KEY_S)) {
            y += 1;
        }
    BeginDrawing();
    ClearBackground(BLACK);
    DrawCircle(x, y, 20,RED);
    EndDrawing();
    }
    CloseWindow();
    return 0;
}