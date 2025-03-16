#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <raylib.h>

class Entity {
public:
    int healthPoints;
    Vector2 position;
    Vector2 targetPosition;
    Vector2 direction;
    Vector2 velocity;
    float width, height, radius, speed;
    Color color;

    Entity(int hp, Vector2 pos, float width, float height, float radius, float speed)
        : healthPoints(hp), position(pos), width(width), height(height), radius(radius), speed(speed),
          targetPosition({0, 0}), direction({0, 0}), velocity({0, 0}), color(WHITE) {}

    virtual ~Entity() {}
};

#endif
