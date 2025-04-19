#ifndef SLIME 
#define SLIME 

#include <raylib.h>
#include <raymath.h>
#include "Entity.hpp"
#include "TileMap.hpp"

#define GAME_SCENE_SPRITE_SLIME "Assets/Sprites/slime.png"

class Slime;

class SlimeState {
public: 
        virtual ~SlimeState() {}
        virtual void Enter(Slime& slime) = 0;
        virtual void Update(Slime& slime, float delta_time) = 0;
        virtual void HandleCollision(Slime& slime, Entity* other_entity) = 0;
};

class slimeWandering : public SlimeState {
public:
    float change_direction_cooldown;
    Vector2 move_direction;
    void Enter(Slime& enemy);
    void Update(Slime& enemy, float delta_time);
    void HandleCollision(Slime& enemy, Entity* other_entity);
};

class slimeChasing : public SlimeState {
public:
    void Enter(Slime& slime);
    void Update(Slime& slime, float delta_time);
    void HandleCollision (Slime& slime, Entity* other_entity);
};

class slimeAttacking : public SlimeState {
public:
    Vector2 attack_direction;
    void Enter(Slime& slime);
    void Update(Slime& slime, float delta_time);
    void HandleCollision(Slime& slime, Entity* other_entity);
};

class Slime : public Entity {
public: 
    TileMap* tile_map;
    Color color;

    bool active;
    float detection_radius;
    float aggro_radius;
    float attack_range;

    enum class Animation_type {
        WANDERING, 
        CHASING, 
        ATTACKING,
        DEATH
    };

    Animation_type animation_state;

    Texture2D slimeSprite;
    Rectangle slimeFrameRect;
    Rectangle slimeDR;
    int currentFrame;
    float frameWidth, frameHeight;
    int direction;
    float animationTimer;
    float frameSpeed;
    int animationStartFrame;
    int maxFrames;
    bool playOnce;

    Vector2 velocity;
    Vector2 acceleration;
    float speed;

    Entity* entity_following;

    slimeWandering wandering;
    slimeChasing chasing;
    slimeAttacking attack;

    void setTileMap(TileMap* map) {
        tile_map = map;
    }

    Slime(Vector2 pos, float spd, float rad, float d_radius, float a_radius, float r_radius, int hp);

    void Update(float delta_time);
    void Draw();
    void SetState(SlimeState* new_state);
    void HandleCollision(Entity* other_entity);

private:
    SlimeState* current_state;
        
};


#endif