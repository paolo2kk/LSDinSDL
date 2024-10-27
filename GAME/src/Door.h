#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"

struct SDL_Texture;

class Door : public Entity
{
public:

	Door();
	virtual ~Door();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();
	
public:

	PhysBody* pbody;

	bool isPicked = false;
	bool imDed = false;

private:

	SDL_Texture* texture;
	const char* texturePath;
	int texW, texH;

	//L08 TODO 4: Add a physics to an item
};
