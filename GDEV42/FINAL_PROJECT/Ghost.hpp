#ifndef GHOST 
#define GHOST 

#include <raylib.h>
#include <raymath.h>
#include "Entity.hpp"
#include "TileMap.hpp"

#define GAME_SCENE_SPRITE_GHOST "Assets/Sprites/ghost.png"

class Ghost;

class GhostState {
public: 
    virtual ~GhostState() {}
    virtual void Enter(Ghost& ghost) = 0;
    virtual void Update(Ghost& ghost, float delta_time) = 0;
    virtual void HandleCollision(Ghost& ghost, Entity* other_entity) = 0;
};

class GhostWandering: public GhostState {
public: 
    float change_direction_cooldown;
    Vector2 move_direction;
    void Enter (Ghost& ghost);
    void Update(Ghost& ghost, float delta_time);
    void HandleCollision(Ghost& ghost, Entity* other_entity);
};

class GhostChasing: public GhostState {
public:
    void Enter (Ghost& ghost);
    void Update(Ghost& ghost, float delta_time);
    void HandleCollision(Ghost& ghost, Entity* other_entity);   
};

class GhostAttacking: public GhostState{
public: 
    void Enter (Ghost& ghost);
    void Update(Ghost& ghost, float delta_time);
    void HandleCollision(Ghost& ghost, Entity* other_entity);   
};

class Ghost : public Entity {
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

    Texture2D ghostSprite; 
    Rectangle ghostFrameRect;
    Rectangle ghostDR;
    int currentFrame;
    float frameWidth, frameHeight;
    int direction; 
    float animationTimer; 
    float frameSpeed;
    int animationStartFrame;
    int maxFrames; 
    bool playOnce; 

    Vector2 velocity;
    Vector2 Acceleration; 
    float speed; 

    Entity* entity_following;

    GhostWandering wandering;
    GhostChasing chasing; 
    GhostAttacking attack;

    void setTileMap(TileMap* map) {
        tile_map = map;
    }

    Ghost(Vector2 pos, float spd, float rad, float d_radius, float a_radius, float r_radius, int hp);

    void Update(float delta_time);
    void Draw();
    void SetState(GhostState* new_state);
    void HandleCollision(Entity* other_entity);

private:
    GhostState* current_state;


};

#endif