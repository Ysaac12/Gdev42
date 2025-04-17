#include <raylib.h>
#include <raymath.h>

#include "Player.hpp"


void Player::Update(float delta_time) {
    animationTimer += delta_time;

    if (animationTimer >= frameSpeed && maxFrames > 1) {
        animationTimer = 0.0f;
        
        if (animation_state == Animation_type::MOVING) {
            currentFrame = (currentFrame + 1) % maxFrames;
        } else if (animation_state == Animation_type::IDLE) {
            currentFrame = (currentFrame + 1) % 2;
        }
    }

    current_state->Update(*this, delta_time);
}


void Player::Draw() {
    Rectangle src = {
        frameWidth * currentFrame,
        frameHeight * direction,
        frameWidth,
        frameHeight
    };

    Rectangle dst = {
        position.x,
        position.y,
        frameWidth,
        frameHeight
    };

    Vector2 origin = { frameWidth / 2, frameHeight / 2 };

    DrawTexturePro(playerSprite, src, dst, origin, 0.0f, WHITE);
}

void Player::SetState(PlayerState* new_state) {
    current_state = new_state;
    current_state->Enter(*this);
}

void Player::HandleCollision(Entity* other_entity) {
    current_state->HandleCollision(*this, other_entity);
}

Player::Player(Vector2 pos, float rad, float spd, int hp) {
    position = pos;
    radius = rad;
    speed = spd;
    health = hp;

    playerSprite = LoadTexture(GAME_SCENE_SPRITE_EYEBALL);
    if (playerSprite.id == 0) {
        std::cout << "Failed to load eyeball sprite!" << std::endl;
    } else {
        std::cout << "Successfully loaded eyeball sprite1" << std::endl;
    }
    frameWidth = (float)(playerSprite.width / 4);
    frameHeight = (float)(playerSprite.height / 4);
    currentFrame = 0;
    maxFrames = 4;
    animationTimer = 0.0f;
    frameSpeed = 0.3f;
    direction = 0;

    std::cout << "Sprite Width: " << playerSprite.width << std::endl;
    std::cout << "Sprite Height: " << playerSprite.height << std::endl;
    std::cout << "Frame Width: " << frameWidth << std::endl;
    std::cout << "Frame Height: " << frameHeight << std::endl;


    velocity = {0, 0};
    acceleration = {0, 0};
    in_attacking = false;
    attack_radius = 40.0f;

    SetState(&idle);
}

void PlayerIdle::Enter(Player& player) {
    player.color = SKYBLUE;
    player.currentFrame = 0;
    player.animation_state = Player::Animation_type::IDLE;
}

void PlayerIdle::Update(Player& player, float delta_time) {
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D)) {
        player.SetState(&player.moving);
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        player.SetState(&player.blocking);
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        player.SetState(&player.attacking);
    }

    if (player.invulnerable_timer > 0.0f) {
        player.invulnerable_timer -= delta_time;
    }
}

void PlayerIdle::HandleCollision(Player& player, Entity* other_entity) {
    if (CheckCollisionCircles(player.position, player.radius, other_entity->position, other_entity->radius) && player.invulnerable_timer <= 0.0f) {
        player.health -= 2;
        player.invulnerable_timer = 1.0f;
    }
}


void PlayerMoving::Enter(Player& player) {
    player.color = GREEN;
    player.currentFrame = 0;
    player.animation_state = Player::Animation_type::MOVING;
}

