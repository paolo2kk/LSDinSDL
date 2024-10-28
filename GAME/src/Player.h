#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Box2D/Box2D.h"
#include "Animation.h"
#include "Physics.h"

struct SDL_Texture;
enum Direction {
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT
};
class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	void GravityChange(b2Vec2& velocity);

	bool CleanUp();

	void SetAnimation(Direction dir);

	void SetPosition(Vector2D newPosition) {
		position = newPosition;
		pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(newPosition.getX()), PIXEL_TO_METERS(newPosition.getY())), 0); 
	}
	// L08 TODO 6: Define OnCollision function for the player. 
	void OnCollision(PhysBody* physA, PhysBody* physB);

	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

	void SetParameters(pugi::xml_node parameters) {
		this->parameters = parameters;
	}

public:

	//Declare player parameters
	float speed = 5.0f;
	SDL_Texture* texture = NULL;
	int texW, texH;

	//Audio fx
	int pickCoinFxId;

	// L08 TODO 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;
	
	float jumpForce = 2.5f; // The force to apply when jumping
	bool isJumping = false; // Flag to check if the player is currently jumping

	pugi::xml_node parameters;
	Animation* currentAnimation = nullptr;
	Animation idle;
	Animation right;
	Animation left;
	Animation up;
	Animation down;

	// Gravity modifiers
	b2Vec2 velocity = b2Vec2(0, -1);
	float gravTimer = 0;
	float gravDesiredTimer = 3000;
	float inputBufferTime = 200;
	float inputBufferTimer = 0;
	bool canExecuteGrav = true;
	Direction lastDirection = NONE;
	ColliderType currentCollider = ColliderType::UNKNOWN;

	bool isDead = false;

	bool canChangeDirection = true;
	bool isCornerCollision = false;

};