#ifndef ENEMY_HPP
#define ENEMY_HPP 

#include <raylib.h>
#include <raymath.h>
#include "Entity.hpp"

class Player;

class Enemy; 

class EnemyState {
public: 
    virtual ~EnemyState() {}
    virtual void Enter(Enemy& enemy) = 0;
    virtual void Update(Enemy& enemy, float deltaTime, Player& player) = 0;  
};

class EnemyWandering : public EnemyState {
public:
    void Enter(Enemy& enemy) override;
    void Update(Enemy& enemy, float deltaTime, Player& player) override;
};

class EnemyChasing : public EnemyState {
public:
    void Enter(Enemy& enemy) override;
    void Update(Enemy& enemy, float deltaTime, Player& player) override;  
};



class EnemyChargingAttack : public EnemyState {
    void Enter(Enemy& enemy) override;
    void Update(Enemy& enemy, float deltaTime, Player& player) override;
};

class EnemyAttack : public EnemyState {
    void Enter(Enemy& enemy) override;
    void Update(Enemy& enemy, float deltaTime, Player& player) override;
};

class EnemyDead : public EnemyState {
    void Enter(Enemy& enemy) override;
    void Update(Enemy& enemy, float deltaTime, Player& player) override;
};

class EnemyWin : public EnemyState {
    void Enter(Enemy& enemy) override;
    void Update(Enemy& enemy, float deltaTime, Player& player) override;
};


class Enemy : public Entity {
public:
    Vector2 chargeVel;
    float detectionRange, attackRange, aggroRange;
    float wanderingTimer, attackChargeTimer;
    bool checkDirection;
    float damageTimer = 0;
    float damageCD = 1.0f;

    EnemyWandering wandering;
    EnemyChasing chase;
    EnemyChargingAttack chargeAttack;
    EnemyAttack attack;
    EnemyDead dead;
    EnemyWin win;

    Enemy(Vector2 pos, float width, float height, float speed, float detectionRange, float aggroRange, float attackRange);
    
    void Update(float deltaTime, Player& player);  
    void Draw();
    void SetState(EnemyState* new_state);

private:
    EnemyState* current_state;
};

#endif