void PlayerMoving::Update(Player& player, float delta_time) {
    player.velocity = {0, 0};

    if (IsKeyDown(KEY_W)) {
        player.velocity.y -= 1.0f;
        player.direction = 0; // UP
    }
    if (IsKeyDown(KEY_S)) {
        player.velocity.y += 1.0f;
        player.direction = 2; // DOWN
    }
    if (IsKeyDown(KEY_A)) {
        player.velocity.x -= 1.0f;
        player.direction = 1; // LEFT
    }
    if (IsKeyDown(KEY_D)) {
        player.velocity.x += 1.0f;
        player.direction = 3; // RIGHT
    }

    player.velocity = Vector2Scale(Vector2Normalize(player.velocity), player.speed * delta_time);
    Vector2 new_position = Vector2Add(player.position, player.velocity);

    // Collision check
    Player temp_player = player;
    temp_player.position = new_position;

    if (player.tile_map && !player.tile_map->CheckTileCollision(&temp_player)) {
        player.position = new_position;
    }

    if (player.invulnerable_timer > 0.0f) {
        player.invulnerable_timer -= delta_time;
    }

    if (IsKeyPressed(KEY_SPACE) && Vector2Length(player.velocity) > 0) {
        player.velocity = Vector2Normalize(player.velocity);
        player.SetState(&player.dodging);
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        player.SetState(&player.attacking);
    }

    if (Vector2Length(player.velocity) == 0) {
        player.SetState(&player.idle);
    }
}

void PlayerMoving::HandleCollision(Player& player, Entity* other_entity) {
    if (CheckCollisionCircles(player.position, player.radius, other_entity->position, other_entity->radius) && player.invulnerable_timer <= 0.0f) {
        player.health -= 2;
        player.invulnerable_timer = 1.0f;
    }
}


void PlayerBlocking::Enter(Player& player) {
    player.color = BLUE;
}

void PlayerBlocking::Update(Player& player, float delta_time) {
    if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
        player.SetState(&player.idle);
    }

    if (player.invulnerable_timer > 0.0f) {
        player.invulnerable_timer -= delta_time;
    }
}

void PlayerBlocking::HandleCollision(Player& player, Entity* other_entity) {
    if (CheckCollisionCircles(player.position, player.radius, other_entity->position, other_entity->radius) && player.invulnerable_timer <= 0.0f) {
        player.health -= 1;
        player.invulnerable_timer = 1.0f;
    }
}


void PlayerAttacking::Enter(Player& player) {
    player.color = RED;

    active_time = 0.25f;
}

void PlayerAttacking::Update(Player& player, float delta_time) {
    if (active_time >= 0.0f) {
        active_time -= delta_time;
    } else {
        active_time = 0.0f;
    }

    if (active_time == 0.0f) {
        player.SetState(&player.idle);
    }

    if (player.invulnerable_timer > 0.0f) {
        player.invulnerable_timer -= delta_time;
    }
}

void PlayerAttacking::HandleCollision(Player& player, Entity* other_entity) {
    if (CheckCollisionCircles(player.position, player.radius, other_entity->position, other_entity->radius) && player.invulnerable_timer <= 0.0f) {
        player.health -= 2;
        player.invulnerable_timer = 1.0f;
    }

    if (CheckCollisionCircles(player.position, player.attack_radius, other_entity->position, other_entity->radius) && other_entity->invulnerable_timer <= 0.0f) {
        other_entity->health -= 1;
        other_entity->invulnerable_timer = 1.0f;
    }
}


void PlayerDodging::Enter(Player& player) {
    player.color = LIME;

    dodge_direction = Vector2Scale(player.velocity, 500.0f);
    player.acceleration = dodge_direction;
}

void PlayerDodging::Update(Player& player, float delta_time) {
    player.velocity = Vector2Add(player.velocity, player.acceleration);
    player.velocity = Vector2Subtract(player.velocity, Vector2Scale(player.velocity, 5.0f * delta_time));

    Vector2 new_position = Vector2Add(player.position, Vector2Scale(player.velocity, delta_time));
    Player temp_player = player;
    temp_player.position = new_position;

    if (player.tile_map && !player.tile_map->CheckTileCollision(&temp_player)) {
        player.position = new_position;

        if (Vector2Length(player.velocity) < 50.0f) {
            player.velocity = Vector2Zero();
            player.SetState(&player.idle);
        }
    } else {
        player.velocity = Vector2Zero();
        player.SetState(&player.idle);
    }

    player.acceleration = Vector2Zero();

    if (player.invulnerable_timer > 0.0f) {
        player.invulnerable_timer -= delta_time;
    }
}

void PlayerDodging::HandleCollision(Player& player, Entity* other_entity) {
    if (CheckCollisionCircles(player.position, player.radius, other_entity->position, other_entity->radius)) {
        player.health -= 0;
    }
}