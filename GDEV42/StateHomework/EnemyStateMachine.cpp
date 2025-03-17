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


    if (CheckAABBCircleCollision(player.position,position, {width, height}, player.radius)) {
        if (damageTimer <= 0)
        {
         player.TakeDamage(10);
         damageTimer = damageCD;
         cout << "Collission" << endl;
         cout << player.healthPoints << endl;
        }
    }

    if (damageTimer >= 0)
    {
        damageTimer -= deltaTime;
    }
    
}


void Enemy::Draw() {
    if (current_state != &dead && current_state != &win) {
        DrawRectangle(position.x-20, position.y-20, width, height, color);
        DrawCircleLinesV(position, attackRange, RED);
        DrawCircleLinesV(position, aggroRange, ORANGE);
        DrawCircleLinesV(position, detectionRange, YELLOW);
    } 
    if (current_state == &dead) {
        ClearBackground(WHITE);
        DrawText("CONGRATULATION! YOU WIN!!", 200, 360, 50, BLACK);
    }
};

void Enemy::SetState(EnemyState* new_state) {
    current_state = new_state;
    current_state ->Enter(*this);
};


Enemy::Enemy(Vector2 pos, float w, float h, float spd, float detectRad, float aggroRad, float attackRad) 
    : Entity(100, pos, w, h, 20.0f, spd), detectionRange(detectRad), aggroRange(aggroRad), attackRange(attackRad), wanderingTimer(3.0f) {
    SetState(&wandering); 
}

void EnemyDead::Enter (Enemy& enemy) {

};

void EnemyDead::Update(Enemy& enemy, float deltaTime, Player& player) {
    
};

void EnemyWin::Enter (Enemy& enemy) {

};

void EnemyWin::Update(Enemy& enemy, float deltaTime, Player& player) {
    
};




void EnemyWandering::Enter(Enemy& enemy) {
    enemy.color = LIME;
    enemy.wanderingTimer = 3.0f;
    float randXCoord = GetRandomValue(0,1160);
    float randYCoord = GetRandomValue(0,680);
    Vector2 tp = {randXCoord, randYCoord};
    enemy.targetPosition = tp;

    if(enemy.healthPoints <= 0) {
        enemy.SetState(&enemy.dead);
    }
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

    if(player.healthPoints <= 0) {
        enemy.SetState(&enemy.win);
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

    if (CheckCircleCollision(enemy.position, enemy.attackRange, player.position, player.radius)) {
        enemy.SetState(&enemy.chargeAttack);
    }
    if(player.healthPoints <= 0) {
        enemy.SetState(&enemy.win);
    }


}

void EnemyChargingAttack::Enter(Enemy& enemy) {
    enemy.color = BLUE;
    enemy.attackChargeTimer = 3.0f;
};

void EnemyChargingAttack::Update(Enemy& enemy, float deltaTime, Player& player) {
    enemy.attackChargeTimer -= deltaTime;

    if(enemy.attackChargeTimer <= 0) {
        enemy.SetState(&enemy.attack);
    };
    
    if(player.healthPoints <= 0) {
        enemy.SetState(&enemy.win);
    }
}

void EnemyAttack::Enter(Enemy& enemy) {
    enemy.checkDirection = true;
    enemy.color = DARKBLUE;
    

    
};

void EnemyAttack::Update(Enemy& enemy, float deltaTime, Player& player) {
    
    if (enemy.checkDirection){
    enemy.targetPosition = player.position;
    enemy.direction = Vector2Normalize(faceTarget(enemy.targetPosition, enemy.position));
    enemy.checkDirection = false;
    }

    enemy.chargeVel = Vector2Scale(enemy.direction, 100/ 0.3f);
    enemy.position = Vector2Add(enemy.position, Vector2Scale(enemy.chargeVel, deltaTime));

    if (Vector2Distance(enemy.position, enemy.targetPosition) <= 10) {
        enemy.position = enemy.targetPosition;
        enemy.SetState(&enemy.wandering);
    };

    if(player.healthPoints <= 0) {
        enemy.SetState(&enemy.win);
    }


};





