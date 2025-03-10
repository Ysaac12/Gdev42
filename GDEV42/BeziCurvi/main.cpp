#include "raylib.h"
#include <vector>
#include <cmath>

// Compute binomial coefficients using Pascal’s triangle
int BinomialCoeff(int n, int k) {
    if (k == 0 || k == n) return 1;
    int res = 1;
    for (int i = 0; i < k; i++) {
        res *= (n - i);
        res /= (i + 1);
    }
    return res;
}

// Compute a Bézier curve point using the parametric function
Vector2 BezierPoint(float t, const std::vector<Vector2>& controlPoints) {
    int n = controlPoints.size() - 1;
    Vector2 result = {0, 0};

    for (int i = 0; i <= n; i++) {
        float coeff = BinomialCoeff(n, i) * pow(1 - t, n - i) * pow(t, i);
        result.x += coeff * controlPoints[i].x;
        result.y += coeff * controlPoints[i].y;
    }
    return result;
}

// Generate points on the Bézier curve
std::vector<Vector2> BezierCurve(const std::vector<Vector2>& controlPoints, int resolution = 100) {
    std::vector<Vector2> curvePoints;
    for (int i = 0; i <= resolution; i++) {
        float t = (float)i / resolution;
        curvePoints.push_back(BezierPoint(t, controlPoints));
    }
    return curvePoints;
}

// Detects and allows dragging of control points
void HandleControlPointDragging(std::vector<Vector2>& controlPoints, Vector2& selectedPoint, bool& isDragging) {
    Vector2 mousePos = GetMousePosition();

    // If mouse is pressed and no point is being dragged, check for collision with control points
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        for (auto& point : controlPoints) {
            if (CheckCollisionPointCircle(mousePos, point, 10)) {
                selectedPoint = point;
                isDragging = true;
                break;
            }
        }
    }

    // If dragging, update the selected point position
    if (isDragging) {
        for (auto& point : controlPoints) {
            if (point.x == selectedPoint.x && point.y == selectedPoint.y) {
                point = mousePos;
                selectedPoint = mousePos;
                break;
            }
        }
    }

    // Stop dragging when mouse is released
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        isDragging = false;
    }
}

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, " Bezier Curve");

    // Example control points
    std::vector<Vector2> controlPoints = {
        {100, 500}, {200, 100}, {300, 500}, {400, 100}, {500, 500}, {600, 100}, {700, 500}, {800, 100}
    };

    Vector2 selectedPoint = {0, 0};
    bool isDragging = false;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Handle dragging of control points
        HandleControlPointDragging(controlPoints, selectedPoint, isDragging);

        // Recompute Bézier curve with updated control points
        std::vector<Vector2> bezierPoints = BezierCurve(controlPoints, 200);

        BeginDrawing();
        ClearBackground(DARKGRAY);

        // Draw control points
        for (const auto& point : controlPoints) {
            DrawCircleV(point, 7, GREEN);
        }

        // Draw Bézier curve
        for (size_t i = 1; i < bezierPoints.size(); i++) {
            DrawLineV(bezierPoints[i - 1], bezierPoints[i], WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
