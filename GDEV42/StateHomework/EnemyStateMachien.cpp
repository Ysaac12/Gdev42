#include <raylib.h>
#include <raymath.h>
#include <iostream>

#include "Enemy.hpp"
#include "Player.hpp"
#include "Collision.hpp"

using namespace std;

Vector2 faceTarget(Vector2 startPos, Vector2 endPos) {
    Vector2 ft;
    ft = Vector2Subtract(startPos,endPos);
    return ft;
};

void Enemy::Update(float deltaTime, Player& player) {
    if (current_state) {
        current_state->Update(*this, deltaTime, player);
    }
}


void Enemy::Draw() {
    DrawRectangle(position.x-20, position.y-20, width, height, color);
    DrawCircleLinesV(position, attackRange, RED);
    DrawCircleLinesV(position, aggroRange, ORANGE);
    DrawCircleLinesV(position, detectionRange, YELLOW);
};

void Enemy::SetState(EnemyState* new_state) {
    current_state = new_state;
    current_state ->Enter(*this);
};

Enemy::Enemy(Vector2 pos, float w, float h, float spd, float detectRad, float aggroRad, float attackRad) 
    : Entity(100, pos, w, h, 20.0f, spd), detectionRange(detectRad), aggroRange(aggroRad), attackRange(attackRad), wanderingTimer(3.0f) {
    SetState(&wandering); 
}

void EnemyWandering::Enter(Enemy& enemy) {
    enemy.color = LIME;
    enemy.wanderingTimer = 3.0f;
    float randXCoord = GetRandomValue(0,1160);
    float randYCoord = GetRandomValue(0,680);
    Vector2 tp = {randXCoord, randYCoord};
    enemy.targetPosition = tp;
}

void EnemyWandering::Update(Enemy& enemy, float deltaTime, Player& player) {

    bool enemyDetect, enemyAggro, enemyAttack;
    
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

    
    if (CheckCircleCollision(enemy.position, enemy.detectionRange, player.position, player.radius)) {
        enemy.SetState(&enemy.chase);
    }

}

void EnemyChasing::Enter(Enemy& enemy) {
    enemy.color = RED;
}

void EnemyChasing::Update(Enemy& enemy, float deltaTime, Player& player) {
    enemy.direction = Vector2Normalize(faceTarget(player.position, enemy.position));
    enemy.position.x += enemy.direction.x * enemy.speed * deltaTime;
    enemy.position.y += enemy.direction.y * enemy.speed * deltaTime;

    if (!CheckCircleCollision(enemy.position, enemy.aggroRange, player.position, player.radius)) {
        enemy.SetState(&enemy.wandering);
    }

    if (!CheckCircleCollision(enemy.position, enemy.attackRange, player.position, player.radius)) {
        enemy.SetState(&enemy.attack);
    }


}



