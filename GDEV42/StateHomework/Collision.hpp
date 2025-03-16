#ifndef COLLISION
# define COLLISION

#include <raylib.h>
#include <raymath.h>

#include "Player.hpp"
#include "Enemy.hpp"

bool CheckCircleCollision(Vector2 center1, float radius1, Vector2 center2, float radius2);

#endif