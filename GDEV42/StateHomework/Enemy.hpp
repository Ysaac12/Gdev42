#ifndef ENEMY
#define ENEMY 

#include <raylib.h>
#include <raymath.h>

class Enemy;

class EnemyState {
public: 
    virtual ~EnemyState() {}
    virtual void Enter(Enemy& enemy) = 0;
    virtual void Update(Enemy& enemy, float deltaTime) = 0;
};

class EnemyWandering : public EnemyState {
public:
    void Enter(Enemy& enemy);
    void Update(Enemy& enemy, float deltaTime);
};

class EnemyChasing : public EnemyState {
public:
    void Enter(Enemy& enemy);
    void Update(Enemy& enemy, float deltaTime);
};

class EnemyAttack : public EnemyState {
public:
    void Enter(Enemy& enemy);
    void Update(Enemy& enemy, float deltaTime);
};

class EnemyAttackCharge : public EnemyState {
    void Enter(Enemy& enemy);
    void Update(Enemy& enemy, float deltaTime);
};

class Enemy {
public:
    Vector2 position;
    Vector2 direction;
    Vector2 velocity;
    float detectionRange;
    float aggroRange;
    float attackRange;
    Vector2 chargeVel;
    int width, height;
    float speed;
    Color color;

    EnemyWandering wandering;
    EnemyChasing chase;
    EnemyAttack attack;
    EnemyAttackCharge charge;


};


#endif