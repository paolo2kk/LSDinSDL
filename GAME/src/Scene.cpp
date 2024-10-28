#include "Engine.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Log.h"
#include "Entity.h"
#include "EntityManager.h"
#include "ParallaxBackground.h"  


Scene::Scene() : Module()
{
	name = "scene";
	img = nullptr;
}

// Destructor
Scene::~Scene()
{}

ParallaxBackground parallax; 
int cameraX = 0; 

bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	// L04: TODO 3b: Instantiate the player using the entity manager
	player = (Player*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PLAYER);
	player->SetParameters(configParameters.child("entities").child("player"));

	//enemies
	enemy_LVL2 = (Enemy*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ENEMY);
	enemy_LVL2->SetParameters(configParameters.child("entities").child("enemy"));
	
	// Crear un nuevo item usando el entity manager

	// Triggers y puertas
	trigger1 = (Trigger*)Engine::GetInstance().entityManager->CreateEntity(EntityType::TRIGGER);
	trigger1->position = Vector2D(704, 17);
	door1 = (Door*)Engine::GetInstance().entityManager->CreateEntity(EntityType::DOOR);
	door1->position = Vector2D(820, 140);

	// L04: Create a box
	box1_LVL1 = (Box*)Engine::GetInstance().entityManager->CreateEntity(EntityType::BOX);

	mv_pltf_LVL2 = (MovingPlatform*)Engine::GetInstance().entityManager->CreateEntity(EntityType::MOVING_PLATFORML);

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	// Cargar el mapa
	Engine::GetInstance().map->Load("Assets/Maps/", "DefMap.tmx");

	// Posicionar al jugador y objetos
	player->position = Vector2D(160, 832);
	playerps1 = Vector2D(160, 910);

	enemy_LVL2->position = Vector2D(1100, 585);
	enemy_LVL2->stage = 2;
	
	box1_LVL1->position = Vector2D(704, 840);
	box1_LVL1->stage = 1;


	mv_pltf_LVL2->position = Vector2D(1200, 835);
	mv_pltf_LVL2->stage = 2;

	
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	// Movimiento de la c�mara basado en el jugador
	cameraX = player->position.getX() - 400;  // Mantener al jugador centrado horizontalmente
	float camSpeed = 1;
	//parallax.updateAndRender();

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_I) == KEY_REPEAT)
		Engine::GetInstance().render.get()->camera.y -= ceil(camSpeed * dt);

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
		Engine::GetInstance().render.get()->camera.y += ceil(camSpeed * dt);

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
		Engine::GetInstance().render.get()->camera.x -= ceil(camSpeed * dt);

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
		Engine::GetInstance().render.get()->camera.x += ceil(camSpeed * dt);

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_R) == KEY_REPEAT)
		isSceneSetted = false;
		

	// Actualizar y renderizar el fondo parallax

	TriggerManagement();
	SceneChange();

	if (player->isDead)
	{
		isSceneSetted = false;
		SetLevel(stage);
		player->isDead = false;

	}

	return true;
}



void Scene::TriggerManagement()
{
	if (box1_LVL1->inTrigger == true && !doneLVL1)
	{
		PhysBody* pbodyofdoor = door1->pbody;
		Engine::GetInstance().physics.get()->DeletePhysBody(pbodyofdoor);
		Engine::GetInstance().entityManager->DestroyEntity(door1);
		box1_LVL1->stage = -1;
		doneLVL1 = true;
	}
}

void Scene::SetLevel(int stage)
{
	if (!isSceneSetted/* && Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_R) == KEY_REPEAT*/)
	{
		switch (stage)
		{
		case 1:
			player->SetPosition(playerps1);
			box1_LVL1->SetPosition(Vector2D(704, 910));
			player->canChangeDirection = true;
			player->velocity.Set(0, 0);
			player->lastDirection = NONE;
			
			break;
		case 2:
			player->SetPosition(Vector2D(1080, 250));
			enemy_LVL2->SetPosition(Vector2D(1100, 585));
			enemy_LVL2->velocity.Set(0, 0);
			mv_pltf_LVL2->SetPosition(Vector2D(1200, 835));

			break;
		}
		isSceneSetted = true;
		
	}
	
}

void Scene::SceneChange()
{

	switch (stage) {
	case 1:
		SetLevel(1);
		if (player->position.getX() >= windowSizeX)
		{
			Engine::GetInstance().render.get()->camera.x = -windowSizeX;
			stage++;
			isSceneSetted = false;
			SetLevel(2);

		}
		

		break;

	case 2:
		if (player->position.getX() < windowSizeX)
		{
			Engine::GetInstance().render.get()->camera.x = 0;
			stage--;
			isSceneSetted = false;

		}
		else
		if (player->position.getX() >= windowSizeX * 2)
		{
			Engine::GetInstance().render.get()->camera.x = -windowSizeX * 2;
			stage++;
		}
		break;
	}

	//entity stage management
	for (Entity* entity : Engine::GetInstance().entityManager.get()->entities) {
		if (entity->stage == stage && player->canChangeDirection)
		{
			entity->canMove = true;
		}
		else
		{
			entity->canMove = false;
			
		}

		if (entity->position.getY() > 1400)
		{
			entity->toDestroy = true;
		}

		
	}

}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	SDL_DestroyTexture(img);

	return true;
}


/*
in Life, the Sensuous Dream
in Limbo, the Silent Dream
in Lunacy, the Savage Dream
in Laughter, the Spiritual Dream
in Leisure, the Sonorous Dream
in Linking, the Sapient Dream
*/