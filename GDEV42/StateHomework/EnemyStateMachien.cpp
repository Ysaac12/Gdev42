#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include "Enemy.hpp"

using namespace std;

Vector2 faceTarget(Vector2 startPos, Vector2 endPos) {
    Vector2 ft;
    ft = Vector2Subtract(startPos,endPos);
    return ft;
};

void Enemy::Update(float deltaTime) {
    current_state ->Update(*this, deltaTime);
};

void Enemy::Draw() {
    DrawRectangle(position.x, position.y, width, height, color);
};

void Enemy::SetState(EnemyState* new_state) {
    current_state = new_state;
    current_state ->Enter(*this);
};

Enemy::Enemy(Vector2 pos, float w, float h, float spd) {
    position = pos;
    width = w;
    height = h;
    speed = spd;
    SetState(&wandering);
}

void EnemyWandering::Enter(Enemy& enemy) {
    enemy.color = LIME;
    enemy.wanderingTimer = 3.0f;
    float randXCoord = GetRandomValue(0,1160);
    float randYCoord = GetRandomValue(0,680);
    Vector2 tp = {randXCoord, randYCoord};
    enemy.targetPosition = tp;

    cout << tp.x << " " << tp.y << endl;
}

void EnemyWandering::Update(Enemy& enemy, float deltaTime) {
    
    enemy.direction = Vector2Normalize(faceTarget(enemy.targetPosition, enemy.position));

    
    enemy.position.x += enemy.direction.x * enemy.speed * deltaTime;
    enemy.position.y += enemy.direction.y * enemy.speed * deltaTime;

    if(Vector2Distance(enemy.position, enemy.targetPosition) <= 10.0f) {
        enemy.position = enemy.targetPosition;
        enemy.wanderingTimer -= deltaTime;
    };

    if(enemy.wanderingTimer <= 0) {
        enemy.SetState(&enemy.wandering);
    };
}



