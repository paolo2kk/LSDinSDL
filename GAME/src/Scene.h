#pragma once

#include "Module.h"
#include "Player.h"
#include "Box.h"
#include "Trigger.h"
#include "Door.h"
#include "Player.h"
#include "Map.h"
#include "Item.h"
#include "Trigger.h"
#include "Box.h"
#include "Door.h"
#include "ParallaxBackground.h"
#include "MovingPlatform.h"
#include "Enemy.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	//bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	void TriggerManagement();

	void SetLevel(int stage);


	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void SceneChange();

public:
	int stage = 1;

private:
	SDL_Texture* img;
	//L03: TODO 3b: Declare a Player attribute
	Player* player;
	Vector2D playerps1;

	Enemy* enemy_LVL2;

	int windowSizeX = 960;
	//LVL1
	Box* box1_LVL1;
	Trigger* trigger1;
	Door* door1;

	//LVL2
	MovingPlatform* mv_pltf_LVL2;

	//LVL7
	MovingPlatform* mv_pltf_LVL7;

	//scenification
	const int totalStages = 10;
	bool isSceneSetted = false;
	const int boundaryBuffer = 20;
	bool doneLVL1 = false;
	bool help = false;

};