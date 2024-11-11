
#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Physics.h"
#include "PerfTimer.h"  // Include the PerfTimer header
#include "Animation.h"
#include "Pathfinding.h"

struct SDL_Texture;

class Enemy : public Entity
{
public:
    Enemy();
    virtual ~Enemy();

    bool Awake();
    bool Start();
    void GravityChange(b2Vec2& velocity);
    bool Update(float dt);
    bool CleanUp();

    void OnCollision(PhysBody* physA, PhysBody* physB);

    void SetParameters(pugi::xml_node parameters) {
        this->parameters = parameters;
    }
    void SetPosition(Vector2D newPosition) {
        position = newPosition;
        pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(newPosition.getX()), PIXEL_TO_METERS(newPosition.getY())), 0);
    }
public:

    pugi::xml_node parameters;

    // Enemy parameters
    float speed = 5.0f;
    SDL_Texture* texture = nullptr;
    int texW, texH;
    bool inTrigger = false;
    // Audio fx
    int pickCoinFxId;

    Animation* currentAnimation = nullptr;
    Animation left;
    Animation right;

    // Physics body
    PhysBody* pbody;

    // Gravity modifiers
    b2Vec2 velocity = b2Vec2(0, -GRAVITY_Y);
    float gravTimer = 0;
    float gravDesiredTimer = 3000;
    float inputBufferTime = 200;
    float inputBufferTimer = 0;
    bool canExecuteGrav = true;

    PerfTimer perfTimer;


};