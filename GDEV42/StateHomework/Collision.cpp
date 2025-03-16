#include "Collision.hpp"
#include <raymath.h>

bool CheckCircleCollision(Vector2 center1, float radius1, Vector2 center2, float radius2) {
    return Vector2Distance(center1, center2) <= (radius1 + radius2);
}
