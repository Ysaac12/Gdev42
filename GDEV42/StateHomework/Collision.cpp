#include "Collision.hpp"
#include <raymath.h>

bool CheckCircleCollision(Vector2 center1, float radius1, Vector2 center2, float radius2) {
    return Vector2Distance(center1, center2) <= (radius1 + radius2);
}

Vector2 GetClosestPointAABB(Vector2 Pos, Vector2 aabbPos, Vector2 aabbSize) {
    Vector2 ret; 
    ret.x = Clamp(Pos.x, aabbPos.x, aabbPos.x + aabbSize.x);
    ret.y = Clamp(Pos.y, aabbPos.y, aabbPos.y + aabbSize.y);
    return ret;
}

bool CheckAABBCircleCollision(Vector2 Pos, Vector2 aabbPos, Vector2 aabbSize, float rad) {
    Vector2 q = GetClosestPointAABB(Pos, aabbPos, aabbSize);
    
    return Vector2Distance(Pos, q) <= rad;
}
