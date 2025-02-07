#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <iostream>
#include <string>

using namespace std;

//----------- CONSTANTS ----------
const float windowHeight = 800;
const float windowWidth = 600; 
const int FPS = 60;

//----------- VARIABLES ----------

//----------- FUNCTIONS ----------

Vector2 lerp(Vector2 p0, Vector2 p1, float t) {
    Vector2 out = Vector2Subtract(p1,p0);
    out = Vector2Scale(out, t);
    out = Vector2Add(out, p0);
    return out;
};

int main() {
    int step, controlPointNum;

    return 0;
}
