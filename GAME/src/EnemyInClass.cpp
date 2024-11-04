#include "EnemyInClass.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "Map.h"
EnemyInClass::EnemyInClass() : Entity(EntityType::ENEMYBFS)
{
}
EnemyInClass::~EnemyInClass() {
	delete pathfinding;
}
bool EnemyInClass::Awake() {
	return true;
}
bool EnemyInClass::Start() {
	//initilize textures
	texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
	position.setX(parameters.attribute("x").as_int());
	position.setY(parameters.attribute("y").as_int());
	texW = parameters.attribute("w").as_int();
	texH = parameters.attribute("h").as_int();
	//Load animations
	idle.LoadAnimations(parameters.child("animations").child("idle"));
	currentAnimation = &idle;

	//Add a physics to an item - initialize the physics body
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX() + texH / 2, (int)position.getY() + texH / 2, texH / 2, bodyType::DYNAMIC);
	//Assign collider type
	pbody->ctype = ColliderType::ENEMYBFS;
	// Set the gravity of the body
	if (!parameters.attribute("gravity").as_bool()) pbody->body->SetGravityScale(0);
	// Initialize pathfinding
	pathfinding = new Pathfinding();
	Vector2D pos = GetPosition();
	Vector2D tilePos = Engine::GetInstance().map.get()->WorldToMap(pos.getX(), pos.getY());
	pathfinding->ResetPath(tilePos);
	return true;
}
bool EnemyInClass::Update(float dt)
{
	// Pathfinding testing inputs
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
		Vector2D pos = GetPosition();
		Vector2D tilePos = Engine::GetInstance().map.get()->WorldToMap(pos.getX(), pos.getY());
		pathfinding->ResetPath(tilePos);
	}
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_Z) == KEY_DOWN) {
		pathfinding->PropagateBFS();
	}
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_Z) == KEY_REPEAT &&
		Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
		pathfinding->PropagateBFS();
	}
	// L08 TODO 4: Add a physics to an item - update the position of the object from the physics.  
	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);
	Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();
	// Draw pathfinding 
	pathfinding->DrawPath();
	return true;
}
bool EnemyInClass::CleanUp()
{
	return true;
}
void EnemyInClass::SetPosition(Vector2D pos) {
	pos.setX(pos.getX() + texW / 2);
	pos.setY(pos.getY() + texH / 2);
	b2Vec2 bodyPos = b2Vec2(PIXEL_TO_METERS(pos.getX()), PIXEL_TO_METERS(pos.getY()));
	pbody->body->SetTransform(bodyPos, 0);
}
Vector2D EnemyInClass::GetPosition() {
	b2Vec2 bodyPos = pbody->body->GetTransform().p;
	Vector2D pos = Vector2D(METERS_TO_PIXELS(bodyPos.x), METERS_TO_PIXELS(bodyPos.y));
	return pos;
}