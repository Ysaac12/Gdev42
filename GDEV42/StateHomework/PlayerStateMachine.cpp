/*
|------------------------------------------------------|
|                 PLAYER STATE MACHINE                 |              
|------------------------------------------------------|
|   This is the file that contains definitions for     |
|   all the functions declared in the Player.hpp       |
|   file.                                              |
|                                                      |
|   All functions from the Player.hpp SHOULD be        |
|   defined here (For Now)                             |
|                                                      |
|------------------------------------------------------|
*/

#include <raylib.h>
#include <raymath.h>
#include <iostream>

#include "Player.hpp"

using namespace std;


// Definition of the Player Class' Update function
// Accepts float delta_time
// Calls the Current State's Update function and passes the Player and delta_time
void Player::Update(float delta_time) {
    current_state->Update(*this, delta_time);
}

// Definition of the Player Class' Draw function
// Draws the Circle representing the Player using the Player's position, radius, and color
void Player::Draw() {
    DrawCircleV(position, radius, color);
}


// Definition of the Player Class' SetState function
// Accepts a PlayerState* new_state
// changes the current_state to the new_state and calls the Enter function of the new current_state and passes the Player
void Player::SetState(PlayerState* new_state) {
    current_state = new_state;
    current_state->Enter(*this);
}


// Definition of the Player Class' Constructor
// Uses pos, rad, and spd to set the Player's initial position, radius, and speed
// Calls the Player::SetState(PlayerState* new_state) function and passes the idle state of the Player 
Player::Player(Vector2 pos, float rad, float spd) {
    position = pos;
    radius = rad;
    speed = spd;
    SetState(&idle);
}


// Definition of the PlayerIdle PlayerState Class' Enter function
// Sets the Player's color to SKYBLUE (Please use your own colors for your implementation)
void PlayerIdle::Enter(Player& player) {
    player.color = SKYBLUE;
}

// Definition of the PlayerMoving PlayerState Class' Enter function
// Sets the Player's color to GREEN (Please use your own colors for your implementation)
void PlayerMoving::Enter(Player& player) {
    player.color = GREEN;
}


// Definition of the PlayerIdle PlayerState Class' Update function
// Whenever the User inputs any Directional Input (W, A, S, or D), the Player's SetState function is called and passes the moving State
void PlayerIdle::Update(Player& player, float delta_time) {

    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        player.SetState(&player.attacking);
    }

    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        player.SetState(&player.blocking);
    } 


    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D)) {
        player.SetState(&player.moving);
    }
}

// Definition of the PlayerMoving PlayerState Class' Update function
// Whenever the Player's velocity is 0 (Player is no longer moving), the Player's SetState function is called and passes the idle State
void PlayerMoving::Update(Player& player, float delta_time) {
    player.velocity = Vector2Zero();
    player.direction = Vector2Zero();

    if(IsKeyDown(KEY_LEFT_SHIFT)) {
        player.speed = 400;
    } else {
        player.speed = 200;
    }

    if(IsKeyDown(KEY_W)) {
        player.direction.y -=1;
        player.velocity.y -= 1.0f * player.speed * delta_time;
    }
    if(IsKeyDown(KEY_A)) {
        player.direction.x -=1;
        player.velocity.x -= 1.0f * player.speed * delta_time;
    }
    if(IsKeyDown(KEY_S)) {
        player.direction.y +=1;
        player.velocity.y += 1.0f * player.speed * delta_time;
    }
    if(IsKeyDown(KEY_D)) {
        player.direction.x +=1;
        player.velocity.x += 1.0f * player.speed * delta_time;
    }

    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        player.SetState(&player.attacking);
    }

    if(IsKeyPressed(KEY_SPACE)) {
        player.SetState(&player.dodging);
    }

    if(Vector2Length(player.velocity) == 0) {
        player.SetState(&player.idle);
    } else {
        player.position = Vector2Add(player.position, player.velocity);
    }
}

void PlayerAttacking::Enter(Player& player) {
    player.color = RED;
    player.attackCD = 2.0f;
}

void PlayerAttacking::Update(Player& player, float deltaTime) {
    player.attackCD -= deltaTime;  

    if(player.attackCD <= 0) {
        player.SetState(&player.idle); 
    }
}

void PlayerDodging::Enter(Player& player) {
    player.color = PINK;
    player.dodgeTimer = 0.0f;  
    player.dodgeTime = 0.2f;   
    player.dashDistance = 100.0f;
    player.direction = Vector2Normalize(player.direction);
    player.dodgeVel = Vector2Scale(player.direction, player.dashDistance / player.dodgeTime);
}

void PlayerDodging::Update(Player& player, float deltaTime) {
    if (player.dodgeTimer < player.dodgeTime) {
        player.position = Vector2Add(player.position, Vector2Scale(player.dodgeVel, deltaTime));
        player.dodgeTimer += deltaTime;
    } else {
        player.SetState(&player.idle);
    }
}

void PlayerBlocking::Enter(Player& player) {
    player.color = DARKPURPLE;
}

void PlayerBlocking::Update(Player& player, float deltaTime) {
    if(IsMouseButtonReleased(MOUSE_RIGHT_BUTTON)) {
        player.SetState(&player.idle);
    }
}
