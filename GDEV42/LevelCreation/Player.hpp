/*
|------------------------------------------------------|
|                    PLAYER HEADER                     |              
|------------------------------------------------------|
|   This is the header file (.hpp) file that contains  |
|   declarations for the Player class along with the   | 
|   PlayerState class and its subclasses.              |
|                                                      |
|   Note that ONLY DECLARATIONS are made here. The     |
|   DEFINITION of all member variables and functions   |
|   are done in the PlayerStateMachine.cpp file        |
|                                                      |
|   When adding new classes to this file               |
|   (i.e. PlayerBlocking, PlayerAttacking, etc.),      |
|   Only have the class declarations and define them   |
|   later in the PlayerStateMachine.cpp file           |
|                                                      |
|------------------------------------------------------|
*/

#ifndef PLAYER
#define PLAYER

#include <raylib.h>
#include <raymath.h>

#include "Entity.hpp"

// Declaring the Player Class so that the PlayerState classes can have a reference to it
class Enemy;
class Player;


/* 
Base PlayerState Class
This class has 3 virtual functions.
virtual ~PlayerState() {} is the destructor.
The next two virtual functions are pure virtual functions that NEED to be implemented be any derived class of PlayerState.
virtual void Enter(Player& player) = 0; is the function that is called once when the Player enters a new State.
virtual void Update(Player& player, float delta_time) = 0; is the function called every frame of the game.
*/
class PlayerState {
public:
    virtual ~PlayerState() {}
    virtual void Enter(Player& player) = 0;
    virtual void Update(Player& player, float delta_time, Enemy& enemy) = 0;
};


/*
PlayerIdle Class
This class is a derived class from PlayerState
This is the Idle State of the player
The two functions are just declarations of the two pure virtual functions of the State Class
*/
class PlayerIdle : public PlayerState {
public:
    void Enter(Player& player);
    void Update(Player& player, float delta_time, Enemy& enemy);
};


/*
PlayerMoving Class
This class is a derived class from PlayerState
This is the State of the Player when the player is in motion
The two functions are just declarations of the two pure virtual functions of the State Class
*/
class PlayerMoving : public PlayerState {
public:
    void Enter(Player& player);
    void Update(Player& player, float delta_time, Enemy& enemy);
};

class PlayerAttacking : public PlayerState {
    public:
        void Enter(Player& player);
        void Update(Player& player, float delta_time, Enemy& enemy);
    };

class PlayerDamage : public PlayerState {
    public:
        void Enter(Player& player);
        void Update(Player& player, float delta_time, Enemy& enemy);
    };

    
class PlayerDodging : public PlayerState {
    public:
        void Enter(Player& player);
        void Update(Player& player, float delta_time, Enemy& enemy);
    };
        
class PlayerBlocking : public PlayerState {
    public:
        void Enter(Player& player);
        void Update(Player& player, float delta_time, Enemy& enemy);
    };

class PlayerWin : public PlayerState {
    public:
        void Enter(Player& player);
        void Update(Player& player, float delta_time, Enemy& enemy);
    };

class PlayerLose : public PlayerState {
    public:
        void Enter(Player& player);
        void Update(Player& player, float deltaTime, Enemy& enemy);
};

        


/*
Player class
This is the Class that all the States are referencing in their functions
*/
class Player : public Entity{
public:
    // Public Members of the Player class
    Vector2 dodgeVel;
    float dashDistance;
    float attackCD;
    float dodgeTimer;
    float dodgeTime;
    int attackDamage;

    // Declarations of all the states the player can be in
    // When Making new States, declare them here
    PlayerIdle idle;
    PlayerMoving moving;
    PlayerAttacking attacking;
    PlayerDodging dodging;
    PlayerBlocking blocking;
    PlayerDamage damage;
    PlayerWin win;
    PlayerLose lose;

    // Player Constructor
    // Accepts Vector2 pos (position), float rad (radius), float spd (speed)
    Player(Vector2 pos, float rad, float spd);

    void TakeDamage(float damage);
    // Player Update Function
    // Also called every frame like the PlayerState update functions
    void Update(float delta_time, Enemy& enemy);

    // Player Draw Function
    // Called Every Frame during the Draw phase 
    void Draw();

    // Player SetState function
    // Accepts a pointer to a PlayerState and changes the state of the player
    void SetState(PlayerState* new_state);

private:
    // Private Member of the Player Class
    // This is the current state of the player
    // Private Member so that only SetState can change the State of the Player
    PlayerState* current_state;
};

#endif