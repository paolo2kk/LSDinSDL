#pragma once

#include "Input.h"
#include "Render.h"

enum class EntityType
{
	PLAYER,
	ITEM,
	TRIGGER,
	BOX,
	DOOR,
	PARTICLE,
	MOVING_PLATFORMR,
	MOVING_PLATFORML,
	ENEMY,
	ENEMYBFS,
	UNKNOWN
};

enum class States
{
	IDLE,
	WALKING_R,
	WALKING_L,
	JUMPING,
	FALLING,
	DEAD
};

class PhysBody;

class Entity
{
public:

	Entity(EntityType type) : type(type), active(true) {}

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	void Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}

	virtual void OnCollision(PhysBody* physA, PhysBody* physB) {

	};

	virtual void OnCollisionEnd(PhysBody* physA, PhysBody* physB) {

	};

public:

	std::string name;
	EntityType type;
	bool active = true;

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	Vector2D position;       
	bool renderable = true;
	bool canMove = false;
	bool toDestroy = false;

	int stage;
};